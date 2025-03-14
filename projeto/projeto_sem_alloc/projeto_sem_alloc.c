#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAXIMO_PALAVRAS 431102
#define MAXIMO_TAMANHO_DE_PALAVRAS 128
#define MAXIMAS_LINHAS 1024

typedef struct {
    char palavras[MAXIMO_PALAVRAS][MAXIMO_TAMANHO_DE_PALAVRAS];
    size_t count;
} dicionario;

dicionario *load_dicionario(const char *filename) {
    printf("A fazer loading do dicionário através do ficheiro: %s\n", filename);
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Erro ao abrir o dicionário");
        exit(EXIT_FAILURE);
    }
    
    dicionario *dict = malloc(sizeof(dicionario));
    if (!dict) {
        perror("Erro ao alocar memória para o dicionário");
        exit(EXIT_FAILURE);
    }
    dict->count = 0;
    
    while (dict->count < MAXIMO_PALAVRAS && fgets(dict->palavras[dict->count], MAXIMO_TAMANHO_DE_PALAVRAS, file)) {
        dict->palavras[dict->count][strcspn(dict->palavras[dict->count], "\n")] = '\0';
        printf("Palavra carregada: %s\n", dict->palavras[dict->count]);
        dict->count++;
    }
    fclose(file);
    
    printf("Dicionário carregado com %zu palavras\n", dict->count);
    return dict;
}

int compare_words(const void *a, const void *b) {
    return strcasecmp(*(const char **)a, *(const char **)b);
}

int palavra_no_dicionario(dicionario *dict, const char *word) {
    for (size_t i = 0; i < dict->count; i++) {
        if (strcasecmp(dict->palavras[i], word) == 0) {
            printf("\"%s\" foi encontrado no dicionário\n", word);
            return 1;
        }
    }
    printf("\"%s\" não foi encontrado no dicionário\n", word);
    return 0;
}

void process_text(dicionario *dict) {
    char line[MAXIMO_TAMANHO_DE_PALAVRAS];
    int line_number = 0;
    while (fgets(line, sizeof(line), stdin)) {
        line_number++;
        char *token = strtok(line, " \t.,!?\"'()\n");
        int has_error = 0;
        while (token) {
            if (!palavra_no_dicionario(dict, token)) {
                if (!has_error) {
                    printf("%d: %s \n", line_number, line);
                    has_error = 1;
                }
                printf("Erro encontrado na palavra: \"%s\"\n", token);
            }
            token = strtok(NULL, " \t.,!?\"'()\n");
        }
    }
}

int main(int argc, char *argv[]) {
    const char *dict_file = "words";
    if (argc > 1) {
        dict_file = argv[1];
    }
    dicionario *dict = load_dicionario(dict_file);
    process_text(dict);
    free(dict);
    printf("Program finished\n");
    
    return EXIT_SUCCESS;
}