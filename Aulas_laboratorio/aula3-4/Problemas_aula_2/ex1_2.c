#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    int num1, num2, resultado, resposta, opcao;
    srand(time(NULL)); // Inicializa a semente do gerador de números aleatórios

    while (1) {
        printf("Escolha o tipo de conta:\n");
        printf("1 - Multiplicação\n");
        printf("2 - Divisão inteira\n");
        printf("3 - Resto da divisão\n");
        printf("Digite -1 para sair\n");
        printf("Opção: ");
        scanf("%d", &opcao);

        if (opcao == -1) {
            printf("Programa encerrado.\n");
            break;
        }

        // Gera dois números aleatórios entre 1 e 100
        num1 = rand() % 100 + 1;
        num2 = rand() % 100 + 1;

        switch (opcao) {
            case 1: // Multiplicação
                printf("Quanto é %d multiplicado por %d?\n", num1, num2);
                resultado = num1 * num2;
                break;
            case 2: // Divisão inteira
                while (num1 % num2 != 0) {
                    num1 = rand() % 100 + 1;
                    num2 = rand() % 100 + 1;
                }
                printf("Quanto é %d dividido por %d?\n", num1, num2);
                resultado = num1 / num2;
                break;
            case 3: // Resto da divisão
                printf("Qual é o resto da divisão de %d por %d?\n", num1, num2);
                resultado = num1 % num2;
                break;
            default:
                printf("Opção inválida! Tente novamente.\n");
                continue;
        }

        while (1) {
            printf("Sua resposta (-1 para sair): ");
            scanf("%d", &resposta);
            
            if (resposta == -1) {
                printf("Programa encerrado.\n");
                return 0;
            }
            
            if (resposta == resultado) {
                printf("Correto! Vamos para outra conta.\n\n");
                break;
            } else {
                printf("Errado! Tente novamente.\n");
            }
        }
    }
    
    return 0;
}