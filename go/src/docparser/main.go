package main

import (
	"code.sajari.com/docconv"
	"fmt"
	"github.com/extrame/xls"
	"github.com/tealeg/xlsx"
	"log"
	"os"
)

func getxls() {
	if xlFile, err := xls.Open("./data/test.xls", "utf-8"); err == nil {
		fmt.Println(xlFile.Author)

		for true  {
			res := xlFile.ReadAllCells(100)
			fmt.Println(res)
		}
	}
}
func getxlsx() {
	file, err := xlsx.OpenFile("./data/test.xlsx")
	if err != nil {
		log.Print("can't open file,", err)
		return
	}
	var mySlice [][][]string

	mySlice, err = file.ToSlice()
	if err != nil {
		log.Print("xlsx convert to string failed,", err)
		return
	}
	for _, sheet := range mySlice {
		for _, sheetRow := range sheet {
			for _, sheetColumn := range sheetRow {
				fmt.Println(sheetColumn)
				fmt.Println("======================================================")
			}
		}
	}
}
func getDoc() {
	if file, err := os.Open("./data/test.doc"); err != nil {
		log.Fatal("open file failed,", err)
	} else {
		defer file.Close()
		str, strmap, err := docconv.ConvertDocx(file)
		if err != nil {
			log.Fatal("conversation failed,", err)
		}

		fmt.Println(str)
		fmt.Println("======================================================")
		for str := range strmap {
			fmt.Println(str)
		}
	}
}
func main() {
	getxls()
}
