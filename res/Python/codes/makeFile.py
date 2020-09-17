#!/usr/bin/env python

'makeFile.py -- create a file'

import os
ls = os.linesep

# get filename
while True:
	fname = raw_input('Input an unused file name >')
	if os.path.exists(fname):
		print "ERROR: '%s' already exists" %fname
	else:
		break

# get file content lines
all = []
print "\nEnter lines (input '.' to quit).\n"

# loop until user terminates input
while True:
	entry = raw_input('>')
	if entry == '.':
		break
	else:
		all.append(entry)

# write lines to file with proper line-ending
fobj = open(fname, 'w')
fobj.writelines(['%s%s' %(x, ls) for x in all])
fobj.close()
print 'DONE'

if __name__ == '__main__':
	print 'innter module'
