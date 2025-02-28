#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define M_PI 3.14159265358979323846

void main(){
    
    float teto, vi, d, h, g; 
    int theta;
    
    g = 9,8;

    printf("Define ceiling height: ");
    scanf("%f", &teto);
    
    printf("Define theta: ");
    scanf("%d", &theta);
    float thetaRad = (theta * M_PI) / 180;
    
    if (theta < 0 || theta > 90){
        printf("Invalid angle\n");
        exit(1);
    }

    else if (vi < 0)
    {
        printf("Invalid velocity\n");
        exit(1);
    }
    

    printf("Define initial velocity: ");
    scanf("%f", &vi);
    
    d = (pow(vi, 2) * (sin(2 * thetaRad))) / g;
    h = ((pow(vi, 2) * (pow(sin(thetaRad), 2)))) / (2 * g);
    
    if (h > teto){
        printf("The ball hits the ceiling \n");
    }

    printf("Distancia: %f\n", d);
    printf("Altura: %f\n", h);
}