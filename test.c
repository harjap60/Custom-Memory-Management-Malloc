#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "my_malloc.h"

//Test function
double test(void* func(long unsigned int),void func2(void*)){

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC,&start);

  
  for(int i = 0;i<500000;i++){
   // Test for malloc and free
    int **a = func(100);
    for(int i = 0; i < 10;i++){
        a[i] = func(150);
        for(int j = 0;j<15;j++){
            a[i][j] = j;
        }
    }

    for(int i = 0;i<10;i++){
        func2(a[i]);
    }
    func2(a);
  }

    // for(int i = 0;i<10000;i++){
    //     func(1);
    // }
    
    clock_gettime(CLOCK_MONOTONIC, &end);
  
    // Program time taken to complete
    double time;
    time = (end.tv_sec - start.tv_sec) * 1e9; //Second to nanosecond
    time = (time + (end.tv_nsec - start.tv_nsec)) * 1e-9; //nano second to second with high precision
    printf("Time taken by program is : %f sec \n", time);
    return time;
}





int main(void)
{
    printf("At first we will be testing the execution time for the built in malloc and free with many calls and memory accesses\n");
    double t1 = test(malloc,free);
    double t2 = test(malloc,free);
    double t3 = test(malloc,free);
    
    double avg1 = (t1+t2+t3)/3;
    printf("The average run for the built in malloc and free is %f sec\n",avg1);

    mem_init();
    printf("Now we will be testing the execution time for our implementation of my_malloc and my_free with many calls and memory accesses\n");
     t1 = test(my_malloc,my_free);
     t2 = test(my_malloc,my_free);
     t3 = test(my_malloc,my_free);
    
     avg1 = (t1+t2+t3)/3;
    printf("The average run for our malloc and free is %f sec\n",avg1);




    return 0;
}