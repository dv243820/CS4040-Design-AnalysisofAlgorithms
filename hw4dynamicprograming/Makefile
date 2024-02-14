CXX = g++
CXXFLAGS = -Wall

TARGET = hw4

all: $(TARGET)

$(TARGET): $(TARGET).cc
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(TARGET).cc

.PHONY: run clean

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
