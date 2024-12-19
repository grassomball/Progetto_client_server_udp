/*
 ============================================================================
 Name        : udp_server.c
 Author      : Gabriele Grasso
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if defined WIN32
#include <winsock2.h>
#else
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#define closesocket close
#endif

#define BUFFER_SIZE 255
#define PORT 12345

// Function to handle errors
void errorhandler(char *errormessage) {
    printf("%s\n", errormessage);
}

void ClearWinSock() {
#if defined WIN32
    WSACleanup();   // Cleanup Winsock
#endif
}
void printHelpMenu() {
    printf("\nPassword Generator Help Menu\n");
    printf("Commands:\n");
    printf(" h        : show this help menu\n");
    printf(" n LENGTH : generate numeric password (digits only)\n");
    printf(" a LENGTH : generate alphabetic password (lowercase letters)\n");
    printf(" m LENGTH : generate mixed password (lowercase letters and numbers)\n");
    printf(" s LENGTH : generate secure password (uppercase, lowercase, numbers, symbols)\n");
    printf(" u LENGTH : generate unambiguous secure password (no similar-looking characters)\n");
    printf(" q        : quit application\n");
    printf("\nLENGTH must be between 6 and 32 characters\n");
    printf("\nAmbiguous characters excluded in 'u' option:\n");
    printf(" 0 O o (zero and letters O)\n");
    printf(" 1 l I i (one and letters l, I)\n");
    printf(" 2 Z z (two and letter Z)\n");
    printf(" 5 S s (five and letter S)\n");
    printf(" 8 B (eight and letter B)\n\n");
}

int main(int argc, char **argv) {
    struct hostent *remoteHost;      // Structure to hold host resolution information
    char *host_name;                 // Hostname or IP address provided by user
    struct in_addr **addr_list;      // List of resolved IP addresses
    int sock;
    struct sockaddr_in serverAddr;   // Server address structure
    char buffer[BUFFER_SIZE];        // Buffer for user input and server response

    // Ensure correct usage
    if (argc != 2) {
        printf("Usage: %s <hostname or IP address>\n", argv[0]);
        return EXIT_FAILURE;
    }

#if defined WIN32
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("Error at WSAStartup(): %d\n", iResult);
        return EXIT_FAILURE;
    }
#endif

    // Resolve the provided hostname or IP address
    host_name = argv[1];
    printf("Resolving host: %s\n", host_name);
    remoteHost = gethostbyname(host_name);

    if (remoteHost == NULL) {
        printf("Error in resolving host\n");
        ClearWinSock();
        return EXIT_FAILURE;
    }

    // Print details about resolved IPs
    addr_list = (struct in_addr **)remoteHost->h_addr_list;
    printf("Resolved hostname: %s\n", remoteHost->h_name);
    printf("IP addresses:\n");
    for (int i = 0; addr_list[i] != NULL; i++) {
        printf("- %s\n", inet_ntoa(*addr_list[i]));
    }

    // Create a UDP socket
    sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        errorhandler("Socket creation failed");
        return EXIT_FAILURE;
    }

    // Define server address using the first resolved IP
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr = *addr_list[0];  // Use the first resolved IP

    printf("Using IP: %s\n", inet_ntoa(serverAddr.sin_addr)); // Verify the IP being used

    while (1) {
        // Read user input for password request type
        printf("Enter password type and length (e.g., 'n 8' for numeric password of length 8): ");
        fgets(buffer, sizeof(buffer), stdin);

        // Debug: Print what is being sent
        printf("Sending: %s", buffer);


        if (buffer[0] == 'h') {
                  printHelpMenu();
                  continue;
              }


        // If user inputs 'q', exit the loop
        if (buffer[0] == 'q') {
            printf("Exiting...\n");
            break;
        }

        // Send the request to the server
        if (sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) != (int)strlen(buffer)) {
            errorhandler("Sendto failed");
            closesocket(sock);
            return EXIT_FAILURE;
        }

        // Receive the generated password from the server
        int len = sizeof(serverAddr);
        int bytesReceived = recvfrom(sock, buffer, sizeof(buffer) - 1, 0, (struct sockaddr *)&serverAddr, &len);
        if (bytesReceived < 0) {
            errorhandler("Recvfrom failed");
            closesocket(sock);
            return EXIT_FAILURE;
        }

        // Null-terminate the received data
        buffer[bytesReceived] = '\0';

        // Print the received password
        printf("Received password: %s\n", buffer);
    }

    ClearWinSock();
    closesocket(sock);
    return EXIT_SUCCESS;
}
