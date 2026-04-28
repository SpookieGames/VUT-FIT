"""! @file generate.py 
@brief Profiling tool for generating input data files for profiling.
@author Kristian Duzek 
"""

import random
import sys


random.seed(42)


n = int(sys.argv[1])

for _ in range(n):
    print(random.randint(0,100), end=" ")