CXX = g++
CXXFLAGS = -Wall
SRC = greedy.cc
EXECUTABLE = greedy.out
TEST_FOLDER = tests

all: $(EXECUTABLE)

$(EXECUTABLE): $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $<

run: $(EXECUTABLE)
	./$(EXECUTABLE)

clean:
	rm -f $(EXECUTABLE)
	rm -f $(TEST_FOLDER)/*.txt

.PHONY: all run clean
