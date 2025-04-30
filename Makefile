# Compiler settings
CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -O3 #-I/usr/include/SFML
LDFLAGS := #-L/usr/lib/x86_64-linux-gnu -lsfml-graphics -lsfml-window -lsfml-system -lstdc++fs

# Project structure
SRC_DIR := .
BUILD_DIR := build
TARGET := pathfinder

# Source files
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))
DEPS := $(OBJS:.o=.d)

# Header files
HEADERS := PathFinder.hpp Vec.hpp ObjectPool.hpp Node.hpp Grid.hpp

# Main rule
all: $(BUILD_DIR)/$(TARGET)

# Link executable
$(BUILD_DIR)/$(TARGET): $(OBJS)
	@mkdir -p $(@D)
	$(CXX) $(LDFLAGS) $^ -o $@

# Compile source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADERS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

# Include dependencies
-include $(DEPS)

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR)

# Run the program
run: all
	./$(BUILD_DIR)/$(TARGET)

.PHONY: all clean run
