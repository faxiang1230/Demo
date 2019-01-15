import time
import re
import matplotlib.pyplot as plt
import datetime
import numpy as np

#-*-coding=utf-8-*-
timestamp = []
procs = []
result_cpu = {}
result_mem = {}

f = open('/home/linux/tmp/RunningInfo', 'r')
line = f.readline().strip('\n')
find = 0
while line:
    if re.match(r'[0-9]*-[0-9]*[0-9]',line, re.M|re.I):
        timetmp = time.strptime(line, '%Y-%m-%d %H:%M:%S')
        timestamp.append(timetmp)
    else:
        comm = line.split()
        #print comm
        for proc in procs:
            for key in proc:
                if comm[0] == key:
                    #print "find " + comm[0]
                    find = 1
                    proc[key].append({timetmp:[comm[1],comm[3]]})
        if not find:
            #print "not find " + comm[0]
            procs.append({comm[0]:[{timetmp:[comm[1],comm[3]]}]})
        find = 0
        #print procs
    line = f.readline().strip('\n')
#print procs
for timetmp in timestamp:
    info = []
    find = 0;
    for proc in procs:
        for pid in proc:
            for element in proc[pid]:
                for t in element:
                    if t == timetmp:
                        info = element[t]
                        #print info
                if not info:
                    info = [0, 0]
            #print info, pid
            for key in result_cpu:
                if key == pid:
                    find = 1
                    #print "cpu find ",info, pid
                    result_cpu[pid].append(info[0])
            if not find:
                result_cpu.update({pid:[info[0]]})
            find = 0
            for res_pid in result_mem:
                if res_pid == pid:
                    #print "mem find ",info, pid
                    find = 1
                    result_mem[res_pid].append(info[1])
            if not find:
                result_mem.update({pid:[info[1]]})
            find = 0

#print result_cpu
#print result_mem

plt.figure(figsize=(256,4))
localdate = []
for timetmp in timestamp:
    #print timetmp
    d = datetime.datetime(*timetmp[0:6])
    localdate.append(d)

plt.xlabel("TIME")
plt.ylabel("Kbytes")
plt.title("ZSAGENT PSS memory")
plt.yticks(np.arange(0, 20*1024, 1024))
for pid in result_mem:
    plt.plot_date(localdate, result_mem[pid],label=pid, linewidth=1)

plt.legend()
plt.show()
#plt.savefig("zsagent.svg", format="svg")