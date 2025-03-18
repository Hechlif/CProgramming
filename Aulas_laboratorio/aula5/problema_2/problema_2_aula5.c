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

int carregarArquivo(const char *filename, char *linhas[]) {
    FILE *file = fopen(filename, "r");
    if (!file) return -1;

    char buffer[TAMANHO_LINHA];
    int count = 0;
    
    fgets(buffer, TAMANHO_LINHA, file);
    
    while (fgets(buffer, TAMANHO_LINHA, file) && count < MAX_LINHAS) {
        linhas[count++] = strdup(buffer);
    }
    
    fclose(file);
    return count;
}

int buscarParticipante(char *linhas[], int numLinhas, const char *nome) {
    for (int i = 0; i < numLinhas; i++) {
        if (strstr(linhas[i], nome) != NULL) {
            return i;
        }
    }
    return -1;
}

void extrairPosicoes(const char *linha, int *posGeral, int *posEscalao, char *escalao) {
    char temp[TAMANHO_LINHA];
    strcpy(temp, linha);
    
    char *token = strtok(temp, "\t");
    *posGeral = atoi(token);
    
    token = strtok(NULL, "\t");
    strcpy(escalao, token);
    
    token = strtok(NULL, "\t");
    *posEscalao = atoi(token);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Uso: %s <arquivo> <nome>\n", argv[0]);
        return 1;
    }
    
    char *linhas[MAX_LINHAS];
    int numLinhas = carregarArquivo(argv[1], linhas);
    
    if (numLinhas <= 0) {
        printf("Erro ao carregar o arquivo ou arquivo vazio\n");
        return 1;
    }
    
    const char *nome = argv[2];
    int indiceLinha = buscarParticipante(linhas, numLinhas, nome);
    
    if (indiceLinha == -1) {
        printf("Participante '%s' não encontrado\n", nome);
    } else {
        int posGeral, posEscalao;
        char escalao[10];
        
        extrairPosicoes(linhas[indiceLinha], &posGeral, &posEscalao, escalao);
        printf("Participante: %s\nPosição Geral: %d\nEscalão: %s\nPosição no Escalão: %d\n", 
               nome, posGeral, escalao, posEscalao);
    }
    
    for (int i = 0; i < numLinhas; i++) {
        free(linhas[i]);
    }
    
    return 0;
}