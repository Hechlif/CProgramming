#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAXIMO_TAMANHO_DE_PALAVRAS 1000
#define MAXIMO_ALTERNATIVAS 10
#define MAXIMO_DIFERENCAS 2

typedef struct {
    char **palavras; // Ponteiro para vetor de strings (memória dinâmica)
    size_t count;
    size_t capacity; // Capacidade atual do vetor
} dicionario;

typedef struct {
    char palavra[MAXIMO_TAMANHO_DE_PALAVRAS];
    int diferencas;
} alternativa;

dicionario d;

void inicializa_dicionario(dicionario *d) {
    d->palavras = NULL;
    d->count = 0;
    d->capacity = 0;
}

void libera_dicionario(dicionario *d) {
    for (size_t i = 0; i < d->count; i++) {
        free(d->palavras[i]);
    }
    free(d->palavras);
}

void adiciona_palavra(dicionario *d, const char *word) {
    if (d->count == d->capacity) {
        d->capacity = (d->capacity == 0) ? 10 : d->capacity * 2;
        d->palavras = realloc(d->palavras, d->capacity * sizeof(char *));
        if (!d->palavras) {
            perror("Erro ao alocar memória");
            exit(EXIT_FAILURE);
        }
    }
    d->palavras[d->count] = strdup(word);
    if (!d->palavras[d->count]) {
        perror("Erro ao duplicar palavra");
        exit(EXIT_FAILURE);
    }
    d->count++;
}

// Função de comparação para qsort
int compara_palavras(const void *a, const void *b) {
    return strcasecmp(*(const char **)a, *(const char **)b);
}

void load_dicionario(dicionario *d, const char *fname) {
    FILE *word = fopen(fname, "r");
    if (!word) {
        perror("Erro ao abrir o dicionário");
        exit(EXIT_FAILURE);
    }
    char buffer[MAXIMO_TAMANHO_DE_PALAVRAS];
    while (fgets(buffer, sizeof(buffer), word)) {
        // Remove quebra de linha e caracteres após '/'
        char *pos = strchr(buffer, '/');
        if (pos) {
            *pos = '\0';
        }
        // Remove quebra de linha
        buffer[strcspn(buffer, "\n")] = '\0';
        // Remove espaços em branco no final
        pos = buffer + strlen(buffer) - 1;
        while (pos >= buffer && isspace(*pos)) {
            *pos = '\0';
            pos--;
        }
        
        if (strlen(buffer) > 0) {
            adiciona_palavra(d, buffer);
        }
    }
    fclose(word);
    
    // Ordena o dicionário alfabeticamente ignorando maiúsculas/minúsculas
    qsort(d->palavras, d->count, sizeof(char*), compara_palavras);
}

int palavra_no_dicionario(dicionario *d, const char *word) {
    // Usa pesquisa binária (mais eficiente com o dicionário ordenado)
    char *key = strdup(word);
    if (!key) {
        perror("Erro ao duplicar palavra para busca");
        exit(EXIT_FAILURE);
    }
    
    char **result = bsearch(&key, d->palavras, d->count, sizeof(char*), compara_palavras);
    free(key);
    
    return result != NULL;
}

// Função para extrair uma substring
char* substring(const char* str, int start, int end) {
    int len = end - start;
    char* substr = (char*)malloc(len + 1);
    if (!substr) {
        perror("Erro ao alocar memória para substring");
        exit(EXIT_FAILURE);
    }
    strncpy(substr, str + start, len);
    substr[len] = '\0';
    return substr;
}

// Implementação do algoritmo de diferenças conforme especificado no PDF
int calcular_diferencas_pdf(const char *palavra_errada, const char *palavra_dic, int max_diferenca) {
    int diferencas = 0;
    size_t len_errada = strlen(palavra_errada);
    size_t len_dic = strlen(palavra_dic);
    
    // Cria cópias minúsculas para comparação
    char errada_lower[MAXIMO_TAMANHO_DE_PALAVRAS];
    char dic_lower[MAXIMO_TAMANHO_DE_PALAVRAS];
    
    strcpy(errada_lower, palavra_errada);
    strcpy(dic_lower, palavra_dic);
    
    for (size_t i = 0; i < len_errada; i++) {
        errada_lower[i] = tolower(errada_lower[i]);
    }
    
    for (size_t i = 0; i < len_dic; i++) {
        dic_lower[i] = tolower(dic_lower[i]);
    }
    
    // 1. Percorrendo do início para o fim
    size_t i = 0, j = 0;
    diferencas = 0;
    
    while (i < len_errada && j < len_dic) {
        if (errada_lower[i] != dic_lower[j]) {
            // Primeira letra diferente
            size_t offset = max_diferenca;
            
            // Tenta saltar offset caracteres na palavra errada
            if (i + offset <= len_errada) {
                size_t temp_i = i + offset;
                size_t temp_j = j;
                int temp_dif = offset;
                
                while (temp_i < len_errada && temp_j < len_dic) {
                    if (errada_lower[temp_i] != dic_lower[temp_j]) {
                        temp_dif++;
                    }
                    temp_i++;
                    temp_j++;
                }
                
                // Adiciona caracteres restantes como diferenças
                temp_dif += (len_errada - temp_i) + (len_dic - temp_j);
                
                if (temp_dif <= max_diferenca) {
                    return temp_dif;
                }
            }
            
            // Tenta saltar offset caracteres na palavra do dicionário
            if (j + offset <= len_dic) {
                size_t temp_i = i;
                size_t temp_j = j + offset;
                int temp_dif = offset;
                
                while (temp_i < len_errada && temp_j < len_dic) {
                    if (errada_lower[temp_i] != dic_lower[temp_j]) {
                        temp_dif++;
                    }
                    temp_i++;
                    temp_j++;
                }
                
                // Adiciona caracteres restantes como diferenças
                temp_dif += (len_errada - temp_i) + (len_dic - temp_j);
                
                if (temp_dif <= max_diferenca) {
                    return temp_dif;
                }
            }
            
            // Verifica se pode dividir em duas palavras do dicionário
            if (max_diferenca >= 1 && i > 0) {
                char *primeira_parte = substring(errada_lower, 0, i);
                char *segunda_parte = substring(errada_lower, i, len_errada);
                
                int encontrado_primeira = palavra_no_dicionario(&d, primeira_parte);
                int encontrado_segunda = palavra_no_dicionario(&d, segunda_parte);
                
                free(primeira_parte);
                free(segunda_parte);
                
                if (encontrado_primeira && encontrado_segunda) {
                    return 1; // Uma diferença para dividir em duas palavras
                }
            }
            
            // Continua com diferenças regulares
            diferencas++;
            
            // Se já ultrapassou o máximo, retorna logo
            if (diferencas > max_diferenca) {
                return diferencas;
            }
            
            i++;
            j++;
        } else {
            i++;
            j++;
        }
    }
    
    // Adiciona diferenças pelos caracteres restantes
    diferencas += (len_errada - i) + (len_dic - j);
    
    // 2. Percorrendo do fim para o início
    if (diferencas > max_diferenca) {
        i = len_errada - 1;
        j = len_dic - 1;
        diferencas = 0;
        
        while (i >= 0 && j >= 0) {
            if (errada_lower[i] != dic_lower[j]) {
                diferencas++;
                
                // Se já ultrapassou o máximo, não vale a pena continuar
                if (diferencas > max_diferenca) {
                    return diferencas;
                }
                
                i--;
                j--;
            } else {
                i--;
                j--;
            }
        }
        
        // Adiciona diferenças pelos caracteres restantes
        diferencas += (i + 1) + (j + 1);
    }
    
    return diferencas;
}

// Função de comparação para qsort das alternativas
int compara_alternativas(const void *a, const void *b) {
    const alternativa *alt_a = (const alternativa *)a;
    const alternativa *alt_b = (const alternativa *)b;
    
    // Primeiro critério: menor número de diferenças
    if (alt_a->diferencas != alt_b->diferencas) {
        return alt_a->diferencas - alt_b->diferencas;
    }
    
    // Segundo critério: ordem alfabética do dicionário
    return strcasecmp(alt_a->palavra, alt_b->palavra);
}

void sugerir_alternativas(dicionario *d, const char *word, int max_alternativas, int max_diferencas) {
    // Array para armazenar alternativas encontradas
    alternativa *alternativas = (alternativa*)malloc(sizeof(alternativa) * max_alternativas);
    if (!alternativas) {
        perror("Erro ao alocar memória para alternativas");
        exit(EXIT_FAILURE);
    }
    
    int alternativas_count = 0;
    
    // Para cada número de diferenças (de 1 até max_diferencas)
    for (int num_dif = 1; num_dif <= max_diferencas && alternativas_count < max_alternativas; num_dif++) {
        // Percorre o dicionário em busca de palavras com esse número de diferenças
        for (size_t i = 0; i < d->count && alternativas_count < max_alternativas; i++) {
            int diferencas = calcular_diferencas_pdf(word, d->palavras[i], max_diferencas);
            
            // Se encontrou uma palavra com exatamente o número de diferenças que buscamos
            if (diferencas == num_dif) {
                // Verifica se essa palavra já foi adicionada às alternativas
                int ja_existe = 0;
                for (int j = 0; j < alternativas_count; j++) {
                    if (strcasecmp(alternativas[j].palavra, d->palavras[i]) == 0) {
                        ja_existe = 1;
                        break;
                    }
                }
                
                // Se não existir, adiciona à lista de alternativas
                if (!ja_existe) {
                    strcpy(alternativas[alternativas_count].palavra, d->palavras[i]);
                    alternativas[alternativas_count].diferencas = diferencas;
                    alternativas_count++;
                }
            }
            
            // Verifica caso especial: dividir a palavra errada em duas palavras do dicionário
            if (num_dif == 1) {
                size_t len_word = strlen(word);
                for (size_t pos = 1; pos < len_word && alternativas_count < max_alternativas; pos++) {
                    char primeira_parte[MAXIMO_TAMANHO_DE_PALAVRAS];
                    char segunda_parte[MAXIMO_TAMANHO_DE_PALAVRAS];
                    
                    strncpy(primeira_parte, word, pos);
                    primeira_parte[pos] = '\0';
                    
                    strcpy(segunda_parte, word + pos);
                    
                    if (palavra_no_dicionario(d, primeira_parte) && palavra_no_dicionario(d, segunda_parte)) {
                        // Combina as duas palavras com espaço no meio
                        char combinacao[MAXIMO_TAMANHO_DE_PALAVRAS * 2];
                        sprintf(combinacao, "%s %s", primeira_parte, segunda_parte);
                        
                        // Verifica se já existe nas alternativas
                        int ja_existe = 0;
                        for (int j = 0; j < alternativas_count; j++) {
                            if (strcasecmp(alternativas[j].palavra, combinacao) == 0) {
                                ja_existe = 1;
                                break;
                            }
                        }
                        
                        if (!ja_existe) {
                            strcpy(alternativas[alternativas_count].palavra, combinacao);
                            alternativas[alternativas_count].diferencas = 1;
                            alternativas_count++;
                        }
                    }
                }
            }
        }
    }
    
    // Ordena as alternativas encontradas
    qsort(alternativas, alternativas_count, sizeof(alternativa), compara_alternativas);
    
    // Imprime as alternativas
    printf("Alternativas: ");
    if (alternativas_count == 0) {
        printf("(nenhuma)");
    } else {
        for (int i = 0; i < alternativas_count; i++) {
            if (i > 0) {
                printf(", ");
            }
            printf("%s", alternativas[i].palavra);
        }
    }
    printf("\n");
    
    free(alternativas);
}

void process_text(dicionario *d, int modo, int max_alternativas, int max_diferencas) {
    char line[MAXIMO_TAMANHO_DE_PALAVRAS];
    int line_number = 0;
    
    while (fgets(line, sizeof(line), stdin)) {
        line_number++;
        char linha_original[MAXIMO_TAMANHO_DE_PALAVRAS];
        strcpy(linha_original, line);
        
        // Array para armazenar as palavras da linha
        char palavras[100][MAXIMO_TAMANHO_DE_PALAVRAS];
        int posicoes_inicio[100]; // Posição inicial de cada palavra na string original
        int num_palavras = 0;
        
        // Extrai palavras da linha
        size_t len = strlen(line);
        int i = 0;
        while (i < len) {
            // Ignora não-letras no início
            while (i < len && !eh_letra_valida(line[i]) && line[i] != '\'') {
                i++;
            }
            
            // Se chegou ao fim da linha, sai do loop
            if (i >= len) {
                break;
            }
            
            // Marca o início da palavra
            int inicio = i;
            posicoes_inicio[num_palavras] = inicio;
            
            // Extrai a palavra
            int j = 0;
            while (i < len) {
                char c = line[i];
                
                // Se for uma plica no meio da palavra
                if (c == '\'' && i > inicio && i < len - 1 && eh_letra_valida(line[i+1])) {
                    palavras[num_palavras][j++] = c;
                    i++;
                    continue;
                }
                
                // Se for uma letra válida
                if (eh_letra_valida(c)) {
                    palavras[num_palavras][j++] = c;
                    i++;
                    continue;
                }
                
                // Se não for letra nem plica válida, termina a palavra
                break;
            }
            
            // Finaliza a palavra
            palavras[num_palavras][j] = '\0';
            
            // Se a palavra não estiver vazia, incrementa o contador
            if (strlen(palavras[num_palavras]) > 0) {
                num_palavras++;
            }
        }
        
        // Verifica cada palavra
        int has_error = 0;
        for (i = 0; i < num_palavras; i++) {
            if (!palavra_no_dicionario(d, palavras[i])) {
                if (!has_error) {
                    has_error = 1;
                    printf("%d: %s", line_number, linha_original);
                }
                printf("Erro na palavra \"%s\"\n", palavras[i]);
                
                if (modo >= 2) {
                    sugerir_alternativas(d, palavras[i], max_alternativas, max_diferencas);
                }
            }
        }
    }
}

int main(int argc, char *argv[]) {
    const char *d_file = "words";
    int modo = 1;
    int max_alternativas = MAXIMO_ALTERNATIVAS;
    int max_diferencas = MAXIMO_DIFERENCAS;
    
    // Se houver argumentos, usa o primeiro como nome do arquivo de dicionário
    if (argc > 1) {
        d_file = argv[1];
    }
    
    inicializa_dicionario(&d);
    load_dicionario(&d, d_file);
    
    printf("Escreve uma frase para analisar:\n");
    process_text(&d, modo, max_alternativas, max_diferencas);
    
    libera_dicionario(&d);
    
    return EXIT_SUCCESS;
}