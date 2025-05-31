CXX = g++
CXXFLAGS = -std=c++23 -Wall

SOURCES = $(wildcard *.cc)
OBJ = $(SOURCES:.cc=.o)

EXECUTABLE = busquedas_no_informadas

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJ) $(EXECUTABLE)
