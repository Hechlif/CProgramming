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

dicionario *load_dicionario(const char *fname) {
    printf("A fazer loading do dicionário através do ficheiro: %s\n", fname);
    FILE *f = fopen(fname, "r");
    if (!f) {
        perror("Erro ao abrir o dicionário");
        return NULL;
    }
    
    dicionario *d = malloc(sizeof(dicionario));
    if (!d) {
        perror("Erro ao alocar memória para o dicionário");
        return NULL;
    }
    d->count = 0;
    
    while (d->count < MAXIMO_PALAVRAS && fgets(d->palavras[d->count], MAXIMO_TAMANHO_DE_PALAVRAS, f)) {
        d->palavras[d->count][strcspn(d->palavras[d->count], "\n")] = '\0';
        d->count++;
    }
    fclose(f);
    
    printf("Dicionário carregado com %zu palavras\n", d->count);
    return d;
}

int palavra_no_dicionario(dicionario *d, const char *word) {
    for (size_t i = 0; i < d->count; i++) {
        if (strcasecmp(d->palavras[i], word) == 0) {
            printf("\"%s\" foi encontrado no dicionário\n", word);
            return 1;
        }
    }
    printf("\"%s\" não foi encontrado no dicionário\n", word);
    return 0;
}

void process_text(dicionario *d) {
    char line[MAXIMO_TAMANHO_DE_PALAVRAS];
    int line_number = 0;
    while (fgets(line, sizeof(line), stdin)) {
        line_number++;
        char *token = strtok(line, " \t.,!?\"'()\n");
        int has_error = 0;
        while (token) {
            if (!palavra_no_dicionario(d, token)) {
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
    const char *d_file = "words";
    if (argc > 1) {
        d_file = argv[1];
    }
    dicionario *d = load_dicionario(d_file);
    process_text(d);
    free(d);
    printf("Program finished\n");
    
    return EXIT_SUCCESS;
}