#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Função para ler os números do usuário
void read_numbers(int *arr, int *size) {
    printf("Digite a quantidade de números: ");
    scanf("%d", size);
    printf("Digite os números (0-9): ");
    for (int i = 0; i < *size; i++) {
        scanf("%d", &arr[i]);
    }
}

// Função para codificar o vetor
void encode(int *arr, int size, char *encoded) {
    int count = 1;
    char temp[10];
    encoded[0] = '\0'; // Inicializa string vazia
    for (int i = 1; i <= size; i++) {
        if (i < size && arr[i] == arr[i - 1] && count < 9) {
            count++;
        } else {
            sprintf(temp, "%d%d", count, arr[i - 1]);
            strcat(encoded, temp);
            count = 1;
        }
    }
}

// Função para decodificar a string
void decode(char *encoded, int *decoded, int *size) {
    *size = 0;
    for (int i = 0; encoded[i] != '\0'; i += 2) {
        int count = encoded[i] - '0';
        int value = encoded[i + 1] - '0';
        for (int j = 0; j < count; j++) {
            decoded[(*size)++] = value;
        }
    }
}

int main() {
    int arr[100], decoded[100], size;
    char encoded[200];
    
    read_numbers(arr, &size);
    encode(arr, size, encoded);
    printf("Codificado: %s\n", encoded);
    
    decode(encoded, decoded, &size);
    printf("Decodificado: ");
    for (int i = 0; i < size; i++) {
        printf("%d", decoded[i]);
    }
    printf("\n");
    return 0;
}
