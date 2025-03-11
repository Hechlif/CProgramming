#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Read numbers from user
void readNumbers(int *numbers, int size){
    for(int i = 0; i < size; i++){
        printf("Enter number %d: ", i+1);
        scanf("%d", &numbers[i]);
    }
}

// Encode number into how many times the digit repeats (112223 into 2*1, 3*2, 1*3)
void encodeNumber(int number, char *encodedNumber){
    char numberString[10];
    sprintf(numberString, "%d", number);
    int count = 1;
    for(int i = 0; i < strlen(numberString); i++){
        if(numberString[i] == numberString[i+1]){
            count++;
        }else{
            sprintf(encodedNumber, "%d*%c", count, numberString[i]);
            if (i < strlen(numberString) - 1) {
                strcat(encodedNumber, ",");
            }
            encodedNumber += strlen(encodedNumber);
            count = 1;
        }
    }
}

// Decode number from encoded number
void decodeNumber(char *encodedNumber, int *decodedNumber){
    int count = 0;
    char digit;
    while(*encodedNumber){
        sscanf(encodedNumber, "%d*%c", &count, &digit);
        for(int j = 0; j < count; j++){
            *decodedNumber = digit - '0';
            decodedNumber++;
        }
        while(*encodedNumber && *encodedNumber != ',') encodedNumber++;
        if(*encodedNumber) encodedNumber++; // skip the comma
    }
}

int main(){
    int size;
    printf("Enter the number of elements: ");
    scanf("%d", &size);
    int numbers[size];
    readNumbers(numbers, size);
    char encodedNumber[50];
    int decodedNumber[50];
    for(int i = 0; i < size; i++){
        memset(encodedNumber, 0, sizeof(encodedNumber));
        memset(decodedNumber, 0, sizeof(decodedNumber));
        encodeNumber(numbers[i], encodedNumber);
        decodeNumber(encodedNumber, decodedNumber);
        printf("Number %d:\nEncoded: %s\nDecoded: ", numbers[i], encodedNumber);
        for(int j = 0; decodedNumber[j] != 0; j++){
            printf("%d", decodedNumber[j]);
        }
        printf("\n");
    }
}
