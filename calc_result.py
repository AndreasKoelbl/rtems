#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from operator import itemgetter

lines = [line.rstrip('\n') for line in open('result.txt')]
secs = []
nsecs = []
for i,line in zip(range(len(lines)), lines):
    result = line.split(':')
    secs.append(int(result[0]))
    nsecs.append(int(result[1]))

diff = []
for i in range(len(secs)-1):
    diff.append((secs[i+1]-secs[i]) * 1000000000 + nsecs[i+1]-nsecs[i])
    print(diff[i])
print('min: {0} max: {1} avg: {2}'.format(min(diff), max(diff),
sum(diff)/len(diff)))
