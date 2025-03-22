package main

import (
	"bufio"
	"fmt"
	"log/slog"
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

	var cmd string
	if len(os.Args) > 1 {
		cmd = os.Args[1]
		if cmd == "PING" {
			cmd = "*1\r\n$4\r\nPING\r\n"
		} else if cmd == "ECHO" {
			if len(os.Args) > 2 {
				text := os.Args[2]
				WriteStringToConn(conn, text)

			} else {
				fmt.Fprint(os.Stderr, "Empty Echo Message")
			}

		}
		_, err := conn.Write([]byte(cmd))
		if err != nil {
			fmt.Fprint(os.Stderr, "Failed to send the data")
		}
		conn.(*net.TCPConn).CloseWrite()
	}
	readedText := ReadStringFromConn(conn)
	fmt.Println(readedText)

}

func WriteStringToConn(conn net.Conn, text string) {
	writer := bufio.NewWriter(conn)
	n, err := writer.WriteString(text)
	if err != nil {
		fmt.Fprintln(os.Stderr, "Failed to write :", err)
		return
	}
	slog.Debug("Successfully Written to server. Number of bytes written : ", n)
}

func ReadStringFromConn(conn net.Conn) string {
	reader := bufio.NewReader(conn)
	response, err := reader.ReadString('\n') // Read until newline
	if err != nil {
		fmt.Fprintln(os.Stderr, "Failed to read response:", err)
		os.Exit(-1)
	}

	slog.Debug("successful read from server:")
	return response
}
