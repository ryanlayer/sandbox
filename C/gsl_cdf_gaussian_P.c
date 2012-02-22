#include <stdio.h>
#include <math.h>
#include <gsl_randist.h>
#include <gsl_cdf.h>

int main (int argc, char *argv[])
{
	if ( argc < 5 ) {
		fprintf(stderr, "%s\t<mean> <stdev> <start> <end>\n", argv[0]);
		return -1;
	}

	double u = atof(argv[1]);
	double s = atof(argv[2]);
	int start = atoi(argv[3]);
	int end = atoi(argv[4]);
	int i;

	for (i = start; i <= end; ++i) {
		//double z = ((double) i) - u;
		//double cdf = gsl_cdf_gaussian_P(z, s);
		//printf("%d\t%f\t%f\n", i,  cdf, 1 - cdf);
		printf("%d\t%f\t%f\n", i,  
			1 - gsl_cdf_gaussian_P(((double) i) - u, s),
			1 - gsl_cdf_gaussian_P((((double) i) - u)/s, s));
	}
}

