#ifndef MEGAPHONE_MESSAGE_LIB_H
#define MEGAPHONE_MESSAGE_LIB_H

#include "../codereq.h"

#include <arpa/inet.h>

/**
 * @brief Fills the minimal header containing the user unique id ans the request code.
 * @param rcd The request code.
 * @param id The user id.
 * @return The header filled correctly.
 */
char *fill_min_header(enum reqcode rcd, uint16_t id);

/**
 * @brief Fills a generic header containing the given informations.
 * @param rcd The request code.
 * @param id The user id.
 * @param nbchat The chat room number.
 * @param nb Specific number related to the request type.
 * @param datalen The size of the data.
 * @param data The data itself.
 * @return The header filled correctly.
 */
char *fill_message(enum reqcode rcd, uint32_t id, uint16_t nbchat, uint16_t nb,
		   uint8_t datalen, char *data);
/**
 * @brief Fills the header requested for the inscription.
 * @param username The username.
 * @param len The username length.
 * @return the filled message.
 */
char *fill_inscription(char *username, int len);

/**
 * @brief Shortened version of fill_message for the push message request.
 * @param id The user id.
 * @param nbchat The chat room number.
 * @param datalen The size of message content.
 * @param data The message content.
 * @return The message filled correctly.
 */
char *push_message(uint16_t id, uint16_t nbchat, uint8_t datalen, char *data);

/**
 * @brief Shortened version of fill_message for the ask messages request.
 * @param id The user id.
 * @param nbchat The chat room number, 0 for all chat room
 * @param nb Number of asked messages, O for all messages
 * @return The message filled correctly.
 */
char *ask_messages(uint16_t id, uint16_t nbchat, uint16_t nb);

/**
 * @brief Shortened version of fill_message for the subscribe request.
 * @param id The user id.
 * @param nbchat The chat room number.
 * @return The message filled correctly.
 */
char *subscribe(uint16_t id, uint16_t nbchat);

/**
 * @brief Shortened version of fill_message for the push file request.
 * @param id The user id.
 * @param nbchat The chat room number.
 * @param datalen The file name length.
 * @param data The file name.
 * @return The message filled correctly.
 */
char *push_file(uint16_t id, uint16_t nbchat, uint8_t datalen, char *data);

/**
 * @brief Shortened version of fill_message for the pull file request.
 * @param id The user id.
 * @param nbchat The chat room number.
 * @param nb The port number.
 * @param datalen The file name length.
 * @param data The file name.
 * @return The message filled correctly
 */
char *pull_file(uint16_t id, uint16_t nbchat, uint16_t nb, uint16_t datalen,
		char *data);

/**
 * @brief Fills a generic header containing the given informations.
 * @param rcd The request code.
 * @param id The user id.
 * @param nb The block number.
 * @param datalen The size of the data.
 * @param data The data itself.
 * @return The messaged filled correctly.
 */
char *fill_udp(enum reqcode rcd, uint16_t id, uint16_t nb, int datalen,
	       char *data);

/**
 * @brief Shortened version of fill_udp for the push file request.
 * @param id The user id.
 * @param nb The block number.
 * @param datalen The size of the data.
 * @param data The data itself.
 * @return The messaged filled correctly.
 */
char *push_file_udp(uint16_t id, uint16_t nb, int datalen, char *data);
#endif
