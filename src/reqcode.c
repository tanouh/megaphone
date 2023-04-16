#include "reqcode.h"

int reqtoi(enum reqcode req)
{
	switch (req) {
	case _NOTIFICATION:
		return 0;
	case INSCRIPTION:
		return 1;
	case PUSH_MESS:
		return 2;
	case ASK_MESS:
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

enum reqcode itoreq(int req)
{
	switch (req) {
	case 0:
		return _NOTIFICATION;
	case 1:
		return INSCRIPTION;
	case 2:
		return PUSH_MESS;
	case 3:
		return ASK_MESS;
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
