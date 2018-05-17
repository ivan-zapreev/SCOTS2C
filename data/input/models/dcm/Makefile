#
# compiler
#
CC        = g++
#CC       	  = clang++
CXXFLAGS 		= -g -Wall -Wextra -std=c++11 -DSCOTS_BDD
CXXFLAGS 		= -Wall -Wextra -std=c++11 -O3 -DNDEBUG -DSCOTS_BDD
#
# scots 
#
SCOTSROOT		= ../../../../ext/SCOTSv0.2
SCOTSINC		= -I$(SCOTSROOT)/src  -I$(SCOTSROOT)/utils/
#
# cudd 
#
CUDD		    =  -I/opt/local/include -L/opt/local/lib -lcudd


.PHONY: dcm

TARGET = dcm

all: $(TARGET)

dcm: 
	$(CC) $(CXXFLAGS) $(SCOTSINC) $(CUDD) dcm.cc -o dcm

clean:
	rm dcm
