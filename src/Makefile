CXX = g++
CXXMPI = mpic++
CXXFLAGS = -Wall -Wextra -pedantic -std=c++11 -O2 -fopenmp

DEPS = kmeans.h kmeans_openmp.h observation.h utils.h benchmark.h

OBJBASE = observation.o kmeans.o kmeans_openmp.o 
OBJ = $(OBJBASE) main.o
OBJMP = $(OBJBASE) mainMP.o
OBJMPI = $(OBJBASE) mainMPI.o

all: main mainMPI mainMP

%.o: %.c $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

main: $(OBJ)
	$(CXX) -o $@ $^ $(CXXFLAGS)

mainMPI: $(OBJMPI)
	$(CXXMPI) -o $@ $^ $(CXXFLAGS)

mainMP: $(OBJMP)
	$(CXX) -o $@ $^ $(CXXFLAGS)

.PHONY: clean
clean: 
	-rm -f $(OBJ) $(OBJMP) $(OBJMPI) main mainMPI mainMP