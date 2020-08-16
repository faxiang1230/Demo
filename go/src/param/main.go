package main

import (
	"fmt"
	_ "unsafe"
)

type param struct {
	val int
}

func param1(p param) {
	fmt.Printf("value param %p\n", &p)
}
func param2(p *param) {
	fmt.Printf("pointer param %p\n", p)
}
func main() {
	p := param{val: 3}
	fmt.Printf("main param, addr:%p\n", &p)
	param1(p)
	param2(&p)

	str := "nihao"
	fmt.Printf("origin string:%p %#v\n", &str, str)
	str = str + "1234"
	fmt.Printf("modified string:%p %#v\n", &str, str)
	str = str + "1234567890"
	fmt.Printf("again modified string:%p %#v\n", &str, str)

	slice := make([]byte, 10)
	fmt.Printf("origin slice:%p\n", slice)
	slice = append(slice, []byte("123456789012345")...)
	fmt.Printf("modified slice:%p\n", slice)

	strs := []string{"123", "456"}
	fmt.Printf("origin string array:%p\n", &strs)
	strs = append(strs, "789")
	fmt.Printf("modified string array:%p\n", &strs)

	var inter interface{}
	fmt.Printf("%#v", inter)
}
