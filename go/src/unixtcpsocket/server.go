package main

import (
	_ "errors"
	"fmt"
	"io"
	"log"
	"net"
	_ "os"
	_ "strconv"
	_ "syscall"
	_ "time"
)

const sockPath = "@docsec-agent"

func recvUnixMsg(c net.Conn) {
	var sum int64 = 0
	buf := make([]byte, 1024)
	defer c.Close()
	for {
		if _, err := c.Read(buf); err != nil && err != io.EOF {
			fmt.Println("recv error ", err)
			break;
		} else {
			println("Server recv ", string(buf))
			sum++
			replystr := fmt.Sprintf("%d %s", sum, string(buf))
			reply := []byte(replystr)
			count, _ := c.Write(reply)
			println("Server reply ", count, reply)
		}
	}
}

func main() {

	l, err := net.Listen("unix", sockPath)
	if err != nil {
		log.Fatal("listen error:", err)
	}
	defer l.Close()
	for {
		// Accept new connections, dispatching them to echoServer
		// in a goroutine.
		conn, err := l.Accept()
		if err != nil {
			log.Fatal("accept error:", err)
		}

		go recvUnixMsg(conn)
	}
}
