CXX = g++
CXXFLAGS = -std=c++17

SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)

ifeq ($(OS),Windows_NT)
    RM = del /Q
    EXECUTABLE = main.exe
else
    RM = rm -f
    EXECUTABLE = main
endif

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
    $(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
    $(CXX) $(CXXFLAGS) -c $< -o $@

clean:
    $(RM) $(EXECUTABLE) $(OBJS)