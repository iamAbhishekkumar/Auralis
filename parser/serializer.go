package parser

import (
	"bytes"
	"fmt"
	"os"
	"strconv"
	"strings"
)

// Nexis Serialization Protocol
type Nesp interface {
	Serialize()
	Deserialzie()
}

type simpleString struct {
	text []byte
}

func (ss *simpleString) Serialize() {
	ss.text = append(append([]byte("+"), ss.text...), '\r', '\n')
}

func (ss *simpleString) Deserialize() {
	if bytes.HasPrefix(ss.text, []byte("+")) {
		ss.text = ss.text[1 : len(ss.text)-2]
	} else {
		fmt.Fprintln(os.Stderr, "Unable to deserialize")
	}
}

type BulkString struct {
	text []byte
}

func (ss *BulkString) Serialize() {
	buf := make([]byte, 0, len(ss.text)+10)
	buf = append(buf, '$')
	buf = append(buf, []byte(strconv.Itoa(len(ss.text)))...)
	buf = append(buf, '\r', '\n')
	buf = append(buf, ss.text...)
	buf = append(buf, '\r', '\n')
	ss.text = buf
}

func (ss *BulkString) Deserialize() {
	value := string(ss.text)
	if !strings.HasPrefix(value, "$") {
		fmt.Fprintln(os.Stderr, "invalid bulk string: missing $ prefix")
	}

	// Find first CRLF (after length prefix)
	crlfIndex := strings.Index(value, "\r\n")
	if crlfIndex == -1 {
		fmt.Fprintln(os.Stderr, "invalid bulk string: missing CRLF after length")
	}

	length, err := strconv.Atoi(value[1:crlfIndex])
	if err != nil {
		fmt.Fprintln(os.Stderr, "invalid bulk string length")
	}
	if length == -1 {
		ss.text = nil
	}

	start := crlfIndex + 2
	end := start + length

	if len(value) < end+2 || value[end:end+2] != "\r\n" {
		fmt.Fprintln(os.Stderr, "invalid bulk string: data length mismatch or missing CRLF at end")
	}

	ss.text = []byte(value[start:end])
}
