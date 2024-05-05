#include <stdio.h>
#include <stdlib.h>

void minisystem()
{
    printf("minisystem\n");
}

float add(float a, float b){
	return a+b;
}

float subtract(float a, float b){
	return a-b;
}

float multiply(float a, float b){
	return a*b;
}

float divide(float a, float b){
	if(b==0){
	printf("can't divide\n");
	return 0;
	}
	return a/b;
}
