#include <stdio.h>

#include "bfp.h"


void SquareRootTests() {
	double d1 = 24.68;

	int32_t i1 = double_to_bfp(d1);

	i1 >>= 1;

	d1 = bfp_to_double(i1);

	printf("\n\n%g\n",d1);

}
