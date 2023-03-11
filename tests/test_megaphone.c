#include "test_constants.h"
#include "testlib.h"

#include <limits.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>

#define NB_TEST 1
#define NONET "-nonet"

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mp = PTHREAD_MUTEX_INITIALIZER;
pthread_t pts[NB_TEST];

int launch_server(int *pfd);
int launch_client(int *pfd);
void *rw_server_client(void *data);
void *rw_client(void *data);
void print_meg(char *msg);

void ign(int sig)
{
	if (sig == SIGUSR1)
		print_meg("Sig received");
}

int test_net()
{
	struct sigaction act = {0};
	act.sa_handler = ign;
	sigaction(SIGUSR1, &act, NULL);
	int *p = malloc(sizeof(int));
	int pfd[2];
	pipe(pfd);
	*p = launch_server(pfd);
	if (*p < 0) {
		free(p);
		return ESYS;
	}
	if (pthread_create(pts, NULL, rw_server_client, p) == -1) {
		perror("Thread rw server");
		free(p);
		return ESYS;
	}
	print_meg("Waiting...");
	pause();
	print_meg("Done");
	if (launch_client(pfd) < 0) {
		free(p);
		return ESYS;
	}
	fflush(stdout);
	free(p);
	return 0;
}

int main(int c, char **argv)
{
	int ntest = NB_TEST;
	if (c == 1 || strcmp(argv[1], NONET) != 0) {
		test_net();
	} else {
		ntest--;
	}
	for (int i = 0; i < ntest; i++) {
		if (pthread_join(pts[i], NULL) == -1)
			perror("Thread join");
	}
	int status;
	int ret = 0;
	while (waitpid(-1, &status, WNOHANG) > 0) {
		if (WIFEXITED(status))
			ret |= WEXITSTATUS(status);
	}

	return ret;
}

void print_meg(char *msg)
{
	pthread_mutex_lock(&mp);
	printf("%s: %s\n", MEGAPHONE, msg);
	pthread_mutex_unlock(&mp);
}

int launch_server(int *pfd)
{
	print_meg("Launching server");
	int pid = getpid();
	switch (fork()) {
	case 0:
		close(pfd[0]);
		dup2(pfd[1], STDOUT_FILENO);
		dup2(pfd[1], STDERR_FILENO);
		execlp(TEST_SERVER, TEST_SERVER, (char *)&pid, NULL);
		exit(1);

	case -1:
		return -1;
	default:
		close(pfd[1]);
		return pfd[0];
	}
}

int launch_client(int *pfd)
{
	print_meg("Launching client");
	int pid = getpid();
	switch (fork()) {
	case 0:
		close(pfd[0]);
		dup2(pfd[1], STDOUT_FILENO);
		dup2(pfd[1], STDERR_FILENO);
		execlp(TEST_CLIENT, TEST_CLIENT, (char *)&pid, NULL);
		exit(1);

	case -1:
		return -1;
	default:
		close(pfd[1]);
		return pfd[0];
	}
}

int print(const char *buf, int size)
{
	pthread_mutex_lock(&m);
	write(STDOUT_FILENO, buf, size);
	pthread_mutex_unlock(&m);
	return 0;
}

void *rw_server_client(void *data)
{
	char buf[PIPE_BUF];
	int ssize = sprintf(buf, "%s: ", SERVER);
	int spipe = *(int *)data;
	while (1) {
		int s = read(spipe, buf, PIPE_BUF - ssize);
		if (s <= 0) {
			print_meg("Pipe closed");
			return 0;
		}

		print(buf, s + ssize);
	}
}
