# Webserv

A non-blocking, event-driven HTTP server implemented in C++98.

## Overview
- **Architecture**: Reactor Pattern (Single Thread + I/O Multiplexing).
- **Standard**: C++98 compliant.
- **External Libs**: None.

## Architecture

### 1. Network Layer (The Server)
**Responsibility**: Runs the program, manages connections, moves bytes.

### 2. Protocol Layer (The Router)
**Responsibility**: Understands HTTP, decides actions, Serializes/Deserializes.

### 3. Service Layer (The Worker)
**Responsibility**: Manipulates files and executes scripts.

## Features
- **I/O Model**: Single-threaded Reactor Pattern (`poll`).
- **Methods**: GET, POST, DELETE.
- **CGI**: Dynamic script execution (PHP/Python).
- **Uploads**: Chunked Transfer Encoding.

## Build & Run

```bash
make
./webserv [configuration_file]
```
