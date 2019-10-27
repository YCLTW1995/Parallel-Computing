CC = mpicc
CXX = mpicxx
CFLAGS = -O3 -std=gnu99
CXXFLAGS = -O3 -std=gnu++11

STUDENTID = $(USER:p%=%)
TARGETS = oesort_multipleelement oesort_singleelement

.PHONY: all
all: $(TARGETS)

.PHONY: clean
clean: 
	rm -f $(TARGETS)
