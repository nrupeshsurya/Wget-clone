#include "headers.h"

int ParseHeader(int sock, char* type)
{
    char c;
    char data[4096]="",*ptr=data+4;
    int bytes_received, status;
    printf("\nHeader: \n\n");
    while(bytes_received = recv(sock, ptr, 1, 0))
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
        if(ptr)
        {
            sscanf(ptr,"content-type: %s",type);
        }   
        printf("Content-Type: %s\n",type);

         *ptr=0;
        ptr=data+4;

        ptr=strstr(ptr,"content-length:");
        if(ptr)
        {
            sscanf(ptr,"content-length: %d",&bytes_received);
        }
        else
            bytes_received=-1; //unknown size

       printf("Content-Length: %d\n",bytes_received);
        
    }
    printf("\n\n\n");
    return  bytes_received ;

}

void http_handler(char* hostname, int port, char* ip, char* path)
{
    int sockfd = create_connection(ip,port);
    char send_data[1024],recv_data[1024];
    snprintf(send_data, sizeof(send_data), "GET /%s HTTP/1.0\r\nHost: %s\r\n\r\n", path, hostname);
    
    if(send(sockfd,send_data,strlen(send_data),0)<0)
    {
        printf("error in transferring data to server");
        exit(3); 
    }

    char type[100]={0};
    // char type1[100];
    int size_of_file = ParseHeader(sockfd,type);
    if(size_of_file<=0)
    {
        perror("size");
        exit(1);
    }
    // printf("size of body is : %d\n",size_of_file);
    sscanf(type,"%*[^/]/%s",type);
    // printf("type of file: %s\n",type);

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
            // printf("%s\n",piece);
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
    if(size_of_file)
    {
        int bytes=0;
        FILE* fd=fopen(file_path,"wb");
        printf("Saving data...\n\n");

        while(bytes_received = recv(sockfd, recv_data, 1024, 0))
        {
            if(bytes_received==-1){
                perror("recieve");
                exit(3);
            }
            fwrite(recv_data,1,bytes_received,fd);
            bzero(recv_data,1024);
            bytes+=bytes_received;
            if(bytes==size_of_file)
                break;
        }
        fclose(fd);
    }
    close(sockfd);
}