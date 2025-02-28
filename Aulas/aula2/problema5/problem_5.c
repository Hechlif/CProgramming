
#include<stdio.h>

#define radiacao(f) ( \
    (f) < 3e9 ? 0 : \
    (f) < 3e12 ? 1 : \
    (f) < 4.3e14 ? 2 : \
    (f) < 7.5e14 ? 3 : \
    (f) < 3e17 ? 4 : \
    (f) < 3e19 ? 5 : 6 )
int main(){
    double freq;
    printf("Insira a frequencia da radiacao.\n\v");
    scanf("%lf", &freq);

    switch(radiacao(freq)) {
    case 0:
        printf("Ondas de rádio\n");
        break;
    case 1:
        printf("Microondas\n");
        break;
    case 2:
        printf("Infravermelho\n");
        break;
    case 3:
        printf("Luz visível\n");
        break;
    case 4:
        printf("Ultravioleta\n");
        break;
    case 5:
        printf("Raios-X\n");
        break;
    case 6:
        printf("Raios gama\n");
        break;
    }
    return 0;
}
