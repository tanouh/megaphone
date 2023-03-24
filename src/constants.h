#ifndef MEGAPHONE_CONSTANTS_H
#define MEGAPHONE_CONSTANTS_H

#define PORT 4243
#define LOCALHOST4 "127.0.0.1"
#define LOCALHOST6 "::1"



#define SERVER "\x1B[35m[SERVER]\x1B[0m"
#define CLIENT "\x1B[36m[CLIENT]\x1B[0m"
#define MEGAPHONE "\x1B[33m[MEGAPHONE]\x1B[0m"

#define SBUF 256
#define NBCLIENTSMAX 2048
#define NAMELEN 10
#define ID_MAX 2048
#define REQ_MAX 31
#define MAX_HEADER 7
#define MIN_HEADER 2
#define HEADER_SERVER 6
#define ADDRMULT_LEN 16
#define NOTIFICATION_CONTENT 20
#define ID_BITS 11
#define REQ_BITS 5
#endif
