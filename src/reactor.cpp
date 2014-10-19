#include "reactor.h"

TCPConnector::TCPConnector(int p) : fd(-1), port(p)
{
	fd = socket(AF_INET, SOCK_STREAM, 0);
	if(fd < 0) handle_error("cannot create socket");

	int flags = fcntl(fd, F_GETFL, 0);
	if(flags < 0) handle_error("cannot get descriptor flags");

	int ret = fcntl(fd, F_SETFL, flags | O_NONBLOCK);
	if(ret) handle_error("cannot set descriptor flags");

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(port);

	ret = bind(fd, (sockaddr*)&addr, sizeof(sockaddr_in));
	if(ret) handle_error("bind");
}

void TCPConnector::safe_listen()
{
	int ret = listen(fd, 10);
	if(ret) handle_error("listen");
}

void TCPConnector::safe_accept(Reactor &r)
{
	sockaddr_in client;
	socklen_t addr_size = sizeof(sockaddr_in);
	int ret = accept(fd, (sockaddr*)&client, &addr_size);
	if(ret == -1)
	{
		if(errno == EAGAIN || errno == EWOULDBLOCK) return;
		else handle_error("accept");
	}
	Descriptor *d = new Descriptor(ret, client);
	d->reactor = &r;
	r.readset.push_front(d);
	r.writeset.push_front(d);
}

void TCPConnector::safe_close()
{
	int ret = close(fd);
	if(ret == -1) handle_error("close");
}

void Reactor::run()
{
	running = 1;
	connector.safe_listen();
	while(running)
	{
		doWork();
		doSelect(this->timeout);
	}
}

void Reactor::stop()
{
	for(It i=readset.begin(); i != readset.end(); i++) (*i)->stop();
	stopping = 1;
}
void Reactor::doSelect(timeval timeout)
{
	fd_set rfds, wfds;
	FD_ZERO(&rfds);
	FD_ZERO(&wfds);
	for(It i=readset.begin(); i != readset.end(); i++) FD_SET((*i)->getfd(), &rfds);
	for(It i=writeset.begin(); i != writeset.end(); i++) FD_SET((*i)->getfd(), &wfds);

	int ret = select(FD_SETSIZE, &rfds, &wfds, NULL, &timeout);
	if(ret == -1) handle_error("select");

	else if(ret) // something avaluable
	{
		// do read/write
		for(It i=readset.begin(); i != readset.end(); i++)
		{
			if(FD_ISSET((*i)->getfd(), &rfds)) (*i)->safe_read();
			if(FD_ISSET((*i)->getfd(), &wfds)) (*i)->safe_write();
		}
	}
}

void Reactor::doWork()
{
	connector.safe_accept(*this);
	for(It i = closedset.begin(); i != closedset.end(); i++)
	{
		readset.remove(*i);
		writeset.remove(*i);
		close((*i)->getfd());
		delete *i;
	}
	closedset.clear();
	for(It i = readset.begin(); i != readset.end(); i++) (*i)->try_parse();
	for(It i = writeset.begin(); i != writeset.end(); i++) (*i)->try_queue_to_buf();
	if(stopping && readset.empty() && writeset.empty())
	{
		connector.safe_close();
		running = 0;
	}
//	std::cout << "working" << std::endl;
}
