/*
 ============================================================================
 Name        : udp_server.c
 Author      : gabriele
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */
#if defined WIN32
#include <winsock2.h>  // Ensure you're including the right Winsock header
#else
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#define closesocket close
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "udp_protocol.h"

// Function to handle errors
void errorhandler(char *errormessage) {
    printf("%s\n", errormessage);  // Print error message
}

void ClearWinSock() {
#if defined WIN32
    WSACleanup();  // Cleanup Winsock
#endif
}

// Password generation functions (using the same ones from the header file)
void generate_numeric(char *buffer, int length) {
    for (int i = 0; i < length; i++) {
        buffer[i] = '0' + rand() % 10;
    }
    buffer[length] = '\0';
}

void generate_alpha(char *buffer, int length) {
    for (int i = 0; i < length; i++) {
        buffer[i] = 'a' + rand() % 26;
    }
    buffer[length] = '\0';
}

void generate_mixed(char *buffer, int length) {
    for (int i = 0; i < length; i++) {
        if (rand() % 2) {
            buffer[i] = 'a' + rand() % 26;
        } else {
            buffer[i] = '0' + rand() % 10;
        }
    }
    buffer[length] = '\0';
}

void generate_secure(char *buffer, int length) {
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()";
    int charset_size = sizeof(charset) - 1;
    for (int i = 0; i < length; i++) {
        buffer[i] = charset[rand() % charset_size];
    }
    buffer[length] = '\0';
}

void generate_unambiguous(char *buffer, int length) {
    const char charset[] = "ABCDEFGHJKLMNPQRSTUVWXYZabcdefghjkmnpqrstuvwxyz345679";
    int charset_size = sizeof(charset) - 1;
    for (int i = 0; i < length; i++) {
        buffer[i] = charset[rand() % charset_size];
    }
    buffer[length] = '\0';
}

int main(int argc, char **argv) {
    int sock;
    struct sockaddr_in serverAddr, clientAddr;
    int clientAddrLen;
    char buffer[BUFFER_SIZE];
    char password[BUFFER_SIZE];
    int nBytes;

    // Initialize Winsock on Windows
#if defined WIN32
    WSADATA wsaData;  // Define Winsock data structure
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);  // Initialize Winsock
    if (iResult != 0) {
        printf("Error at WSAStartup(): %d\n", iResult);
        return EXIT_FAILURE;  // Return failure if WSAStartup fails
    }
#endif

    // Create a UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        errorhandler("Socket creation failed");
        return EXIT_FAILURE;
    }

    // Define server address
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;  // Listen on all available interfaces

    // Bind the socket to the server address and port
    if (bind(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        errorhandler("Bind failed");
        closesocket(sock);
        return EXIT_FAILURE;
    }

    printf("Server listening on port %d...\n", PORT);

    while (1) {
        clientAddrLen = sizeof(clientAddr);

        // Receive a request from a client
        nBytes = recvfrom(sock, buffer, sizeof(buffer), 0, (struct sockaddr*)&clientAddr, &clientAddrLen);
        if (nBytes < 0) {
            errorhandler("Recvfrom failed");
            continue;
        }

        buffer[nBytes] = '\0';  // Null terminate the received string
        printf("New request from %s:%d\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));

        // Parse the request
        char type = buffer[0];  // First character is the password type
        int length = atoi(&buffer[2]);  // The rest is the password length (after space)

        if (length < MIN_PASSWORD_LENGTH || length > MAX_PASSWORD_LENGTH) {
            snprintf(password, sizeof(password), "Invalid length. Must be between %d and %d.", MIN_PASSWORD_LENGTH, MAX_PASSWORD_LENGTH);
            sendto(sock, password, strlen(password), 0, (struct sockaddr*)&clientAddr, clientAddrLen);
            continue;
        }

        // Generate password based on the type
        switch (type) {
            case PASSWORD_NUMERIC:
                generate_numeric(password, length);
                break;
            case PASSWORD_ALPHA:
                generate_alpha(password, length);
                break;
            case PASSWORD_MIXED:
                generate_mixed(password, length);
                break;
            case PASSWORD_SECURE:
                generate_secure(password, length);
                break;
            case PASSWORD_UNAMBIGUOUS:
                generate_unambiguous(password, length);
                break;
            default:
                snprintf(password, sizeof(password), "Invalid password type.");
                break;
        }

        // Send the generated password back to the client
        sendto(sock, password, strlen(password), 0, (struct sockaddr*)&clientAddr, clientAddrLen);
    }

    // Cleanup Winsock on Windows
    ClearWinSock();

    // Close the socket
    closesocket(sock);
    return EXIT_SUCCESS;
}
