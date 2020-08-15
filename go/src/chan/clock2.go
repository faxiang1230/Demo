package main

import (
	"io"
	"log"
	"net"
	"time"
	"os"
	"strings"
)
type serverInfo struct {
	Location *time.Location
	Conn string
}
func main() {
	var servers = []serverInfo{}
	var err error

	if len(os.Args) <= 1 {
		log.Fatalf("Expect %s NewYork=localhost:8010", os.Args[0])
	}
	for index, arg := range os.Args {
		if index == 0 { continue}
		s := strings.Split(arg, "=")
		if len(s) <= 1 {
			log.Fatalf("Unexpect arg:%s", s)
		}
		var server serverInfo
		server.Location, err = time.LoadLocation(s[0]);
		if err != nil {
			log.Fatalf("Unexpect arg:%s", s)
		}
		server.Conn = s[1]
		servers = append(servers, server)
	}

	for _, server := range servers {
		go func(l *time.Location) {
			listener, err := net.Listen("tcp", server.Conn)
			if err != nil {
				log.Fatal(err)
			}

			for {
				conn, err := listener.Accept()
				if err != nil {
					log.Print(err) // e.g., connection aborted
					continue
				}
				go handleConn(conn, l) // handle one connection at a time
			}

		}(server.Location)
	}
	stop := make(chan bool)
	<-stop
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
