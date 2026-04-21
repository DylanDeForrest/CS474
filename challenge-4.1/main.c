#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdatomic.h>

int global_v = 40; 

int recurs(int n){
    if (n == 0){
        return 0;
    }

    int localVarible;
    printf("stack: %p\n", (void*)&localVarible);
    int *pp = malloc(8);
    printf("heap:  %p\n", (void*)&pp);
    return recurs(n - 1);

}

int main(){

    recurs(5);


}