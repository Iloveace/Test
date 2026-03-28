# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -O2

# Target executable name
TARGET = Test

# Source files
SRCS = Main.cpp Point3d.cpp Vector3d.cpp 

# Object files (automatically generated from SRCS)
OBJS = $(SRCS:.cpp=.o)

# Default rule: build the app
all: $(TARGET)

# Debug build (no optimization, with debug symbols)
debug: CXXFLAGS := -std=c++17 -Wall -Wextra -pedantic -g -O0
debug: clean $(TARGET)

# Link object files to create the executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Compile .cpp files into .o files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJS) $(TARGET)

# Phony targets to prevent conflicts with files named 'all' or 'clean'
.PHONY: all clean
