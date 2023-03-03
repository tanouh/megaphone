#ifndef MEGAPHONE_CODEREQ_H
#define MEGAPHONE_CODEREQ_H

enum codereq {
	INSCRIPTION,
	PUSH_MESS,
	ASK_N,
	SUBSCRIBE,
	PUSH_FILE,
	PULL_FILE,
	ERROR
};

int to_num(enum codereq crq);
enum codereq to_crq(int cqr);

#endif
