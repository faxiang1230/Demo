package main

import (
	_ "fmt"
	"log"
	"time"
)

func main() {
	log.Print("chan demo")
	c1 := make(chan string, 1)
	c2 := make(chan string, 2)

	go func() {			//匿名协程
		time.Sleep(3 * time.Second)
		c1 <- "hello"
	}()
	select {
	case res := <-c1:
		log.Println(res)
	case <-time.After(time.Second * 2):
		log.Println("timeout 1")
	}
	
	go func() {
		time.Sleep(1 * time.Second)
		c2 <- "world"
	}()

	select {
	case msg2 := <-c2:
		log.Print(msg2)
	case <-time.After(time.Second * 2):
		log.Println("timeout 2")
	}
}
