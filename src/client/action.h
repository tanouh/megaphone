#ifndef MEGAPHONE_ACTION_H
#define MEGAPHONE_ACTION_H
/**
 * @brief 
 * 
 * @param id pointer of the newly created identifier
 * @param arg  socketfd
 * @return int 
 */
int registering(void *id, void *sock);

/**
 * @brief Post a ticket in a given discussion feed
 * 
 * @param id 
 * @param arg 
 * @return int 
 */
int post(void *id, void *sock);

int add_file();
/**
 * @brief Get the last n messages in a/all discussion feed(s)
 * 
 * @param id 
 * @param arg 
 * @return int 
 */
int see_old_ticket(void *id, void *sock);

int download();

int subscribe();

#endif