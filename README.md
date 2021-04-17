# Wget clone

C program to perform GET requests with HTTP and HTTPS protocol. 

## Required packages
We need to install openssl in order to make HTTPS requests.
```bash
sudo apt-get install libssl-dev
```

## Compilation

The files can be compiled traditionally using gcc
```bash
gcc -c http.c 
gcc -c https.c 
gcc -c common.c 
gcc -o main.out main.c http.o https.o common.o -lssl -lcrypto 
```
The same can be achieved using the makefile
```bash
make
```

## Usage

```bash
./main.out https://i.insider.com/602ee9ced3ad27001837f2ac > log.txt
#output of stdout piped to log.txt for better visibility
```
Files appear in the /Downloads/ directory

## Other Shortcuts 

```bash
make clean #removing the binary files
make clean_downloads #cleaning the downloads directory 
make test_http_pdf #testcase for http which is a pdf
make test_https_pdf #testcase for https which is a pdf
make test_https_img #testcase for https which is an image
```

##Footnotes

Code may be buggy so opening issues are welcome.