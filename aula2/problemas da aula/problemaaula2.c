#include <stdio.h>
#include <stdlib.h>

void main(){
    int a;
    float n;

    printf("Digite um numero: ");
    scanf("%f", &n);
    
    printf("Quer ter quantas casas décimais? (1 - 7) : ");
    scanf("%d", &a);

    while (a > 0)
    {
        n = n * (1/10);
        a = a - 1;
    }

    printf("%f", n);
}