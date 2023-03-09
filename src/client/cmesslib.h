#ifndef MEGAPHONE_CMESSLIB_H
#define MEGAPHONE_CMESSLIB_H

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
char *fill_message(enum reqcode req, uint16_t id, uint16_t chat, uint16_t nb,
		   uint8_t datalen, const char *data);
/**
 * @brief Fills the header requested for the inscription.
 * @param username The username.
 * @param len The username length.
 * @return the filled message.
 */
char *fill_inscription(const char *username, int len);

/**
 * @brief Shortened version of fill_message for the push message request.
 * @param id The user id.
 * @param chat The chat room number.
 * @param datalen The size of message content.
 * @param data The message content.
 * @return The message filled correctly.
 */
char *fill_push_message(uint16_t id, uint16_t chat, uint8_t datalen,
			const char *data);

/**
 * @brief Shortened version of fill_message for the ask messages request.
 * @param id The user id.
 * @param chat The chat room number, 0 for all chat room
 * @param nb Number of asked messages, O for all messages
 * @return The message filled correctly.
 */
char *fill_ask_messages(uint16_t id, uint16_t chat, uint16_t nb);

/**
 * @brief Shortened version of fill_message for the fill_subscribe request.
 * @param id The user id.
 * @param chat The chat room number.
 * @return The message filled correctly.
 */
char *fill_subscribe(uint16_t id, uint16_t chat);

/**
 * @brief Shortened version of fill_message for the push file request.
 * @param id The user id.
 * @param chat The chat room number.
 * @param datalen The file name length.
 * @param data The file name.
 * @return The message filled correctly.
 */
char *fill_push_file(uint16_t id, uint16_t chat, uint8_t datalen,
		     const char *data);

/**
 * @brief Shortened version of fill_message for the pull file request.
 * @param id The user id.
 * @param chat The chat room number.
 * @param nb The port number.
 * @param datalen The file name length.
 * @param data The file name.
 * @return The message filled correctly
 */
char *fill_pull_file(uint16_t id, uint16_t chat, uint16_t nb, uint16_t datalen,
		     const char *data);

/**
 * @brief Fills a generic header containing the given informations.
 * @param req The request code.
 * @param id The user id.
 * @param nb The block number.
 * @param datalen The size of the data.
 * @param data The data itself.
 * @return The messaged filled correctly.
 */
char *fill_udp(enum reqcode req, uint16_t id, uint16_t nb, int datalen,
	       const char *data);

/**
 * @brief Shortened version of fill_udp for the push file request.
 * @param id The user id.
 * @param nb The block number.
 * @param datalen The size of the data.
 * @param data The data itself.
 * @return The messaged filled correctly.
 */
char *fill_push_file_udp(uint16_t id, uint16_t nb, int datalen,
			 const char *data);

int get_message(const char *msg, enum reqcode *req, uint16_t *id,
		uint16_t *chat, uint16_t *nb);

int get_asked_messages(const char *msg, uint16_t *chat, char **origin,
		       char **owner, uint8_t *datalen, char **data);

int get_subscribed_message(const char *msg, enum reqcode *req, uint16_t *id,
			   uint16_t *chat, uint16_t *nb, char **addr);

int get_notification_message(const char *msg, enum reqcode *req, uint16_t *id,
			     uint16_t *chat, char **owner, char **data);

int get_udp_message(const char *msg, int msglen, enum reqcode *req,
		    uint16_t *id, uint16_t *block, uint16_t *datalen,
		    char **data);
#endif
