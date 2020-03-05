CXX = mpic++
CXXFLAGS = -Wall -Wextra -pedantic -std=c++11 -O2
DEPS = kmeans.h
OBJMPI = kmeans.o mainMPI.o
OBJ = kmeans.o main.o 

all: main mainMPI

%.o: %.c $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

main: $(OBJ)
	$(CXX) -o $@ $^ $(CXXFLAGS)

mainMPI: $(OBJMPI)
	$(CXX) -o $@ $^ $(CXXFLAGS)

clean: 
	-rm -f $(OBJ) $(OBJMPI) main mainMPI