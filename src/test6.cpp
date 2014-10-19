#include "buffer.h"
#include "reactor.h"

using namespace std;

int ff(int x, int y)
{
	return x+y;
}

int main()
{
	auto cb = Callback(ff, 3, 4);
	cout << cb.call() << endl;
	Buffer<4> buf;
	char b2[17];
	cout << buf.write("Hello, world!", 12) << endl;
	cout << buf.write("testtesttest", 11) << endl;
	memset(b2, 0, 17);
	cout << buf.read(b2, 10) << endl;
	cout << b2 << endl;
	cout << buf.write("TESTTESTTEST", 11) << endl;
	memset(b2, 0, 17);
	cout << buf.read(b2, 10) << endl;
	cout << b2 << endl;
	memcpy(b2, buf.getbuf(), 16);
	cout << b2 << endl;
	return 0;
}

