#include "bfp.h"

double p = (double) binaryPointRemainderMask + 1.0;


int32_t double_to_bfp( double f ) {
	int32_t i;
	i = (int32_t)f << binaryPoint;
	i += (int32_t) ( (f - (double)((int32_t)f)) * p );
	return i;
}

double bfp_to_double( int32_t i ) {
	return (double) (i >> binaryPoint) + (double) (i & binaryPointRemainderMask) / p;
}


int32_t bfp_multiply( int32_t i1, int32_t i2 ) {
	int32_t i3;
	int32_t j1 = (i1 >> 16);
	int32_t j2 = (i2 >> 16);
	i3 = ( j1 * j2 ) << 8;
	i3 += ( j1 * (i2 & 0xFFFF) ) >> 8;
	i3 += ( j2 * (i1 & 0xFFFF) ) >> 8;

	return i3;
}
