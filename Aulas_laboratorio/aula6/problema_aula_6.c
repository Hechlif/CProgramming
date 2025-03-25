#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>

typedef struct Participante {
    int posicao;
    char escalao[5];
    int posicao_esc;
    int dorsal;
    char *nome;
    char sexo;
    char tempo[10];
    struct Participante *proximo;
} Participante;

Participante* criar_no(int posicao, char *escalao, int posicao_esc, int dorsal, char *nome, char sexo, char *tempo) {
    Participante *novo = (Participante*)malloc(sizeof(Participante));
    if (!novo) {
        printf("Erro ao alocar memória!\n");
        exit(1);
    }
    novo->posicao = posicao;
    strcpy(novo->escalao, escalao);
    novo->posicao_esc = posicao_esc;
    novo->dorsal = dorsal;
    novo->nome = strdup(nome);
    novo->sexo = sexo;
    strcpy(novo->tempo, tempo);
    novo->proximo = NULL;
    return novo;
}

void adicionar(Participante **head, Participante *novo) {
    if (*head == NULL) {
        *head = novo;
    } else {
        Participante *atual = *head;
        while (atual->proximo)
            atual = atual->proximo;
        atual->proximo = novo;
    }
}

void liberar_lista(Participante *head) {
    Participante *atual;
    while (head) {
        atual = head;
        head = head->proximo;
        free(atual->nome);
        free(atual);
    }
}

Participante* ler_ficheiro(const char *nome_arquivo) {
    FILE *file = fopen(nome_arquivo, "r");
    if (!file) {
        printf("Erro ao abrir o arquivo %s\n", nome_arquivo);
        exit(1);
    }

    char linha[200];
    fgets(linha, sizeof(linha), file);

    Participante *head = NULL;
    int posicao, posicao_esc, dorsal;
    char escalao[5], nome[100], sexo, tempo[10];

    while (fgets(linha, sizeof(linha), file)) {
        sscanf(linha, "%d\t%s\t%d\t%d\t%[^	]\t%c\t%s", &posicao, escalao, &posicao_esc, &dorsal, nome, &sexo, tempo);
        adicionar(&head, criar_no(posicao, escalao, posicao_esc, dorsal, nome, sexo, tempo));
    }
    
    fclose(file);
    return head;
}

Participante* filtrar_escalao(Participante *head, char *escalao) {
    Participante *nova_lista = NULL;
    Participante *atual = head;
    
    while (atual) {
        if (strcmp(atual->escalao, escalao) == 0) {
            Participante *novo = criar_no(atual->posicao, atual->escalao, atual->posicao_esc, 
                                          atual->dorsal, atual->nome, atual->sexo, atual->tempo);
            adicionar(&nova_lista, novo);
        }
        atual = atual->proximo;
    }
    
    liberar_lista(head);
    return nova_lista;
}

Participante* filtrar_nome(Participante *head, char *nome) {
    Participante *nova_lista = NULL;
    Participante *atual = head;
    
    while (atual) {
        if (strstr(atual->nome, nome) != NULL) {
            Participante *novo = criar_no(atual->posicao, atual->escalao, atual->posicao_esc, 
                                          atual->dorsal, atual->nome, atual->sexo, atual->tempo);
            adicionar(&nova_lista, novo);
        }
        atual = atual->proximo;
    }
    
    liberar_lista(head);
    return nova_lista;
}

Participante* filtrar_masculino(Participante *head) {
    Participante *nova_lista = NULL;
    Participante *atual = head;
    
    while (atual) {
        if (atual->sexo == 'M') {
            Participante *novo = criar_no(atual->posicao, atual->escalao, atual->posicao_esc, 
                                          atual->dorsal, atual->nome, atual->sexo, atual->tempo);
            adicionar(&nova_lista, novo);
        }
        atual = atual->proximo;
    }
    
    liberar_lista(head);
    return nova_lista;
}

void imprimir_lista(Participante *head) {
    while (head) {
        printf("%d\t%s\t%d\t%d\t%s\t%c\t%s\n", 
               head->posicao, head->escalao, head->posicao_esc, 
               head->dorsal, head->nome, head->sexo, head->tempo);
        head = head->proximo;
    }
}

void mostrar_ajuda() {
    printf("Uso: ./problema_2 [-h] [-i ficheiro] [-e escalao] [-n nome] [-m]\n");
    printf("  -h            Mostra esta mensagem de ajuda\n");
    printf("  -i ficheiro   Usa o ficheiro especificado como entrada\n");
    printf("  -e escalao    Pesquisa participantes de um escalão específico\n");
    printf("  -n nome       Pesquisa participantes com o nome especificado\n");
    printf("  -m            Pesquisa apenas participantes masculinos\n");
}

int main(int argc, char *argv[]) {
    char *arquivo = "RunResults.txt";
    char *escalao = NULL;
    char *nome = NULL;
    int masculino = 0;
    int opt;
    int encontrou = 0;

    while ((opt = getopt(argc, argv, "hi:e:n:m")) != -1) {
        switch (opt) {
            case 'h':
                mostrar_ajuda();
                return 0;
            case 'i':
                arquivo = optarg;
                break;
            case 'e':
                escalao = optarg;
                encontrou = 1;
                break;
            case 'n':
                nome = optarg;
                encontrou = 1;
                break;
            case 'm':
                masculino = 1;
                encontrou = 1;
                break;
            default:
                mostrar_ajuda();
                return 1;
        }
    }

    Participante *lista = ler_ficheiro(arquivo);

    if (escalao) {
        lista = filtrar_escalao(lista, escalao);
    }
    if (nome) {
        lista = filtrar_nome(lista, nome);
    }
    if (masculino) {
        lista = filtrar_masculino(lista);
    }
    
    if (lista) {
        imprimir_lista(lista);
    } else {
        printf("Nenhum resultado encontrado.\n");
    }
    
    liberar_lista(lista);
    return 0;
}