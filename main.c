#include "headers.h"

int main(int argc, char* argv[])
{
    if(argc<2)
    {
        printf("you need to have the URL as the arguement");
        exit(1);
    }
    if(strlen(argv[1])<8)
    {
        printf("URL not of sufficient size\n");
        exit(1);
    }

    char ip[100];
    char url[url_max];
    strcpy(url,argv[1]);
    char protocol[20];
    char hostname[hostname_max];
    char path[path_max]="";
    sscanf(url,"%[^:/]://%[^/]/%s",protocol,hostname,path);
    printf("protocol : %s, hostname : %s, dest: %s\n",protocol,hostname,path);

    ip_address(hostname,ip);
    printf("ip address is : %s\n",ip);   

    if(strcmp("http",protocol)==0)
    {
        http_handler(hostname,80,ip,path);
        printf("file has been saved!\n");
    }
    else if(strcmp("https",protocol)==0)
    {
        https_handler(hostname,443,path,ip);
        printf("file has been saved!\n");
    }
    else
        printf("It seems the url is wrong!\n");

    return 0;
}