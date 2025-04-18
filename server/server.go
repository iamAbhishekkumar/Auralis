package main

import (
	"fmt"
	"net"
	"os"

	cm "github.com/iamAbhishekkumar/Auralis/common"
	"golang.org/x/sys/unix"
)

func main() {
	const port int = 8389
	const mode string = "Standalone"

	ln, err := net.Listen("tcp", "0.0.0.0:"+fmt.Sprint(port))
	if err != nil {
		fmt.Fprint(os.Stderr, "Failed to bind to port 6379")
		os.Exit(1)
	}

	// Extracting file desc
	tcpLn := ln.(*net.TCPListener)
	lnFile, _ := tcpLn.File()
	listenFD := int(lnFile.Fd())
	unix.SetNonblock(listenFD, true)

	cm.ServerGreetMessage()
	cm.ServerInfo(mode, os.Getpid(), port)

	peers := make(map[int]net.Conn)

	for {
		// Prepare FD sets
		readFDs := &unix.FdSet{}
		readFDs.Set(listenFD)

		maxFD := listenFD
		for fd := range peers {
			readFDs.Set(fd)
			if fd > maxFD {
				maxFD = fd
			}
		}

		// Wait for activity
		n, err := unix.Select(maxFD+1, readFDs, nil, nil, nil)
		if err != nil {
			fmt.Println("select err:", err)
			continue
		}
		if n == 0 {
			continue
		}

		// New connection?
		if readFDs.IsSet(listenFD) {
			conn, err := tcpLn.Accept()
			if err == nil {
				fd := getFD(conn)
				unix.SetNonblock(fd, true)
				peers[fd] = conn
				cm.InfoLog("Accepted connection from : ", conn.RemoteAddr())
			}
		}

		// Existing clients
		for fd, conn := range peers {
			if readFDs.IsSet(fd) {
				resp := cm.NewResp(conn)
				value, err := resp.Read()
				if err != nil {
					conn.Close()
					delete(peers, fd)
					cm.InfoLog("Disconnected connection from : ", conn.RemoteAddr())
					cm.DebugLog("Asscoiated Fd : ", fd)
					continue
				}

				_ = value

				writer := cm.NewWriter(conn)
				writer.Write(cm.ValueType{Typ: "string", Str: "OK"})
			}

		}
	}
}

func getFD(conn net.Conn) int {
	tcpConn := conn.(*net.TCPConn)
	file, _ := tcpConn.File()
	return int(file.Fd())
}
