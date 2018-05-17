#
# compiler
#
CC         	= g++
CXXFLAGS 	= -Wall -Wextra -std=c++11 -O3 -DNDEBUG -DSCOTS_BDD
#
# scots
#
SCOTSROOT	= ../../../../ext/SCOTSv2.0
SCOTSINC	= -I$(SCOTSROOT)/src -I$(SCOTSROOT)/utils
#
# cudd
#
CUDD            = -I/usr/local/include -L/usr/local/lib -lcudd

.PHONY: dcdc_rec

TARGET = dcdc_rec

all: $(TARGET)

dcdc_rec:
	$(CC) $(CXXFLAGS) $(SCOTSINC) dcdc_rec.cc -o dcdc_rec $(CUDD)

clean:
	rm dcdc_rec
