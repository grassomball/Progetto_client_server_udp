# Password Generator UDP Client/Server

This project implements a UDP-based client-server application where the server acts as a password generator responding to client requests. Below is the detailed information about the project setup and functionality.

## Features

- **Server**: Continuously listens for client requests, generates passwords based on the specified criteria, and responds via UDP.
- **Client**: Sends requests to the server, specifying the password type and desired length, and displays the server-generated password.
- **Password Types**:
  - **Numeric (`n`)**: Password with digits only.
  - **Alphabetic (`a`)**: Password with lowercase letters only.
  - **Mixed (`m`)**: Password with a mix of lowercase letters and digits.
  - **Secure (`s`)**: Password with uppercase letters, lowercase letters, digits, and symbols.
  - **Unambiguous (`u`)**: Secure password without ambiguous characters (e.g., `0` and `O`).
- **Help Menu**: Accessible via the `h` command.
- **Portability**: Runs on Windows, MacOS, and Linux without platform-specific dependencies.

## Requirements

- UDP protocol for communication.
- Default server address: `passwdgen.uniba.it`
- Default port: `12345`
- Password length between 6 and 32 characters.
- Separate projects for client and server.

## Project Structure

### Header File (`udp_protocol.h`)
The protocol between the client and server is defined in `udp_protocol.h`, including:

- Password types.
- Buffer size.
- Default server and port configurations.
- Function prototypes for password generation.

### Server (`server.c`)
The server:

1. Listens on a UDP socket at the default address and port.
2. Receives requests from clients, logging the sender's address and port.
3. Generates passwords based on the requested type and length.
4. Sends the generated password back to the client.

### Client (`client.c`)
The client:

1. Reads password generation requests from the user (e.g., `n 8` for a numeric password of 8 characters).
2. Sends the request to the server via UDP.
3. Waits for the server's response and displays the generated password.
4. Provides a help menu (`h`) and an exit option (`q`).

### Compilation

#### Using Eclipse
- Make sure both `client` and `server` projects are imported into Eclipse.
- Build the projects using Eclipse's built-in build functionality.
- Then open two command prompt; one for server, one for client
  1) SERVER => Navigate to the server directory => cd c: path_unzip/server/Debug and after run the server => udp_server.exe
  2) CLIENT => Navigate to the client directory => cd c:path_unzip/client/Debug and after run the client with the ip address (passwdgen.uniba.it) => udp_client.exe passwdgen.uniba.it
 
  N.B. <i> path_unzip = percorso dove è stata scompattato il progetto github in riferimento a client e server

