#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_PALAVRAS 5000
#define MAX_TAMANHO_PALAVRA 128
#define MAX_LINHAS 1024

typedef struct {
    char palavras[MAX_PALAVRAS][MAX_TAMANHO_PALAVRA];
    size_t count;
} dicionario;

dicionario load_dicionario(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Erro ao abrir o dicionário");
        exit(EXIT_FAILURE);
    }
    
    dicionario dict;
    dict.count = 0;
    
    while (dict.count < MAX_PALAVRAS && fgets(dict.palavras[dict.count], MAX_TAMANHO_PALAVRA, file)) {
        dict.palavras[dict.count][strcspn(dict.palavras[dict.count], "\n")] = 0;
        dict.count++;
    }
    fclose(file);
    
    return dict;
}

int compare_words(const void *a, const void *b) {
    return strcasecmp((const char *)a, (const char *)b);
}

int palavra_no_dicionario(dicionario *dict, const char *word) {
    for (size_t i = 0;
        i < dict->count;
        i++) {
        if (strcasecmp(dict->palavras[i], word) == 0) {
            return 1;
        }
    }
    return 0;
}

void process_text(dicionario *dict) {
    char line[MAX_LINHAS];
    int line_number = 0;
    while (fgets(line, sizeof(line), stdin)) {
        line_number++;
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
    
    dicionario dict = load_dicionario(dict_file);
    process_text(&dict);
    
    return EXIT_SUCCESS;
}
