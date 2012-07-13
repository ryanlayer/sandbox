gsl_rand: gsl_rand.c
	gcc -I/usr/include/gsl -lgsl -lblas gsl_rand.c -o gsl_rand

i_bsearch: i_bsearch.o timer.o bsearch_lib.o
	gcc -o i_bsearch i_bsearch.o timer.o bsearch_lib.o -lm

t_bsearch: t_bsearch.o timer.o bsearch_lib.o
	gcc -o t_bsearch t_bsearch.o timer.o bsearch_lib.o -lm

bsearch: bsearch.o timer.o bsearch_lib.o
	gcc -o bsearch bsearch.o timer.o bsearch_lib.o -lm

rand_chr: rand_chr.c 
	gcc -o rand_chr rand_chr.c lib/mt/mt.c -lm -Ilib/mt
