package main

import (
	"log"
	"net"
	"os"
	"io"
	"bufio"
	"encoding/binary"
)

type msg_header struct {
	Type int32
	Len  int32
}

const sockPath = "/var/run/sensor_logsend.sock"
/*
func dispatch(buf []byte) {
	Type int32;
 72     int len;
}
*/
func logReceiver(conn io.ReadWriter) error{
	r := bufio.NewReader(conn)
	msghdr := new(msg_header)
	//msgHdrBytes = make([]byte, unsafe.Sizeof(msg_header))
	
	if err := binary.Read(r, binary.LittleEndian, msghdr); err != nil && err != io.EOF {
		log.Print("logReceiver hdr error:", err, msghdr)
	} else {
		log.Print("logRecev type:", msghdr.Type, msghdr.Len)
		//buf := bytes.NewBuffer(msgHdrBytes)
		//binary.Read(buf, binary.LittleEndian, &msghdr)
	}
	if msghdr.Len == 0 {return nil}
	buf := make([]byte, msghdr.Len)
	if recvlen, err := io.ReadFull(r, buf); err != nil && err != io.EOF {
		log.Print("logReceiver error:", recvlen, err, string(buf))
		//return err
	} else {
		log.Print("recv:", string(buf))
		//dispatch(line)
	}
	return nil
}

func Server(done chan bool) {
	os.RemoveAll(sockPath)

	l, err := net.Listen("unix", sockPath)
	if err != nil {
		log.Fatal("listen error:", err)
	}

	defer l.Close()
	for {
		conn, err := l.Accept()
		if err != nil {
			log.Fatal("accept error:", err)
		}

		go logReceiver(conn)
	}

	done <- true
}
