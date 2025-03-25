#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

Participante* ler_arquivo(const char *nome_arquivo) {
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

int main() {
    Participante *lista = ler_arquivo("RunResults.txt");
    liberar_lista(lista);
    return 0;
}
