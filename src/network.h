#ifndef NETWORK_H
#define NETWORK_H

#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>
// #include "util.h"

int set_nonblocking(int);

int set_tcp_nodelay(int);

int create_and_bind(const char *, const char *, int);

int make_listen(const char *, const char *, int);

int accept_connection(int);

ssize_t send_bytes(int, const unsigned char *, size_t);

ssize_t recv_bytes(int, unsigned char *, size_t);

#endif
