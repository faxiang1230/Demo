#!/usr/bin/python
# -*- coding: UTF-8 -*-
import xlrd
import sys
import csv

mark = ['A01', 'A02', 'A03', 'A04', 'A05', 'A06', 'A07', 'A08', 'A09']
tmp = []
outpu = ['first', 'second', 'A01', 'A02', 'A03', 'A04', 'A05', 'A06', 'A07']
csv_file = csv.reader(open(sys.argv[1]))

for col in csv_file:
    for substring in mark:
        if len(col) > 0 and col[0] == substring:
            if col[0] == 'A01':
                tmp.append([col[2],col[3], col[4]])
            else:
                for c in tmp:
                    #print(c[1], col[3])
                    if c[1]==col[3]:
                        tmp[tmp.index(c)] += [col[4]]
#print(tmp)
path = sys.argv[1].split('/')
path[len(path) -1]='output-'+ path[len(path) -1]
outFilePath=''

for i in path:
    outFilePath=outFilePath + '/'+ i

print(outFilePath)
out = open(outFilePath,'wb')
csv_write = csv.writer(out,dialect='excel')
csv_write.writerow(outpu)
print(outpu);
for i in tmp:
    csv_write.writerow(i)