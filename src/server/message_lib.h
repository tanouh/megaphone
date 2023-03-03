#ifndef MEGAPHONE_MESSAGE_LIB_H
#define MEGAPHONE_MESSAGE_LIB_H

#include "../reqcode.h"

#include <arpa/inet.h>

/**
 * @brief Fills the minimal header containing the user unique id ans the request
 * code.
 * @param req The request code.
 * @param id The user id.
 * @return The header filled correctly.
 */
char *fill_min_header(enum reqcode req, uint16_t id);

/**
 * @brief Fills a generic header containing the given informations.
 * @param req The request code.
 * @param id The user id.
 * @param nbchat The chat room number.
 * @param nb Specific number related to the request type.
 * @return The header filled correctly.
 */
char *fill_message(enum reqcode req, uint32_t id, uint16_t nbchat, uint16_t nb);
/**
 * @brief Fills the header requested for the inscription.
 * @param id The user id.
 * @return the filled message.
 */
char *fill_inscription(uint16_t id);

/**
 * @brief Shortened version of fill_message for the push message request.
 * @param id The user id.
 * @param nbchat The chat room number.
 * @return The message filled correctly.
 */
char *fill_push_message(uint16_t id, uint16_t nbchat);

/**
 * @brief Shortened version of fill_message for the ask messages request.
 * @param id The user id.
 * @param nbchat The chat room number, 0 for all chat room
 * @param nb Number of asked messages, O for all messages
 * @return The message filled correctly.
 */
char *fill_ask_messages(uint16_t id, uint16_t nbchat, uint16_t nb);

/**
 * @brief Shortened version of fill_message for the fill_subscribe request.
 * @param id The user id.
 * @param nbchat The chat room number.
 * @param nb The port number for multi-broadcast.
 * @param addrmult The ipv6 multi-broadcast address.
 * @return The message filled correctly.
 */
char *fill_subscribe(uint16_t id, uint16_t nbchat, uint16_t nb, char *addrmult);

/**
 * @brief Shortened version of fill_message for the push file request.
 * @param id The user id.
 * @param nbchat The chat room number.
 * @return The message filled correctly.
 */
char *fill_push_file(uint16_t id, uint16_t nbchat);

/**
 * @brief Shortened version of fill_message for the pull file request.
 * @param id The user id.
 * @param nbchat The chat room number.
 * @param nb The port number.
 * @return The message filled correctly
 */
char *fill_pull_file(uint16_t id, uint16_t nbchat, uint16_t nb);

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
	       char *data);

/**
 * @brief Shortened version of fill_udp for the push file request.
 * @param id The user id.
 * @param nb The block number.
 * @param datalen The size of the data.
 * @param data The data itself.
 * @return The messaged filled correctly.
 */
char *fill_push_file_udp(uint16_t id, uint16_t nb, int datalen, char *data);

/**
 * @brief Fills error message.
 * @return The messaged filled correctly.
 */
char *fill_error();

/**
 * @brief Fills the notification message with the given arguments.
 * @param nbchat The chat room number.
 * @param owner The owner of the message
 * @param data The message preview
 * @param datalen the message length, must be below 20 octets
 * @return The messaged filled correctly.
 */
char *fill_notification(uint16_t nbchat, char *owner, char *data, int datalen);

#endif
