CC=g++
CFLAGS=-g -I.
DEPS = Socket.h BlackJack.h
OBJ = Socket.o BlackJack.o
LIBS=-lpthread

%.o: %.cc $(DEPS)
	$(CC) -g -c -o $@ $< $(CFLAGS)

all: cs cc

cs: $(OBJ) BlackJackServer.o
	g++ -o $@ $^ $(CFLAGS) $(LIBS)

cc: $(OBJ) BlackJackClient.o
	g++ -o $@ $^ $(CFLAGS) $(LIBS)


.PHONY: clean

clean:
	rm -f *.o cs cc

