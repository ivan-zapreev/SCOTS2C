#
# compiler
#
CC        = g++
#CC       	  = clang++
#CXXFLAGS 		= -g -Wall -Wextra -std=c++11
CXXFLAGS 		= -Wall -Wextra -std=c++11 -O3 -DNDEBUG -DSCOTS_BDD
#
# scots 
#
SCOTSROOT		= ../../../../ext/SCOTSv0.2
SCOTSINC		= -I$(SCOTSROOT)/src -I$(SCOTSROOT)/utils
#
# cudd 
#
CUDD		    =  -I/usr/local/include -L/usr/local/lib -lcudd
#
# optdet
#
OPTDETROOT              = ../../../../src/
OPTDETINC               = -I$(OPTDETROOT)/optdet/

.PHONY: aircraft

TARGET = aircraft

all: $(TARGET)

aircraft:
	$(CC) aircraft.cc -o aircraft $(CUDD) $(CXXFLAGS) $(SCOTSINC) $(OPTDETINC)

clean:
	rm -f aircraft
