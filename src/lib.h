#ifndef MEGAPHONE_LIB_H
#define MEGAPHONE_LIB_H

#include "constants.h"
#include "msghead.h"
#include "reqcode.h"

#include <arpa/inet.h>

#define ID_MASK(id) ((id) >> REQ_BITS)
#define REQ_MASK(rq) ((rq)&REQ_MAX)

/**
 * @brief Fills the minimal header containing the user unique id ans the request
 * code.
 * @param req The request code.
 * @param id The user id.
 * @return The header filled correctly.
 */
int fill_min_header(struct msghead h, char *buf, int bufsize);

int get_min_header(const void *msg, struct msghead *h);

void *malloc_return(int ret);

void print_s(char *msg);

void print_c(char *msg);

int init_allchats();

#endif
