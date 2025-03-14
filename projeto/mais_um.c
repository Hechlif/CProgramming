#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_PALAVRAS 5000
#define MAX_TAMANHO 128
#define MAX_LINHA 1024

typedef struct {
    char palavras[MAX_PALAVRAS][MAX_TAMANHO];
    size_t total;
} Dicionario;

Dicionario carregar_dicionario(const char *ficheiro) {
    FILE *file = fopen(ficheiro, "r");
    if (!file) {
        perror("Erro ao abrir o dicionário");
        exit(EXIT_FAILURE);
    }

    Dicionario dict;
    dict.total = 0;

    while (dict.total < MAX_PALAVRAS && fgets(dict.palavras[dict.total], MAX_TAMANHO, file)) {
        dict.palavras[dict.total][strcspn(dict.palavras[dict.total], "\n")] = '\0';
        dict.total++;
    }

    fclose(file);

    qsort(dict.palavras, dict.total, MAX_TAMANHO, (int (*)(const void *, const void *))strcasecmp);

    return dict;
}


int existe_palavra(const Dicionario *dict, const char *palavra) {
    return bsearch(&palavra, dict->palavras, dict->total, MAX_TAMANHO, 
                   (int (*)(const void *, const void *))strcasecmp) != NULL;
}


void verificar_texto(const Dicionario *dict) {
    char linha[MAX_LINHA];
    int num_linha = 0;

    while (fgets(linha, sizeof(linha), stdin)) {
        num_linha++;
        char *token = strtok(linha, " \t.,!?\"'()\n");
        int erro_encontrado = 0;

        while (token) {
            if (!existe_palavra(dict, token)) {
                if (!erro_encontrado) {
                    printf("Linha %d: %s", num_linha, linha);
                    erro_encontrado = 1;
                }
                printf("  -> Palavra desconhecida: \"%s\"\n", token);
            }
            token = strtok(NULL, " \t.,!?\"'()\n");
        }
    }
}

int main(int argc, char *argv[]) {
    const char *ficheiro_dict = "words";

    if (argc > 1) {
        ficheiro_dict = argv[1];
    }

    Dicionario dict = carregar_dicionario(ficheiro_dict);
    verificar_texto(&dict);

    return EXIT_SUCCESS;
}
