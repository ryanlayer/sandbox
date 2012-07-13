ALL_C=$(wildcard *.c)
#GSL_INCLUDE=-I/usr/include/gsl
GSL_INCLUDE=-I/sw/include/ \
			-I/sw/include/gsl
GSL_LINK=-L/sw/lib/

gsl_cdf_gaussian_P: gsl_cdf_gaussian_P.c
	gcc $(GSL_INCLUDE) \
		$(GSL_LINK) \
		-lgsl -lblas gsl_cdf_gaussian_P.c \
		-o gsl_cdf_gaussian_P

gsl_rand: gsl_rand.c
	gcc $(GSL_INCLUDE) \
		$(GSL_LINK) \
		-lgsl -lblas gsl_rand.c \
		-o gsl_rand

i_bsearch: i_bsearch.o timer.o bsearch_lib.o
	gcc -o i_bsearch i_bsearch.o timer.o bsearch_lib.o -lm

t_bsearch: t_bsearch.o timer.o bsearch_lib.o
	gcc -o t_bsearch t_bsearch.o timer.o bsearch_lib.o -lm

bsearch: bsearch.o timer.o bsearch_lib.o
	gcc -o bsearch bsearch.o timer.o bsearch_lib.o -lm

rand_chr: rand_chr.c 
	gcc -o rand_chr rand_chr.c lib/mt/mt.c -lm -Ilib/mt

log_space_test: log_space_test.c log_space.c log_space.h
	gcc -o log_space_test log_space_test.c log_space.c -lm

test:
	echo $(ALL_C)
