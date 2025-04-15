BUILD_DIR := build
SERVER := $(BUILD_DIR)/auralis
CLIENT := $(BUILD_DIR)/auralis-cli
SERVER_SRC := server/server.go
CLIENT_SRC := client/client.go

.PHONY: all clean

all: $(SERVER) $(CLIENT)

$(SERVER): $(SERVER_SRC)
	@if [ ! -d $(BUILD_DIR) ]; then mkdir -p $(BUILD_DIR); fi
	go build -o $(SERVER) $(SERVER_SRC)

$(CLIENT): $(CLIENT_SRC)
	@if [ ! -d $(BUILD_DIR) ]; then mkdir -p $(BUILD_DIR); fi
	go build -o $(CLIENT) $(CLIENT_SRC)

clean:
	@if [ -d $(BUILD_DIR) ]; then rm -rf $(BUILD_DIR); fi
