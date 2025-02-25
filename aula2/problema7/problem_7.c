#include <stdio.h>

int main() {
    float numero;
    int n;

    printf("Digite um número real positivo: ");
    scanf("%lf", &numero);
    
    printf("Digite o número de casas decimais (de 1 a 7): ");
    scanf("%d", &n);
    
    if (n < 1 || n > 7) {
        printf("Número de casas decimais inválido!\n");
        return 1;
    }

    float fator = 1.0;
    for (int i = 0; i < n; i++) {
        fator *= 10.0;
    }
    
    numero = (int)(numero * fator + 0.5) / fator;
    
    printf("Número arredondado: %.*f\n", n, numero);
    
    return 0;
}
