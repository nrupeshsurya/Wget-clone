#include "headers.h"

int HTTPS_ParseHeader(SSL* ssl, char* type)
{
    char c;
    char data[10000]="",*ptr=data+4;
    int bytes_received, status;
    printf("\n\nHeader: \n\n");
    while(bytes_received = SSL_read(ssl, ptr, 1))
    {
        if(bytes_received==-1)
        {
            perror("Parse Header");
            exit(1);
        }
        *ptr = tolower(*ptr);
        printf("%c",*ptr);
        fflush(stdout);
        if((ptr[-3]=='\r') && (ptr[-2]=='\n') && (ptr[-1]=='\r') && (*ptr=='\n')) 
            break;
        ptr++;
    }
    printf("Header end\n\n");
    *ptr=0;
    ptr=data+4;
    if(bytes_received)
    {
        
        ptr=strstr(ptr,"content-type: ");
        if(ptr){
            sscanf(ptr,"content-type: %[^; \r\n]",type);
        }
        printf("content-type: %s\n",type);

        *ptr=0;
        ptr=data+4;
        ptr=strstr(ptr,"content-length:");
        if(ptr!=NULL)
        {
            sscanf(ptr,"content-length: %d",&bytes_received);
        }
        else
        {
            bytes_received=0;
        }
       printf("content-length: %d\n",bytes_received);
    }
    return bytes_received;
}

void https_handler(char* hostname, int port, char* path, char* ip)
{
    SSL_library_init();
    SSL* ssl;
    SSL_CTX* ctx;
    ctx = SSL_CTX_new(SSLv23_client_method());
    ssl = SSL_new(ctx);
    int sockfd = create_connection(ip,port);
    printf("here!\n");
    SSL_set_fd(ssl, sockfd);
    if (SSL_connect(ssl) == -1)
    {
        ERR_print_errors_fp(stderr);
        exit(0);
    }
    char write_buf[100]="";
    snprintf(write_buf, sizeof(write_buf), "GET /%s HTTP/1.0\r\nHost: %s\r\n\r\n",path,hostname);

    if (SSL_write(ssl, write_buf, strlen(write_buf)) == -1)
    {
        perror("Error in Sending Request!");
        close(sockfd);
        exit(0);
    }

    char type[100]="";
    int size_of_file = HTTPS_ParseHeader(ssl,type);
    if(size_of_file<=0&&strcmp(type,"")==0)
    {
        printf("error in headers\n");
        exit(1);
    }
    int cloudflare = 0;
    if(size_of_file<=0)
    {
        cloudflare=1;
    }
    printf("size of body is : %d\n",size_of_file);
    sscanf(type,"%*[^/]/%s",type);
    printf("type of file: %s\n",type);

    int bytes_received;
    char main_piece[100];
    if(strlen(path)!=0)
    {
        char* piece = strtok(path,"/");
        char* piece1 = piece;
        while (piece!=NULL)
        {
            if(strcmp("",piece)!=0)
                piece1 = piece;
            printf("%s\n",piece);
            piece = strtok(NULL,"/");
        }
        strcpy(main_piece,piece1);
        char *f_ptr = strstr(main_piece,type);
        if(!f_ptr&&strcmp("jpeg",type)!=0)
        {
            strcat(main_piece,".");
            strcat(main_piece,type);
        }
    }
    else
    {
        strcpy(main_piece,"index.html");
    }
    char file_path[100] = "Downloads/";
    
    strcat(file_path,main_piece);
    char recv_data[1024];
    if(1)
    {
        int bytes=0;
        FILE* fd=fopen(file_path,"wb");
        printf("Saving data...\n\n");
        while(bytes_received = SSL_read(ssl, recv_data, 1024))
        {
            if(bytes_received==-1){
                perror("recieve");
                exit(3);
            }
            fwrite(recv_data,1,bytes_received,fd);
            bzero(recv_data,1024);
            bytes+=bytes_received;
            if(bytes==size_of_file&&!cloudflare)
                break;
        }
        fclose(fd);
    }    
}