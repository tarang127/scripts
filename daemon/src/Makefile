CC = g++
CFLAGS = -g -Wall `xml2-config --cflags`
INC=-I/home5/investa5/include -I/home5/investa5/scripts/daemon/include
CXXFLAGS = $(CFLAGS) $(INC)

LDFLAGS = -L/home5/investa5/lib
LIBS = -lcurl `xml2-config --libs`
SRCXX = downloadDaemon.cpp files.cpp
OBJS = $(SRCXX:.cpp=.o) 

all: clean daemon

daemon: $(OBJS) 
	$(CC) $(CXXFLAGS) -o daemon $(OBJS) $(LDFLAGS) $(LIBS)

clean:
	rm -rf ../files/*.xml
	rm -rf ../files/*.xml.html
	rm -rf daemon
	rm -rf *o 
