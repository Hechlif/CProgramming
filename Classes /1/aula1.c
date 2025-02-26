#include <stdio.h>
#include <stdlib.h>

int main(){
    int number = 9;

    int div = 1;

    for (div = 1; div <= number; div++){
        if (number % div == 0){
            printf("Divisor %d\n", div);
        }
    }
    return EXIT_SUCCESS;
}