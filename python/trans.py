#!/usr/bin/env python
# -*- coding: UTF-8 -*-
import sys
import csv
import re

tmp = []
marks = []
csv_file = csv.reader(open(sys.argv[1]))

for col in csv_file:
    if len(col):
        match = re.match(r'[A-G][00-12]',col[0])
        if match:
            if col[0] in marks:
                #print('%s exist' %col[0])
            else:#find main key
                marks.append(col[0])
                if len(tmp) == 0:#first insert comment
                    tmp.append([col[2], col[3]])
            #print('string %s' %(col))
            find = 0
            for c in tmp:
                if c[1]==col[3]:
                    #print('1string %s %s' %(col[3], c[1]))
                    tmp[tmp.index(c)] += [col[4]]
                    find = 1
                    break
            if find is not 1:
                #print('2string %s %s' %(col[3], c[1]))
                tmp.append([col[2], col[3], col[4]])

#print('key:%s\n com:%s' %(marks, tmp))

#print(tmp)
'''
path = sys.argv[1].split('/')
path[len(path) -1]='output-'+ path[len(path) -1]
outFilePath=''

for i in path:
    outFilePath=outFilePath + '/'+ i
'''
outFilePath = 'output-' + sys.argv[1]

print(outFilePath)
out = open(outFilePath,'wb')
csv_write = csv.writer(out,dialect='excel')
output=['first', 'second'] + marks
csv_write.writerow(output)
print(output);
for i in tmp:
    csv_write.writerow(i)
