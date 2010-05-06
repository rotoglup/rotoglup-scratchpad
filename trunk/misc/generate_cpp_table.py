import os
import sys

filename = sys.argv[1] 
with open(filename, "r") as f:
  lines = f.readlines()

names = lines[0].strip().split("\t")
types = lines[1].strip().split("\t")
lines = lines[2:]

#

print "/* Generated from '%s' */" % os.path.split(filename)[1]

# data structures

data_type = names[0]

print ""
print "struct %s" % data_type
print "{"
print "  %s %s;" % (types[0], "key")
for entry in zip(types[1:], names[1:]):
  print "  %s %s;" % entry
print "};"

# search function prototype

print ""
print "%s const* find_%s(%s index);" % (data_type, data_type, types[0])

# entries tables

table_name = '%s_table' % data_type

print ""
print "int const %s_len = %d;" % (table_name, len(lines))
print "%s const %s[%s_len] = {" % (data_type, table_name, table_name)
table_lines = []
for line in lines:
  values = line.strip().split("\t")
  print "  { " + (",".join(values)) + " },"
  table_lines.append(line)
print "};"

# search function prototype

print ""
print "%s const* find_%s(%s key)" % (data_type, data_type, types[0])
print "{"
print "  switch (key)"
print "  {"
for index, line in enumerate(table_lines):
  values = line.strip().split("\t")
  print "  case %s: return &%s[%d];" % (values[0], table_name, index)
print "  }"
print "  return NULL;"
print "};"
