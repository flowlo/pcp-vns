package main

import (
	"flag"
	"fmt"
	"os"
)

var debugLevel = flag.Int("debug", 0, "set debug level")
var printFile = flag.String("print", "", "set print file (*.gv)")

func main() {
	var s Solution
	s.ReadPcp(os.Stdin)
	fmt.Printf("%v\n", s.Graph.Partitions[1])
}
