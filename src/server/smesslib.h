#ifndef MEGAPHONE_SMESSLIB_H
#define MEGAPHONE_SMESSLIB_H

#include "../msghead.h"
#include "../reqcode.h"

#include <arpa/inet.h>

/**
 * @brief Fills a generic header containing the given informations.
 * @param req The request code.
 * @param id The user id.
 * @param nbchat The chat room number.
 * @param nb Specific number related to the request type.
 * @return The header filled correctly.
 */
int fill_message(struct msghead h, char *buf, int bufsize);

int fill_asked_message(struct msghead h, char *buf, int bufsize, void *origin,
		       void *owner, void *data);

/**
 * @brief Shortened version of fill_message for the fill_subscribe request.
 * @param id The user id.
 * @param nbchat The chat room number.
 * @param nb The port number for multi-broadcast.
 * @param addrmult The ipv6 multi-broadcast address.
 * @return The message filled correctly.
 */
int fill_subscribe(struct msghead h, char *buf, int bufsize, void *addrmult);

/**
 * @brief Fills a generic header containing the given informations.
 * @param req The request code.
 * @param id The user id.
 * @param nb The block number.
 * @param datalen The size of the data.
 * @param data The data itself.
 * @return The messaged filled correctly.
 */
int fill_udp(struct msghead h, char *buf, int bufsize, void *data);

/**
 * @brief Fills the notification message with the given arguments.
 * @param nbchat The chat room number.
 * @param owner The owner of the message
 * @param data The message preview
 * @param datalen the message length, must be below 20 octets
 * @return The messaged filled correctly.
 */
int fill_notification(struct msghead h, char *buf, int bufsize, void *owner,
		      void *data);

int get_message(const void *msg, struct msghead *h, char *buf, int bufsize);

int get_inscription(void *msg, struct msghead *h, char *buf);

int get_udp(const char *msg, uint16_t msglen, struct msghead *h, char *buf,
	    int bufsize);

#endif
