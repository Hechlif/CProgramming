#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <getopt.h>

#define MAXIMO_TAMANHO_DE_PALAVRAS 1000
#define MAXIMO_ALTERNATIVAS 10
#define MAXIMO_DIFERENCAS 2
#define DEFAULT_DICT_FILE "words"

typedef struct {
    char **palavras;
    size_t count;
    size_t capacity;
} dicionario;

typedef struct {
    char palavra[MAXIMO_TAMANHO_DE_PALAVRAS];
    int diferencas;
} alternativa;

dicionario d;

void mostrar_ajuda() {
    printf("Uso: ./ortografia [OPCOES]\n");
    printf("Opções disponíveis:\n");
    printf("  -h               mostra esta ajuda e termina\n");
    printf("  -i filename      nome do ficheiro de entrada, em alternativa a stdin\n");
    printf("  -o filename      nome do ficheiro de saída, em alternativa a stdout\n");
    printf("  -d filename      nome do ficheiro de dicionário a usar (padrão: words)\n");
    printf("  -a nn            o número máximo de alternativas a mostrar com cada erro ortográfico (padrão: 10)\n");
    printf("  -n nn            o número máximo de diferenças a considerar nos erros ortográficos (padrão: 2)\n");
    printf("  -m nn            o modo de funcionamento do programa (padrão: 1)\n");
    printf("\nModos de funcionamento:\n");
    printf("  1: identifica palavras erradas\n");
    printf("  2: identifica palavras erradas e sugere alternativas\n");
    printf("  3: corrige automaticamente o texto\n");
}

bool validar_modo(int modo) {
    return modo >= 1 && modo <= 3;
}

bool validar_max_alternativas(int max_alternativas) {
    return max_alternativas > 0;
}

bool validar_max_diferencas(int max_diferencas) {
    return max_diferencas > 0;
}

int letra_valida(char c) {
    return isalpha((unsigned char)c);
}

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
        char *pos = strchr(buffer, '/');
        if (pos) {
            *pos = '\0';
        }
        buffer[strcspn(buffer, "\n")] = '\0';
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
    qsort(d->palavras, d->count, sizeof(char*), compara_palavras);
}

int palavra_no_dicionario(dicionario *d, const char *word) {
    char *key = strdup(word);
    if (!key) {
        perror("Erro ao duplicar palavra para busca");
        exit(EXIT_FAILURE);
    }
    
    // Usando bsearch corretamente
    void *result = bsearch(&key, d->palavras, d->count, sizeof(char*), compara_palavras);
    free(key);
    return result != NULL;
}

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

// Função para calcular diferenças entre palavras conforme as regras descritas
int calcular_diferencas(const char *palavra_errada, const char *palavra_dic, int max_diferenca) {
    size_t len_errada = strlen(palavra_errada);
    size_t len_dic = strlen(palavra_dic);
    
    // Convertendo para minúsculas para comparação
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
    
    // Verificar se a palavra pode ser dividida em duas que existem no dicionário
    if (max_diferenca >= 1) {
        for (size_t pos = 1; pos < len_errada; pos++) {
            char primeira_parte[MAXIMO_TAMANHO_DE_PALAVRAS];
            char segunda_parte[MAXIMO_TAMANHO_DE_PALAVRAS];
            strncpy(primeira_parte, errada_lower, pos);
            primeira_parte[pos] = '\0';
            strcpy(segunda_parte, errada_lower + pos);
            
            if (palavra_no_dicionario(&d, primeira_parte) && palavra_no_dicionario(&d, segunda_parte)) {
                return 1; // Consideramos uma diferença neste caso
            }
        }
    }
    
    // Comparação do início para o fim com offset
    int diferencas_inicio = 0;
    size_t i = 0, j = 0;
    
    while (i < len_errada && j < len_dic) {
        if (errada_lower[i] != dic_lower[j]) {
            // Tentar usar offset na palavra errada
            if (max_diferenca >= 1) {
                for (size_t offset = 1; offset <= max_diferenca && i + offset < len_errada; offset++) {
                    if (errada_lower[i + offset] == dic_lower[j]) {
                        // Saltar offset caracteres na palavra errada
                        i += offset;
                        diferencas_inicio += offset;
                        if (diferencas_inicio > max_diferenca) return diferencas_inicio;
                        goto continue_comparing_inicio;
                    }
                }
            }
            
            // Tentar usar offset na palavra do dicionário
            if (max_diferenca >= 1) {
                for (size_t offset = 1; offset <= max_diferenca && j + offset < len_dic; offset++) {
                    if (errada_lower[i] == dic_lower[j + offset]) {
                        // Saltar offset caracteres na palavra do dicionário
                        j += offset;
                        diferencas_inicio += offset;
                        if (diferencas_inicio > max_diferenca) return diferencas_inicio;
                        goto continue_comparing_inicio;
                    }
                }
            }
            
            // Se não conseguiu usar offset, conta diferença simples
            diferencas_inicio++;
            if (diferencas_inicio > max_diferenca) return diferencas_inicio;
        }
        
continue_comparing_inicio:
        i++;
        j++;
    }
    
    // Adicionar diferenças para caracteres restantes
    diferencas_inicio += (len_errada - i) + (len_dic - j);
    
    // Se a diferença já excedeu o máximo, não precisamos continuar
    if (diferencas_inicio <= max_diferenca) {
        return diferencas_inicio;
    }
    
    // Comparação do fim para o início
    int diferencas_fim = 0;
    i = len_errada - 1;
    j = len_dic - 1;
    
    while (i >= 0 && j >= 0) {
        if (errada_lower[i] != dic_lower[j]) {
            diferencas_fim++;
            if (diferencas_fim > max_diferenca) return diferencas_fim;
        }
        i--;
        j--;
    }
    
    // Adicionar diferenças para caracteres restantes
    diferencas_fim += (i + 1) + (j + 1);
    
    // Retornar o menor número de diferenças encontrado
    return (diferencas_inicio < diferencas_fim) ? diferencas_inicio : diferencas_fim;
}

int compara_alternativas(const void *a, const void *b) {
    const alternativa *alt_a = (const alternativa *)a;
    const alternativa *alt_b = (const alternativa *)b;
    if (alt_a->diferencas != alt_b->diferencas) {
        return alt_a->diferencas - alt_b->diferencas;
    }
    return strcasecmp(alt_a->palavra, alt_b->palavra);
}

char* obter_primeira_alternativa(dicionario *d, const char *word, int max_diferencas) {
    char *melhor_alternativa = NULL;
    int menor_diferencas = max_diferencas + 1;
    
    // Para cada número de diferenças (começando por 1)
    for (int num_dif = 1; num_dif <= max_diferencas; num_dif++) {
        // Verificar divisão em duas palavras (apenas para 1 diferença)
        if (num_dif == 1) {
            size_t len_word = strlen(word);
            for (size_t pos = 1; pos < len_word; pos++) {
                char primeira_parte[MAXIMO_TAMANHO_DE_PALAVRAS];
                char segunda_parte[MAXIMO_TAMANHO_DE_PALAVRAS];
                strncpy(primeira_parte, word, pos);
                primeira_parte[pos] = '\0';
                strcpy(segunda_parte, word + pos);
                
                if (palavra_no_dicionario(d, primeira_parte) && palavra_no_dicionario(d, segunda_parte)) {
                    char *combinacao = malloc(strlen(primeira_parte) + strlen(segunda_parte) + 2);
                    if (!combinacao) {
                        perror("Erro ao alocar memória para combinação");
                        exit(EXIT_FAILURE);
                    }
                    sprintf(combinacao, "%s %s", primeira_parte, segunda_parte);
                    return combinacao; // Retorna imediatamente a combinação de palavras
                }
            }
        }
        
        // Percorrer todo o dicionário para este número de diferenças
        for (size_t i = 0; i < d->count; i++) {
            int diferencas = calcular_diferencas(word, d->palavras[i], max_diferencas);
            
            if (diferencas == num_dif) {
                // Se esta é a primeira alternativa encontrada com o menor número de diferenças
                if (diferencas < menor_diferencas) {
                    // Liberar alternativa anterior se existir
                    if (melhor_alternativa) {
                        free(melhor_alternativa);
                    }
                    melhor_alternativa = strdup(d->palavras[i]);
                    menor_diferencas = diferencas;
                    
                    // Se encontramos uma alternativa com 1 diferença, retornamos imediatamente
                    if (num_dif == 1) {
                        return melhor_alternativa;
                    }
                } else if (diferencas == menor_diferencas && strcasecmp(d->palavras[i], melhor_alternativa) < 0) {
                    // Se temos a mesma diferença mas esta é alfabeticamente anterior
                    free(melhor_alternativa);
                    melhor_alternativa = strdup(d->palavras[i]);
                }
            }
        }
        
        // Se encontramos uma alternativa com este número de diferenças, retornamos
        if (melhor_alternativa && menor_diferencas == num_dif) {
            return melhor_alternativa;
        }
    }
    
    // Se não encontrou nenhuma alternativa
    return NULL;
}

void sugerir_alternativas(dicionario *d, const char *word, int max_alternativas, int max_diferencas) {
    alternativa alternativas[MAXIMO_ALTERNATIVAS];
    int alternativas_count = 0;
    
    // Para cada número de diferenças (começando por 1)
    for (int num_dif = 1; num_dif <= max_diferencas && alternativas_count < max_alternativas; num_dif++) {
        // Verificar divisão em duas palavras (apenas para 1 diferença)
        if (num_dif == 1) {
            size_t len_word = strlen(word);
            for (size_t pos = 1; pos < len_word && alternativas_count < max_alternativas; pos++) {
                char primeira_parte[MAXIMO_TAMANHO_DE_PALAVRAS];
                char segunda_parte[MAXIMO_TAMANHO_DE_PALAVRAS];
                strncpy(primeira_parte, word, pos);
                primeira_parte[pos] = '\0';
                strcpy(segunda_parte, word + pos);
                
                if (palavra_no_dicionario(d, primeira_parte) && palavra_no_dicionario(d, segunda_parte)) {
                    char combinacao[MAXIMO_TAMANHO_DE_PALAVRAS * 2];
                    sprintf(combinacao, "%s %s", primeira_parte, segunda_parte);
                    
                    // Verificar se já existe esta alternativa
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
        
        // Percorrer todo o dicionário para este número de diferenças
        for (size_t i = 0; i < d->count && alternativas_count < max_alternativas; i++) {
            int diferencas = calcular_diferencas(word, d->palavras[i], max_diferencas);
            
            if (diferencas == num_dif) {
                // Verificar se já existe esta alternativa
                int ja_existe = 0;
                for (int j = 0; j < alternativas_count; j++) {
                    if (strcasecmp(alternativas[j].palavra, d->palavras[i]) == 0) {
                        ja_existe = 1;
                        break;
                    }
                }
                
                if (!ja_existe) {
                    strcpy(alternativas[alternativas_count].palavra, d->palavras[i]);
                    alternativas[alternativas_count].diferencas = diferencas;
                    alternativas_count++;
                }
            }
        }
    }
    
    // Ordenar alternativas pelo número de diferenças e depois alfabeticamente
    qsort(alternativas, alternativas_count, sizeof(alternativa), compara_alternativas);
    
    // Imprimir alternativas
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
}

void corrigir_texto(dicionario *d, int max_diferencas, FILE *entrada, FILE *saida) {
    char line[MAXIMO_TAMANHO_DE_PALAVRAS];
    int line_number = 0;
    
    while (fgets(line, sizeof(line), entrada)) {
        line_number++;
        char linha_original[MAXIMO_TAMANHO_DE_PALAVRAS];
        strcpy(linha_original, line);
        
        // Cria uma cópia da linha para modificar
        char linha_corrigida[MAXIMO_TAMANHO_DE_PALAVRAS];
        strcpy(linha_corrigida, line);
        
        // Encontra as palavras e suas posições
        char palavras[100][MAXIMO_TAMANHO_DE_PALAVRAS];
        int posicoes_inicio[100];
        int posicoes_fim[100];
        int num_palavras = 0;
        
        size_t len = strlen(line);
        int i = 0;
        
        // Extrair palavras da linha e suas posições
        while (i < len) {
            
            // Pular caracteres não alfabéticos
            while ((i < len && !letra_valida(line[i])) || (line[i] == '\'' && 
                ((i == 0) ? 1 : (!letra_valida(line[i - 1]) || !letra_valida(line[i + 1]))))) {
                i++;
            }
            
            if (i >= len) {
                break;
            }
            
            // Marcamos a posição inicial da palavra
            posicoes_inicio[num_palavras] = i;
            
            int j = 0;
            while (i < len) {
                char c = line[i];
                if (c == '\'' && ((i == 0) ? 0 : (letra_valida(line[i - 1]) && 
                                                letra_valida(line[i + 1])))) {
                    palavras[num_palavras][j++] = c;
                    i++;
                    continue;
                }
                if (letra_valida(c)) {
                    palavras[num_palavras][j++] = c;
                    i++;
                    continue;
                }
                break;
            }
            
            palavras[num_palavras][j] = '\0';
            posicoes_fim[num_palavras] = i - 1;
            
            if (strlen(palavras[num_palavras]) > 0) {
                num_palavras++;
            }
        }
        
        // Corrigir palavras erradas
        int offset = 0; // Offset para ajustar posições após substituições
        for (i = 0; i < num_palavras; i++) {
            if (!palavra_no_dicionario(d, palavras[i])) {
                // Obter a primeira alternativa para esta palavra
                char *alternativa = obter_primeira_alternativa(d, palavras[i], max_diferencas);
                
                if (alternativa) {
                    // Posição ajustada com o offset atual
                    int pos_inicio = posicoes_inicio[i] + offset;
                    int pos_fim = posicoes_fim[i] + offset;
                    int tamanho_original = pos_fim - pos_inicio + 1;
                    int tamanho_alternativa = strlen(alternativa);
                    
                    // Calcular novo tamanho da linha e verificar se cabe
                    size_t novo_tamanho = strlen(linha_corrigida) - tamanho_original + tamanho_alternativa;
                    if (novo_tamanho < MAXIMO_TAMANHO_DE_PALAVRAS) {
                        // Deslocar o restante da linha para a direita (se maior) ou esquerda (se menor)
                        memmove(linha_corrigida + pos_inicio + tamanho_alternativa, 
                                linha_corrigida + pos_fim + 1, 
                                strlen(linha_corrigida) - pos_fim);
                        
                        // Inserir a alternativa
                        memcpy(linha_corrigida + pos_inicio, alternativa, tamanho_alternativa);
                        
                        // Atualizar o offset para as próximas substituições
                        offset += (tamanho_alternativa - tamanho_original);
                        
                        // Garantir que a string termina corretamente
                        linha_corrigida[novo_tamanho] = '\0';
                    }
                    
                    free(alternativa);
                }
            }
        }
        
        // Escrever a linha corrigida no arquivo de saída
        fputs(linha_corrigida, saida);
    }
}

void process_text(dicionario *d, int modo, int max_alternativas, int max_diferencas, FILE *entrada, FILE *saida) {
    if (modo == 3) {
        // Modo 3: Corrigir automaticamente o texto
        corrigir_texto(d, max_diferencas, entrada, saida);
        return;
    }
    
    // Modos 1 e 2
    char line[MAXIMO_TAMANHO_DE_PALAVRAS];
    int line_number = 0;
    
    while (fgets(line, sizeof(line), entrada)) {
        line_number++;
        char linha_original[MAXIMO_TAMANHO_DE_PALAVRAS];
        strcpy(linha_original, line);
        
        char palavras[100][MAXIMO_TAMANHO_DE_PALAVRAS];
        int num_palavras = 0;
        size_t len = strlen(line);
        int i = 0;
        
        // Extrair palavras da linha
        while (i < len) {
            // Pular caracteres não alfabéticos
            while ((i < len && !letra_valida(line[i])) || (line[i] == '\'' && ((i == 0) ? 1 : (!letra_valida(line[i - 1]) || !letra_valida(line[i + 1]))))) {
                i++;
            }
            
            if (i >= len) {
                break;
            }
            
            int j = 0;
            while (i < len) {
                char c = line[i];
                if (c == '\'' && ((i == 0) ? 0 : (letra_valida(line[i - 1]) && letra_valida(line[i + 1])))) {
                    palavras[num_palavras][j++] = c;
                    i++;
                    continue;
                }
                if (letra_valida(c)) {
                    palavras[num_palavras][j++] = c;
                    i++;
                    continue;
                }
                break;
            }
            
            palavras[num_palavras][j] = '\0';
            if (strlen(palavras[num_palavras]) > 0) {
                num_palavras++;
            }
        }
        
        int has_error = 0;
        for (i = 0; i < num_palavras; i++) {
            if (!palavra_no_dicionario(d, palavras[i])) {
                if (!has_error) {
                    has_error = 1;
                    printf("%d: %s", line_number, linha_original);
                }
                
                printf("Erro na palavra \"%s\"\n", palavras[i]);
                
                if (modo == 2) {
                    sugerir_alternativas(d, palavras[i], max_alternativas, max_diferencas);
                }
            }
        }
        
        // No modo 1 e 2, também escrevemos a linha original no arquivo de saída se fornecido
        if (saida) {
            fputs(linha_original, saida);
        }
    }
}

int main(int argc, char *argv[]) {
    const char *dict_file = DEFAULT_DICT_FILE;
    int modo = 1;
    int max_alternativas = MAXIMO_ALTERNATIVAS;
    int max_diferencas = MAXIMO_DIFERENCAS;
    const char *input_file = NULL;
    const char *output_file = NULL;
    FILE *entrada = stdin;
    FILE *saida = NULL;
    FILE *flag = NULL;
    int opt;
    
    // Processar argumentos de linha de comando usando getopt
    while ((opt = getopt(argc, argv, "hi:o:d:a:n:m:")) != -1) {
        switch (opt) {
            case 'h':
                mostrar_ajuda();
                return EXIT_SUCCESS;
            case 'i':
                input_file = optarg;
                if (!(flag = fopen(optarg, "r"))) {
                    fprintf(stderr, "O ficheiro de texto \"%s\" não existe verifique se esta correto.\n", optarg);
                    return EXIT_FAILURE;
                }
                fclose(flag);
                break;
            case 'o':
                output_file = optarg;
                break;
            case 'd':
                if (!(flag = fopen(optarg, "r"))) {
                    fprintf(stderr, "O dicinario \"%s\" não existe verifique se esta correto.\n", optarg);
                    return EXIT_FAILURE;
                }
                fclose(flag);
                dict_file = optarg;
                break;
            case 'a':
                max_alternativas = atoi(optarg);
                if (max_alternativas == 0) {
                    fprintf(stderr, "O numero de alternativas tem de ser um numero, nao pode ser uma string.\n");
                    return EXIT_FAILURE;
                }
                if (max_alternativas < 0) {
                    fprintf(stderr, "O valor de alternativas nao pode ser menor ao igual a zero.\n");
                    return EXIT_FAILURE;
                }
                break;
            case 'n':
                max_diferencas = atoi(optarg);
                if (max_diferencas == 0) {
                    fprintf(stderr, "O numero de diferenaças tem de ser um numero, nao pode ser uma string.\n");
                    return EXIT_FAILURE;
                }
                if (max_diferencas < 0) {
                    fprintf(stderr, "O valor de diferenças nao pode ser menor que 0.\n");
                    return EXIT_FAILURE;
                }
                break;
            case 'm':
                modo = atoi(optarg);
                if (modo == 0) {
                    fprintf(stderr, "O modo \"%s\" não é um numero sequer, verifique o -h para ver os modos possiveis.\n", optarg);
                    return EXIT_FAILURE;
                }
                if (!(modo <= 3 && modo >= 1)) {
                    fprintf(stderr, "O modo %d não existe verifique o -h para ver os modos possiveis.\n", modo);
                    return EXIT_FAILURE;
                }
                break;
            case '?':
                fprintf(stderr, "Argumento invalido, utilize %s -h para ver os argumentos possiveis.\n", argv[0]);
                return EXIT_FAILURE;
        }
    }
    
    // Abrir arquivo de entrada se especificado
    if (input_file) {
        entrada = fopen(input_file, "r");
        if (!entrada) {
            perror("Erro ao abrir arquivo de entrada");
            return EXIT_FAILURE;
        }
    }
    
    // Definir arquivo de saída
    if (output_file) {
        saida = fopen(output_file, "w");
        if (!saida) {
            perror("Erro ao abrir arquivo de saída");
            if (entrada != stdin) {
                fclose(entrada);
            }
            return EXIT_FAILURE;
        }
    } else {
        // Se não foi especificado, usamos stdout para modo 3, e nulo para os outros modos
        saida = (modo == 3) ? stdout : NULL;
    }
    
    inicializa_dicionario(&d);
    load_dicionario(&d, dict_file);
    process_text(&d, modo, max_alternativas, max_diferencas, entrada, saida);
    libera_dicionario(&d);
    
    // Fechar arquivos se necessário
    if (entrada != stdin) {
        fclose(entrada);
    }
    if (saida && saida != stdout) {
        fclose(saida);
    }
    
    return EXIT_SUCCESS;
}