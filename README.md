# Auralis
A simple in memory database.

## What is RESP
RESP3 (REdis Serialization Protocol version 3) is the latest version of Redis's protocol used for communication between clients and the server.

* This will be send to server instead of PING, `*1\r\n$4\r\nPING\r\n`, which is the Redis protocol encoding of the PING command.
* The outtput should be PONG (`+PONG\r\n`). Later will do client side parsing.

## How to Build
```bash
docker build -t auralis_db .
```

```bash
docker run -it --rm -p 7389:7389 auralis_db
```