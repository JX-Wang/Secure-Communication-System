#include <openssl/aes.h>
#include <stdio.h>
#include <stdlib.h>

int main(){
	double x=10.000000;
	char y[10];
	sprintf(y, "%.0lf", x);
	printf("char y is:%s\n", y);
}

