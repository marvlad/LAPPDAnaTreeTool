# Define the compiler
CXX = g++

CXXFLAGS = -std=c++11

ROOTFLAGS = `root-config --cflags --glibs --libs`

SOURCES = MultiAnalysis.cpp Macro.cpp 

OBJECTS = $(SOURCES:.cpp=.o)

EXECUTABLE = MultiAnalysis

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(ROOTFLAGS) $(OBJECTS) -o $(EXECUTABLE)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(ROOTFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
