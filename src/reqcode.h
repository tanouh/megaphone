#ifndef MEGAPHONE_REQCODE_H
#define MEGAPHONE_REQCODE_H

enum reqcode {
	INSCRIPTION,
	PUSH_MESS,
	ASK_MESS,
	SUBSCRIBE,
	PUSH_FILE,
	PULL_FILE,
	ERROR
};

int reqtoi(enum reqcode req);
enum reqcode itoreq(int req);

#endif
