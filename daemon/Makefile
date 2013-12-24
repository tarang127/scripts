CC = g++
CFLAGS = -g -Wall
INC=-I/home5/investa5/include -I/home5/investa5/scripts/daemon/include
CXXFLAGS = $(CFLAGS) $(INC)

LDFLAGS = -L/home5/investa5/lib
LIBS = -lcurl
SRCXX = downloadDaemon.cpp
OBJS = $(SRCXX:.cpp=.o) 

all: clean daemon

daemon: $(OBJS) 
	$(CC) $(CXXFLAGS) -o daemon $(OBJS) $(LDFLAGS) $(LIBS)

clean:
	rm -rf daemon
	rm -rf *o 
