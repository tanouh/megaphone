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

#define NB_TEST 0

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mp = PTHREAD_MUTEX_INITIALIZER;

int launch_server();
int launch_client();
void print_meg(char *msg);
int server_created = 0;

// clear code and the part with the pipe or try to fix it

void servalrm(int sig)
{
	if (sig == SIGUSR1)
		server_created = 1;
}

int test_net()
{
	struct sigaction act = {0};
	act.sa_handler = servalrm;
	sigaction(SIGUSR1, &act, NULL);
	if (launch_server()) {

		return ESYS;
	}
	print_meg("Waiting...");
	if (!server_created)
		pause();
	if (launch_client() < 0) {
		return ESYS;
	}
	return 0;
}

int main()
{
	pthread_t pts[NB_TEST];
	int cpt = 0;
	test_net();
	int *tmp;
	int ret = 0;
	for (int i = 0; i < cpt; i++) {
		if (pthread_join(pts[i], (void **)&tmp) == -1)
			perror("Thread join");
		if (tmp == NULL) {
			ret = 1;
		} else {
			ret |= *tmp;
			free(tmp);
		}
	}
	int status;
	while (waitpid(-1, &status, 0) > 0) {
		if (WIFEXITED(status))
			ret |= WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			ret = WTERMSIG(status);
	}
	print_meg("End of Megaphone");

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
	print_meg("Launching server");
	switch (fork()) {
	case 0:
		execlp(TEST_SERVER, TEST_SERVER, NULL);
		exit(1);

	case -1:
		return -1;
	default:
		return 0;
	}
}

int launch_client()
{
	print_meg("Launching client");
	switch (fork()) {
	case 0:
		execlp(TEST_CLIENT, TEST_CLIENT, NULL);
		exit(1);

	case -1:
		return -1;
	default:
		return 0;
	}
}

int print(const char *buf, int size)
{
	pthread_mutex_lock(&m);
	write(STDOUT_FILENO, buf, size);
	pthread_mutex_unlock(&m);
	return 0;
}