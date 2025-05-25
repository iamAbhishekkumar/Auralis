FROM debian:bookworm-slim

# Install tini and dependencies
RUN apt-get update && apt-get install -y \
    build-essential cmake tini \
    && rm -rf /var/lib/apt/lists/*

# Use tini as the init system
ENTRYPOINT ["/usr/bin/tini", "--"]

# Build stuff
WORKDIR /app
COPY . .
RUN mkdir build && cd build && cmake .. && make

EXPOSE 7389

CMD ["./build/auralis-server"]
