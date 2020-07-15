package main 

import (
	"io/ioutil"
	"log"
	"fmt"
	"strconv"
	_"bufio"
	_"bytes"
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
			
		}
	}

}

