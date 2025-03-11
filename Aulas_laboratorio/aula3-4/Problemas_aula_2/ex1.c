// Escrever um programa em C que ajude a aprendizagem da tabuada para alunos do 1º ciclo. Use a
// função rand() para gerar dois inteiros entre 1 e 9, que são mostrados ao aluno. O aluno deverá introduzir o
// resultado e este deverá ser comparado com a solução correta. O programa deverá permitir que o aluno faça várias
// tentativas, até acertar, e em cada tentativa indicar se o aluno acertou ou errou. O programa apenas deve sair
// quando o aluno introduzir o número zero.

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    int numero1, numero2, resposta, correta;

    srand(time(NULL));
    
    while (1) {
        numero1 = rand() % 9 + 1;
        numero2 = rand() % 9 + 1;
        correta = numero1 * numero2;
        
        printf("Quanto é %d x %d? (Digite 0 para sair do programa)\n", numero1, numero2);
        
        while (1) {
            printf("Sua resposta: ");
            scanf("%d", &resposta);
            
            if (resposta == 0) {
                printf("A sair do programa. Bye bye!\n");
                return 0;
            }
            
            if (resposta == correta) {
                printf("A resposta está correta. Bom trabalho!\n\n");
                break;
            } else {
                printf("Burro de merda não sabes a tabuada. Tenta outra vez caralho\n");
            }
        }
    }
    
    return 0;
}


