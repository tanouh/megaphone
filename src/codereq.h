#ifndef MEGAPHONE_CODEREQ_H
#define MEGAPHONE_CODEREQ_H

enum reqcode {
	INSCRIPTION,
	PUSH_MESS,
	ASK_MESS,
	SUBSCRIBE,
	PUSH_FILE,
	PULL_FILE,
	ERROR
};

int to_num(enum reqcode crq);
enum reqcode to_crq(int cqr);

#endif
