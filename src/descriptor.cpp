#include <limits.h>

#include "descriptor.h"
#include "reactor.h"
#include "Record.h"

const int sync_conn = 1;

void Descriptor::try_parse()
{
	if(sync_conn && !msg_queue.empty()) return;
	int ret;
	int c = readbuf.findchar(';');
	if(c == -1) return;
	c++;
	char buf[8192];
	memset(buf, 0, 8192 * sizeof(char));
	readbuf.read(buf, c);
	int state = 1; // parsing, at the beginning
	int format = 0;
	int field = 0;
	int op = 0;
	char name[256];
	int value = 0;
	int group = -1; // group number, if we only have one
	int minphone = INT_MIN, maxphone = INT_MAX;
	int n = 0;
	int del = 0;
	BasePredicate<DB::Record>* pr=0;
	for(int i=0; i < c; i++)
	{
//		std::cerr << "S: " << state << ' ' << buf[i] << std::endl;
		switch(state)
		{
		case 1: // command start
			switch(buf[i])
			{
			case ' ':
			case '\n':
				// skip
			break;
			case 'p':
				i++;
				if(buf[i] == 'i') // ping
				{
					msg_queue.push(new Pong());
					i += 2;
//					std::cerr << "Got here!" << i << std::endl;
				}
				else if(buf[i] == 'r') // print
				{
					reactor->db->ph.print(std::cout);
					i += 3;
				}
			break;
			case 'i': // insert
				char name[8192];
				int phone, group, len;
				memset(name, 0, 8192 * sizeof(char));
				ret = sscanf(buf + i, "insert (%[a-zA-Z0-9], %d, %d);%n", name, &phone, &group, &len);
				if(ret == 3 || ret == 4)
				{
					reactor->db->add(group, phone, name);
				}
				else if(ret < 0) handle_error("sscanf in parsing insert");
				else
				{
					std::cerr << "Failed to parse insert" << ret << std::endl;
					std::cerr << buf << std::endl;
					std::cerr << name << std::endl;
					exit(EXIT_FAILURE);
				}
//				std::cerr << len << std::endl;
				i += len - 2;
			break;
			case 'g': // getall
				i += 5;
				msg_queue.push(reactor->db->get_all());
			break;
			case 's':
				if(buf[i+1] == 'e') // select
				{
					i += 5;
					del = 0;
					state = 2;
				}
				else if(buf[i+1] == 'i') // size
				{
					i += 3;
					std::cerr << "Size: " << reactor->db->ph.size() << std::endl;
				}
			break;
			case 'd': // delete
				i += 5;
				del = 1;
				state = 4;
			break;
			case 'q': // quit
				i += 3;
				stp = 1;
			break;
			case 'e': // exit
				i += 3;
				reactor->stop();
			break;
			case ';': // got to end
				std::cerr << "Got here too!" << i << std::endl;
				format = 0;
				int cnt = readbuf.inc_reads(c);
				assert(cnt == c);
			break;
			}
		break;
		case 2: // select fields
			switch(buf[i])
			{
			case '*':
				format <<= 6;
				format += 57;
			break;
			case 'n': // name
				i += 3;
				format <<= 2;
				format += 1;
			break;
			case 'p': // phone
				i += 4;
				format <<= 2;
				format += 2;
			break;
			case 'g': // group
				i += 4;
				format <<= 2;
				format += 3;
			break;
			case 'w': // end of fields
				i--;
				state = 3;
			case ';': // end
				i--;
				state = 3;
			}
		break;
		case 3: // where condition after select, begin
		case 4: // where condition after delete, begin
			switch(buf[i])
			{
			case 'w': // where
				i += 4;
				assert(pr == 0);
			break;
			case 'n': // name
				i += 3;
				field = 1;
			break;
			case 'p': // phone
				i += 4;
				field = 2;
			break;
			case 'g': // group
				i += 4;
				field = 3;
			break;
			case '=':
				op = 1;
				state += 2;
			break;
			case '<':
				if(buf[i+1] == '>') { op = 2; i++; }
				else if(buf[i+1] == '=') { op = 5; i++; }
				else if(buf[i+1] == ' ') op = 3;
				state += 2;
			break;
			case '>':
				if(buf[i+1] == '=') { op = 6; i++; }
				else if(buf[i+1] == ' ') op = 4;
				state += 2;
			break;
			case 'a': // and
				i += 2;
			break;
			case ';': // end, real work here
				i--;
			case 'o': // or
				if(del)
				{
					for(int j=0;j<600;j++)
					{
						msg_queue.push(new AVLTree<DB::Record, 2, 5>::AVLResultIterator(
							&(reactor->db->group_ph[j]),
							minphone,
							maxphone,
							pr,
							format,
							1
						));
					}
					msg_queue.push(new AVLTree<DB::Record, 3, 5>::AVLResultIterator(
						&(reactor->db->ph),
						minphone,
						maxphone,
						pr,
						format,
						2
					));
				}
				else if(group == -1)
				{
					msg_queue.push(new AVLTree<DB::Record, 3, 5>::AVLResultIterator(
						&(reactor->db->ph),
						minphone,
						maxphone,
						pr,
						format,
						0
					));
				}
				else
				{
					msg_queue.push(new AVLTree<DB::Record, 2, 5>::AVLResultIterator(
						&(reactor->db->group_ph[group]),
						minphone,
						maxphone,
						pr,
						format,
						0
					));
				}
				pr = 0;
				group = -1;
				minphone = INT_MIN;
				maxphone = INT_MAX;
//				std::cerr << "How vould this happen?" << i << std::endl;
				if(buf[i+1] == ';') state = 1;
			break;
			}
		break;
		case 5: // where condition after select, value
		case 6: // where condition after delete, value
			while(buf[i] == ' ') i++;
			n = 0;
			if(field == 1) // name
			{
				memset(name, 0, 256 * sizeof(char));
				ret = sscanf(buf+i, "%[a-zA-Z0-9]%n", name, &n);
				assert(ret > 0);
				pr = new Predicate<DB::Record, 6>(op, String(name), pr);
			}
			else if(field == 2) // phone
			{
				ret = sscanf(buf+i, "%d%n", &value, &n);
				assert(ret > 0);
//				std::cerr << "OP: " << op << ' ' << value << std::endl;
				switch(op)
				{
				case 1: // =
					minphone = std::max(minphone, value);
					maxphone = std::min(maxphone, value);
				break;
				case 2: // <>
					pr = new Predicate<DB::Record, 5>(op, value, pr);
				break;
				case 3: // <
					maxphone = std::min(maxphone, value - 1);
				break;
				case 4: // >
					minphone = std::max(minphone, value + 1);
				break;
				case 5: // <=
					maxphone = std::min(maxphone, value);
				break;
				case 6: // >=
					minphone = std::max(minphone, value);
				break;
				}
			}
			else if(field == 3) // group
			{
				ret = sscanf(buf+i, "%d%n", &value, &n);
				assert(ret > 0);
				if(op == 1) group = value;
				pr = new Predicate<DB::Record, 4>(op, value, pr);
			}
			i += (n - 1);
			state -= 2;
		break;
		}
	}
}

size_t Descriptor::safe_read()
{
	if(!readbuf.free())
	{
		//std::cerr << "buffer overflow in Descriptor::safe_read()" << std::endl;
		return 0;
	}
	char buf[8192];
	size_t ret;
	ret = read(fd, buf, std::min(size_t(8192), readbuf.free()));
	if(ret == -1)
	{
		if(errno == EAGAIN || errno == EWOULDBLOCK) return 0;
		else handle_error("Descriptor::safe_read()");
	}
	if(ret)
	{
		size_t cnt = readbuf.write_inc(buf, ret);
		assert(cnt == ret);
		return cnt;
	}
	return 0;
}

size_t Descriptor::safe_write()
{
	if(writebuf.free() == 8192)
	{
		//std::cerr << "buffer underflow in Descriptor::safe_write()" << std::endl;
		return 0;
	}
	char buf[8192];
	size_t ret, cnt;
	cnt = writebuf.read(buf, writebuf.count());
//	if(cnt) std::cerr << "succ2! " << cnt << std::endl;
	ret = write(fd, buf, cnt);
	if(ret == -1)
	{
		if(errno == EAGAIN || errno == EWOULDBLOCK) return 0;
		else handle_error("Descriptor::safe_write()");
	}
	if(ret)
	{
		cnt = writebuf.inc_reads(ret);
		assert(cnt == ret);
		return cnt;
	}
	return 0;
}

void Descriptor::try_queue_to_buf()
{
/*	if(!msg_queue.empty())
	{
		std::cerr << msg_queue.front()->size() << ' ';
		std::cerr << writebuf.free() << ' ';
		std::cerr << std::endl;
	}*/
	if(cur_string && cur_string->size() + 1 <= writebuf.free())
	{
		writebuf.write_inc(cur_string->c_str(), cur_string->size());
		writebuf.write_inc("\n", 1);
//		std::cerr << "success" << std::endl;
		delete cur_string;
		cur_string = 0;
	}
	if(!msg_queue.empty() && !cur_string)
	{
		cur_string = msg_queue.front()->next();
		if(!cur_string)
		{
			delete msg_queue.front();
			msg_queue.pop();
		}
	}
	if(stp && !cur_string && msg_queue.empty() && writebuf.count() == 0)
	{
		reactor->closedset.push_back(this);
	}
}

