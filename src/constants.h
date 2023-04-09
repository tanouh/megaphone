#ifndef MEGAPHONE_CONSTANTS_H
#define MEGAPHONE_CONSTANTS_H

#define LOCALHOST4 "127.0.0.1"
#define LOCALHOST6 "::1"

#define SERVER	  "\x1B[35m[SERVER]\x1B[0m"
#define CLIENT	  "\x1B[36m[CLIENT]\x1B[0m"
#define MEGAPHONE "\x1B[33m[MEGAPHONE]\x1B[0m"

#define SIZE_MSG 256
#define SBUF	 1024

#define NAMELEN		     10
#define ID_MAX		     2048
#define REQ_MAX		     31
#define MAX_HEADER	     7
#define MIN_HEADER	     2
#define HEADER_SERVER	     6
#define ADDRMULT_LEN	     16
#define NOTIFICATION_CONTENT 20
#define ID_BITS		     11
#define REQ_BITS	     5

#define DEFAULT_SERVER_ADDR "fdc7:9dd5:2c66:be86:4849:43ff:fe49:79be/64"
#define DEFAULT_SERVER_PORT 4242

#endif
