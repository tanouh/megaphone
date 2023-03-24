#include "slib.h"

#include "../map.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int compare_identifiers(void *key1, void *key2)
{
	return strcmp((char *)key1, (char *)key2) == 0;
}

void free_identifier(void *id)
{
	free(id);
}

void free_nickname(void *nickname)
{
	free(nickname);
}
