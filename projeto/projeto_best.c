#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_PALAVRAS 50000
#define MAX_LINE 1024

typedef struct {
    char **palavras;
    size_t count;
} dictionary;

dictionary *load_dictionary(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Erro ao abrir o dicionário");
        return NULL;
    }
    
    dictionary *dict = malloc(sizeof(dictionary));
    dict->palavras = malloc(MAX_PALAVRAS * sizeof(char *));
    dict->count = 0;
    
    char buffer[128];
    while (fgets(buffer, sizeof(buffer), file) && dict->count < MAX_PALAVRAS) {
        buffer[strcspn(buffer, "\n")] = 0;
        dict->palavras[dict->count] = strdup(buffer);
        dict->count++;
    }
    fclose(file);
    
    return dict;
}

int compare_words(const void *a, const void *b) {
    return strcasecmp(*(const char **)a, *(const char **)b);
}

int palavra_no_dicionario(dictionary *dict, const char *word) {
    return bsearch(&word, dict->palavras, dict->count, sizeof(char *), compare_words) != NULL;
}

void process_text(dictionary *dict) {
    char line[MAX_LINE];
    int line_number = 0;
    while (fgets(line, sizeof(line), stdin)) {
        line_number++;
        char *token = strtok(line, " \t.,!?\"\'()\n");
        int has_error = 0;
        while (token) {
            if (!palavra_no_dicionario(dict, token)) {
                if (!has_error) {
                    printf("%d: %s", line_number, line);
                    has_error = 1;
                }
                printf("Erro na palavra \"%s\"\n", token);
            }
            token = strtok(NULL, " \t.,!?\"\'()\n");
        }
    }
}

int main(int argc, char *argv[]) {
    const char *dict_file = "words";
    if (argc > 1) {
        dict_file = argv[1];
    }
    
    dictionary *dict = load_dictionary(dict_file);
    if (!dict) return EXIT_FAILURE;
    
    qsort(dict->palavras, dict->count, sizeof(char *), compare_words);
    process_text(dict);
    
    for (size_t i = 0;
        i < dict->count;
         i++) {
        free(dict->palavras[i]);
    }
    free(dict->palavras);
    free(dict);
    
    return EXIT_SUCCESS;
}

// como posso adicionar um dicionario ao codigo