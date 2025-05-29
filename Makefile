# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I. # -I. to find printf_utils.h in the current directory

# Source files, object files, and target executable
SRCS = main.cpp printf_utils.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = universal_printf_test

# Default target
all: $(TARGET)

# Link target
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Compile target for .cpp files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean target
clean:
	rm -f $(OBJS) $(TARGET)

# Phony targets
.PHONY: all clean
