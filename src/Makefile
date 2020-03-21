CXX = g++
CXXMPI = mpic++
CXXFLAGS = -Wall -Wextra -pedantic -std=c++11 -O2 -fopenmp

DEPS = observation.h utils.h benchmark.h measure_time.h

OBJBASE = observation.o
OBJ = $(OBJBASE) kmeans.o main.o
OBJMP = $(OBJBASE) kmeans_openmp.o mainOpenMP.o
OBJMPI = $(OBJBASE) kmeans_mpi.o mainMPI.o kmeans.o

all: main mainMPI mainOpenMP

%.o: %.c $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

main: $(OBJ) kmeans.h benchmark_single.h
	$(CXX) -o $@ $^ $(CXXFLAGS)

mainMPI: $(OBJMPI) kmeans_mpi.h 
	$(CXXMPI) -o $@ $^ $(CXXFLAGS)

mainOpenMP: $(OBJMP) kmeans_openmp.h benchmark_openmp.h
	$(CXX) -o $@ $^ $(CXXFLAGS)

.PHONY: clean
clean: 
	-rm -f $(OBJ) $(OBJMP) $(OBJMPI) main mainMPI mainOpenMP