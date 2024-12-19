/*
 ============================================================================
 Name        : udp_server.c
 Author      : Gabriele Grasso
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#ifndef UDP_PROTOCOL_H_
#define UDP_PROTOCOL_H_

#define MAX_PASSWORD_LENGTH 32     // max password length
#define MIN_PASSWORD_LENGTH 6      // min password length

#define BUFFER_SIZE 255            // Define buffer size for communication between client and server
#define PORT 12345                 // Define port number that the server will listen on
#define SERVER "passwdgen.uniba.it" // Define server address

// Password types
#define PASSWORD_NUMERIC 'n'
#define PASSWORD_ALPHA 'a'
#define PASSWORD_MIXED 'm'
#define PASSWORD_SECURE 's'
#define PASSWORD_UNAMBIGUOUS 'u'
#define PASSWORD_HELP 'h'
#define PASSWORD_QUIT 'q'

// Password generation functions declarations
void generate_numeric(char *buffer, int length);
void generate_alpha(char *buffer, int length);
void generate_mixed(char *buffer, int length);
void generate_secure(char *buffer, int length);
void generate_unambiguous(char *buffer, int length);
#endif /* UDP_PROTOCOL_H_ */
