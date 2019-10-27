CC = mpicc
CXX = mpicxx
LDFLAGS = -lpng
CFLAGS = -O3 -std=gnu99
CXXFLAGS = -O3 -std=gnu++11 -pthread
TARGETS =  APSP_Pthread APSP_MPI_sync APSP_MPI_async APSP_Hyprid

.PHONY: all
all: $(TARGETS) 

.PHONY: clean
clean:
	rm -f $(TARGETS) $(TARGETS:=.o)
