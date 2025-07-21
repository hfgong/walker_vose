CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -I./include
TESTFLAGS = -lgtest -lgtest_main -pthread

# Directories
BUILD_DIR = build
TEST_DIR = tests
EXAMPLE_DIR = examples

# Files
TEST_SRC = $(TEST_DIR)/test_walker_vose.cpp
EXAMPLE_SRC = $(EXAMPLE_DIR)/basic_example.cpp

# Targets
TEST_TARGET = $(BUILD_DIR)/test_walker_vose
EXAMPLE_TARGET = $(BUILD_DIR)/basic_example

.PHONY: all test example clean

all: test example

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

test: $(TEST_TARGET)
	@echo "Running tests..."
	./$(TEST_TARGET)

example: $(EXAMPLE_TARGET)
	@echo "Running example..."
	./$(EXAMPLE_TARGET)

$(TEST_TARGET): $(TEST_SRC) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $< -o $@ $(TESTFLAGS)

$(EXAMPLE_TARGET): $(EXAMPLE_SRC) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm -rf $(BUILD_DIR)