CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Iinclude
TARGET := invaricheck
SRC := src/main.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) *.txt

.PHONY: all run clean
