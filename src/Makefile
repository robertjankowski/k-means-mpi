CXX = g++
CXXFLAGS = -Wall -Wextra -pedantic -std=c++11 # -02
DEPS = kmeans.h
OBJ = kmeans.o main.o 

%.o: %.c $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

main: $(OBJ)
	$(CXX) -o $@ $^ $(CXXFLAGS)

clean: 
	-rm $(OBJ) main