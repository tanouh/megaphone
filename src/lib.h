#ifndef MEGAPHONE_LIB_H
#define MEGAPHONE_LIB_H

#define ID_MASK(id) ((id)&ID_MAX)

char *htonx(char *h, int len);
char *ntohx(char *h, int len);

#endif
