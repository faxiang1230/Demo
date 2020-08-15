package main

import (
	"log"
	"flag"
	"net"
	"strconv"
	"strings"
	"bufio"
)

func handleConnection(conn net.Conn) {
	conn.Write([]byte("200 Ready.\r\n"))
	s := bufio.NewScanner(conn)

	var cmd, request string
	var args []string

	for s.Scan() {
		request = s.Text()
		args = strings.Fields(request)
		if len(args) < 1 {continue}

		cmd = strings.ToUpper(args[0])
		log.Println("cmd:", cmd);
		switch cmd {
			case "LIST":
			case "QUIT":
			case "USER":
				conn.Write([]byte("230 Login successful.\r\n"))
			default:
				log.Println("unknown cmd:", cmd)
		}
	}
}
func main() {
	port := flag.Int("port", 8000, "ftpd server port")
	flag.Parse()

	serverInfo := ":" + strconv.Itoa(*port)
	ln, err := net.Listen("tcp", serverInfo)
	if err != nil {
		log.Fatal("failed to listen server:", err)
	}
	for {
		conn, err := ln.Accept()
		if err != nil {
			log.Print("failed to accept client:", err)
			// handle error
			continue
		}
		go handleConnection(conn)
	}
}
