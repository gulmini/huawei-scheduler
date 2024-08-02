#!/bin/env python3

import sys
import os
import subprocess
import time

if __name__ == '__main__':
    if not 'BENCH_OUTPUT_FOLDER' in os.environ:
        print("Missing benchmark folder!", file=sys.stderr)
        sys.exit(1)
    
    if len(sys.argv) != 4:
        print("Usage: benchmark.py <scheduler> <start> <end>", file=sys.stderr)
        sys.exit(1)
    
    sched = os.path.dirname(__file__) + "/../schedulers/" + sys.argv[1] + "/sched"
    start = int(sys.argv[2])
    end = int(sys.argv[3])

    if not os.path.isfile(sched):
        print("Invalid scheduler!", file=sys.stderr)
        sys.exit(1)

    times = []
    for i in range(start, end + 1):
        tc = "{:03d}".format(i)
        t = time.time()
        
        tt = "Timed out"
        try:
            subprocess.run([ sched, tc ], cwd=os.path.dirname(__file__) + "/../", timeout=10, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
            t2 = time.time()

            tt = int((t2 - t) * 1000)
            times.append(tt)
        except:
            times.append(-1)

        print(f"Testcase {tc}: {tt}")
    
    with open(os.path.dirname(__file__) + "/../" + os.environ['BENCH_OUTPUT_FOLDER'] + "/" + sys.argv[1] + ".bench", "a") as f:
        for k in range(len(times)):
            f.write("{:03d}".format(k + start) + "," + str(times[k]) + "\n")