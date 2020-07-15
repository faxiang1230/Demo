package main 

import (

)

func main() {
	done := make(chan bool, 1)
	Server(done)
	<- done
}

