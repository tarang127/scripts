all: clean daemon

INC=-I/home5/investa5/include

LDLIBS=-L/home5/investa5/lib -lcurl

CXX=g++

daemon: downloadDaemon.o
	g++ downloadDaemon.o -o daemon $(LDLIBS)

%.o: %.c
	$(CXX) $(INC) -g -c $<
clean:
	rm -rf *o 
