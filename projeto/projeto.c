#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define maximo_palavras 100000
#define maximo_linhas_texto 1024

// Vetor para armazenar palavras do dicionário; Número de palavras no dicionário (atualmente, antes de)//
char *dictionary[maximo_palavras];
int dict_size = 0;

void load_dictionary(const char *filename) {
    FILE *file = fopen(filename, "r"); // Abre o ficheiro em modo de leitura//
    if (!file) {
        perror("Erro ao abrir o dicionário. Verifique se o ficheiro existe e tem permissões de leitura.");
        exit(EXIT_FAILURE);
 
   }
}