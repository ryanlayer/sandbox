#include <stdio.h>
#include <math.h>

double log_b(double x, double base)
{
	return log(x) / log(base);
}

int main(int argc, char* argv[]) {
	int d, n = 8;
	printf("%lf\n", log_b(n, 2));
	for (d = 0; d < log_b(n, 2); d ++) {
		printf("d:%d\n", d);
		int k;
		for (k = 0; k < n; k+=pow(2, d+1)) {
			int p_1 = pow(2,d+1);
			int p = pow(2,d);
			int l_1 = k + p_1 - 1;
			int l = k + p - 1;
			//printf("\tk:%d 2^(d+1):%d %d\t", k, p_1, l);
			//printf("\tk:%d 2^(d):%d %d\t", k, p, l);
			printf("\tx[%d] = x[%d] + x[%d]\n", l_1, l, l_1);
		}
	}

	printf("---\n");

	for (d = log_b(n,2); d >=0; d--) {
		int k;
		printf("d:%d\n", d);
		for (k = 0; k < n; k+= pow(2, d+1)) {
			int l_1 = k + pow(2, d + 1) - 1;
			int l = k + pow(2, d) - 1;
			printf("\tt=x[%d]\tx[%d]=x[%d]\tx[%d]=x[%d]+t\n",
					l, l, l_1, l_1, l_1);
		}	
	}
}
