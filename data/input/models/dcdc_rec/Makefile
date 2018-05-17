#
# compiler
#
CXX       	  	= g++
#CXXFLAGS 		= -Wall -Wextra -std=c++11 -DSCOTS_BDD
CXXFLAGS 		= -Wall -Wextra -std=c++11 -O3 -DNDEBUG -DSCOTS_BDD
#
# scots 
#
SCOTSROOT		= ../../../../ext/SCOTSv0.2
SCOTSINC		= -I$(SCOTSROOT)/src  -I$(SCOTSROOT)/utils/
#
# cudd 
#
CUDDINC =  -I/usr/local/include 
CUDDLIB =  -L/usr/local/lib -lcudd


.PHONY: dcdc_rec 

TARGET = dcdc_rec 

all: $(TARGET)

dcdc_rec: 
	$(CXX) $(CXXFLAGS) $(CUDDINC) $(SCOTSINC)  dcdc_rec.cc -o dcdc_rec $(CUDDLIB)

clean:
	rm dcdc_rec
