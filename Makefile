# Allow environment/workflow to override these
CXX ?= g++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -pedantic -O2
TARGET ?= Test

# Source files to compile
SRCS = Main.cpp

# Object files derived from source files
OBJS = $(SRCS:.cpp=.o)

# Default build
all: $(TARGET)

# Debug build with no optimizations and debug symbols
debug: CXXFLAGS := -std=c++17 -Wall -Wextra -pedantic -g -O0
debug: clean $(TARGET)

# Link object files to create the executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Compile cpp to object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(OBJS) $(TARGET) Test.exe Test

# Phony targets to prevent conflicts with files of the same name
.PHONY: all debug clean