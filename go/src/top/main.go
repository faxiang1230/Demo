package main 

import (
	"io/ioutil"
	"log"
	"fmt"
	"strconv"
	_"bufio"
	"bytes"
)

func main() {
	//pidStatFile := make([]byte, 64)
	
	if pidFiles, err := ioutil.ReadDir("/proc"); err != nil {
		log.Fatal("Can't open /proc/ dir:", err)
	} else {
		for _, pidFile := range pidFiles {
			if pid,_ := strconv.Atoi(pidFile.Name());pid <= 0 || pidFile.IsDir() != true {
				continue
			}
			pidStats := "/proc/"+pidFile.Name()+"/stat"
			//fmt.Print(pidStats)
			comment,err := ioutil.ReadFile(pidStats);
			if err ! = nil {
				continue
			}
			var pid, ppid, pgrp, sid, tty_nr, tty_pgrp, flags, min_flt, cmin_flt, maj_flt, cmaj_flt, utime, stime, cutime, cstime int64
			var comm, state string
			r := bytes.NewReader(comment)
			scanCount, err := fmt.Fscanf(r, 
				"%d %s %s %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
				&pid, &comm, &state, &ppid, &pgrp, &sid, &tty_nr, &tty_pgrp, &flags, &min_flt, &cmin_flt, &maj_flt, &cmaj_flt,
				&utime, &stime, &cutime, &cstime)
			if err != nil {
				fmt.Print(string(comment), scanCount, err, "\n")	
			}
			

		}
	}

}

