.DEFAULT: all

vpath %.h ./src
vpath %.cpp ./src

.PHONY: all
all: main.o reactor.o descriptor.o String.o
	g++ -g -o a.out $^

main.o: main.cpp
	g++ -c -g -o $@ $<

reactor.o: reactor.cpp
	g++ -c -g -o $@ $<

descriptor.o: descriptor.cpp
	g++ -c -g -o $@ $<

String.o: String.cpp
	g++ -c -g -o $@ $<

.PHONY: clean

clean:
	rm -f *.o
