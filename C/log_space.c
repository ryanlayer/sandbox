#include "log_space.h"

double get_p(log_space ls)
{
	if ( isfinite(ls) )
		return exp(ls);
	else
		return 0;
}

log_space get_ls(double p)
{
	if (p == 0)
		return -INFINITY;
	else
		return log(p);
}

// x*y
log_space ls_multiply(log_space x, log_space y)
{
	return (log_space)(x + y);
}
// x/y
log_space ls_divide(log_space x, log_space y)
{
	return (log_space)(x - y);
}

// x + y
log_space ls_add(log_space x, log_space y)
{
	if (isinf(x))
		return y;

	if (isinf(y))
		return x;

	if (x == 0 || y == 0)
		return 0;

	log_space tmp;
	if (x < y) {
		tmp = y;
		y = x;
		x = y;
	}

	return x + log(1 + exp(y - x));
}
