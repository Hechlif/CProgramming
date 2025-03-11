#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    int num1, num2, resultado, resposta, opcao, total_perguntas = 0, respostas_corretas = 0;
    srand(time(NULL)); 

    while (1) {
        opcao = rand() % 3 + 1;

        num1 = rand() % 100 + 1;
        num2 = rand() % 100 + 1;

        switch (opcao) {
            case 1: 
                printf("Quanto é %d multiplicado por %d?\n", num1, num2);
                resultado = num1 * num2;
                break;
            case 2: 
                while (num1 % num2 != 0) {
                    num1 = rand() % 100 + 1;
                    num2 = rand() % 100 + 1;
                }
                printf("Quanto é %d dividido por %d?\n", num1, num2);
                resultado = num1 / num2;
                break;
            case 3:
                printf("Qual é o resto da divisão de %d por %d?\n", num1, num2);
                resultado = num1 % num2;
                break;
        }

        total_perguntas++;

        while (1) {
            printf("Sua resposta (-1 para sair): ");
            scanf("%d", &resposta);
            
            if (resposta == -1) {
                float percentagem = (total_perguntas == 0) ? 0 : ((float)respostas_corretas / total_perguntas) * 100;
                printf("Programa encerrado. Você acertou %d de %d perguntas (%.2f%%).\n", respostas_corretas, total_perguntas, percentagem);
                return 0;
            }
            
            if (resposta == resultado) {
                printf("Correto! Vamos para outra conta.\n\n");
                respostas_corretas++;
                break;
            } else {
                printf("Errado! Tente novamente.\n");
            }
        }
    }
    
    return 0;
}
