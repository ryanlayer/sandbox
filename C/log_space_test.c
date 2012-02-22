#include "log_space.h"
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char *argv[])
{
	if (argc < 3) {
		fprintf(stderr, "%s\t<x> <y>\n", argv[0]);
		return 1;
	}
	double x = atof(argv[1]);
	double y = atof(argv[2]);

	log_space lx = get_ls(x);
	log_space ly = get_ls(y);

	/*
	printf("x*y x:%f(%f) y:%f(%f)\t%f\n",
			x, lx, y, ly,
			ls_multiply(lx, ly));
	*/

	printf("x*y x:%f(%f) y:%f(%f)\t%f(%f)\n",
			x, lx, y, ly,
			get_p(ls_multiply(lx, ly)),
			ls_multiply(lx, ly));

	printf("x/y x:%f(%f) y:%f(%f)\t%f(%f)\n",
			x, lx, y, ly,
			get_p(ls_divide(lx, ly)),
			ls_divide(lx, ly));

	printf("x+y x:%f(%f) y:%f(%f)\t%f(%f)\n",
			x, lx, y, ly,
			get_p(ls_add(lx, ly)),
			ls_add(lx, ly));
}

/*
typedef double log_space;

log_space get_ls(double p);

// x*y
log_space ls_multiply(log_space x, log_space y);

// x/y
log_space ls_divide(log_space x, log_space y);

// x + y
log_space ls_add(log_space x, log_space y);

#endif
*/
