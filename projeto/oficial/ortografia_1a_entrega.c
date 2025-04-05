#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAXIMO_PALAVRAS 431102
#define MAXIMO_TAMANHO_DE_PALAVRAS 1000
#define MAXIMAS_LINHAS 400

typedef struct {
    char palavras[MAXIMO_PALAVRAS][MAXIMO_TAMANHO_DE_PALAVRAS];
    size_t count;
} dicionario;

dicionario d;

void load_dicionario(const char *fname) {
    FILE *word = fopen(fname, "r");
    if (!word) {
        perror("Erro ao abrir o dicionario");
        exit(EXIT_FAILURE);
    }
    
    d.count = 0;
    
    while (d.count < MAXIMO_PALAVRAS && fgets(d.palavras[d.count], MAXIMO_TAMANHO_DE_PALAVRAS, word)) {
        d.palavras[d.count][strcspn(d.palavras[d.count], "\n")] = '\0';
        d.count++;
    }
    fclose(word);
}

int palavra_no_dicionario(dicionario *d, const char *word) {
    for (size_t i = 0; i < d->count; i++) {
        if (strcasecmp(d->palavras[i], word) == 0) {
            return 1;
        }
    }
    return 0;
}

void process_text(dicionario *d) {
    int try = 0;
    char line[MAXIMO_TAMANHO_DE_PALAVRAS];
    int line_number = 0;
    while (fgets(line, sizeof(line), stdin)) {
        line_number++;
        char *token = strtok(line, " \t.,!?\"'()-;:1234567890\n");
        int has_error = 0;
        while (token) {
            if (!palavra_no_dicionario(d, token)) {
                if (!has_error) {
                    has_error = 1;
                    printf("%d: %s\n", line_number, line);
                }
                printf("Erro na palavra \"%s\"\n", token);
            }
            token = strtok(NULL, " \t.,!?\"'()-;:1234567890\n");
        }
    }
}

int main(int argc, char *argv[]) {
    const char *d_file = "words";
    if (argc > 1) {
        d_file = argv[1];
    }
    load_dicionario(d_file);
    printf("Escreve uma frase para analizar:\n");
    process_text(&d); 
    
    return EXIT_SUCCESS;
}