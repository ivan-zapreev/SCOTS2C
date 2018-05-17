#
# compiler
#
CC        = g++
#CC       	  = clang++
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

.PHONY: vehicle

TARGET = vehicle

all: $(TARGET)

vehicle:
	$(CC) vehicle.cc -o vehicle $(CXXFLAGS) $(SCOTSINC) $(CUDD) $(OPTDETINC)

clean:
	rm -f vehicle
