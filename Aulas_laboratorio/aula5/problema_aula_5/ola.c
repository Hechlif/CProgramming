#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINHAS 140
#define TAMANHO_LINHA 200

int order;

int comparar_nomes(const void *a, const void *b) {
    char nomeA[20], nomeB[20];
    sscanf(*(char **)a, "%*d %*s %*d %*d %19[^]", nomeA);
    sscanf(*(char **)b, "%*d %*s %*d %*d %19[^]", nomeB);
    return (order == 1) ? strcmp(nomeA, nomeB) : strcmp(nomeB, nomeA);
}

int main() {
    printf("Escolha a ordem de ordenação:\n");
    printf("1 - Crescente\n2 - Decrescente\n");
    printf("Digite sua escolha: ");
    
    if (scanf("%d", &order) != 1 || (order != 1 && order != 2)) {
        fprintf(stderr, "Opção inválida! Use '1' para crescente ou '2' para decrescente.\n");
        return 1;
    }
    
    FILE *inicial = fopen("RunResults.txt", "r");
    if (!inicial)
        return 1;

    char *linhas[MAX_LINHAS], buffer[TAMANHO_LINHA];
    int count = 0;
    
    while (fgets(buffer, TAMANHO_LINHA, inicial) && count < MAX_LINHAS) {
        linhas[count++] = strdup(buffer);
    }
    fclose(inicial);

    qsort(linhas, count, sizeof(char *), comparar_nomes);

    FILE *final = fopen("RunResults_out.txt", "w");
    if (!final) {
        fprintf(stderr, "Erro ao criar o arquivo RunResults_out.txt\n");
        return 1;
    }
    
    for (int i = 0; i < count; i++) {
        fprintf(final, "%s", linhas[i]);
        free(linhas[i]);
    }
    fclose(final);
    
    return 0;
}
