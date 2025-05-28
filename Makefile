# Compiler and C++ standard
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11

# Main program configuration
TARGET = cpu_status
MAIN_SRC = cpu_status.cpp proc_parser.cpp

# Google Test configuration
GTEST_DIR = tests
TEST_TARGET = test_runner

# Source files for tests
# proc_parser.cpp is compiled into the test runner as it contains logic to be tested.
# cpu_status.cpp is NOT included as it has its own main()
TEST_CPU_STATUS_SRC = $(GTEST_DIR)/test_cpu_status.cpp
PROC_PARSER_SRC = proc_parser.cpp # Source file for the code being tested
GTEST_SRC = $(GTEST_DIR)/gtest-all.cc

# Include paths and Linker flags
# Using CPPFLAGS for preprocessor flags (like -I)
# Using CXXFLAGS for other compiler flags (like -Wall)
# Using LDFLAGS for linker flags (like -pthread)
CPPFLAGS = -isystem $(GTEST_DIR)/include
LDFLAGS = -pthread

# Default rule: build the main program
all: $(TARGET)

# Rule to build the main program
$(TARGET): $(MAIN_SRC) proc_parser.h
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $(TARGET) $(MAIN_SRC) $(LDFLAGS)

# Rule to build the test runner
# It compiles the test definitions, the code being tested (proc_parser), and gtest sources.
$(TEST_TARGET): $(TEST_CPU_STATUS_SRC) $(PROC_PARSER_SRC) $(GTEST_SRC) proc_parser.h $(GTEST_DIR)/include/gtest/gtest.h
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $(TEST_TARGET) $(TEST_CPU_STATUS_SRC) $(PROC_PARSER_SRC) $(GTEST_SRC) $(LDFLAGS)

# Rule to run the tests
test: $(TEST_TARGET)
	./$(TEST_TARGET)

# Rule to run Cppcheck
cppcheck:
	@echo "Running Cppcheck..."
	@cppcheck --enable=all --suppress=missingIncludeSystem --error-exitcode=0 cpu_status.cpp proc_parser.cpp

# Rule to clean up build artifacts
clean:
	rm -f $(TARGET) $(TEST_TARGET) *.o

# Phony targets
.PHONY: all test clean cppcheck
