ALL_C=$(wildcard *.c)
#GSL_INCLUDE=-I/usr/include/gsl
GSL_INCLUDE=-I/sw/include/ \
			-I/sw/include/gsl
GSL_LINK=-L/sw/lib/
UNITY_ROOT=/Users/rl6sf/src/sandbox/C/unity

all:

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

pool_ex: pool_ex.c cthreadpool/threadpool.c
	gcc -o pool_ex pool_ex.c cthreadpool/threadpool.c -I cthreadpool/

pool_q: pool_q.c cthreadpool/threadpool.c
	gcc -o pool_q pool_q.c cthreadpool/threadpool.c -I cthreadpool/

q_tails: q_tails.c 
	gcc -g -o q_tails q_tails.c

q_tails_thread: q_tails_thread.c cthreadpool/threadpool.c
	gcc -g -o q_tails_thread q_tails_thread.c cthreadpool/threadpool.c -I cthreadpool/

p_q: p_q.c 
	gcc -g -o p_q p_q.c

grabix:
	g++ -g -o grabix_test grabix_test.cpp -I/Users/rl6sf/src/grabix /Users/rl6sf/src/grabix/grabix.cpp /Users/rl6sf/src/grabix/bgzf.c -lstdc++ -lz

test:
	echo $(ALL_C)
%: %.c timer.c
	gcc -o $@ $^ -O3

sse_gt_sum: sse_gt_sum.c
	gcc -o sse_gt_sum sse_gt_sum.c timer.c -O3

avx: avx.c
	gcc -mavx -mavx2 -o avx avx.c

avx_test: avx_test.c
	gcc -mavx -mavx2 -o avx_test avx_test.c

bit_op_sum: bit_op_sum.c
	gcc -mavx -mavx2 -o bit_op_sum bit_op_sum.c timer.c -g

pull_32_from_31: pull_32_from_31.c
	gcc -o pull_32_from_31 pull_32_from_31.c

avx_bed_intersect: avx_bed_intersect.c
	gcc -mavx -o avx_bed_intersect avx_bed_intersect.c

bcf_test: bcf_test.c
	gcc -g -I ~/src/htslib -o bcf_test bcf_test.c ~/src/htslib/hts.c -lz \
	    -DSAMTOOLS=1 ~/src/htslib/libhts.a

bcf_stats: bcf_stats.c
	gcc -g -I ~/src/htslib -o bcf_stats bcf_stats.c ~/src/htslib/hts.c -lz \
	    -DSAMTOOLS=1 ~/src/htslib/libhts.a

bcf_pool: bcf_pool.c
	gcc -O3 -I ~/src/htslib -o bcf_pool bcf_pool.c ~/src/htslib/hts.c -lz \
	    timer.c pq.c -DSAMTOOLS=1 ~/src/htslib/libhts.a

bcf_seq: bcf_seq.c
	gcc -g -I ~/src/htslib -o bcf_seq bcf_seq.c ~/src/htslib/hts.c -lz \
	    timer.c pq.c -DSAMTOOLS=1 ~/src/htslib/libhts.a

hdf5_test: hdf5_test.c
	gcc -I$(HOME)/src/hdf5-1.8.7-mac-intel-x86_64-static/include \
	    -L$(HOME)/src/hdf5-1.8.7-mac-intel-x86_64-static/lib \
	    -lhdf5 -lz -lsz \
	    -o hdf5_test \
	    hdf5_test.c

h5_cmprss: h5_cmprss.c
	gcc -I$(HOME)/src/hdf5-1.8.7-mac-intel-x86_64-static/include \
	    -L$(HOME)/src/hdf5-1.8.7-mac-intel-x86_64-static/lib \
	    -lhdf5 -lz -lsz \
	    -o h5_cmprss \
	    h5_cmprss.c

sql_ped: sql_ped.c
	gcc -o sql_ped \
 		-Isqlite-amalgamation-3080100 \
		sql_ped.c \
		sqlite-amalgamation-3080100/sqlite3.c

zlib_test: zlib_test.c
	gcc -O3 -o zlib_test zlib_test.c -lz

lz4_test: lz4_test.c
	gcc -O3 -o lz4_test \
	    -I/Users/rl6sf/src/lz4-r123 \
	    -L/Users/rl6sf/src/lz4-r123 \
	    	/Users/rl6sf/src/lz4-r123/lz4.c \
		lz4_test.c

bcf_by_line: bcf_by_line.c
	gcc -g \
	    	-I ~/src/htslib \
	    	-o bcf_by_line \
		bcf_by_line.c \
		-lz timer.c -DSAMTOOLS=1 ~/src/htslib/libhts.a

time_read_read_dc: time_read_read_dc.c
	gcc -g -o time_read_read_dc timer.c time_read_read_dc.c -lz

vcf_format_test: vcf_format_test.c
	gcc -g -I ~/src/htslib -o vcf_format_test vcf_format_test.c \
	    ~/src/htslib/hts.c -lz \
	    -DSAMTOOLS=1 ~/src/htslib/libhts.a

buf_test: buf_test.c
	gcc -g -o buf_test buf_test.c -lz

test_bpt: test_bpt.c
	ruby $(UNITY_ROOT)/generate_test_runner.rb test_bpt.c
	gcc -I$(UNITY_ROOT) \
	    -DTEST $(UNITY_ROOT)/unity.c \
	    -Werror -g -o test_bpt test_bpt.c test_bpt_Runner.c bpt.c 
	rm test_bpt_Runner.c
	./test_bpt
	
ftp_file: ftp_file.c
	gcc -g -o ftp_file ftp_file.c

knet_test: knet_test.c
	gcc -o knet_test -I$(HOME)/src/htslib \
		knet_test.c \
		$(HOME)/src/htslib/libhts.a
gqt_bm: gqt_bm.c
	gcc -o gqt_bm \
		-I$(HOME)/src/gqt/src \
		-I$(HOME)/src/htslib \
		$(HOME)/src/gqt/src/bm.c \
		$(HOME)/src/gqt/src/genotq.c \
		$(HOME)/src/gqt/src/wah.c \
    		gqt_bm.c

vcf_bcf_index: vcf_bcf_index.c
	gcc -g -o vcf_bcf_index \
		-I$(HOME)/src/htslib \
		$(HOME)/src/htslib/libhts.a \
		-lz \
    		vcf_bcf_index.c

vcf_bcf_get: vcf_bcf_get.c
	gcc -g -o vcf_bcf_get \
		-I$(HOME)/src/htslib \
		$(HOME)/src/htslib/libhts.a \
		-lz \
    		vcf_bcf_get.c

bcf_tell: bcf_tell.c
	gcc -g -o bcf_tell \
		-I$(HOME)/src/htslib \
		$(HOME)/src/htslib/libhts.a \
		-lz \
    		bcf_tell.c

bcf_seek: bcf_seek.c
	gcc -g -o bcf_seek \
		-I$(HOME)/src/htslib \
		$(HOME)/src/htslib/libhts.a \
		-lz \
    		bcf_seek.c

bcf_utell: bcf_utell.c
	gcc -g -o bcf_utell \
		-I$(HOME)/src/htslib \
		$(HOME)/src/htslib/libhts.a \
		-lz \
    		bcf_utell.c

bcf_useek: bcf_useek.c
	gcc -g -o bcf_useek \
		-I$(HOME)/src/htslib \
		$(HOME)/src/htslib/libhts.a \
		-lz \
    		bcf_useek.c

bgzf_scan: bgzf_scan.c
	gcc -g -o bgzf_scan \
		-I$(HOME)/src/htslib \
		$(HOME)/src/htslib/libhts.a \
		-lz \
    		bgzf_scan.c

bcf_scan: bcf_scan.c
	gcc -g -o bcf_scan \
		-I$(HOME)/src/htslib \
		$(HOME)/src/htslib/libhts.a \
		-lz \
    		bcf_scan.c
