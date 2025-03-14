#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_PALAVRAS 431102
#define MAX_TAMANHO_PALAVRA 128
#define MAX_LINHAS 1024

typedef struct {
    char palavras[MAX_PALAVRAS][MAX_TAMANHO_PALAVRA];
    size_t count;
} dicionario;

dicionario *load_dicionario(const char *filename) {
    printf("Loading dictionary from file: %s\n", filename);
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
    
    while (dict->count < MAX_PALAVRAS && fgets(dict->palavras[dict->count], MAX_TAMANHO_PALAVRA, file)) {
        dict->palavras[dict->count][strcspn(dict->palavras[dict->count], "\n")] = '\0';
        printf("Loaded word: %s\n", dict->palavras[dict->count]);
        dict->count++;
    }
    fclose(file);
    
    printf("Dictionary loaded with %zu words\n", dict->count);
    return dict;
}

int compare_words(const void *a, const void *b) {
    return strcasecmp(*(const char **)a, *(const char **)b);
}

int palavra_no_dicionario(dicionario *dict, const char *word) {
    printf("Checking if word \"%s\" is in the dictionary\n", word);
    for (size_t i = 0; i < dict->count; i++) {
        if (strcasecmp(dict->palavras[i], word) == 0) {
            printf("Word \"%s\" found in the dictionary\n", word);
            return 1;
        }
    }
    printf("Word \"%s\" not found in the dictionary\n", word);
    return 0;
}

void process_text(dicionario *dict) {
    char line[MAX_TAMANHO_PALAVRA];
    int line_number = 0;
    while (fgets(line, sizeof(line), stdin)) {
        line_number++;
        printf("Processing line %d: %s", line_number, line);
        char *token = strtok(line, " \t.,!?\"'()\n");
        int has_error = 0;
        while (token) {
            if (!palavra_no_dicionario(dict, token)) {
                if (!has_error) {
                    printf("%d: %s", line_number, line);
                    has_error = 1;
                }
                printf("Erro na palavra \"%s\"\n", token);
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
    printf("Starting program with dictionary file: %s\n", dict_file);
    dicionario *dict = load_dicionario(dict_file);
    process_text(dict);
    free(dict);
    printf("Program finished\n");
    
    return EXIT_SUCCESS;
}