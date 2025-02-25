// Escreva um programa para ler um numero real positivo e que realize o arredondamento para a n-esima potencia de 10, com n 
// introduzido pelo utilizador (n assume valores entre 1 e 7, n=1 arredonda a dezena, n=2 arredonda a centena, etc.). O valor deve 
// ficar guardado arredondado numa variavel. Use apenas o stdio.h e o stdlib.h.
// Exemplo: para os valores de 2163, 547 e 997, e supondo que se especifica n = 2, o programa deve imprimir, respectivamente, 2200
// 500 e 1000

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