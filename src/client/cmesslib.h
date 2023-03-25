#ifndef MEGAPHONE_CMESSLIB_H
#define MEGAPHONE_CMESSLIB_H

#include "../msghead.h"
#include "../reqcode.h"

#include <arpa/inet.h>

/**
 * @brief Fills a generic header containing the given informations.
 * @param req The request code.
 * @param id The user id.
 * @param chat The chat room number.
 * @param nb Specific number related to the request type.
 * @param datalen The size of the data.
 * @param data The data itself.
 * @return The header filled correctly.
 */
int fill_message(struct msghead h, char *buf, size_t bufsize, const void *data);

/**
 * @brief Fills the header requested for the inscription.
 * @param username The username.
 * @param len The username length.
 * @return the filled message.
 */
int fill_inscription(struct msghead h, char *buf, size_t bufsize,
		     char const *nickname, int len);

/**
 * @brief Fills a generic header containing the given informations.
 * @param req The request code.
 * @param id The user id.
 * @param nb The block number.
 * @param datalen The size of the data.
 * @param data The data itself.
 * @return The messaged filled correctly.
 */
int fill_udp(struct msghead h, char *buf, size_t bufsize, const void *data);
int get_message(const void *msg, struct msghead *h);

int get_asked_messages(const void *msg, struct msghead *h, void *origin,
		       void *owner, void *buf, size_t bufsize);

int get_subscribed_message(const void *msg, struct msghead *h, void *addr);

int get_notification_message(const void *msg, struct msghead *h, void *owner,
			     void *data);

int get_udp_message(const void *msg, size_t msglen, struct msghead *h,
		    void *buf, size_t bufsize);
#endif
