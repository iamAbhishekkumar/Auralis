package main

import (
	"bytes"
	"fmt"
	"io"
	"net"
	"os"
	"strings"

	"github.com/iamAbhishekkumar/NexisDB/nesp"
)

func main() {
	l, err := net.Listen("tcp", "0.0.0.0:6379")
	if err != nil {
		fmt.Fprint(os.Stderr, "Failed to bind to port 6379")
		os.Exit(1)
	}
	fmt.Println("Started Accepting Conenctions......")
	for {
		c, err := l.Accept()
		if err != nil {
			fmt.Fprint(os.Stderr, "Error accepting connection: ", err.Error())
			os.Exit(1)
		}
		go handleConnection(c)
	}

}

func handleConnection(conn net.Conn) {
	defer conn.Close()
	fmt.Println("Accepted Connection from ", conn.RemoteAddr())
	buffer := make([]byte, 1024)
	for {
		// Read data from the client
		n, err := conn.Read(buffer)
		if err != nil {
			if err == io.EOF {
				fmt.Println("Client disconnected gracefully")
			} else {
				fmt.Fprintf(os.Stderr, "Error: %s", err)
			}
			return
		}

		cmdExecutor(conn, buffer[:n])
	}
}

func cmdExecutor(conn net.Conn, buf []byte) {
	if bytes.Equal([]byte("*1\r\n+PING\r\n"), buf) {
		pong := []byte("+PONG\r\n")
		conn.Write(pong)
	} else {
		val, _ := nesp.DeserializeValue(buf)
		if val[0] == "ECHO" {
			conn.Write([]byte(strings.Join(val[1:], " ") + "\r\n"))
		}
	}
}
