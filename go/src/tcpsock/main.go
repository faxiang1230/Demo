package main

import (
	"log"
	"net"
	"bufio"
	"time"
	"fmt"
	"os"
)

func client() {
	time.Sleep(time.Second)
	conn, err := net.Dial("tcp", ":8080")
	defer conn.Close()
	if err != nil {
		log.Fatal("[Client] Dial error", err)
		return
	}
	fmt.Fprintf(conn, "GET / HTTP/1.0\r\n\r\n")
	res, err := bufio.NewReader(conn).ReadString('\n')
	log.Print("[Client] recv:", res, err)
}
func handleConnection(c net.Conn) {
	buf := make([]byte, 1024)
	if count, err := c.Read(buf); err != nil {
		log.Fatal("[Server] recv error", err)
		return
	} else {
		reply := fmt.Sprintf("%s %d\n", string(buf), count)
		_, err := c.Write([]byte(reply))
		log.Print("[Server] reply:", reply, err)
		c.Close()
	}
}
func main() {
	go client()
	
	ln, err := net.Listen("tcp", ":8080")
	if err != nil {
		log.Fatal("Can't Listen tcp:8080")
	}
	for {
		conn, err := ln.Accept()
		if err != nil {
			// handle error
			continue
		}
		go handleConnection(conn)
	}
}
