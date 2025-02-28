// Escreva um programa para ler o número de um mês e que imprima de seguida a estação do ano no dia 1 desse mês, 
// seguido de uma m udança de linha. Utilize o switch/case.
// Por exemplo, se o input for: o programa deve escrever apenas:
// Inverno


#include <stdio.h>

int main() {
    int mes;
    printf("Digite o número do mês (1-12): ");
    scanf("%d", &mes);
    
    switch (mes) {
        case 12: case 1: case 2:
            printf("O mês %d é inverno.\n", mes);
            break;
        case 3: case 4: case 5:
            printf("O mês %d é primavera.\n", mes);
            break;
        case 6: case 7: case 8:
            printf("O mês %d é verão.\n", mes);
            break;
        case 9: case 10: case 11:
            printf("O mês %d é outono.\n", mes);
            break;
        default:
            printf("Número inválido! Digite um número inteiro entre 1 e 12.\n");
    }
    
    return 0;
}
