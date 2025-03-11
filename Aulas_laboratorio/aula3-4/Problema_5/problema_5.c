#include <stdio.h>
#include <stdbool.h>
#include <math.h>

void FlagMultipleNumbers(int prime, bool sieve[], int n) {
    // Marca os múltiplos de um número primo no vetor
    for (int multiple = prime * prime; multiple <= n; multiple += prime) {
        sieve[multiple] = false;
    }
}

void SieveOfEratosthenes(int n) {
    // Inicializa um vetor de booleanos, onde true significa que o número é primo
    bool sieve[n + 1];
    for (int i = 0; i <= n; i++) {
        sieve[i] = true; // Assume que todos os números são primos
    }
    sieve[0] = sieve[1] = false; // 0 e 1 não são primos

    for (int number = 2; number <= sqrt(n); number++) {
        if (sieve[number]) { // Se o número é primo
            FlagMultipleNumbers(number, sieve, n);
        }
    }

    // Exibe os números primos encontrados
    printf("Números primos até %d: ", n);
    for (int i = 2; i <= n; i++) {
        if (sieve[i]) {
            printf("%d ", i);
        }
    }
    printf("\n");
}

int main() {
    int n;

    // Lê um número inteiro positivo do teclado
    printf("Digite um número inteiro positivo entre 1 e 1000: ");
    scanf("%d", &n);

    if (n < 1 || n > 1000) {
        printf("Por favor, insira um número entre 1 e 1000.\n");
        return 1;
    }

    // Chama a função para encontrar números primos
    SieveOfEratosthenes(n);

    return 0;
}