#ifndef MEGAPHONE_LIB_H
#define MEGAPHONE_LIB_H

#include "constants.h"
#include "reqcode.h"

#include <arpa/inet.h>

#define ID_MASK(id) ((id)&ID_MAX)
#define REQ_MASK(rq) ((rq) >> ID_BITS)

char *htonx(char *h, int len);
char *ntohx(char *h, int len);

/**
 * @brief Fills the minimal header containing the user unique id ans the request
 * code.
 * @param req The request code.
 * @param id The user id.
 * @return The header filled correctly.
 */
char *fill_min_header(enum reqcode req, uint16_t id);

int fill_buffer(const char *msg, char **buf, int size);

#endif
