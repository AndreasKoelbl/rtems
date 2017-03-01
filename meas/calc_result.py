#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# Je länger NUM_TICKS desto länger jitter, skaliert linear
import sys

def main():
    try:
      file = ""
      if len(sys.argv) > 1:
        file = sys.argv[1]
      else:
        file = "result.txt"

      lines = [line.rstrip('\n') for line in open(file)]
    except IOError as ioe:
        sys.stderr.write("Caught IOError: {}".format(repr(ioe)))
        sys.exit(1)

    secs = []
    nsecs = []
    for i,line in zip(range(len(lines)), lines):
        result = line.split(':')
        secs.append(int(result[0].strip()))
        nsecs.append(int(result[1].strip()))

    diff = []
    for i in range(len(secs)-1):
        diff.append((secs[i+1]-secs[i]) * 1000000000 + nsecs[i+1]-nsecs[i])
        print(diff[i])
    print('min: {0} max: {1} avg: {2}'.format(min(diff), max(diff),
    sum(diff)/len(diff)))

if __name__ == '__main__':
    main()
