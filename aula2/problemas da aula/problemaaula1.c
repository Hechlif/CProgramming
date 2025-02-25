#include <stdio.h>

int main() {
    int mes;
    printf("Digite o numero do mes (1-12): ");
    scanf("%d", &mes);
    
    switch (mes) {
        case 12: case 1: case 2:
            printf("O mês %d é inverno.\n", mes);
            break;
        case 3: case 4: case 5:
            printf("O mês %d é primavera.\n", mes);
            break;
        case 6: case 7: case 8:
            printf("O mês %d é verao.\n", mes);
            break;
        case 9: case 10: case 11:
            printf("O mês %d é outono.\n", mes);
            break;
        default:
            printf("Número inválido! Digite um número inteiro entre 1 e 12.\n");
    }
    
    return 0;
}
