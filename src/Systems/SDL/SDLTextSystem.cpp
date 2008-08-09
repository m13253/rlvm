// -*- Mode: C++; tab-width:2; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi:tw=80:et:ts=2:sts=2
//
// -----------------------------------------------------------------------
//
// This file is part of RLVM, a RealLive virtual machine clone.
//
// -----------------------------------------------------------------------
//
// Copyright (C) 2006, 2007 Elliot Glaysher
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
// -----------------------------------------------------------------------

#include "Precompiled.hpp"

// -----------------------------------------------------------------------

/**
 * @file   SDLTextSystem.cpp
 * @brief  SDL specialization of the text system
 * @author Elliot Glaysher
 * @date   Wed Mar  7 22:04:25 2007
 *
 */

#include "MachineBase/RLMachine.hpp"

#include "Systems/SDL/SDLTextSystem.hpp"
#include "Systems/SDL/SDLTextWindow.hpp"
#include "Systems/SDL/SDLSurface.hpp"
#include "Systems/Base/System.hpp"
#include "Systems/Base/SystemError.hpp"
#include "Systems/Base/TextKeyCursor.hpp"
#include "Systems/Base/Rect.hpp"

#include "libReallive/gameexe.h"

#include "algoplus.hpp"
#include "Utilities.h"
#include "findFontFile.h"

#include <boost/bind.hpp>
#include "SDL_ttf.h"
#include <sstream>
#include <stdexcept>

using namespace std;
using namespace boost;

// -----------------------------------------------------------------------

SDLTextSystem::SDLTextSystem(Gameexe& gameexe)
  : TextSystem(gameexe)
{
  if(TTF_Init()==-1) {
    ostringstream oss;
    oss << "Error initializing SDL_ttf: " << TTF_GetError();
    throw SystemError(oss.str());
  }
}

// -----------------------------------------------------------------------

SDLTextSystem::~SDLTextSystem()
{
  TTF_Quit();
}

// -----------------------------------------------------------------------

void SDLTextSystem::render(RLMachine& machine)
{
  if(systemVisible())
  {
    for(WindowMap::iterator it = text_window_.begin(); it != text_window_.end(); ++it)
    {
      it->second->render(machine);
    }

    WindowMap::iterator it = text_window_.find(active_window_);

    if(it != text_window_.end() && it->second->isVisible() &&
       in_pause_state_ && !isReadingBacklog())
    {
      if(!text_key_cursor_)
        setKeyCursor(machine, 0);

      text_key_cursor_->render(machine, *it->second);
    }
  }
}

// -----------------------------------------------------------------------

TextWindow& SDLTextSystem::textWindow(RLMachine& machine, int textWindow)
{
  WindowMap::iterator it = text_window_.find(textWindow);
  if(it == text_window_.end())
  {
    it = text_window_.insert(
      textWindow, new SDLTextWindow(machine, textWindow)).first;
  }

  return *it->second;
}

// -----------------------------------------------------------------------

void SDLTextSystem::updateWindowsForChangeToWindowAttr()
{
  // Check each text window to see if it needs updating
  for(WindowMap::iterator it = text_window_.begin();
      it != text_window_.end(); ++it)
  {
    if(!it->second->windowAttrMod())
      it->second->setRGBAF(windowAttr());
  }
}

// -----------------------------------------------------------------------

void SDLTextSystem::setDefaultWindowAttr(const std::vector<int>& attr)
{
  TextSystem::setDefaultWindowAttr(attr);
  updateWindowsForChangeToWindowAttr();
}

// -----------------------------------------------------------------------

void SDLTextSystem::setWindowAttrR(int i)
{
  TextSystem::setWindowAttrR(i);
  updateWindowsForChangeToWindowAttr();
}

// -----------------------------------------------------------------------

void SDLTextSystem::setWindowAttrG(int i)
{
  TextSystem::setWindowAttrG(i);
  updateWindowsForChangeToWindowAttr();
}

// -----------------------------------------------------------------------

void SDLTextSystem::setWindowAttrB(int i)
{
  TextSystem::setWindowAttrB(i);
  updateWindowsForChangeToWindowAttr();
}

// -----------------------------------------------------------------------

void SDLTextSystem::setWindowAttrA(int i)
{
  TextSystem::setWindowAttrA(i);
  updateWindowsForChangeToWindowAttr();
}

// -----------------------------------------------------------------------

void SDLTextSystem::setWindowAttrF(int i)
{
  TextSystem::setWindowAttrF(i);
  updateWindowsForChangeToWindowAttr();
}

// -----------------------------------------------------------------------

void SDLTextSystem::setMousePosition(RLMachine& machine, const Point& pos)
{
  for(WindowMap::iterator it = text_window_.begin();
      it != text_window_.end(); ++it)
  {
    it->second->setMousePosition(machine, pos);
  }
}

// -----------------------------------------------------------------------

bool SDLTextSystem::handleMouseClick(RLMachine& machine, const Point& pos,
                                     bool pressed)
{
  if(systemVisible())
  {
    for(WindowMap::iterator it = text_window_.begin();
        it != text_window_.end(); ++it)
    {
      if(it->second->handleMouseClick(machine, pos, pressed))
        return true;
    }

    return false;
  }
  else
  {
    return false;
  }
}

// -----------------------------------------------------------------------

boost::shared_ptr<Surface> SDLTextSystem::renderText(
  RLMachine& machine, const std::string& utf8str, int size, int xspace,
  int yspace, int colour)
{
  // Pick the correct font
  shared_ptr<TTF_Font> font = getFontOfSize(machine, size);

  // Pick the correct font colour
  Gameexe& gexe = machine.system().gameexe();
  vector<int> colourVec = gexe("COLOR_TABLE", colour);
  SDL_Color color = {colourVec.at(0), colourVec.at(1), colourVec.at(2)};

  // Naively render. Ignore most of the arguments for now
  if(utf8str.size())
  {
    SDL_Surface* tmp =
      TTF_RenderUTF8_Blended(font.get(), utf8str.c_str(), color);
    if(tmp == NULL)
    {
      ostringstream oss;
      oss << "Error printing \"" << utf8str << "\" in font size " << size;
      throw rlvm::Exception(oss.str());
    }
    return shared_ptr<Surface>(new SDLSurface(tmp));
  }
  else
  {
    // Allocate a 1x1 SDL_Surface
    return shared_ptr<Surface>(new SDLSurface(buildNewSurface(Size(1, 1))));
  }
}

// -----------------------------------------------------------------------

boost::shared_ptr<TTF_Font> SDLTextSystem::getFontOfSize(
  RLMachine& machine, int size)
{
  FontSizeMap::iterator it = map_.find(size);
  if(it == map_.end())
  {
    string filename = findFontFile(machine).external_file_string();
    TTF_Font* f = TTF_OpenFont(filename.c_str(), size);
    if(f == NULL)
    {
      ostringstream oss;
      oss << "Error loading font: " << TTF_GetError();
      throw SystemError(oss.str());
    }

    TTF_SetFontStyle(f, TTF_STYLE_NORMAL);

    // Build a smart_ptr to own this font, and set a deleter function.
    shared_ptr<TTF_Font> font(f, TTF_CloseFont);

    map_[size] = font;
    return font;
  }
  else
  {
    return it->second;
  }
}
