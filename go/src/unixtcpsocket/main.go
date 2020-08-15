package main

import (
	_ "errors"
	"fmt"
	"io"
	"log"
	"net"
	"os"
	_ "strconv"
	_ "syscall"
	"time"
)

func checkError(err error) {
	if err != nil {
		fmt.Printf("Error: %s\n", err.Error())
		os.Exit(1)
	}
}

const sockPath = "/tmp/unix_gram_sock"

func recvUnixMsg(c net.Conn) {
	var sum int64 = 0
	buf := make([]byte, 1024)
	fmt.Println("recvUnixMsg enter")
	for {
		if _, err := c.Read(buf); err != nil && err != io.EOF {
			fmt.Println("recv error ", err)
			return
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
func reader(r io.Reader) {
	buf := make([]byte, 1024)
	n, err := r.Read(buf[:])
	if err != nil {
		return
	}
	println("Client got:", n, string(buf[0:n]))
}

func unixclient(serverAddr string) {
	c, err := net.Dial("unixpacket", serverAddr)
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
	os.RemoveAll(sockPath)
	//laddr, err := net.ResolveUnixAddr("unix", sockPath)
	//checkError(err)

	l, err := net.Listen("unix", sockPath)
	if err != nil {
		log.Fatal("listen error:", err)
	}
	go unixclient(sockPath)
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
