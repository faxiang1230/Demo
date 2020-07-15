package main

import (
	"bufio"
	"fmt"
	"log"
	"net"
	"os"
	"time"
)
var count = 1
func client() {
	for {
		time.Sleep(1 * time.Second)
		conn, err := net.Dial("udp", ":8080")
		if err != nil {
			log.Fatal("[Client] Dial error", err)
			continue
		}
		fmt.Fprintf(conn, "GET / HTTP/1.0\r\n\r\n")
		res, err := bufio.NewReader(conn).ReadString('\n')
		log.Print("[Client] recv:", res)
		if err != nil {
			log.Fatal("[Client] recv error:", err)
		}
		conn.Close()
	}
}
func handleConnection(c *net.UDPConn, addr *net.UDPAddr, buf []byte) {
	reply := fmt.Sprintf("%d %s", count, string(buf))
	c.WriteToUDP([]byte(reply), addr)
	log.Print("[Server] reply:", reply)
	count++
}
func main() {
	go client()
	var laddr *net.UDPAddr
	var err error
	if laddr, err = net.ResolveUDPAddr("udp", ":8080"); err != nil {
		log.Fatal("[Server] address fatal", err)
		return
	}
	ln, err := net.ListenUDP("udp", laddr)
	if err != nil {
		log.Fatal("Can't Listen tcp:8080")
		os.Exit(-1)
	}
	defer ln.Close()
	for {
		buf := make([]byte, 1024)
		n, raddr, err := ln.ReadFromUDP(buf)
		if err != nil {
			//log.Fatal("Read error:", err)
			continue
		}
		go handleConnection(ln, raddr, buf[:n])
	}
}
