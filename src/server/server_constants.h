#ifndef MEGAPHONE_SERVER_CONSTANTS_H
#define MEGAPHONE_SERVER_CONSTANTS_H
#include <arpa/inet.h>

extern uint16_t server_port;
extern uint16_t mult_port;
extern char *mult_index_name;

#define DEFAULT_MULT_PORT 10405
#define NBCLIENTSMAX	  2048

#define DEFAULT_INDEX_NAME "eth0"

#endif
