package nesp

import (
	"bytes"
	"errors"
	"fmt"
	"strconv"
	"strings"
)

type BulkString struct {
	Data string
}

func (bs *BulkString) Serialize() ([]byte, error) {
	data := []byte(bs.Data)
	length := strconv.Itoa(len(data))
	var buf bytes.Buffer

	buf.WriteByte('$')
	buf.WriteString(length)
	buf.WriteString("\r\n")
	buf.Write(data)
	buf.WriteString("\r\n")

	return buf.Bytes(), nil
}

func (bs *BulkString) Deserialize(input []byte) error {
	str := string(input)
	if !strings.HasPrefix(str, "$") {
		return errors.New("invalid bulk string: missing $ prefix")
	}

	crlfIdx := strings.Index(str, "\r\n")
	if crlfIdx == -1 {
		return errors.New("invalid bulk string: missing CRLF")
	}

	length, err := strconv.Atoi(str[1:crlfIdx])
	if err != nil {
		return fmt.Errorf("invalid length: %v", err)
	}
	if length == -1 {
		bs.Data = ""
		return nil
	}

	start := crlfIdx + 2
	end := start + length

	if len(str) < end+2 || str[end:end+2] != "\r\n" {
		return errors.New("data length mismatch or missing final CRLF")
	}

	bs.Data = str[start:end]
	return nil
}
