#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define PI 3.14159265358979323846

int main() {
    float d, vi, c, g = 9.8;
    int theta_min = 0, theta_max = 90;
    int theta;

    printf("Define a distancia ao cesto: ");
    scanf("%f", &d);

    printf("Define a velocidade inicial: ");
    scanf("%f", &vi);

    printf("Define o diametro do cesto: ");
    scanf("%f", &c);

    if (vi <= 0 || d <= 0 || c <= 0) {
        printf("Valores inválidos! Certifique-se de que são positivos.\n");
        return 1;
    }

    printf("Gama de ângulos aceitáveis: ");
    int found = 0;
    theta = theta_min;
    while (theta <= theta_max) {
        float thetaRad = (theta * PI) / 180;
        float alcance = (pow(vi, 2) * sin(2 * thetaRad)) / g;

        if (alcance >= (d - c / 2) && alcance <= (d + c / 2)) {
            printf("%d ", theta);
            found = 1;
        }
        theta++;
    }

    if (!found) {
        printf("Nenhum ângulo encontrado.");
    }

    printf("\n");
    return 0;
}
