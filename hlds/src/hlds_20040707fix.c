/*
 * Half-Life engine (before 07 July 2004) remote server/client crash workaround 0.1
 * Found by: Terry Henning (aka Soul Beaver)
 * Advisory by: Luigi Auriemma <aluigi@autistici.org>
 *              http://aluigi.altervista.org/adv/hlboom-adv.txt
 * Workaround (fix) code by: Tomas Janousek <tomi@nomi.cz>
 *
 * Compilation:
 *  gcc -D_GNU_SOURCE -fPIC -g -shared -ldl -o hlds_20040707fix.so hlds_20040707fix.c
 *
 * Usage: launch hlds with these env vars:
 *  HLDS_PORT=<port> LD_PRELOAD=/path/to/hlds_20040707fix.so
 */

#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <dlfcn.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>

static ssize_t (*old_recvfrom)(int s, void *buf, size_t len, int flags,
	struct sockaddr *from, socklen_t *fromlen) = 0;
static int (*old_bind)(int sockfd, const struct sockaddr *my_addr, socklen_t addrlen);
static uint16_t port = 0;
static int sock = -1;

int bind(int sockfd, const struct sockaddr *my_addr, socklen_t addrlen)
{
    if (!old_bind)
	old_bind = dlsym(RTLD_NEXT,"bind");
    if (!old_bind)
	exit(-1);

    if (!port) {
	const char *PORT = getenv("HLDS_PORT");
	if (!PORT)
	    exit(-1);
	port = atol(PORT);
	if (!port)
	    exit(-1);
    }

    if (my_addr && my_addr->sa_family == AF_INET &&
	    ntohs(((struct sockaddr_in*)my_addr)->sin_port) == port) {
	sock = sockfd;
    }

    return old_bind(sockfd, my_addr, addrlen);
}

ssize_t recvfrom(int s, void *buf, size_t len, int flags,
	struct sockaddr *from, socklen_t *fromlen)
{
    if (!old_recvfrom)
	old_recvfrom = dlsym(RTLD_NEXT,"recvfrom");
    if (!old_recvfrom)
	exit(-1);

    ssize_t ret = old_recvfrom(s, buf, len, flags, from, fromlen);

    if (ret == 8 && s == sock) {
	return 0;
    }

    return ret;
}

