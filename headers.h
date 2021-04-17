#ifndef __HEADERS
#define __HEADERS

#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/ssl.h>
#include <ctype.h>



#define url_max 1200
#define hostname_max 500
#define path_max 500


void http_handler(char* hostname, int port, char* ip, char* path);
int ParseHeader(int sock, char* type);
int create_connection(char* ip, int port);
void ip_address(char* hostname, char *ip);
int HTTPS_ParseHeader(SSL* ssl, char* type);
void https_handler(char* hostname, int port, char* path, char* ip);
int create_socket();


#endif