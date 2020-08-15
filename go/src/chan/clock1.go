package main

import (
	"io"
	"log"
	"net"
	"time"
	"flag"
	"os"
	"strconv"
)
var port = flag.Int("port", 8000, "server listen port")

func main() {
	var location *time.Location
	var err error
	flag.Parse()
	timezone := os.Getenv("TZ")
	if timezone == "" {timezone = "Local"}
	if location, err = time.LoadLocation(timezone); err != nil {
		log.Fatal(err)
	}

	server := "localhost:" + strconv.Itoa(*port)
	listener, err := net.Listen("tcp", server)
	if err != nil {
		log.Fatal(err)
	}

	for {
		conn, err := listener.Accept()
		if err != nil {
			log.Print(err) // e.g., connection aborted
			continue
		}
		go handleConn(conn, location) // handle one connection at a time
	}
}
func handleConn(c net.Conn, location *time.Location) {
	defer c.Close()
	for {
		t := time.Now()
		_, err := io.WriteString(c, t.In(location).Format("15:04:05\n"))
		if err != nil {
			return // e.g., client disconnected
		}
		time.Sleep(1 * time.Second)
	}
}
