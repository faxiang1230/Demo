//package simpleHTTPServer
package main

import (
	"fmt"
	"io/ioutil"
	"net/http"
	"os/exec"
	"strings"
)

type fileServer struct {
}

func (f *fileServer) ServeHTTP(w http.ResponseWriter, r *http.Request) {

	fmt.Printf("Get Request%v\n", *r)
	requestPath := "." + r.URL.Path
	fmt.Printf("Get request%v\n", requestPath)
	getTypeCmd := exec.Command("file", requestPath)
	fileType, err := getTypeCmd.Output()

	if err != nil {
		w.Write([]byte(fmt.Sprintf("Get %s error: %v", requestPath, err)))
	}

	if strings.Contains(string(fileType[:]), "directory") {
		cmd := exec.Command("ls", requestPath)
		fileListData, err := cmd.Output()
		//if err :=cmd.Run();err !=nil{
		//   fmt.Printf("Execute ls error:%v\n",err)
		//}
		if err != nil {
			fmt.Printf("Get output of cmd ls error %v\n", err)
		}
		fileList := strings.Split(string(fileListData[:]), "\n")
		//w.Write(fileListData)
		fileListLink := GenLinkForFile(r.URL.Path, fileList)
		//fmt.Print(fileList)
		w.Write([]byte(fileListLink))
	} else {
		fileData, err := ioutil.ReadFile(requestPath)
		if err != nil {
			w.Write([]byte(fmt.Sprintf("Get file %s error: %v", requestPath, err)))
		}
		w.Write(fileData)
	}
}

func GenLinkForFile(basePath string, fileList []string) string {
	var fileListLink string = `<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 3.2 Final//EN"><html>
<title>Directory listing for ` + basePath + `</title>
<body>
<h2>Directory listing for ` + basePath + `</h2>
<hr>
<ul>
`
	for i := 0; i < len(fileList); i++ {
		if fileList[i] == "" {
			continue
		}
		if basePath == "/" {
			fileList[i] = "<li><a href=\"" + basePath + fileList[i] + "\">" + fileList[i] + "</a>\n"
		} else {
			fileList[i] = "<li><a href=\"" + basePath + `/` + fileList[i] + "\">" + fileList[i] + "</a>\n"
		}
		fileListLink = fileListLink + fileList[i]
	}
	fileListLink = fileListLink + `</ul>
<hr>
</body>
</html>`
	return fileListLink
}

func SimpleServer() {
	http.Handle("/", &fileServer{})
	err := http.ListenAndServe(":8000", nil)
	if err != nil {
		fmt.Printf("Listen 8000 error:%v\n", err)
	}
}

func main() {
	SimpleServer()
}
