package nesp

import (
	"bytes"
	"errors"
)

type SimpleString struct {
	Data string
}

func (ss *SimpleString) Serialize() ([]byte, error) {
	return []byte("+" + ss.Data + "\r\n"), nil
}

func (ss *SimpleString) Deserialize(input []byte) error {
	if !bytes.HasPrefix(input, []byte("+")) || !bytes.HasSuffix(input, []byte("\r\n")) {
		return errors.New("invalid simple string format")
	}
	ss.Data = string(input[1 : len(input)-2])
	return nil
}
