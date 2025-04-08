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

func DeserializeValue(input []byte) ([]string, error) {
	if len(input) == 0 {
		return nil, fmt.Errorf("empty input")
	}

	switch input[0] {
	case '+': // Simple String
		ss := &SimpleString{}
		err := ss.Deserialize(input)
		return []string{ss.Data}, err

	case '$': // Bulk String
		bs := &BulkString{}
		err := bs.Deserialize(input)
		return []string{bs.Data}, err

	case '*': // Array
		arr := &Array{}
		val, err := arr.DeserializeArray(input)
		return val, err

	default:
		return nil, fmt.Errorf("unknown NESP type: %c", input[0])
	}
}
