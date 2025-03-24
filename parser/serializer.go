package parser

import (
	"bytes"
	"fmt"
	"os"
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

type bulkString struct {
	text []byte
}

func (ss *bulkString) Serialize() {
	prefix := append(append([]byte("$"), []byte(string(len(ss.text)))...), '\r', 'n')
	ss.text = append(append(prefix, ss.text...), '\r', '\n')
}

func (ss *bulkString) Deserialize() {
	// TODO
}
