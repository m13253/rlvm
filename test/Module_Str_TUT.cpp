// This file is part of RLVM, a RealLive virutal machine clone.
//
// -----------------------------------------------------------------------
//
// Copyright (C) 2006 El Riot
//  
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
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

/**
 * @file   Module_Str_TUT.cpp
 * @author Elliot Glaysher
 * @date   Sat Sep 16 14:22:35 2006
 * 
 * Test cases for the string module
 */


#include "Module_Str.hpp"
#include "Archive.h"
#include "RLMachine.hpp"

#include "tut.hpp"


#include <iostream>
using namespace std;
/**
 * @file
 * @ingroup TestCase
 *
 * This test group tests the String module. 
 */


namespace tut
{

/**
 * Struct which may contain test data members.
 * Test object (class that contains test methods)
 * will inherite from it, so each test method can
 * access members directly.
 *
 * Additionally, for each test, test object is re-created
 * using defaut constructor. Thus, any prepare work can be put
 * into default constructor.
 *
 * Finally, after each test, test object is destroyed independently
 * of test result, so any cleanup work should be located in destructor.
 */
struct Module_Str_data
{ 

};

/**
 * This group of declarations is just to register
 * test group in test-application-wide singleton.
 * Name of test group object (auto_ptr_group) shall
 * be unique in tut:: namespace. Alternatively, you
 * you may put it into anonymous namespace.
 */
typedef test_group<Module_Str_data> tf;
typedef tf::object object;
tf module_str_group("Module_Str");

// -----------------------------------------------------------------------

/**
 * Tests strcpy_0, which should copy the string valid int strS[0].
 *
 * Corresponding kepago listing:
 * @code
 * strS[0] = "valid"
 * @endcode
 */
template<>
template<>
void object::test<1>()
{
  Reallive::Archive arc("test/seenFiles/strcpy_0.TXT");
  RLMachine rlmachine(arc);
  rlmachine.attatchModule(new StrModule);
  rlmachine.executeUntilHalted();

  string one = rlmachine.getStringValue(0x12, 0);
  ensure_equals("strcpy_0 script failed to set value", one, "valid");
}

// -----------------------------------------------------------------------

/** 
 * Tests strcpy_1, which should copy the first 2 chracters into strS[0].
 * 
 * Corresponding kepago listing:
 * @code
 * strcpy(strS[0], "valid", 2)
 * @endcode
 */
template<>
template<>
void object::test<2>()
{
  Reallive::Archive arc("test/seenFiles/strcpy_1.TXT");
  RLMachine rlmachine(arc);
  rlmachine.attatchModule(new StrModule);
  rlmachine.executeUntilHalted();

  string one = rlmachine.getStringValue(0x12, 0);
  ensure_equals("strcpy_1 script failed to set value", one, "va");
}

// -----------------------------------------------------------------------

/** 
 * Tests strclear_0, which should clear the value of a string.
 * 
 * Corresponding kepago listing:
 * @code
 * strS[0] = "valid"
 * strS[1] = "valid"
 * strclear(strS[0])
 * @endcode
 */
template<>
template<>
void object::test<3>()
{
  Reallive::Archive arc("test/seenFiles/strclear_0.TXT");
  RLMachine rlmachine(arc);
  rlmachine.attatchModule(new StrModule);
  rlmachine.executeUntilHalted();

  string one = rlmachine.getStringValue(0x12, 0);
  ensure_equals("strclear_0 script failed to unset value", one, "");

  // We include this check to make sure the machine is sane and that
  // the first assignment works, so strclear doesn't appear to work
  // because assignment failed.
  string two = rlmachine.getStringValue(0x12, 1);
  ensure_equals("strclear_0 script failed to set value", two, "valid");
}

// -----------------------------------------------------------------------

/** 
 * Tests strclear_1, which should clear out a whole range of values.
 * 
 * @code
 * strS[0] = "valid"
 * strS[1] = "valid"
 * strS[2] = "valid"
 * strclear(strS[0], strS[1])
 * @endcode
 */
template<>
template<>
void object::test<4>()
{
  Reallive::Archive arc("test/seenFiles/strclear_1.TXT");
  RLMachine rlmachine(arc);
  rlmachine.attatchModule(new StrModule);
  rlmachine.executeUntilHalted();

  string one = rlmachine.getStringValue(0x12, 0);
  ensure_equals("strclear_1 script failed to unset value", one, "");
  string two = rlmachine.getStringValue(0x12, 1);
  ensure_equals("strclear_1 script failed to unset value", two, "");

  // We include this check to make sure the machine is sane and that
  // the first assignment works, so strclear doesn't appear to work
  // because assignment failed.
  string three = rlmachine.getStringValue(0x12, 2);
  ensure_equals("strclear_1 script failed to set value", three, "valid");
}

// -----------------------------------------------------------------------

/**
 * Tests strcat, which should end up with the string "valid" in strS[0].
 *
 * Corresponding kepago listing:
 * @code
 * strS[0] = "va"
 * strS[0] += "lid"
 * @endcode
 */
template<>
template<>
void object::test<5>()
{
  Reallive::Archive arc("test/seenFiles/strcat_0.TXT");
  RLMachine rlmachine(arc);
  rlmachine.attatchModule(new StrModule);
  rlmachine.executeUntilHalted();

  string one = rlmachine.getStringValue(0x12, 0);
  ensure_equals("strcat script failed to set value", one, "valid");
}

// -----------------------------------------------------------------------

/**
 * Tests strlen. Should be 5, for "vaild"
 *
 * Corresponding kepago listing:
 * @code
 * strS[0] = "valid"
 * intA[0] = strlen(strS[0])
 * @endcode
 */
template<>
template<>
void object::test<6>()
{
  Reallive::Archive arc("test/seenFiles/strlen_0.TXT");
  RLMachine rlmachine(arc);
  rlmachine.attatchModule(new StrModule);
  rlmachine.executeUntilHalted();

  int one = rlmachine.getIntValue(0, 0);
  ensure_equals("strlen script failed to set the strlen(\"valid\") to 5", one, 5);
}

// -----------------------------------------------------------------------

/** 
 * Tests strcmp; make sure that it gives the same values as normal strcmp.
 *
 * Corresponding kepago listing:
 * @code
 * strS[0] = "a"
 * strS[1] = "b"
 * strS[2] = "b"
 * strS[3] = "c"
 * intA[0] = strcmp(strS[0], strS[1])
 * intA[1] = strcmp(strS[1], strS[2])
 * intA[2] = strcmp(strS[2], strS[3])
 * @endcode
 */
template<>
template<>
void object::test<7>()
{
  Reallive::Archive arc("test/seenFiles/strcmp_0.TXT");
  RLMachine rlmachine(arc);
  rlmachine.attatchModule(new StrModule);
  rlmachine.executeUntilHalted();

  ensure_equals("Different values for strcmp(\"a\", \"b\")",
                rlmachine.getIntValue(0, 0),
                strcmp("a", "b"));
  ensure_equals("Different values for strcmp(\"b\", \"b\")",
                rlmachine.getIntValue(0, 1),
                strcmp("b", "b"));
  ensure_equals("Different values for strcmp(\"b\", \"c\")",
                rlmachine.getIntValue(0, 2),
                strcmp("b", "c"));
}

// -----------------------------------------------------------------------

/** 
 * First strsub_0 test. Make sure it works on ASCII strings.
 */
template<>
template<>
void object::test<8>()
{
  Reallive::Archive arc("test/seenFiles/strsub_0.TXT");
  RLMachine rlmachine(arc);
  rlmachine.attatchModule(new StrModule);
  rlmachine.executeUntilHalted();

  ensure_equals("strsub returned wrong value",
                rlmachine.getStringValue(0x12, 1),
                "lid");
}

// -----------------------------------------------------------------------

/** 
 * Second strsub_0 test. Make sure it works on Shift_JIS strings.
 *
 * @code
 * strS[0] = "わたしの名前、まだ覚えてる？"
 * strS[1] = strsub(strS[0], 7)
 * @endcode
 */
template<>
template<>
void object::test<9>()
{
  Reallive::Archive arc("test/seenFiles/strsub_1.TXT");
  RLMachine rlmachine(arc);
  rlmachine.attatchModule(new StrModule);
  rlmachine.executeUntilHalted();

  ensure_equals("strsub returned wrong value",
                rlmachine.getStringValue(0x12, 1),
                "\x82\xDC\x82\xBE\x8A\x6F\x82\xA6\x82\xC4\x82\xE9\x81\x48");
}

// -----------------------------------------------------------------------

/** 
 * First strsub_1 test. Make sure it deals with ASCII.
 * 
 * @code
 * strS[0] = "valid"
 * strS[1] = strsub(strS[0], 1, 2)
 * @endcode
 */
template<>
template<>
void object::test<10>()
{
  Reallive::Archive arc("test/seenFiles/strsub_2.TXT");
  RLMachine rlmachine(arc);
  rlmachine.attatchModule(new StrModule);
  rlmachine.executeUntilHalted();

  ensure_equals("strsub returned wrong value",
                rlmachine.getStringValue(0x12, 1),
                "al");
}

// -----------------------------------------------------------------------

/** 
 * Second strsub_1 test. Make sure it works on Shift_JIS strings.
 *
 * @code
 * strS[0] = "わたしの名前、まだ覚えてる？"
 * strS[1] = strsub(strS[0], 7)
 * @endcode
 */
template<>
template<>
void object::test<11>()
{
  Reallive::Archive arc("test/seenFiles/strsub_3.TXT");
  RLMachine rlmachine(arc);
  rlmachine.attatchModule(new StrModule);
  rlmachine.executeUntilHalted();

  ensure_equals("strsub returned wrong value",
                rlmachine.getStringValue(0x12, 1),
                "\x96\xBC\x91\x4F");
}

// -----------------------------------------------------------------------

/** 
 * Strrsub test. (We don't need to test this on Shift_JIS since this
 * calls the same code path as strsub, with just a different offset
 * method.)
 * 
 * @code
 * strS[0] = "valid"
 * strS[1] = strrsub(strS[0], 2)
 * @endcode
 */
template<>
template<>
void object::test<12>()
{
  Reallive::Archive arc("test/seenFiles/strrsub_0.TXT");
  RLMachine rlmachine(arc);
  rlmachine.attatchModule(new StrModule);
  rlmachine.executeUntilHalted();

  ensure_equals("strrsub returned wrong value",
                rlmachine.getStringValue(0x12, 1),
                "id"); 
}

// -----------------------------------------------------------------------

/** 
 * Strrsub test.
 * 
 * @code
 * strS[0] = "valid"
 * strS[1] = strrsub(strS[0], 2, 1)
 * @endcode
 */
template<>
template<>
void object::test<13>()
{
  Reallive::Archive arc("test/seenFiles/strrsub_1.TXT");
  RLMachine rlmachine(arc);
  rlmachine.attatchModule(new StrModule);
  rlmachine.executeUntilHalted();

  ensure_equals("strrsub returned wrong value",
                rlmachine.getStringValue(0x12, 1),
                "i"); 
}

// -----------------------------------------------------------------------

/** 
 * strcharlen test on ASCII strings
 * 
 * @code
 * strS[0] = "valid"
 * intA[0] = strcharlen(strS[0])
 * @endcode
 */
template<>
template<>
void object::test<14>()
{
  Reallive::Archive arc("test/seenFiles/strcharlen_0.TXT");
  RLMachine rlmachine(arc);
  rlmachine.attatchModule(new StrModule);
  rlmachine.executeUntilHalted();

  ensure_equals("strcharlen returned wrong value",
                rlmachine.getIntValue(0, 0),
                5);
}

// -----------------------------------------------------------------------

/** 
 * strcharlen test on Shift_JIS strings
 * 
 * @code
 * strS[0] = "わたしの名前、まだ覚えてる？"
 * intA[0] = strcharlen(strS[0])
 * @endcode
 */
template<>
template<>
void object::test<15>()
{
  Reallive::Archive arc("test/seenFiles/strcharlen_1.TXT");
  RLMachine rlmachine(arc);
  rlmachine.attatchModule(new StrModule);
  rlmachine.executeUntilHalted();

  ensure_equals("strcharlen returned wrong value",
                rlmachine.getIntValue(0, 0),
                14);
}

// -----------------------------------------------------------------------

/** 
 * strstrunc test on ASCII characters
 * 
 * @code
 * strS[0] = "valid"
 * strtrunc(strS[0], 2)
 * @endcode
 */
template<>
template<>
void object::test<16>()
{
  Reallive::Archive arc("test/seenFiles/strtrunc_0.TXT");
  RLMachine rlmachine(arc);
  rlmachine.attatchModule(new StrModule);
  rlmachine.executeUntilHalted();

  ensure_equals("strtrunc returned wrong value",
                rlmachine.getStringValue(0x12, 0),
                "va"); 
}

// -----------------------------------------------------------------------

/** 
 * strstrunc test on ASCII characters
 * 
 * @todo comment is wrong; recopy the code
 * @code
 * strS[0] = "valid"
 * strtrunc(strS[0], 2)
 * @endcode
 */
template<>
template<>
void object::test<17>()
{
  Reallive::Archive arc("test/seenFiles/strtrunc_1.TXT");
  RLMachine rlmachine(arc);
  rlmachine.attatchModule(new StrModule);
  rlmachine.executeUntilHalted();

  ensure_equals("strtrunc returned wrong value",
                rlmachine.getStringValue(0x12, 0),
                "\x82\xED\x82\xBD\x82\xB5"); 
}

// -----------------------------------------------------------------------

/** 
 * Test hantozen() on ASCII numbers
 * 
 * @code
 * strS[0] = "12345"
 * strS[0] = hantozen()
 * @endcode
 */
template<>
template<>
void object::test<18>()
{
  Reallive::Archive arc("test/seenFiles/hantozen_0.TXT");
  RLMachine rlmachine(arc);
  rlmachine.attatchModule(new StrModule);
  rlmachine.executeUntilHalted();

  ensure_equals("hantozen returned wrong value",
                rlmachine.getStringValue(0x12, 0),
                "\x82\x50\x82\x51\x82\x52\x82\x53\x82\x54");
}

// -----------------------------------------------------------------------

/** 
 * Test hantozen() on half width katakana; should convert them to full
 * width katakana.
 * 
 * @code
 * strS[0] = "ﾜﾀｼﾉﾅﾏｴ"
 * strS[0] = hantozen()
 * @endcode
 */
template<>
template<>
void object::test<19>()
{
  Reallive::Archive arc("test/seenFiles/hantozen_1.TXT");
  RLMachine rlmachine(arc);
  rlmachine.attatchModule(new StrModule);
  rlmachine.executeUntilHalted();

  ensure_equals("hantozen returned wrong value",
                rlmachine.getStringValue(0x12, 0),
                "\x83\x8F\x83\x5E\x83\x56\x83\x6D\x83\x69\x83\x7D\x83\x47");
}

// -----------------------------------------------------------------------

/** 
 * Tests zentohan on fullwidth ASCII characters.
 * 
 * @code
 * strS[0] = "１２３４５"
 * strS[0] = zentohan()
 * @endcode
 */
template<>
template<>
void object::test<20>()
{
  Reallive::Archive arc("test/seenFiles/zentohan_0.TXT");
  RLMachine rlmachine(arc);
  rlmachine.attatchModule(new StrModule);
  rlmachine.executeUntilHalted();

  ensure_equals("zentohan returned wrong value",
                rlmachine.getStringValue(0x12, 0),
                "12345");
}

// -----------------------------------------------------------------------

/** 
 * Tests zentohan on fullwidth katakana characters.
 * 
 * @code
 * strS[0] = "ワタシノナマエ"
 * strS[0] = zentohan()
 * @endcode
 */
template<>
template<>
void object::test<21>()
{
  Reallive::Archive arc("test/seenFiles/zentohan_1.TXT");
  RLMachine rlmachine(arc);
  rlmachine.attatchModule(new StrModule);
  rlmachine.executeUntilHalted();

  ensure_equals("zentohan returned wrong value",
                rlmachine.getStringValue(0x12, 0),
                "\xDC\xC0\xBC\xC9\xC5\xCF\xB4");
}

/** 
 * Test Uppercase_0
 * 
 * @code
 * strS[0] = "Valid"
 * // This syntax hurts my brain
 * strS[0] = Uppercase()
 * @endcode
 */
template<>
template<>
void object::test<22>()
{
  Reallive::Archive arc("test/seenFiles/uppercase_0.TXT");
  RLMachine rlmachine(arc);
  rlmachine.attatchModule(new StrModule);
  rlmachine.executeUntilHalted();

  ensure_equals("Uppercase returned wrong value",
                rlmachine.getStringValue(0x12, 0),
                "VALID"); 
}

// -----------------------------------------------------------------------

/** 
 * Test Uppercase_1
 * 
 * @code
 * strS[0] = "Valid"
 * // This syntax hurts my brain
 * strS[0] = Uppercase()
 * @endcode
 */
template<>
template<>
void object::test<23>()
{
  Reallive::Archive arc("test/seenFiles/uppercase_1.TXT");
  RLMachine rlmachine(arc);
  rlmachine.attatchModule(new StrModule);
  rlmachine.executeUntilHalted();

  ensure_equals("Uppercase touched strS[0]",
                rlmachine.getStringValue(0x12, 0),
                "Valid"); 
  ensure_equals("Uppercase returned wrong value",
                rlmachine.getStringValue(0x12, 1),
                "VALID"); 
}

// -----------------------------------------------------------------------

/** 
 * Test Lowercase_0
 * 
 * @code
 * strS[0] = "Valid"
 * // This syntax hurts my brain
 * strS[0] = Lowercase()
 * @endcode
 */
template<>
template<>
void object::test<24>()
{
  Reallive::Archive arc("test/seenFiles/lowercase_0.TXT");
  RLMachine rlmachine(arc);
  rlmachine.attatchModule(new StrModule);
  rlmachine.executeUntilHalted();

  ensure_equals("Lowercase returned wrong value",
                rlmachine.getStringValue(0x12, 0),
                "valid"); 
}

// -----------------------------------------------------------------------

/** 
 * Test Lowercase_1.
 * 
 * @code
 * strS[0] = "Valid"
 * // This syntax hurts my brain
 * strS[1] = Lowercase(strS[0])
 * @endcode
 */
template<>
template<>
void object::test<25>()
{
  Reallive::Archive arc("test/seenFiles/lowercase_1.TXT");
  RLMachine rlmachine(arc);
  rlmachine.attatchModule(new StrModule);
  rlmachine.executeUntilHalted();

  ensure_equals("Lowercase touched strS[0]",
                rlmachine.getStringValue(0x12, 0),
                "Valid"); 
  ensure_equals("Lowercase returned wrong value",
                rlmachine.getStringValue(0x12, 1),
                "valid"); 
}

}

