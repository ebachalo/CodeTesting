#include <stdio.h>

extern int BinaryFixedPointMathTests();
extern void FastSineTests();


int main() {

	printf("\n **** Hello from main.c! ****\n\n");
	BinaryFixedPointMathTests();
	FastSineTests();
	return(0);
}
