#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINHAS 140
#define TAMANHO_LINHA 200

int comparar_nomes(const void *a, const void *b) {
    char nomeA[20], nomeB[20];
    sscanf(*(char **)a, "%*d %*s %*d %*d %19[^\n]", nomeA);
    sscanf(*(char **)b, "%*d %*s %*d %*d %19[^\n]", nomeB);
    return strcmp(nomeA, nomeB);
}

int main() {
    FILE *inicial = fopen("RunResults.txt", "r");
    if (!inicial) return 1;

    char *linhas[MAX_LINHAS], buffer[TAMANHO_LINHA];
    int count = 0;
    
    fgets(buffer, TAMANHO_LINHA, inicial);
    char *cabecalho = strdup(buffer);

    while (fgets(buffer, TAMANHO_LINHA, inicial) && count < MAX_LINHAS) {
        linhas[count++] = strdup(buffer);
    }
    fclose(inicial);

    qsort(linhas, count, sizeof(char *), comparar_nomes);

    FILE *final = fopen("RunResults_out.txt", "w");
    if (!final) return 1;

    for (int i = 0; i < count; i++) {
        fprintf(final, "%s", linhas[i]);
        free(linhas[i]);
    }
    fclose(final);
    
    return 0;
}