package main

import (
	"errors"
	"golang.org/x/sys/unix"
	"log"
	_"syscall"
)

func netlink_listener(protocol int) (int, error) {
	if fd, err := unix.Socket(unix.AF_NETLINK, unix.SOCK_RAW|unix.SOCK_CLOEXEC, protocol); err != nil {
		log.Print("Create netlink socket failed:", err)
		return -1, err
	} else {
		sal := &unix.SockaddrNetlink{Family: unix.AF_NETLINK}
		sal.Pad = 0
		sal.Pid = uint32(unix.Getpid())
		sal.Groups = 1
		if err := unix.Bind(fd, sal); err != nil {
			log.Print("Bind netlink socket failed:", err)
			unix.Close(fd)
			return -1, err
		}

		return fd, nil
	}

	return -1, errors.New("Unexpected code branch")
}

const RECEIVE_BUFFER_SIZE = 65536

func main() {
	if fd, err := netlink_listener(unix.NETLINK_KOBJECT_UEVENT); err != nil {
		log.Print("Failed to create netlink listener")
		return
	} else {
		defer unix.Close(fd)
		for {
			//var fromAddr *unix.SockaddrNetlink
			var rb [RECEIVE_BUFFER_SIZE]byte
			nr, _, err := unix.Recvfrom(fd, rb[:], 0)
			if err != nil {
				log.Print("Recv from netlink error:", err)
			} else {
				/*
				rb2 := make([]byte, nr)
				copy(rb2, rb[:nr])
				log.Print("monitor kobject uevent:", string(rb2))
				*/
				log.Print("monitor kobject uevent:", string(rb[:nr]))
			}
		}
	}
}
