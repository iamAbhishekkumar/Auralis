package nesp

import (
	"bytes"
	"errors"
	"fmt"
	"strconv"
)

type Array struct {
}

// SerializeArray takes a []string and converts it into RESP-like format.
func (a *Array) SerializeArray(arr []string) ([]byte, error) {
	var buf bytes.Buffer

	buf.WriteByte('*')
	buf.WriteString(strconv.Itoa(len(arr)))
	buf.WriteString("\r\n")

	for _, val := range arr {
		buf.WriteByte('$')
		buf.WriteString(strconv.Itoa(len(val)))
		buf.WriteString("\r\n")
		buf.WriteString(val)
		buf.WriteString("\r\n")
	}

	return buf.Bytes(), nil
}

// DeserializeArray converts RESP-formatted array bytes into a []string.
func (a *Array) DeserializeArray(input []byte) ([]string, error) {
	if len(input) == 0 || input[0] != '*' {
		return nil, errors.New("not a RESP array")
	}

	cursor := 1
	crlfIndex := bytes.Index(input[cursor:], []byte("\r\n"))
	if crlfIndex == -1 {
		return nil, errors.New("missing CRLF after array length")
	}
	crlfIndex += cursor

	countStr := string(input[cursor:crlfIndex])
	count, err := strconv.Atoi(countStr)
	if err != nil {
		return nil, fmt.Errorf("invalid array length: %v", err)
	}

	cursor = crlfIndex + 2
	result := make([]string, 0, count)

	for i := 0; i < count; i++ {
		if cursor >= len(input) || input[cursor] != '$' {
			return nil, errors.New("expected bulk string")
		}

		lenEnd := bytes.Index(input[cursor:], []byte("\r\n"))
		if lenEnd == -1 {
			return nil, errors.New("invalid bulk string header")
		}
		lenEnd += cursor

		strLen, err := strconv.Atoi(string(input[cursor+1 : lenEnd]))
		if err != nil {
			return nil, fmt.Errorf("invalid string length: %v", err)
		}

		start := lenEnd + 2
		end := start + strLen
		if end+2 > len(input) || string(input[end:end+2]) != "\r\n" {
			return nil, errors.New("invalid string data or missing CRLF")
		}

		result = append(result, string(input[start:end]))
		cursor = end + 2
	}

	return result, nil
}
