# Chat-Link: C-Console program

## Description

This is a simple chat server application written in C. It uses sockets and the `poll` function to handle multiple client connections. The server listens for incoming connections on a specified port via command line when launching the program, accepts new client connections, and facilitates communication between connected clients by broadcasting messages.

## Table of Contents

- [Chat-Link: C-Console program](#chat-link-c-console-program)
  - [Description](#description)
  - [Table of Contents](#table-of-contents)
  - [Features](#features)
  - [Requirements](#requirements)
  - [Installation](#installation)
  - [Compilation](#compilation)
  - [Usage](#usage)
  - [Contributing](#contributing)
    - [Needed](#needed)
  - [License](#license)

## Features

- **Listening for Client Connections**: The server listens on a specified port and accepts incoming client connections.
- **Handling Multiple Clients**: Uses `poll` to manage multiple client connections simultaneously.
- **Broadcast Messages**: Receives messages from clients and broadcasts them to all other connected clients.
- **Client Management**: Adds and removes client sockets from the poll list as they connect and disconnect.

## Requirements

- **Operating System**: Linux or any POSIX-compliant OS
- **C Compiler**: `gcc`
- **Libraries**: Standard C libraries

## Installation


1. Clone the repository:

   ```bash
   git clone https://github.com/whitecodename/CHAT-LINK
   ```

2. Navigate to the project directory:

   ```bash
   cd CHAT-LINK
   ```

## Compilation

To compile the chat-link program, use the following command:

```bash
gcc -o chat-link chat-link.c socket.c utils.c
```

## Usage

1. **Start the Server**: Run the compiled server program giving the port which should be used.

   ```bash
   ./chat-link <port>
   ```

   **Example :**
   ```bash
   ./chat-link 4900
   ```

2. **Connect Clients**: Use a client such as Netcat or any other socket client to connect to the server:

   ```bash
   nc localhost <port>
   ```

   **Example :**
   ```bash
   nc localhost 4900
   ```

   Multiple clients can connect to the server simultaneously.

3. **Chat**: Once connected, you can start sending messages. Messages from any client will be broadcast to all other connected clients.

## Contributing

Contributions are welcome! Please submit an issue or a pull request for any improvements.

### Needed

When a client disconnects and reconnects, the server has to wait for the same client to send a message before it can continue running.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.