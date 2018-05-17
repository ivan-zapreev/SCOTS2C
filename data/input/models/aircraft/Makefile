#
# compiler
#
CC		= g++
CXXFLAGS 	= -Wall -Wextra -std=c++11 -O3 -DNDEBUG -DSCOTS_BDD
#
# scots 
#
SCOTSROOT	= ../../../../ext/SCOTSv2.0
SCOTSINC	= -I$(SCOTSROOT)/src -I$(SCOTSROOT)/utils
#
# cudd 
#
CUDD    	=  -I/opt/local/include -L/opt/local/lib -I/usr/local/include -L/usr/local/lib -lcudd

.PHONY: aircraft

TARGET = aircraft

all: $(TARGET)

aircraft: 
	$(CC) $(CXXFLAGS) $(SCOTSINC) aircraft.cc -o aircraft $(CUDD)

clean:
	rm aircraft
