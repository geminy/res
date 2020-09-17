#!/usr/bin/env python

from warnings import warn

class ReqStrSugRepr(type):
	def __init__(cls, name, bases, attrd):
		print '*** Defined ReqStrSugRepr (meta)class\n'

		super(ReqStrSugRepr, cls).__init__(name, bases, attrd)

		if '__str__' not in attrd:
			aerror = 'Class %r requires overriding of __str__()' %name
			raise TypeError(aerror)

		if '__repr__' not in attrd:
			awarn = 'Class %r suggests overriding of __repr__()\n' %name
			warn(awarn, stacklevel = 3)

		print '*** Created %r class\n' %name


class Foo(object):
	__metaclass__ = ReqStrSugRepr

	def __str__(self):
		return 'Instance of class:', self.__class__.__name__

	def __repr__(self):
		return self.__class__.__name__


class Bar(object):
	__metaclass__ = ReqStrSugRepr

	def __str__(self):
		return 'Instance of class:', self.__class__,__name__


class FooBar(object):
	__metaclass__ = ReqStrSugRepr