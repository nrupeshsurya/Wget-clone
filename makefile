all : main.out

test_http_pdf : main.out
	./main.out http://www.science.smith.edu/~jcardell/Courses/EGR328/Readings/KRch5Link.pdf > log.txt

test_https_pdf : main.out 
	./main.out https://people.cs.umass.edu/~arun/cs453/lectures/Chapter5.pdf > log.txt

test_https_img : main.out
	./main.out https://i.insider.com/602ee9ced3ad27001837f2ac > log.txt

test_1 : main.out 
	./main.out https://git.20202060.xyz/dog.jpg > log.txt

test_2 : main.out 
	./main.out https://www.google.com > log.txt

http.o : http.c headers.h
	gcc -c http.c 

https.o : https.c headers.h
	gcc -c https.c 

common.o : common.c headers.h
	gcc -c common.c 

main.out: main.c http.o https.o common.o headers.h
	gcc -o main.out main.c http.o https.o common.o -lssl -lcrypto

clean_downloads : 
	rm Downloads/*.*

clean:
	rm -f *.o *.out