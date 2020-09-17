#!/usr/bin/env python

from distutils.core import setup, Extension

MOD = 'CustomPy'
setup(name = MOD, ext_modules = [Extension(MOD, sources = [PythonEx.c])])