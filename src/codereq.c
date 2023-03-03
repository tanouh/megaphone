#include "codereq.h"

int to_num(enum codereq crq)
{
	switch (crq) {
	case INSCRIPTION:
		return 1;
	case PUSH_MESS:
		return 2;
	case ASK_N:
		return 3;
	case SUBSCRIBE:
		return 4;
	case PUSH_FILE:
		return 5;
	case PULL_FILE:
		return 6;
	case ERROR:
		return 31;
	default:
		return -1;
	}
}

enum codereq to_crq(int cdr)
{
	switch (cdr) {
	case 1:
		return INSCRIPTION;
	case 2:
		return PUSH_MESS;
	case 3:
		return ASK_N;
	case 4:
		return SUBSCRIBE;
	case 5:
		return PUSH_FILE;
	case 6:
		return PULL_FILE;
	default:
		return ERROR;
	}
}
