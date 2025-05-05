# Compiler settings
CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -O3 #-I/usr/include/SFML
LDFLAGS := #-L/usr/lib/x86_64-linux-gnu -lsfml-graphics -lsfml-window -lsfml-system -lstdc++fs

# Project structure
SRC_DIR := .
BUILD_DIR := build
RELEASE_DIR := $(BUILD_DIR)/release
DEBUG_DIR := $(BUILD_DIR)/debug
TARGET := pathfinder

# Source files
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
RELEASE_OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(RELEASE_DIR)/%.o,$(SRCS))
DEBUG_OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(DEBUG_DIR)/%.o,$(SRCS))
DEPS := $(RELEASE_OBJS:.o=.d) $(DEBUG_OBJS:.o=.d)

# Header files
HEADERS := PathFinder.hpp Vec.hpp ObjectPool.hpp Node.hpp Grid.hpp

# Build configurations
RELEASE_FLAGS := -O3 -DNDEBUG
DEBUG_FLAGS := -O0 -g -DDEBUG -fsanitize=address -fno-omit-frame-pointer
DEBUG_LDFLAGS := -fsanitize=address #-static-libasan

all: release

release: $(RELEASE_DIR)/$(TARGET)

debug: $(DEBUG_DIR)/$(TARGET)

# Link executables
$(RELEASE_DIR)/$(TARGET): $(RELEASE_OBJS)
	@mkdir -p $(@D)
	$(CXX) $(LDFLAGS) $^ -o $@

$(DEBUG_DIR)/$(TARGET): $(DEBUG_OBJS)
	@mkdir -p $(@D)
	$(CXX) $(LDFLAGS) $(DEBUG_LDFLAGS) $^ -o $@

# Compile source files (release)
$(RELEASE_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADERS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(RELEASE_FLAGS) -MMD -MP -c $< -o $@

# Compile source files (debug)
$(DEBUG_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADERS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(DEBUG_FLAGS) -MMD -MP -c $< -o $@

# Include dependencies
-include $(DEPS)

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR)

# Run targets
run: release
	./$(RELEASE_DIR)/$(TARGET)

debug-run: debug
	./$(DEBUG_DIR)/$(TARGET)

.PHONY: all release debug clean run debug-run
