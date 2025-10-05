# Simple Makefile for building the example on Raspberry Pi OS (C++17)
CXX := g++
LIB_PATH ?= ../ICM42688
CXXFLAGS := -std=gnu++17 -I../src -I$(LIB_PATH)/src -O2 -Wall
LDFLAGS := -lpthread

SRCS := $(wildcard *.cpp) $(LIB_PATH)/src/ICM42688.cpp
OBJS := $(SRCS:.cpp=.o)
TARGET := lib_test

all: $(TARGET)

$(OBJS): %.o: %.cpp
	$(CXX) $(CXXFLAGS) -DLIB_PATH=\"$(LIB_PATH)\" -c $< -o $@

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

clean:
	rm -f $(TARGET) $(OBJS)

.PHONY: all clean
