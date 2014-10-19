#ifndef __REACTOR_H
#define __REACTOR_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <errno.h>
#include <assert.h>

#include <algorithm>
#include <iostream>
#include <list>
#include <queue>

#include "descriptor.h"
#include "Record.h"

inline void handle_error(const char * msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}

class Reactor;

class TCPConnector
{
	int fd;
	int port;
	sockaddr_in addr;
public:
	TCPConnector(int p);
	void safe_listen();
	void safe_accept(Reactor &r);
	void safe_close();
};

class Reactor
{
	std::list<Descriptor*> readset, writeset, closedset;
	typedef std::list<Descriptor*>::iterator It;
	timeval timeout;
	int running;
	int stopping;
	TCPConnector connector;

	DB *db;

	void doWork();
	void doSelect(timeval timeout);
public:
	Reactor(int port, DB* d) : running(0), stopping(0), connector(port), db(d)
	{
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;
	}
	void run();
	void stop();
	friend class TCPConnector;
	friend class Descriptor;
};

#endif // __REACTOR_H
