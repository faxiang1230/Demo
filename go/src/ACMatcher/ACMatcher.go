package main 

import (
    "bufio"
    "bytes"
    "fmt"
    "io"
    "os"
 )

import (
	"github.com/cloudflare/ahocorasick"
)

func ReadRunes(filename string) ([]byte, error) {
    var dict []byte

    f, err := os.OpenFile(filename, os.O_RDONLY, 0660)
    if err != nil {
        return nil, err
    }

    r := bufio.NewReader(f)
    for {
        l, err := r.ReadBytes('\n')
        if err != nil || err == io.EOF {
            break
        }
        l = bytes.TrimSpace(l)
        dict = append(dict, l...)
        //dict = append(dict, bytes.Runes(l))
    }

    return dict, nil
}

func main() {
    dict, err := ReadRunes("/home/linux/project/linux-bfx-gfy/java_code/src/main_app/ContentRecognizer.java")
    if err != nil {
        fmt.Println(err)
        return
    }

	var key_strs []string
	var match_strs []string
	var max_len, max_index int
	key_strs = append(key_strs, "HAHHAPolicy");
	key_strs = append(key_strs, "Policy");
	key_strs = append(key_strs, "PolicyByte");
    matcher := ahocorasick.NewStringMatcher(key_strs);

	index := matcher.Match(dict);
	for _, i := range index {
		fmt.Println(i);
		match_strs = append(match_strs, key_strs[i])
		if (len(key_strs[i]) > max_len) {
			max_len = len(key_strs[i])
			max_index = i
		}
	}
	fmt.Println(key_strs[max_index])
}