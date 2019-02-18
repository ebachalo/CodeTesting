#ifndef BFP_H_INCLUDED
#define BFP_H_INCLUDED

#include <stdint.h>

#define binaryPoint 24
#define binaryPointRemainderMask 0xFFFFFF

int32_t double_to_bfp( double f );

double bfp_to_double( int32_t i );

int32_t bfp_multiply( int32_t i1, int32_t i2 );


#endif // BFP_H_INCLUDED
