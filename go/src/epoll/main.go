package main

import (
	"log"
	"syscall"
	"errors"
	"net"
)
//unfinished
type epollInterface interface {
	callback(interface{}) error
}
type epollEvent struct {
	param interface{}
	fun   interface{}
}
type epoll struct {
	efd       int
	callbacks map[int]epollEvent
}

func NewEpoll() *epoll {
	fd, err := syscall.EpollCreate1(syscall.EPOLL_CLOEXEC)
	if err != nil {
		log.Fatal(err)
	}
	return &epoll{efd: fd}
}

//func EpollCtl(epfd int, op int, fd int, event *EpollEvent) (err error)
//func EpollWait(epfd int, events []EpollEvent, msec int) (n int, err error)
func (e *epoll) AddWatcher(fd int, events uint32, cb interface{}, p interface{}) error {
	_, ok := e.callbacks[fd]
	if !ok {
		e.callbacks[fd] = epollEvent{fun: cb, param: p}
	} else {
		return errors.New("epoll duplicated watch fd")
	}

	err := syscall.EpollCtl(e.efd, syscall.EPOLL_CTL_ADD, fd, &syscall.EpollEvent{Events: events, Fd: int32(fd)})
	if err != nil {
		delete(e.callbacks, fd)
		return err
	}
	return err
}
func (e *epoll) DelWatcher(fd int) error {
	return syscall.EpollCtl(e.efd, syscall.EPOLL_CTL_DEL, fd, &syscall.EpollEvent{})
}
func (e *epoll) ModWatcher(fd int, events uint32) error {
	return syscall.EpollCtl(e.efd, syscall.EPOLL_CTL_MOD, fd, &syscall.EpollEvent{Events: events, Fd: int32(fd)})
}

//func (e *epoll) Wait(timeout int)([]syscall.EpollEvent, error) {
func (e *epoll) Wait(timeout int) {
	var events = []syscall.EpollEvent{}
	for {
		num, err := syscall.EpollWait(e.efd, events, timeout)
		switch err {
		case syscall.EINTR, syscall.EAGAIN:
			continue
		}
		for _, event := range events[:num] {
			e.callbacks[event.Fd].fun(e.callbacks[event.Fd].param)
		}

	}
}
func (e *epoll) Close() {
	syscall.Close(e.efd)
}

var addr = "127.0.0.1:19771"
var ep *epoll

func myRecv(c interface{}) {
	buffer := make([]byte)
	if conn, ok := c.(net.Conn); ok {
		n, err := conn.Read(buffer[:])
		switch err {
		case nil:
			fmt.Println("Recv:", string(buffer[:n]))
		case io.EOF:
			fmt.Println("Remote Close Connection")
			//ep.DelWatcher(int(event.Fd))
			conn.Close()
		default:
			panic(err)
		}
	}
}
func myListen(l interface{}) {
	if listener, ok := l.(net.ListenerTCP); ok {
		c, err := listener.Accept()
		switch err {
		case nil:
			rawc, err := c.SyscallConn()
			if err != nil {
				panic(err)
			}

			var cfd int
			if err := rawc.Control(func(fd uintptr) {
				cfd = int(fd)
				syscall.SetNonblock(cfd, true)
			}); err != nil {
				panic(err)
			}

			err = ep.AddWatcher(cfd, syscall.EPOLLIN, myRecv, c)
			if err != nil {
				panic(err)
			}
			fmt.Println("Accept Connection")
		case syscall.EAGAIN, syscall.EINTR:
		// retry
		default:
			panic(err)
		}
	}
}
func main() {
	ep = NewEpoll()
	if epoll == nil {
		log.Fatal("couldn't create epoll")
	}
	defer ep.Close()

	taddr, err := net.ResolveTCPAddr("tcp", addr)
	if err != nil {
		panic(err)
	}
	l, err := net.ListenTCP("tcp", taddr)
	if err != nil {
		panic(err)
	}
	// Add to epoll
	var lfd int
	rawl, err := l.SyscallConn()
	if err != nil {
		panic(err)
	}
	if err := rawl.Control(func(fd uintptr) {
		lfd = int(fd)
		ep.AddWatcher(lfd, syscall.EPOLLIN, myListen, l)
		//p.Add(lfd, syscall.EPOLLIN)
	}); err != nil {
		panic(err)
	}
	ep.Wait()
}
