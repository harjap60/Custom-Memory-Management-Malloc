all: 
	gcc -Wall -Werror -o mem_test mem_test.c my_malloc.c 

clean:
	rm mem_test