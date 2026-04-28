"""! @file profiling.py
@brief Profiling tool calculating deviation using the math library
@author Kristian Duzek 
"""

from mathlib import *
import sys
import cProfile

def deviation():
    sum_x = 0
    sum_x2 = 0 
    avg = 0
    N = 0

    for line in sys.stdin:
        for num in line.split():
            x = float(num)
            sum_x = add(sum_x, x)
            sum_x2 = add(sum_x2, pow(x,2))
            N += 1

    avg = div(sum_x, N)
    s = 0
    s = sub(sum_x2, mul(N, pow(avg, 2)))
    N -= 1
    s = div(s, N)
    s = sqt(s, 2)

    print(s)

if __name__=="__main__":
    profiler = cProfile.Profile()

    profiler.enable()
    deviation()
    profiler.disable()

    profiler.dump_stats("stats.prof")
