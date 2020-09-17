#!/usr/bin/env python

def counter(start_at):
	count = start_at
	print "count = start_at", count
	while True:
		val = (yield count)
		print "val = (yield count)", val
		if val is not None:
			count = val
			print 'count = val', count
		else:
			count += 1
			print 'count += 1', count