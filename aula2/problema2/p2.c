#include<stdio.h>
#include<stdlib.h>

void main(){

    int a, b, hours, minutes, seconds, hours2, minutes2, seconds2, diffhours, diffminutes, diffseconds;
    
    a, b , hours, minutes, seconds, hours2, minutes2, seconds2, diffhours, diffminutes, diffseconds = 0;
    
    printf("Times must be in this format: h(0-23),m(0-59),s(0-59)\n");
    
    printf("Enter bigger time in hours, minutes, seconds: ");
    
    scanf("%2d,%2d,%2d", &hours, &minutes, &seconds);
    
    printf("Enter smaller time in hours, minutes, seconds: ");
    
    scanf("%2d,%2d,%2d", &hours2, &minutes2, &seconds2);
    
    printf("1st Time: %2d h:%2d m:%2d s\n", hours, minutes, seconds);
    
    printf("2nd Time: %2d h:%2d m:%2d s\n", hours, minutes, seconds);
    
    
    if (hours < 0 || hours > 23 || hours2 < 0 || hours2 > 23 || minutes < 0 || minutes > 59 || minutes2 < 0 || minutes2 > 59 || seconds < 0 || seconds > 59 || seconds2 < 0 || seconds2 > 59){
        printf("Invalid time entered\n");
        exit(1);
    }
    
    else if (hours2 > hours){
        printf("Time 1 must be higher than time 2 \n");
        exit(1);
    }
    
     if (seconds - seconds2 < 0){
        a = 1;
        diffseconds = seconds - seconds2 + 60;
    }
    
    else if(seconds - seconds2 >= 0){
        diffseconds = seconds - seconds2;
    }
    
    if (minutes - minutes2 - a < 0){
        b = 1;
        diffminutes = minutes - minutes2 - a + 60;
    }
    
    else if (minutes - minutes2 - a >= 0){
        diffminutes = minutes - minutes2 - a;
    }
   
    diffhours = hours - hours2 - b;    

    printf("The difference between the times is %2d h:%2d m:%2d s\n", diffhours, diffminutes, diffseconds);
}