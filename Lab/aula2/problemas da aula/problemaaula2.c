#include <stdio.h>
#include <stdlib.h>

int main() {
    double numero;
    int n, potencia = 1;

    printf("Digite um número real positivo: ");
    if (scanf("%lf", &numero) != 1 || numero <= 0) {
        printf("Entrada inválida. Digite ser um número real positivo.\n");
        return 1;
    }

    printf("Digite um valor para n (1 a 7): ");
    if (scanf("%d", &n) != 1 || n < 1 || n > 7) {
        printf("Entrada inválida. n deve estar entre 1 e 7.\n");
        return 1;
    }

     for (int i = 0; i < n; i++) {
        potencia *= 10;
    }

    int numero_arredondado = (int) (numero / potencia + 0.5) * potencia;

    printf("Número arredondado: %d\n", numero_arredondado);

    return 0;
}