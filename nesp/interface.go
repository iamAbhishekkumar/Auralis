package nesp

type Nesp interface {
	Serialize() ([]byte, error)
	Deserialize([]byte) error
}
