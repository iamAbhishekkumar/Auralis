BUILD_DIR := build
OUTPUT := $(BUILD_DIR)/nexis-server.exe
SRC := cmd/main.go

.PHONY: all clean

all: $(OUTPUT)

$(OUTPUT): $(SRC)
	@if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)
	go build -o $(OUTPUT) $(SRC)

clean:
	if exist $(BUILD_DIR) rmdir /s /q $(BUILD_DIR)
