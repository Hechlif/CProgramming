#include <stdio.h>

int main() {
    int mes;
    printf("Digite o número do mês (1-12): ");
    scanf("%d", &mes);
    
    switch (mes) {
        case 1: case 3: case 5: case 7: case 8: case 10: case 12:
            printf("O mês %d tem 31 dias.\n", mes);
            break;
        case 4: case 6: case 9: case 11:
            printf("O mês %d tem 30 dias.\n", mes);
            break;
        case 2:
            printf("O mês %d tem 28 dias.\n", mes);
            break;
        default:
            printf("Número inválido caralho! Digite um número entre 1 e 12.\n");
    }
    
    return 0;
}
