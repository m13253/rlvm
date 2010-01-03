#!/usr/bin/python
#
# A fairly naive script which takes a JSON file generated from dumphik.py and
# tries to generate a valid HIK file.

import json
import struct
import sys

# TODO(erg): THIS HEADER ISN'T THE SAME IN ALL HIK FILES. IT WAS CRIBBED FROM
# BG26o.HIK in PLANETARIAN WHICH MEANS THIS SCRIPT PROBABLY ONLY WORKS WITH
# THAT FILE.
HIK_HEADER = (
  "10270000102700007427000001000000"
  "75270000000000007627000000000000"
  "772700002003000058020000204e0000").decode('hex')

def write_property(output, property):
  for x in property:
    if isinstance(x, int):
      output.write(struct.pack("i", x))
    elif isinstance(x, str) or isinstance(x, unicode):
      output.write(struct.pack("i", len(x)))
      output.write(x)
    else:
      print type(x)

if len(sys.argv) != 3:
  print "Usage: " + sys.argv[0] + " <input json file> <output hik>"
  exit(-1)

with open(sys.argv[1], "rb") as input:
  data = json.load(input)
  with open(sys.argv[2], "wb") as output:
    output.write(HIK_HEADER)

    output.write(struct.pack("i", len(data)))
    for record in data:
      for entry in record:
        property = int(entry[0])
        output.write(struct.pack("i", property))
        write_property(output, entry[1])

      # Write the terminating -1
      output.write(struct.pack("i", -1))

      # Emit the weird 100. TODO(erg): Only bg26o.hik has 100 as the
      # terminating value here. Investigate more.
      output.write(struct.pack("i", 100))
