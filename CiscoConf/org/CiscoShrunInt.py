#!/usr/bin/python
# -*- coding: utf-8 -*-
##!/usr/bin/python
##!/usr/local/bin/python

import os
import sys
import re
from CTextFsm import CiscoShrun
from CTextFsm import CTextFsm

if __name__ == '__main__':
    basename = os.path.basename(sys.argv[0])
    dirname = os.path.dirname(sys.argv[0])

    for fname in sys.argv[1:]:
        parsedobj = CiscoShrun.parse_shrunint(fname)
        header = Csv_Header['rint']
        CTextFsm.csvprint(parsedobj, header)

