package main

import (
	_ "debug/elf"
	"errors"
	"flag"
	"fmt"
	"os"
	"unsafe"
	"encoding/binary"
	"bytes"
)

const APP_VERSION = "0.1"
const EI_NIDENT = 16

type elf64_header struct {
	E_ident     [EI_NIDENT]uint8 /* Magic number and other info */
	E_type      uint16           /* Object file type */
	E_machine   uint16           /* Architecture */
	E_version   uint32           /* Object file version */
	E_entry     uint64           /* Entry point virtual address */
	E_phoff     uint64           /* Program header table file offset */
	E_shoff     uint64           /* Section header table file offset */
	E_flags     uint32           /* Processor-specific flags */
	E_ehsize    uint16           /* ELF header size in bytes */
	E_phentsize uint16           /* Program header table entry size */
	E_phnum     uint16           /* Program header table entry count */
	E_shentsize uint16           /* Section header table entry size */
	E_shnum     uint16           /* Section header table entry count */
	E_shstrndx  uint16           /* Section header string table index */
}

// The flag package provides a default help printer via -h switch
var versionFlag *bool = flag.Bool("v", false, "Print the version number.")

func elfparse(f string) error {
	file, err := os.Open(f)
	if err != nil {
		return errors.New("can't open file:")
	}
	var ehdr elf64_header
	data := make([]byte, unsafe.Sizeof(ehdr))
	fmt.Println("elf64_header size is", unsafe.Sizeof(ehdr))
	file.Read(data)
	buf := bytes.NewBuffer(data)
	binary.Read(buf, binary.LittleEndian, &ehdr)
	//ehdr = *(*elf64_header)(unsafe.Pointer(&data[0]))
	fmt.Println("%+v\n", ehdr)
	file.Close()
	
	return nil
}
func main() {

	var file string
	flag.StringVar(&file, "filename", "1234", "help message for flagname")
	flag.Parse() // Scan the arguments list

	if *versionFlag {
		fmt.Println("Version:", APP_VERSION)
	}

	fmt.Println(file)
	elfparse(file)
}
