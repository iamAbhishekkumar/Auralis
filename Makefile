BUILD_DIR := build
SERVER := $(BUILD_DIR)\nexis-server.exe
CLIENT := $(BUILD_DIR)\nexis-cli.exe
SERVER_SRC := server\server.go
CLIENT_SRC := client\client.go

.PHONY: all clean

all: $(SERVER) $(CLIENT)

$(SERVER): $(SERVER_SRC)
	@if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)
	go build -o $(SERVER) $(SERVER_SRC)

$(CLIENT): $(CLIENT_SRC)
	@if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)
	go build -o $(CLIENT) $(CLIENT_SRC)

clean:
	@if exist $(BUILD_DIR) rmdir /s /q $(BUILD_DIR)