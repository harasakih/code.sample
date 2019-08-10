#!/usr/bin/python2.6
# -*- coding: utf-8 -*-

import	os
import	sys
import	Gnuplot, Gnuplot.funcutils

gp	=	Gnuplot.Gnuplot( debug=1 )
# terminalをNullにする
# 
gp('set terminal png')
gp('set output "./tmp.png"')
gp.plot( 'sin(x)' )
