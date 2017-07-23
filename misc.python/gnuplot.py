#!/usr/bin/python
# -*- coding: utf-8 -*-

import	os
import	sys
import	Gnuplot, Gnuplot.funcutils

gp	=	Gnuplot.Gnuplot( debug=1 )
gp.plot( 'sin(x)' )
gp.hardcopy(filename='./tmp.png', terminal='png')
raw_input('Hit any key')

