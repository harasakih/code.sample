#!/bin/bash
#　行列状の標準入力の、行と列を入れ替える
#

awk '
{
  if (max_nf < NF) max_nf = NF;
  max_nr = NR;
  for (x = 1; x <= NF; x++) { vector[x, NR] = $x; }
}
END {
  for (x = 1; x <= max_nf; x++) {
    for (y = 1; y <= max_nr; y++) { printf("%s ", vector[x, y]) ; }
    printf("\n")
  }
}'

