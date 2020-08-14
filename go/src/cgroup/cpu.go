package main

import (
	"log"
	"os"
	_"encoding/binary"
	_"io"
	"io/ioutil"
	"time"
)
func main() {
	const cpuGroupDir = "/sys/fs/cgroup/cpu,cpuacct/zshield"
	err := os.MkdirAll(cpuGroupDir, 0755)
	if err != nil {
		log.Printf("can't limit self's cpu utilization", err)
		return
	}
	cpuPeriodFile := cpuGroupDir + "/cpu.cfs_period_us"

	err = ioutil.WriteFile(cpuPeriodFile, []byte("5000"), 0644)
	if err != nil { log.Printf("failed to set cpu limit", err)}

	<-time.After(5 * time.Minute)
}
