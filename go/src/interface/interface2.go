package main

import (
	"io"
	"os"
	"fmt"
)
/*
	练习7.2 写一个带有如下函数签名的函数CountingWriter，传入一个io.Writer接口类型，返回一个
	新的Writer类型，把原来的Writer封装在里面和一个表示写入新的Writer字节数的int64类型指针

	func CountingWriter(w io.Writer) (io.Writer, *int64)
*/
type CountWriter struct {
	cw io.Writer
	C int64
}

func (c *CountWriter) Write(p []byte)(int,error)  {
	c.C = int64(len(p))
	n,err := c.cw.Write(p)
	return n,err
}

func CountingWriter(w io.Writer) (io.Writer, *int64){
	c := &CountWriter{cw:w}
	return c,&c.C
}

func main() {
	rw, ilen := CountingWriter(os.Stdout)
	rw.Write([]byte("hello,world\n"))
	fmt.Println(*ilen)
}
