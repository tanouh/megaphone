#include "msghead.h"

struct msghead fill_msghead(enum reqcode req, uint16_t id, uint16_t chat,
			    uint16_t nb, uint8_t datalen)
{
	return (struct msghead){.req = req,
				.id = id,
				.chat = chat,
				.nb = nb,
				.datalen = datalen};
}

struct msghead fill_errhead(uint16_t errcode) {
	return fill_msghead(ERROR, errcode, 0, 0, 0);
}
