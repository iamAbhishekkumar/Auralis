package common

import (
	"bufio"
	"fmt"
	"io"
	"strconv"
)

const (
	STR  = '+'
	ERR  = '-'
	INT  = ':'
	BULK = '$'
	ARR  = '*'
)

type ValueType struct {
	Typ   string
	Str   string
	Num   string
	Bulk  string
	Array []ValueType
}

type Resp struct {
	reader *bufio.Reader
}

func NewResp(rd io.Reader) *Resp {
	return &Resp{
		reader: bufio.NewReader(rd),
	}
}

// Need optmization
func (r *Resp) readLine() (line []byte, n int, err error) {
	for {
		b, err := r.reader.ReadByte()
		if err != nil {
			return nil, 0, err
		}
		n += 1
		line = append(line, b)
		if len(line) >= 2 && line[len(line)-2] == '\r' {
			break
		}
	}
	return line[:len(line)-2], n, nil
}

func (r *Resp) readInteger() (x int, n int, err error) {
	line, n, err := r.readLine()
	if err != nil {
		return 0, 0, err
	}
	i64, err := strconv.ParseInt(string(line), 10, 64)
	if err != nil {
		return 0, n, err
	}
	return int(i64), n, nil
}

func (r *Resp) Read() (ValueType, error) {
	_type, err := r.reader.ReadByte()

	if err != nil {
		return ValueType{}, err
	}

	switch _type {
	case ARR:
		return r.readArray()
	case BULK:
		return r.readBulk()
	default:
		fmt.Printf("Unknown type: %v", string(_type))
		return ValueType{}, nil
	}
}

func (r *Resp) readArray() (ValueType, error) {
	v := ValueType{}
	v.Typ = "array"

	// read length of array
	len, _, err := r.readInteger()
	if err != nil {
		return v, err
	}

	// foreach line, parse and read the ValueType
	v.Array = make([]ValueType, 0)
	for i := 0; i < len; i++ {
		val, err := r.Read()
		if err != nil {
			return v, err
		}

		// append parsed ValueType to array
		v.Array = append(v.Array, val)
	}

	return v, nil
}

func (r *Resp) readBulk() (ValueType, error) {
	v := ValueType{}

	v.Typ = "bulk"

	len, _, err := r.readInteger()
	if err != nil {
		return v, err
	}

	bulk := make([]byte, len)

	r.reader.Read(bulk)
	v.Bulk = string(bulk)
	// Read the trailing CRLF
	r.readLine()

	return v, nil
}

// Marshal ValueType to bytes
func (v ValueType) Marshal() []byte {
	switch v.Typ {
	case "array":
		return v.marshalArray()
	case "bulk":
		return v.marshalBulk()
	case "string":
		return v.marshalString()
	case "null":
		return v.marshallNull()
	case "error":
		return v.marshallError()
	default:
		return []byte{}
	}
}

func (v ValueType) marshalString() []byte {
	var bytes []byte
	bytes = append(bytes, STR)
	bytes = append(bytes, v.Str...)
	bytes = append(bytes, '\r', '\n')

	return bytes
}

func (v ValueType) marshalBulk() []byte {
	var bytes []byte
	bytes = append(bytes, BULK)
	bytes = append(bytes, strconv.Itoa(len(v.Bulk))...)
	bytes = append(bytes, '\r', '\n')
	bytes = append(bytes, v.Bulk...)
	bytes = append(bytes, '\r', '\n')

	return bytes
}

func (v ValueType) marshalArray() []byte {
	len := len(v.Array)
	var bytes []byte
	bytes = append(bytes, ARR)
	bytes = append(bytes, strconv.Itoa(len)...)
	bytes = append(bytes, '\r', '\n')

	for i := 0; i < len; i++ {
		bytes = append(bytes, v.Array[i].Marshal()...)
	}

	return bytes
}

func (v ValueType) marshallError() []byte {
	var bytes []byte
	bytes = append(bytes, ERR)
	bytes = append(bytes, v.Str...)
	bytes = append(bytes, '\r', '\n')

	return bytes
}

func (v ValueType) marshallNull() []byte {
	return []byte("$-1\r\n")
}

type Writer struct {
	writer io.Writer
}

func NewWriter(w io.Writer) *Writer {
	return &Writer{writer: w}
}

func (w *Writer) Write(v ValueType) error {
	var bytes = v.Marshal()

	_, err := w.writer.Write(bytes)
	if err != nil {
		return err
	}

	return nil
}
