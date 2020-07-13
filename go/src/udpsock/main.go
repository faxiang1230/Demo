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
	conn, err := net.Dial("udp", ":8080")
	if err != nil {
		log.Fatal("[Client] Dial error", err)
		return
	}
	fmt.Fprintf(conn, "GET / HTTP/1.0\r\n\r\n")
	res, err := bufio.NewReader(conn).ReadString('\n')
	log.Fatal("[Client] recv:", res, err)
}
func handleConnection(c net.PacketConn, addr net.Addr, buf []byte) {

		reply := fmt.Sprintf("%s %d\n", string(buf), count)
		_, err := c.Write([]byte(reply))
		log.Print("[Server] reply:", reply, err)
		c.Close()
}
func main() {
	go client()
	
	ln, err := net.Listen("udp", ":8080")
	if err != nil {
		log.Fatal("Can't Listen tcp:8080")
		os.Exit(-1)
	}
	defer ln.Close()
	for {
		buf := make([]byte, 1024)
		n, addr, err := ln.ReadFrom(buf)
		if err != nil {
			continue
		}
		go handleConnection(ln, addr, buf[:n])
	}
}
