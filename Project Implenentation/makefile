# Makefile for C++ project

# Compiler and compiler options
CXX = g++
CXXFLAGS = -g -Wall -Weffc++ -std=c++11 -Iinclude

# Directories
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = bin

# Source files and object files
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

# Executable name
TARGET = bin/warehouse

# Default target
all: $(TARGET)

# Rule to build the executable
$(TARGET): $(OBJS)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Rule to build object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -c -o $@ $<

# Run target with specified command line arguments
RUN_ARGS = "bin/rest/input_file.txt"
run: $(TARGET)
	./$(TARGET) $(RUN_ARGS)

# Clean rule
clean:
	rm -rf $(BUILD_DIR)/*o $(TARGET)

# PHONY targets
.PHONY: all clean
