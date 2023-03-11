#ifndef MEGAPHONE_LIB_H
#define MEGAPHONE_LIB_H

#include "constants.h"
#include "reqcode.h"

#include <arpa/inet.h>

#define ID_MASK(id) ((id) << REQ_BITS)
#define REQ_MASK(rq) ((rq)&REQ_MAX)

/**
 * @brief Fills the minimal header containing the user unique id ans the request
 * code.
 * @param req The request code.
 * @param id The user id.
 * @return The header filled correctly.
 */
void *fill_min_header(enum reqcode req, uint16_t id);

int fill_buffer(const void *msg, void **buf, int size);

int get_min_header(const void *msg, enum reqcode *req, uint16_t *id);

#endif
