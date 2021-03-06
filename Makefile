SHELL := /bin/bash
CC = gcc
CFLAGS = -O0 -std=gnu99 -Wall -fopenmp -mavx
EXECUTABLE = \
	time_test_baseline time_test_openmp_2 time_test_openmp_4 \
	time_test_avx time_test_avxunroll \
	benchmark_clock_gettime \
	compute_error time_test_leibniz_openmp_4

GIT_HOOKS := .git/hooks/pre-commit

$(GIT_HOOKS):
	@scripts/install-git-hooks
	@echo

default: $(GIT_HOOKS) computepi.o
	$(CC) $(CFLAGS) computepi.o time_test.c -DBASELINE -o time_test_baseline
	$(CC) $(CFLAGS) computepi.o time_test.c -DOPENMP_2 -o time_test_openmp_2
	$(CC) $(CFLAGS) computepi.o time_test.c -DOPENMP_4 -o time_test_openmp_4
	$(CC) $(CFLAGS) computepi.o time_test.c -DAVX -o time_test_avx
	$(CC) $(CFLAGS) computepi.o time_test.c -DAVXUNROLL -o time_test_avxunroll
	$(CC) $(CFLAGS) computepi.o time_test.c -DL_OPENMP_4 -o time_test_leibniz_openmp_4
	$(CC) $(CFLAGS) computepi.o benchmark_clock_gettime.c -o benchmark_clock_gettime -lm
	$(CC) $(CFLAGS) computepi.o compute_error.c -o compute_error -lm

.PHONY: clean default

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

check: default
	time ./time_test_baseline
	time ./time_test_openmp_2
	time ./time_test_openmp_4
	time ./time_test_avx
	time ./time_test_avxunroll
	time ./time_test_leibniz_openmp_4

gencsv: default
	i=1000; while [[ $$i -le 50000 ]]; do \
		printf "%d," $$i;\
		./benchmark_clock_gettime $$i; \
		((i = i + 1000)); \
	done > result_clock_gettime.csv	

gerrcsv: default
	./compute_error 0 > result_pi_error.csv

berrcsv: default
	./compute_error 1 > result_pi_base_error.csv

lerrcsv: default
	./compute_error 2 > result_pi_lei_error.csv

plot: result_clock_gettime.csv
	gnuplot scripts/runtime.gp

ploterr: result_pi_error.csv
	gnuplot scripts/pi_error.gp

plotberr: result_pi_base_error.csv
	gnuplot scripts/pi_berror.gp

plotlerr: result_pi_lei_error.csv
	gnuplot scripts/pi_lerror.gp

clean:
	rm -f $(EXECUTABLE) *.o *.s result_clock_gettime.csv \
		result_pi_base_error.csv result_pi_lei_error.csv result_pi_error.csv
