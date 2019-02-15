#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdint.h>



//If you change binaryPoint you will need to change printing of hex output
#define binaryPoint 24
#define binaryPointRemainderMask 0xFFFFFF

int BinaryFixedPointMathTests()
{

	int32_t i1,i2,i3;
	int16_t j1,j2;
	double f1,f2,f3;
	double tmp1;

	time_t t;
	unsigned short s[3];

	double p = pow(2,binaryPoint);

	int i;

	for (i=0; i < 3; i++) {
	  s[i] = (unsigned short) time(&t);
	}

	seed48(s);

	printf("\n\n\n **** Binary Fixed Point Math Example ****\n\n");

	for(i=0; i<4; i++) {
		f1 = (drand48() -0.5)*11;
		f2 = (drand48() -0.5)*11;

		i1 = (int32_t)f1 << binaryPoint;
		i1 += (int32_t) ( (f1 - (double)((int32_t)f1)) * p );

		i2 = (int32_t)f2 << binaryPoint;
		i2 += (int32_t) ( (f2 - (double)((int32_t)f2)) * p );

		printf("%g ~ 0x%02X.%06X  :  %g ~ 0x%02X.%06X\n",f1,(i1>>binaryPoint)&0xFF,i1&binaryPointRemainderMask,f2,(i2>>binaryPoint)&0xFF,i2&binaryPointRemainderMask);

		f3 = f1*f2;

		j1 = (i1 >> 16);
		j2 = (i2 >> 16);

		i3 = ( j1 * j2 ) << 8;
		i3 += ( j1 * (i2 & 0xFFFF) ) >> 8;
		i3 += ( j2 * (i1 & 0xFFFF) ) >> 8;

		tmp1 = (float) (i3 >> binaryPoint) + (float) (i3 & binaryPointRemainderMask) / p;

		printf("%f x %f = %f  ~  %f = 0x%02X.%06X = 0x%02X.%06X x 0x%02X.%06X\n\n",f1,f2,f3,tmp1,(i3>>binaryPoint)&0xFF,i3&binaryPointRemainderMask,(i1>>binaryPoint)&0xFF,i1&binaryPointRemainderMask,f2,(i2>>binaryPoint)&0xFF,i2&binaryPointRemainderMask);

	}

	return 0;
}
