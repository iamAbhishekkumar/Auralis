package nesp

import "fmt"

func SerializeValue(val interface{}) ([]byte, error) {
	switch v := val.(type) {
	case string:
		// Treat all strings as bulk strings for safety
		bs := &BulkString{Data: v}
		return bs.Serialize()
	case []string:
		arr := &Array{}
		return arr.SerializeArray(v)
	default:
		return nil, fmt.Errorf("unsupported type for serialization: %T", v)
	}
}

func DeserializeValue(input []byte) (interface{}, error) {
	if len(input) == 0 {
		return nil, fmt.Errorf("empty input")
	}

	switch input[0] {
	case '+':
		ss := &SimpleString{}
		err := ss.Deserialize(input)
		return string(ss.Data), err
	case '$':
		bs := &BulkString{}
		err := bs.Deserialize(input)
		return string(bs.Data), err
	case '*':
		arr := &Array{}
		return arr.DeserializeArray(input)
	default:
		return nil, fmt.Errorf("unknown NESP type: %c", input[0])
	}
}
