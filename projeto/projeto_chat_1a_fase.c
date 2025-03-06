// 1a parte do projeto dado pelo chat, inspira-te hahaha ns se ta certo, ainda tou a tentar aprender umas merdas
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORDS 100000   // Número máximo de palavras no dicionário
#define MAX_WORD_LEN 100    // Tamanho máximo de uma palavra
#define MAX_LINE_LEN 1000   // Tamanho máximo de uma linha de texto

char *dictionary[MAX_WORDS]; // Vetor para armazenar palavras do dicionário
int dict_size = 0;           // Número atual de palavras no dicionário

// Função de comparação para ordenação do dicionário (ignora maiúsculas/minúsculas)
int compare(const void *a, const void *b) {
    return strcasecmp(*(const char **)a, *(const char **)b);
}

// Função para carregar o dicionário a partir de um ficheiro
int load_dictionary(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Erro ao abrir dicionário");
        return 0;
    }
    char word[MAX_WORD_LEN];
    
    // Lê palavras do ficheiro e armazena no vetor do dicionário
    while (fscanf(file, "%99s", word) == 1 && dict_size < MAX_WORDS) {
        dictionary[dict_size] = strdup(word); // Aloca memória e armazena a palavra
        dict_size++;
    }
    fclose(file);
    
    // Ordena o dicionário alfabeticamente para permitir busca binária
    qsort(dictionary, dict_size, sizeof(char *), compare);
    return 1;
}

// Função para verificar se uma palavra existe no dicionário usando busca binária
int is_in_dictionary(const char *word) {
    return bsearch(&word, dictionary, dict_size, sizeof(char *), compare) != NULL;
}

// Função para processar o texto de entrada e verificar palavras erradas
void check_text() {
    char line[MAX_LINE_LEN]; // Buffer para armazenar uma linha do texto
    int line_num = 1;        // Contador de linhas
    
    // Lê o texto do stdin linha por linha
    while (fgets(line, sizeof(line), stdin)) {
        char *word = strtok(line, " .,;!?\n\t"); // Tokeniza a linha separando palavras
        int first_error = 1; // Flag para imprimir a linha apenas uma vez
        
        while (word) {
            // Verifica se a palavra não está no dicionário
            if (!is_in_dictionary(word)) {
                if (first_error) {
                    printf("%d: %s", line_num, line); // Imprime a linha original com erro
                    first_error = 0;
                }
                printf("Erro na palavra \"%s\"\n", word); // Indica a palavra errada
            }
            word = strtok(NULL, " .,;!?\n\t"); // Obtém a próxima palavra
        }
        line_num++; // Incrementa o contador de linhas
    }
}

// Função principal para gerenciar argumentos e executar o programa
int main(int argc, char *argv[]) {
    // Verifica se o nome do ficheiro do dicionário foi fornecido
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <dicionário>\n", argv[0]);
        return EXIT_FAILURE;
    }
    
    // Carrega o dicionário
    if (!load_dictionary(argv[1])) {
        return EXIT_FAILURE;
    }
    
    // Verifica a ortografia do texto de entrada
    check_text();
    
    return EXIT_SUCCESS;
}
