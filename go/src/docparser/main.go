package main

import (
	"code.sajari.com/docconv"
	"fmt"
	"github.com/extrame/xls"
	_"github.com/ktye/pptx"
	"github.com/tealeg/xlsx"
	"log"
	"os"
	"strings"
	"github.com/unidoc/unioffice/presentation"
)

func getxls() {
	if xlFile, err := xls.Open("./data/test.xls", "utf-8"); err == nil {
		//fmt.Println(xlFile.Author)
		var build strings.Builder
		//for true  {
		res := xlFile.ReadAllCells(1000)
		for _, row := range res {
			for _, cell := range row {
				if cell != "" {
					build.WriteString(cell)
					//fmt.Println(cell)
				}
			}
		}
		fmt.Println(build.String())
		//fmt.Println(res)
		//}
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
	var build strings.Builder
	for _, sheet := range mySlice {
		for _, sheetRow := range sheet {
			for _, sheetColumn := range sheetRow {
				//fmt.Println(sheetColumn)
				//fmt.Println("======================================================")
				if sheetColumn != "" {
					build.WriteString(sheetColumn)
				}
			}
		}
	}
	fmt.Print(build.String())
}
func getDoc() {
	if file, err := os.Open("./data/test.docx"); err != nil {
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
func getPPTX() {
	if file, err := os.Open("./data/test.pptx"); err != nil {
		log.Fatal("open file failed,", err)
	} else {
		defer file.Close()
		str, strmap, err := docconv.ConvertPptx(file)
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
func getPPT() {
	if file, err := os.Open("./data/test.ppt"); err != nil {
		log.Fatal("open file failed,", err)
	} else {
		defer file.Close()
		slides, err := pptx.DecodeSlides(file)

		if err != nil {
			log.Print("conversation failed,", err)
		}

		for slide := range slides {
			fmt.Println(slide)
//			for textbox := range slide.TextBoxes {
//				fmt.Println(textbox.Lines)
//			}

		}
	}
}
func main() {
	//getDoc()
	//getxls()
	//getxlsx()
	//getPPTX()
	getPPT()
}
