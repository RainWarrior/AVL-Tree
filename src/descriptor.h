#ifndef __DESCRIPTOR_H
#define __DESCRIPTOR_H

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

#include "string.h"
#include "buffer.h"
#include "resultiterator.h"

class Reactor;

class Descriptor
{
	Buffer<13> readbuf;
	Buffer<13> writebuf;
	String* cur_string;
	std::queue<ResultIterator*> msg_queue;
	int fd;
	sockaddr_in client;
	Reactor *reactor;
	int stp;
public:
	Descriptor(int desc, const sockaddr_in& addr)
		: readbuf()
		, writebuf()
		, cur_string(0)
		, msg_queue()
		, fd(desc)
		, client(addr)
		, stp(0)
	{
		cur_string = new String("Welcome");
	}
	void stop() { stp = 1; }
	int getfd() { return fd; }
	size_t safe_read();
	size_t safe_write();
	void try_queue_to_buf();
	void try_parse();
	friend class TCPConnector;
};

#endif // __DESCRIPTOR_H
