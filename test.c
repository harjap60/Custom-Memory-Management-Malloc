#include <stdio.h>
#include <time.h>
#include <stdlib.h>
int main(void)
{

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC,&start);

    //Test for built in malloc and free
    int **a = malloc(20000*sizeof(*a));
    for(int i = 0; i < 20000;i++){
        a[i] = malloc(20000*sizeof(a));
        for(int j = 0;j<20000;j++){
            a[i][j] = j;
        }
    }

    for(int i = 0;i<20000;i++){
        free(a[i]);
    }
    free(a);

 //Test for running the same program but no memory accesses
// int a= 2;
//     for(int i = 0; i < 20000;i++){
//         a = 3;
//         for(int j = 0;j<20000;j++){
//             a = 4;
//         }
//     }

//     for(int i = 0;i<20000;i++){
//         a = 6;
//     }
//     a= 7;


    clock_gettime(CLOCK_MONOTONIC, &end);
  
    // Program time taken to complete
    double time;
    time = (end.tv_sec - start.tv_sec) * 1e9; //Second to nanosecond
    time = (time + (end.tv_nsec - start.tv_nsec)) * 1e-9; //nano second to second with high precision
    printf("Time taken by program is : %f sec \n", time);

    return 0;
}