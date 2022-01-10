all: 
	gcc -Wall -Werror -o test mem_test.c my_malloc.c 

clean:
	rm test