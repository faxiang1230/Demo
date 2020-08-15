package main

import (
	"conent"
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

const sensor_unix_path = "/tmp/fileparse.sock"

func listen_sensor(conn net.Conn) {

	msgHead := make([]byte, unsafe.Sizeof(Message))

	if n, err := r.Read(msgHead[:]); err != nil || n != len(msgHead) {
		log.Print("Failed to read msgHead")
		return
	}

	request := *(*content.Message)(unsafe.Pointer(&msgHead[0]))
	fmt.Println(request)
	if request.magic != MSG_HEAD_MAGIC || request.msg_type != MSG_TYPE_POLICY {
		fmt.Println("Error msg magic or msgtype")
		return
	}
	var policyArray []Policy
	for i := 0; i < request.size; i++ {
		msg := make([]byte, unsafe.Sizeof(Policy))
		if n, err := r.Read(msg[:]); err != nil || n != len(msg) {
			log.Print("Failed to read msg")
			break
		}
		policy := *(*content.Policy)(unsafe.Pointer(&msg[0]))
		policyArray = append(policyArray, policy)
	}
	fmt.Println(policyArray)

	resArray[] := match
	
}
func unixclient(serverAddr string) {
	c, err := net.Dial("unix", serverAddr)
	if err != nil {
		log.Fatal(err)
	}
	defer c.Close()

	_, err = c.Write([]byte("hi"))
	if err != nil {
		log.Fatal("write error:", err)
	}
	for {
		listen_sensor(c)
		time.Sleep(time.Second)
	}
}
func main() {
	unixClient(sensor_unix_path)
}
