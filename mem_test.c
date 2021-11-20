#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "my_malloc.h"

  //Test function
double test1(void* func(long unsigned int),void func2(void*)){
    mem_init();
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC,&start);

  // Testing for time, 25000 my_malloc calls
  for(int i = 0;i<25000;i++){
    int **a; 
    a = func(1);

  }
    clock_gettime(CLOCK_MONOTONIC, &end);
  
    // Program time taken to complete
    double time;
    time = (end.tv_sec - start.tv_sec) * 1e9; //Second to nanosecond
    time = (time + (end.tv_nsec - start.tv_nsec)) * 1e-9; //nano second to second with high precision
    printf("\n\nTime taken by program for running my_malloc 25000 times : %f sec \n ^^^^TEST 1 PASSED!^^^\n\n", time);
    return time;
}


void test2(void* func(long unsigned int),void func2(void*)){
    mem_init();
    // 25000 total
    int* memory1 = func(2500);
    int* memory2 = func(2500);
    int* memory3 = func(2500);
    int* memory4 = func(2500);
    int* memory5 = func(2500);
    int* memory6 = func(2500);
    int* memory7 = func(2500);
    int* memory8 = func(2500);
    int* memory9 = func(2500);
    int* memory10 =func(2500);

    if(func(2500) == NULL){
      printf("^^^^TEST 2 PASSED!^^^\n\n");
    }
    func2(memory1);
    func2(memory2);
    func2(memory3);
    func2(memory4);
    func2(memory5);
    func2(memory6);
    func2(memory7);
    func2(memory8);
    func2(memory9);
    func2(memory10);
}

void test3(void* func(long unsigned int),void func2(void*)){
    // 25000
    int* memory1 = func(2500);
    int* memory2 = func(2500);
    int* memory3 = func(2500);
    int* memory4 = func(2500);
    int* memory5 = func(2500);
    int* memory6 = func(2500);
    int* memory7 = func(2500);
    int* memory8 = func(2500);
    int* memory9 = func(2500);
    int* memory10 =func(2500);

    func2(memory2); // freeing random blocks in the list
    func2(memory8);
    func2(memory9);

    int* memory11 = func(2500); // reallocating 7500 memory
    int* memory12 = func(2500);
    int* memory13 = func(2500);

    if(func(1) != NULL){
      printf("^^^^TEST 3 FAILED!^^^\n\n");
    }

    func2(memory1);
    func2(memory11);
    func2(memory3);
    func2(memory4);
    func2(memory5);
    func2(memory6);
    func2(memory7);
    func2(memory12);
    func2(memory13);
    func2(memory10);

    if(func(25000) == NULL){
      printf("^^^^TEST 3 FAILED!^^^\n");
    } else {
      printf("^^^^TEST 3 PASSED!^^^\n");
    }
    
}

int main(void)
{
    test1(my_malloc,my_free);
    test2(my_malloc,my_free);
    test3(my_malloc,my_free);
    return 0;
}