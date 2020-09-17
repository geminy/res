#!/usr/bin/env python
# Joseph Problem
def joseph(total, begins, count):
	queue = range(1, total + 1)
	death = (begins + count - 2) % len(queue)
	for times in range(total - 1):
		print 'out: ', queue[death]
		del queue[death]
		death = (death + count -1) % len(queue)
	print 'survivor: ', queue[0]
