#include "headers.h"


int create_socket()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd== -1)
    {
        perror("Socket Creation Failed!");
        exit(0);
    }
    printf("Socket Creation Successful!\n");

    return sockfd;
}

int create_connection(char* ip, int port)
{
    int sockfd;
    struct sockaddr_in servaddr;

    if((sockfd = socket(AF_INET, SOCK_STREAM, 0))<0)
    {
        printf("error in creating a socket.\n");
        exit(2);
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    servaddr.sin_addr.s_addr = inet_addr(ip);

    if(connect(sockfd,(struct sockaddr*)&servaddr, sizeof(servaddr))<0)
    {
        // perror("Connect");
        printf("error connecting to server!\n");
        exit(3);
    }

    return sockfd;

}

void ip_address(char* hostname, char *ip)
{
    struct addrinfo hints, *results, *p;
    struct sockaddr_in *ip_access;
    int err1;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; 
    hints.ai_socktype = SOCK_STREAM;

    if((err1=getaddrinfo(hostname,"domain",&hints,&results))!=0) 
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(err1));
        exit(1);
    }
    for(p = results;p != NULL;p = p->ai_next) 
    {
        ip_access = (struct sockaddr_in *) p->ai_addr;
        if(strcmp("0.0.0.0",inet_ntoa( ip_access->sin_addr ))!=0)
        {
            strcpy(ip,inet_ntoa( ip_access->sin_addr));
        }
    }
    freeaddrinfo(results);
}