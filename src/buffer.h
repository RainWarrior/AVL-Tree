#include <cstring>

template<int sz>
class Buffer
{
	static const size_t size = 1 << sz;
	static const size_t mask = size - 1;
	char buffer[size];
	size_t reads, writes;
public:
	Buffer() : reads(0), writes(0)
	{
		memset(buffer, '*', sizeof(char) * size); 
	}
	size_t count()
	{
		return writes - reads;
	}
	size_t free()
	{
		return size + reads - writes;
	}
	size_t write_inc(const char *data, size_t count)
	{
		size_t i=0;
		while(writes - reads < size && i < count)
			buffer[(writes++) & mask] = data[i++];
		return i;
	}
	size_t write(const char *data, size_t count)
	{
		size_t i=0, j=writes;
		while(j - reads < size && i < count)
			buffer[(j++) & mask] = data[i++];
		return i;
	}
	size_t read_inc(char *data, size_t count)
	{
		size_t i=0;
		while(reads < writes && i < count)
			data[i++] = buffer[(reads++) & mask];
		if(reads > size << 2)
		{
			reads -= size << 2;
			writes -= size << 2;
		}
		return i;
	}
	size_t read(char *data, size_t count)
	{
		size_t i=0, j=reads;
		while(j < writes && i < count)
			data[i++] = buffer[(j++) & mask];
		return i;
	}
	size_t inc_reads(size_t v)
	{
		if(reads + v < writes)
		{
			reads += v;
		}
		else
		{
			v = writes - reads;
			reads = writes;
		}
		if(reads > size << 2)
		{
			reads -= size << 2;
			writes -= size << 2;
		}
		return v;
	}
	size_t inc_writes(size_t v)
	{
		if(writes + v < reads + size)
		{
			writes += v;
		}
		else
		{
			v = reads + size - writes;
			writes = reads + size;
		}
		return v;
	}
	const char * getbuf()
	{
		return buffer;
	}
	int findchar(char c)
	{
		size_t i = reads;
		while(i != writes)
		{
			if(c == buffer[i])
			{
				return i - reads;
			}
			i++;
		}
		return -1;
	}
};
