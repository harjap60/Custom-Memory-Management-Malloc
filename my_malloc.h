
#include <stdlib.h>
#include <stddef.h>

void* my_malloc(size_t size);

void my_free(void *ptr);

void mem_init();


int mem_holes();
int mem_allocated();
int mem_free();
int mem_total();
int mem_largest_free();
int mem_small_free(int size);
char mem_is_alloc(void *ptr);
void* mem_pool();
void print_memory();
void print_memory_status();
void try_mymem(int argc, char **argv);