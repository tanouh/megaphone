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

#define NB_TEST 2
#define NONET "-nonet"

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mp = PTHREAD_MUTEX_INITIALIZER;
pthread_t pts[NB_TEST];

int launch_server();
int launch_client();
void *rw_server(void *data);
void *rw_client(void *data);
void print_meg(char *msg);
int server_created = 0;
static int spipe;
static int cpipe;

// TODO: clear code and the part with the pipe or try to fix it

void ign(int sig)
{
	if (sig == SIGUSR1)
		server_created = 1;
}

int test_net()
{
	struct sigaction act = {0};
	act.sa_handler = ign;
	sigaction(SIGUSR1, &act, NULL);

	spipe = launch_server();
	if (spipe < 0) {

		return ESYS;
	}
	printf("%d\n", spipe);
	if (pthread_create(pts, NULL, rw_server, NULL) == -1) {
		perror("Thread rw server");
		return ESYS;
	}
	print_meg("Waiting...");
	if (!server_created)
		pause();
	print_meg("Done");
	if ((cpipe = launch_client()) < 0) {
		return ESYS;
	}
	if (pthread_create(pts + 1, NULL, rw_client, NULL) == -1) {
		perror("Thread rw server");
		return ESYS;
	}
	fflush(stdout);
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

int launch_server()
{
	int pfd[2];
	pipe(pfd);
	print_meg("Launching server");
	int pid = getpid();
	switch (fork()) {
	case 0:
		close(pfd[0]);
		// dup2(pfd[1], STDOUT_FILENO);
		execlp(TEST_SERVER, TEST_SERVER, (char *)&pid, NULL);
		exit(1);

	case -1:
		return -1;
	default:
		close(pfd[1]);
		return pfd[0];
	}
}

int launch_client()
{
	print_meg("Launching client");
	int pfd[2];
	pipe(pfd);
	int pid = getpid();
	switch (fork()) {
	case 0:
		close(pfd[0]);
		// dup2(spipe, STDOUT_FILENO);
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

void *rw_server(void *data)
{
	char buf[PIPE_BUF];
	printf("%d\n", spipe);
	while (1) {
		int s = read(spipe, buf, PIPE_BUF);
		if (s <= 0) {
			print_meg("Pipe closed");
			return NULL;
		}
		print(buf, s);
	}
}

void *rw_client(void *data)
{
	char buf[PIPE_BUF];
	while (1) {
		int s = read(cpipe, buf, PIPE_BUF);
		if (s <= 0) {
			print_meg("Pipe closed");
			return NULL;
		}
		print(buf, s);
	}
}
