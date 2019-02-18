#include <stdio.h>
#include <math.h>
#include <stdint.h>

#ifdef __linux__
#include <time.h>
#endif // __linux__

#include "bfp.h"


/*

Testing fast and accurate sine functions specifically
  https://www.gamedev.net/forums/topic/621589-extremely-fast-sin-approximation/

*/

int32_t a;
int32_t b;
int32_t c;


const double PI	=  3.14159265358979323846264338327950288;
const double INVPI =  0.31830988618379067153776752674502872;
const double A	 =  0.00735246819687011731341356165096815;
const double B	 = -0.16528911397014738207016302002888890;
const double C	 =  0.99969198629596757779830113868360584;



void myInit() {
	a = double_to_bfp(A);
	b = double_to_bfp(B);
	c = double_to_bfp(C);
}

int32_t myFastSine( int32_t rad ) {
//	x2 = x * x;
//	x = x*(C + x2*(B + A*x2));
	int32_t rad2 = bfp_multiply(rad,rad);
	return bfp_multiply(rad, (c + bfp_multiply(rad2, (b + bfp_multiply(a,rad2)))));
}


float Silverware_fastsin( float x ) {
//always wrap input angle to -PI..PI
/*
	while (x < -3.14159265f)
		x += 6.28318531f;

	while (x >  3.14159265f)
		x -= 6.28318531f;
*/
	float sin1;
//compute sine
	if (x < 0)
		sin1 = (1.27323954f + .405284735f * x) *x;
	else
		sin1 = (1.27323954f - .405284735f * x) *x;

	return sin1;
}


/* uncomment the next line if you're on a big-endian system */
/* #define BIG_ENDIAN */

/* uncomment the next line if you can not assume double-precision FPU or IEEE754 */
/* #define NO_FAST_TRUNCATE */

/* we need to do some custom hacking for MSVC */
#ifdef _MSC_VER
typedef __int32 int32_t;
#else
#include <stdint.h>
#endif

inline int32_t fast_round(double x) {

#ifndef NO_FAST_TRUNCATE
	const double MAGIC_ROUND = 6755399441055744.0; /* http://stereopsis.com/sree/fpu2006.html */

	union {
		double d;

		struct {
#ifdef BIG_ENDIAN
			int32_t hw;
			int32_t lw;
#else
			int32_t lw;
			int32_t hw;
#endif
		};
	} fast_trunc;

	fast_trunc.d = x;
	fast_trunc.d += MAGIC_ROUND;

	return fast_trunc.lw;

#else

	if (x < 0) {
		return (int32_t) (x - 0.5);
	} else {
		return (int32_t) (x + 0.5);
	}

#endif
}

double fast_sin(double x) {

//	int32_t k;
	double x2;

	/* find offset of x from the range -pi/2 to pi/2 */
//	k = fast_round(INVPI * x);

	/* bring x into range */
//	x -= k * PI;

	/* calculate sine */
	x2 = x * x;
	x = x*(C + x2*(B + A*x2));

	/* if x is in an odd pi count we must flip */
//	if (k % 2) x = -x;

	return x;
}

#define numRads 180

void FastSineTests() {
	float fRads[numRads];
	int32_t iRads[numRads];

	float fSilver[numRads];
	float fFast[numRads];
	float fsin[numRads];
	float fmine[numRads];
	int32_t imine[numRads];

	double timeSilver = 0;
	double timeFast = 0;
	double timeSin = 0;
	double timeMine = 0;
	double timeTmp = 0;

#ifdef __linux__
	struct timespec  t;
#endif // __linux__



	myInit();

	for(int i=0; i<numRads; i++){
		fRads[i] = -PI/2 + (double)i/(double)numRads*PI;
		iRads[i] = double_to_bfp(fRads[i]);
//		printf("%g ~ 0x%02X.%06X\n",fRads[i],(iRads[i]>>binaryPoint)&0xFF,iRads[i]&binaryPointRemainderMask);
	}

#ifdef __linux__
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t);
	timeTmp = (double) t.tv_sec + (double) t.tv_nsec * 0.000000001;
#endif // __linux__
	for(int i=0; i<numRads; i++){
		fsin[i] = sin(fRads[i]);
  }
#ifdef __linux__
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t);
	timeSin = (double) t.tv_sec + (double) t.tv_nsec * 0.000000001 - timeTmp;
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t);
	timeTmp = (double) t.tv_sec + (double) t.tv_nsec * 0.000000001;
#endif // __linux__
	for(int i=0; i<numRads; i++){
		fFast[i] = fast_sin(fRads[i]);
  }
#ifdef __linux__
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t);
	timeFast = (double) t.tv_sec + (double) t.tv_nsec * 0.000000001 - timeTmp;
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t);
	timeTmp = (double) t.tv_sec + (double) t.tv_nsec * 0.000000001;
#endif // __linux__
	for(int i=0; i<numRads; i++){
		fSilver[i] = Silverware_fastsin(fRads[i]);
  }
#ifdef __linux__
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t);
	timeSilver = (double) t.tv_sec + (double) t.tv_nsec * 0.000000001 - timeTmp;
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t);
	timeTmp = (double) t.tv_sec + (double) t.tv_nsec * 0.000000001;
#endif // __linux__
	for(int i=0; i<numRads; i++){
		imine[i] = myFastSine(iRads[i]);
  }
#ifdef __linux__
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t);
	timeMine = (double) t.tv_sec + (double) t.tv_nsec * 0.000000001 - timeTmp;
#endif // __linux__
	for(int i=0; i<numRads; i++){
		fmine[i] = bfp_to_double(imine[i]);
  }

	printf(" fsin[i]      \t fFast[i]      \t fmine[i]       \t fSilver[i]\n");
	printf(" diff fsin[i] \t diff fFast[i] \t diff fmine[i]  \t diff fSilver[i]\n\n");

	for(int i=0; i<numRads; i++){
		printf("%12.8f\t%12.8f\t%12.8f\t%12.8f\n",fsin[i],fFast[i],fmine[i],fSilver[i]);
		printf("%12.8f\t%12.8f\t%12.8f\t%12.8f\n\n",0.0,fsin[i]-fFast[i],fsin[i]-fmine[i],fsin[i]-fSilver[i]);
	}

	printf(" %8g s\t %8g s\t %8g s\t %8g s\n",timeSin, timeFast, timeMine, timeSilver);

}
