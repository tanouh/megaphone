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
void *fill_message(enum reqcode req, uint16_t id, uint16_t chat, uint16_t nb,
		   uint8_t datalen, const void *data, int *size_msg);
/**
 * @brief Fills the header requested for the inscription.
 * @param username The username.
 * @param len The username length.
 * @return the filled message.
 */
void *fill_inscription(const void *username, int len, int *size_msg);

/**
 * @brief Shortened version of fill_message for the push message request.
 * @param id The user id.
 * @param chat The chat room number.
 * @param datalen The size of message content.
 * @param data The message content.
 * @return The message filled correctly.
 */
void *fill_push_message(uint16_t id, uint16_t chat, uint8_t datalen,
			const void *data, int *size_msg);

/**
 * @brief Shortened version of fill_message for the ask messages request.
 * @param id The user id.
 * @param chat The chat room number, 0 for all chat room
 * @param nb Number of asked messages, O for all messages
 * @return The message filled correctly.
 */
void *fill_ask_messages(uint16_t id, uint16_t chat, uint16_t nb, int *size_msg);

/**
 * @brief Shortened version of fill_message for the fill_subscribe request.
 * @param id The user id.
 * @param chat The chat room number.
 * @return The message filled correctly.
 */
void *fill_subscribe(uint16_t id, uint16_t chat, int *size_msg);

/**
 * @brief Shortened version of fill_message for the push file request.
 * @param id The user id.
 * @param chat The chat room number.
 * @param datalen The file name length.
 * @param data The file name.
 * @return The message filled correctly.
 */
void *fill_push_file(uint16_t id, uint16_t chat, uint8_t datalen,
		     const void *data, int *size_msg);

/**
 * @brief Shortened version of fill_message for the pull file request.
 * @param id The user id.
 * @param chat The chat room number.
 * @param nb The port number.
 * @param datalen The file name length.
 * @param data The file name.
 * @return The message filled correctly
 */
void *fill_pull_file(uint16_t id, uint16_t chat, uint16_t nb, uint16_t datalen,
		     const void *data, int *size_msg);

/**
 * @brief Fills a generic header containing the given informations.
 * @param req The request code.
 * @param id The user id.
 * @param nb The block number.
 * @param datalen The size of the data.
 * @param data The data itself.
 * @return The messaged filled correctly.
 */
void *fill_udp(enum reqcode req, uint16_t id, uint16_t nb, int datalen,
	       const void *data, int *size_msg);

/**
 * @brief Shortened version of fill_udp for the push file request.
 * @param id The user id.
 * @param nb The block number.
 * @param datalen The size of the data.
 * @param data The data itself.
 * @return The messaged filled correctly.
 */
void *fill_push_file_udp(uint16_t id, uint16_t nb, int datalen,
			 const void *data, int *size_msg);

int get_message(const void *msg, enum reqcode *req, uint16_t *id,
		uint16_t *chat, uint16_t *nb);

int get_asked_messages(const void *msg, uint16_t *chat, void **origin,
		       void **owner, uint8_t *datalen, void **data);

int get_subscribed_message(const void *msg, enum reqcode *req, uint16_t *id,
			   uint16_t *chat, uint16_t *nb, void **addr);

int get_notification_message(const void *msg, enum reqcode *req, uint16_t *id,
			     uint16_t *chat, void **owner, void **data);

int get_udp_message(const void *msg, int msglen, enum reqcode *req,
		    uint16_t *id, uint16_t *block, uint16_t *datalen,
		    void **data);
#endif
