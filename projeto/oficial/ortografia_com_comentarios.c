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
    char line[MAXIMO_TAMANHO_DE_PALAVRAS];
    int line_number = 0;
    while (fgets(line, sizeof(line), stdin)) {
        line_number++;
        printf("%d : %s", line_number, line); // imprime o numero da linha e a frase digitada
        char *token = strtok(line, " \t.,!?\"'()-\n");
        int has_error = 0;
        while (token) {
            if (!palavra_no_dicionario(d, token)) {
                if (!has_error) {
                    has_error = 1;
                }
                printf("Erro encontrado na palavra: \"%s\"\n", token); // imprime as palavras com erros separadamente
            }
            token = strtok(NULL, " \t.,!?\"'()-\n");
        }
    }
}

int main(int argc, char *argv[]) {
    const char *d_file = "words"; // Define o arquivo de dicionário padrão como "words"
    if (argc > 1) {
        d_file = argv[1]; // Se um arquivo de dicionário for fornecido como argumento, usa esse arquivo
    }
    load_dicionario(d_file); // Carrega o dicionário a partir do arquivo especificado
    printf("Escreve uma frase para analizar:\n"); // frase para ser analisada
    process_text(&d); // verifica se as palavras da frase estão no dicionário
    printf("Programa encerrado\n"); 
    
    return EXIT_SUCCESS; // Retorna o status de sucesso
}