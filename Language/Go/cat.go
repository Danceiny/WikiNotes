/* implement of cat tool */
/* with a bug that will crush if the last line has no '\n'. */
package main

import (
	"io"
	"os"
	"fmt"
	"bufio"
	"flag"
)

//improved version??
/*
import (
	"bufio"
	"flag"
	"fmt"
	"io"
	"os"
)
*/

var numberFlag = flag.Bool("n",false, "number each line")	//定义新的开关"n",默认关闭。

//
func cat(r *bufio.Reader){
	i := 1
	for {
		buf,e := r.ReadBytes('\n')	//每次读一行
		if e==io.EOF{
			break
		}
		if *numberFlag {	//如果设定了行号（打开行号显示开关）
			fmt.Fprintf(os.Stdout, "%5d %s",i,buf)
			i++
		}else {
			fmt.Fprintf(os.Stdout,"%s",buf)
		}
	}
	return
}

func main(){
	flag.Parse()
	if flag.NArg() == 0{
		cat(bufio.NewReader(os.Stdin))
	}
	for i:=0; i<flag.NArg(); i++{
		f,e := os.Open(flag.Arg(i))
		if e!=nil {
			fmt.Fprintf(os.Stderr, "%s: error reading from %s: %s\n",os.Args[0], flag.Arg(i),e.Error())
			continue
		}
		cat(bufio.NewReader(f))
	}
}
