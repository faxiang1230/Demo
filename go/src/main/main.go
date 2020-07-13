package main

import (
	"fmt"
	"os"
	"log"
)
func funcrange(){
	var valmap map[int](string)
	valmap = make(map[int](string))
	valmap[1] = "hello"
	valmap[32] = "nihao"
	valmap[1124] = "world"
	
	for k,tmp := range valmap {
		fmt.Println(k, tmp)
	}
}
func readfile(f string) (int, []byte){
	file, err := os.Open(f) // For read access.
	if err != nil {
		log.Fatal(err)
	}
	data := make([]byte, 100)
	count,_:= file.Read(data)
	fmt.Println(file.Name())
	return count,data
}
func main() {
	var a ,ret int = 4, 0
	var ptr *int
	var buffer []byte
	ptr = &a
	fmt.Println(a, "address:", ptr, *ptr)
	ret,buffer = readfile("/home/linux/kylin")
	str := string(buffer)
	fmt.Println(ret, str)
	
	funcrange()
}
