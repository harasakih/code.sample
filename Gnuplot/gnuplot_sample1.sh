#!/bin/bash
#

gnuplot << _EOT_
#set terminal svg ; set output "sample1.svg"
#set terminal pdf ; set output "sample1.pdf"
#set terminal canvas; set output "sample1.html"
set terminal png; set output "sample1.png"
#set terminal aqua
# set xrange[1:1e8]
# set yrange[0.01:1000]
# set logscale x


plot sin(x) with line, cos(x) with line
_EOT_

