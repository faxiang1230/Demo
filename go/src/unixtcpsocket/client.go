package main

import (
	_ "errors"
	_ "fmt"
	"io"
	"log"
	"net"
	_ "os"
	_ "strconv"
	_ "syscall"
	"time"
)
const sockPath = "@docsec-agent"
func reader(r io.Reader) {
	buf := make([]byte, 1024)
	n, err := r.Read(buf[:])
	if err != nil {
		return
	}
	println("Client got:", n, string(buf[0:n]))
}

func unixclient(serverAddr string) {
	c, err := net.Dial("unix", serverAddr)
	if err != nil {
		log.Fatal(err)
	}
	defer c.Close()

	_, err = c.Write([]byte("hello server"))
	if err != nil {
		log.Fatal("write error:", err)
	}
	for {
		reader(c)
		time.Sleep(time.Second)
	}
}
func main() {
	unixclient(sockPath);
}
