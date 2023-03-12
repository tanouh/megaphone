#include "testlib.h"

#include <pthread.h>
#include <stdio.h>
#include <time.h>

#define C_RED "\x1B[31m"
#define C_GREEN "\x1B[32m"
#define C_CLEAR "\x1B[0m"

pthread_mutex_t m1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t m2 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t m3 = PTHREAD_MUTEX_INITIALIZER;

int test(int (*f)(), const char *name)
{
	pthread_mutex_lock(&m1);
	printf("[TEST] %s...\n", name);
	pthread_mutex_lock(&m1);
	const clock_t tic = clock();
	int res = f();
	const time_t toc = clock();
	const double delta = ((double)toc - tic) * 1000.0 / CLOCKS_PER_SEC;
	if (res) {
		pthread_mutex_lock(&m2);
		printf("[%sPASSED%s] %s in %f ms \n\n", C_GREEN, C_CLEAR, name,
		       delta);
		pthread_mutex_unlock(&m2);
	}
	return res;
}

int test_c(int (*f)(), const char *name, void(print)(char *))
{
	char buf[4096];
	sprintf(buf, "[TEST] %s...\n", name);
	print(buf);
	const clock_t tic = clock();
	int res = f();
	const time_t toc = clock();
	const double delta = ((double)toc - tic) * 1000.0 / CLOCKS_PER_SEC;
	if (res) {
		sprintf(buf, "[%sPASSED%s] %s in %f ms \n\n", C_GREEN, C_CLEAR,
			name, delta);
		print(buf);
	}
	return res;
}

int test_arg(int (*f)(void *), void *arg, const char *name)
{
	pthread_mutex_lock(&m1);
	printf("[TEST] %s...\n", name);
	pthread_mutex_lock(&m1);
	const clock_t tic = clock();
	int res = f(arg);
	const time_t toc = clock();
	const double delta = ((double)toc - tic) * 1000.0 / CLOCKS_PER_SEC;
	if (res) {
		pthread_mutex_lock(&m2);
		printf("[%sPASSED%s] %s in %f ms \n\n", C_GREEN, C_CLEAR, name,
		       delta);
		pthread_mutex_unlock(&m2);
	}
	return res;
}

int test_carg(int (*f)(void *), void *arg, const char *name,
	      void(print)(char *))
{
	char buf[4096];
	sprintf(buf, "[TEST] %s...\n", name);
	print(buf);
	const clock_t tic = clock();
	int res = f(arg);
	const time_t toc = clock();
	const double delta = ((double)toc - tic) * 1000.0 / CLOCKS_PER_SEC;
	if (res) {

		sprintf(buf, "[%sPASSED%s] %s in %f ms \n\n", C_GREEN, C_CLEAR,
			name, delta);
		print(buf);
	}
	return res;
}

int assert(int b, int line, const char *filename)
{
	if (b) {
		return 1;
	}
	pthread_mutex_lock(&m3);
	printf("[%sFAILED%s] at line %d in file %s\n\n", C_RED, C_CLEAR, line,
	       filename);
	pthread_mutex_unlock(&m3);
	return 0;
}

int assert_c(int b, int line, const char *filename, void *(print)(char *))
{
	if (b) {
		return 1;
	}
	char buf[4096];
	sprintf(buf, "[%sFAILED%s] at line %d in file %s\n\n", C_RED, C_CLEAR,
		line, filename);
	print(buf);
	return 0;
}
