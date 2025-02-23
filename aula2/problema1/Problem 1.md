```c
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

void main(){
    int n;
    printf("Segundos a converter:");

    if (scanf("%10d", &n) != 1){
        printf("Invalid Input \n");
        exit(1);
    }
    else if (n < 0){
        printf("Invalid Input \n");
        exit(1);
    }   
        printf("%d segundos \n", n);
        int seconds = n%60;
        int minutes = (n/60)%60;
        int hours = n/3600;
        printf("%d seconds are %d hours, %d minutes e %d seconds \n", n, hours, minutes, seconds);
        n = 0;
    return;
}
```
