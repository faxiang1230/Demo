package main

import (
	_ "bufio"
	"bytes"
	"errors"
	"fmt"
	"io/ioutil"
	"log"
	"strconv"
	"time"
)

type procInfo struct {
	pid, ppid, pgrp, sid, tty_nr, tty_pgrp      int64
	flags, min_flt, cmin_flt, maj_flt, cmaj_flt int64
	utime, stime, cutime, cstime                int64
	comm, state                                 string
	
	
	delta                                       int64
	scanned                                     bool
}
func scanProcess() ([]procInfo, error) {
	procs := make([]procInfo, 1000)
	if pidFiles, err := ioutil.ReadDir("/proc"); err != nil {
		log.Print("Can't open /proc/ dir:", err)
		return nil, errors.New("Failed to open /proc")
	} else {
		for _, pidFile := range pidFiles {
			if pid, _ := strconv.Atoi(pidFile.Name()); pid <= 0 || pidFile.IsDir() != true {
				continue
			}
			pidStats := "/proc/" + pidFile.Name() + "/stat"
			//fmt.Print(pidStats)
			comment, err := ioutil.ReadFile(pidStats)
			if err != nil {
				continue
			}
			proc := new(procInfo)
			r := bytes.NewReader(comment)
			scanCount, err := fmt.Fscanf(r,
				"%d %s %s %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
				&proc.pid, &proc.comm, &proc.state, &proc.ppid, &proc.pgrp,
				&proc.sid, &proc.tty_nr, &proc.tty_pgrp, &proc.flags,
				&proc.min_flt, &proc.cmin_flt, &proc.maj_flt, &proc.cmaj_flt,
				&proc.utime, &proc.stime, &proc.cutime, &proc.cstime)
			if err != nil {
				fmt.Print(proc, scanCount, err, "\n")
			}
			proc.delta = 0
			procs = append(procs, *proc)
		}
	}
	return procs, nil
}
func sortProcs(procs map[int64]procInfo)([]procInfo) {
	procArray := make([]procInfo, len(procs))
	maxLen := 0
	for _,proc := range procs {
		//顺序插入数组，从高到低
		var index int
		for index = 0; index < maxLen; index++ {
			if (proc.delta <= procArray[index].delta) {
				continue
			} else {
				for movIndex := maxLen; movIndex >index; movIndex-- {
					procArray[movIndex] = procArray[movIndex - 1]
				}
				procArray[index] = proc
				break;
			}
		}

		if index == maxLen {
			procArray[index] = proc
		}
		maxLen++
	}
	return procArray
}
func main() {
	procs := make(map[int64]procInfo)
	for {
		for _, proc := range procs {
			proc.scanned = false
		}
		if currentProcs, err := scanProcess(); err != nil {
			log.Fatal(err)
		} else {
			for _, proc := range currentProcs {
				tmp, exist := procs[proc.pid]
				proc.delta = 0
				proc.scanned = true
				if exist != true {
					//新创建的进程
					procs[proc.pid] = proc
				} else {
					//距离上次扫描仍然存在的进程
					if proc.comm == tmp.comm {
						proc.delta = proc.utime + proc.stime + proc.cutime + proc.cstime -
						(tmp.utime + tmp.stime + tmp.cutime + tmp.cstime)
						procs[proc.pid] = proc
					} else {
						//距离上次扫描变更的进程
						proc.delta = 0
						procs[proc.pid] = proc
					}
				}
			}
		}
		//排序数组，并打印前10个进程
		procArray := sortProcs(procs)
		for index:= 0; index < 5; index ++ {
			log.Println(procArray[index].comm, procArray[index].delta)
		}
		log.Println("===========================")
		time.Sleep(5*time.Second)
	}

}
