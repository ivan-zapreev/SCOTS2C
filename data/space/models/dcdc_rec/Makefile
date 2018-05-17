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
CUDD =  -I/usr/local/include -L/usr/local/lib -lcudd
#
# optdet
#
OPTDETROOT              = ../../../../src/
OPTDETINC               = -I$(OPTDETROOT)/optdet/

.PHONY: dcdc_rec 

TARGET = dcdc_rec 

all: $(TARGET)

dcdc_rec: 
	$(CXX) dcdc_rec.cc -o dcdc_rec $(CXXFLAGS) $(SCOTSINC) $(CUDD) $(OPTDETINC)

clean:
	rm -f dcdc_rec
