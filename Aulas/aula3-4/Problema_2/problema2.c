#include <stdio.h>

long fatorial(int n) {
    if (n == 0 || n == 1)
        return 1;
    return n * fatorial(n - 1);
}

long combinacao(int n, int k) {
    return fatorial(n) / (fatorial(k) * fatorial(n - k));
}

int main() {
    int M;
    printf("Digite o número de linhas do Triângulo de Pascal: ");
    scanf("%d", &M);
    
    for (int n = 0; n < M; n++) {
        for (int k = 0; k <= n; k++) {
            printf("%ld ", combinacao(n, k));
        }
        printf("\n");
    }
    
    return 0;
}