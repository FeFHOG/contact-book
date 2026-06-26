CXX = clang++
CXXFLAGS = -std=c++17 -Wall -Wextra -g -I./inc

TARGET_DIR = target
TARGET = $(TARGET_DIR)/contact_book

SRC = main.cpp $(wildcard src/*.cpp)
OBJ = $(patsubst %.cpp,$(TARGET_DIR)/%.o,$(SRC))

all: $(TARGET)

$(TARGET): $(OBJ)
	@mkdir -p $(TARGET_DIR)/src
	$(CXX) $(CXXFLAGS) -o $@ $^

$(TARGET_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: all
	./$(TARGET)

clean:
	rm -rf $(TARGET_DIR)

.PHONY: all run clean
