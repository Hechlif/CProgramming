#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINHAS 140
#define TAMANHO_LINHA 200

const char *ordem_escalao[] = {"F20", "F40", "F45", "M20", "M35", "M40", "M45", "M50", "M55", "M60"};

int obter_posicao_escalao(const char *escalao) {
    for (int i = 0; i < 10; i++) {
        if (strcmp(escalao, ordem_escalao[i]) == 0)
            return i;
    }
    return 10;
}

int comparar_escaloes(const void *a, const void *b) {
    char escalaoA[10], escalaoB[10];
    sscanf(*(char **)a, "%*d %s", escalaoA);
    sscanf(*(char **)b, "%*d %s", escalaoB);
    return obter_posicao_escalao(escalaoA) - obter_posicao_escalao(escalaoB);
}

int main() {
    FILE *inicial = fopen("RunResults.txt", "r");
    if (!inicial) return 1;

    char *linhas[MAX_LINHAS], buffer[TAMANHO_LINHA];
    int count = 0;
    
    fgets(buffer, TAMANHO_LINHA, inicial);

    while (fgets(buffer, TAMANHO_LINHA, inicial) && count < MAX_LINHAS) {
        linhas[count++] = strdup(buffer);
    }
    fclose(inicial);

    qsort(linhas, count, sizeof(char *), comparar_escaloes);

    FILE *final = fopen("RunResults_out.txt", "w");
    if (!final) return 1;
    
    for (int i = 0; i < count; i++) {
        fprintf(final, "%s", linhas[i]);
        free(linhas[i]);
    }
    fclose(final);
    
    return 0;
}
