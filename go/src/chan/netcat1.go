package main

import (
    "io"
    "log"
    "net"
    "os"
	"flag"
)
var port = flag.Int("port", 8000, "server port")
func main() {
	server := "localhost:" + strconv.Itoa(*port)
    conn, err := net.Dial("tcp", server)
    if err != nil {
        log.Fatal(err)
    }
    defer conn.Close()
    mustCopy(os.Stdout, conn)
}

func mustCopy(dst io.Writer, src io.Reader) {
    if _, err := io.Copy(dst, src); err != nil {
        log.Fatal(err)
    }
}
