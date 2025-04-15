package main

// rewrite client

// import (
// 	"bufio"
// 	"errors"
// 	"fmt"
// 	"log/slog"
// 	"net"
// 	"os"

// 	"github.com/iamAbhishekkumar/NexisDB/nesp"
// )

// func main() {
// 	if len(os.Args) < 2 {
// 		fmt.Fprintln(os.Stderr, "No command provided")
// 		return
// 	}

// 	conn, err := net.Dial("tcp", "127.0.0.1:8389")
// 	if err != nil {
// 		fmt.Fprint(os.Stderr, "Failed to bind to port 6379")
// 		os.Exit(1)
// 	}

// 	defer conn.(*net.TCPConn).CloseWrite()

// 	cmd := os.Args[1]
// 	switch cmd {
// 	case "PING":
// 		_, err = conn.Write([]byte("*1\r\n+PING\r\n"))
// 		if err != nil {
// 			fmt.Fprint(os.Stderr, "Failed to send the data")
// 		}
// 	case "ECHO":
// 		if len(os.Args) > 2 {
// 			serializedData, err := nesp.SerializeValue(os.Args[1:])
// 			if err != nil {
// 				panic(errors.New("unable to serialize"))
// 			}
// 			conn.Write(serializedData)
// 		} else {
// 			panic(errors.New("empty Echo Message"))
// 		}
// 	default:
// 		panic(errors.New("unknown command"))
// 	}
// 	readedText := ReadStringFromConn(conn)
// 	fmt.Print(readedText)

// }

// func WriteStringToConn(conn net.Conn, text string) {
// 	writer := bufio.NewWriter(conn)
// 	n, err := writer.WriteString(text)
// 	if err != nil {
// 		fmt.Fprintln(os.Stderr, "Failed to write :", err)
// 		return
// 	}
// 	slog.Debug("Successfully Written to server. Number of bytes written : ", n)
// }

// func ReadStringFromConn(conn net.Conn) string {
// 	reader := bufio.NewReader(conn)
// 	response, err := reader.ReadString('\n') // Read until newline
// 	if err != nil {
// 		fmt.Fprintln(os.Stderr, "Failed to read response:", err)
// 		os.Exit(-1)
// 	}

// 	slog.Debug("successful read from server:")
// 	return response
// }
