#ifndef LIBSHOUTCAST
#define LIBSHOUTCAST_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <shout/shout.h>

char *shout_last_error();

shout_t *shout_start(char *host, int port, unsigned int protocol, char *password, char *user, char *mount, unsigned int format, unsigned int nonblocking);

int shout_send_buff(shout_t *shout, const unsigned char *buff, int bytes, unsigned int sync=1);

int shout_stop(shout_t *shout);

#endif
