package main

import "fmt"
import "bufio"

type scanwordInterface interface {
	scanwords([]byte, bool) (int, []byte, error)
}
type Demo struct {}

func (d *Demo)scanwords(data []byte, atEOF bool)(int, []byte, error) {
	var advance int
	var token []byte
	var err error
	var b byte
	for advance, b = range data {
		if b == byte(' ') {
			break
		} else {
			token = append(token, b)
		}
	}
	return advance+1, token, err
}

func main() {
	num, token, err := bufio.ScanWords([]byte("hello nihao"), true)
	fmt.Print(num, string(token), err)
	var demo Demo
	num, token, err = demo.scanwords([]byte("hello nihao"), true)
	fmt.Print(num, string(token), err)

	var demo2 scanwordInterface
	demo2 = &Demo{}
	num, token, err = demo2.scanwords([]byte("hello nihao"), true)
	fmt.Print(num, string(token), err)
}
