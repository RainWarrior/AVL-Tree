#include "reactor.h"

#include <signal.h>
#include <fstream>
//#include <stdio.h>

int main(int argc, char **argv)
{
	signal(SIGPIPE, SIG_IGN);
	std::ifstream fin("a.txt");
//	FILE *f = fopen("a.txt", "r");
	char name[256];
	memset(name, 0, 256 * sizeof(char));
	int phone, group;
	DB db;
	while(fin >> name >> phone >> group)
//	while(fscanf(f, "%s %d %d", name, &phone, &group) == 3)
	{
		db.add(group, phone, name);
	}
	Reactor r(atoi(argv[1]), &db);
	std::cerr << "Ready" << std::endl;
	r.run();
}
