#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Read numbers from user
void readNumbers(char numbers[][10], int size){
    for(int i = 0; i < size; i++){
        printf("Enter number %d: ", i+1);
        scanf("%s", numbers[i]);
    }
}

// Encode number into how many times the digit repeats (112223 into 2*1, 3*2, 1*3)
void encodeNumber(char *number, char *encodedNumber){
    int count = 1;
    for(int i = 0; i < strlen(number); i++){
        if(number[i] == number[i+1]){
            count++;
        }else{
            sprintf(encodedNumber, "%d*%c", count, number[i]);
            if (i < strlen(number) - 1) {
                strcat(encodedNumber, ",");
            }
            encodedNumber += strlen(encodedNumber);
            count = 1;
        }
    }
}

// Decode number from encoded number
void decodeNumber(char *encodedNumber, char *decodedNumber){
    int count = 0;
    char digit;
    while(*encodedNumber){
        sscanf(encodedNumber, "%d*%c", &count, &digit);
        for(int j = 0; j < count; j++){
            *decodedNumber = digit;
            decodedNumber++;
        }
        while(*encodedNumber && *encodedNumber != ',') encodedNumber++;
        if(*encodedNumber) encodedNumber++; // skip the comma
    }
    *decodedNumber = '\0';
}

int main(){
    int size;
    printf("Enter the number of elements: ");
    scanf("%d", &size);
    char numbers[size][10];
    readNumbers(numbers, size);
    char encodedNumber[50];
    char decodedNumber[50];
    for(int i = 0; i < size; i++){
        memset(encodedNumber, 0, sizeof(encodedNumber));
        memset(decodedNumber, 0, sizeof(decodedNumber));
        encodeNumber(numbers[i], encodedNumber);
        decodeNumber(encodedNumber, decodedNumber);
        printf("Number %s:\nEncoded: %s\nDecoded: %s\n", numbers[i], encodedNumber, decodedNumber);
    }
}
