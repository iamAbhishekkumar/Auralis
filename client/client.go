package main

import (
	"bufio"
	"fmt"
	"net"
	"os"
)

func main() {
	conn, err := net.Dial("tcp", "127.0.0.1:6379")
	if err != nil {
		fmt.Fprint(os.Stderr, "Failed to bind to port 6379")
		os.Exit(1)
	}

	defer conn.Close()

	var text string
	if len(os.Args) > 1 {
		fmt.Println("Written to server")
		text = os.Args[1]
		if text == "PING" {
			text = "*1\r\n$4\r\nPING\r\n"
		}
		_, err := conn.Write([]byte(text))
		if err != nil {
			fmt.Fprint(os.Stderr, "Failed to send the data")
		}
		conn.(*net.TCPConn).CloseWrite()
	}

	reader := bufio.NewReader(conn)
	response, err := reader.ReadString('\n') // Read until newline
	if err != nil {
		fmt.Fprintln(os.Stderr, "Failed to read response:", err)
		return
	}

	fmt.Println("Server Response:", response)

}
