gsl_rand: gsl_rand.c
	gcc -I/usr/include/gsl -lgsl -lblas gsl_rand.c -o gsl_rand

i_bsearch: i_bsearch.o timer.o
	gcc -o i_bsearch i_bsearch.o timer.o -lm
