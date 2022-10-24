#Copyright Nioata Alexandra 312CAa 2021-2022


build: 
	gcc -Wall -Wextra -std=c99 functions.c main.c hashtable.c library.c user.c -o main
	
clean:
	rm -rf main
