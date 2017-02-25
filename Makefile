#
# CMPSC473 - S17 Project #1
# Makefile - makefile for the project
#

# Locations

# Make environment
OBJS = myscheduler.o Project1.o
CXX = g++ 
CXXFLAGS = -std=c++11 -c -g -Wall
LFLAGS = -std=c++11 -Wall -g

# Productions
all : p1

p1 : $(OBJS)
	$(CXX) $(LFLAGS) $(OBJS) -o p1

Project1.o : scheduler.h myscheduler.h Project1.cpp
	$(CXX) $(CXXFLAGS) Project1.cpp

myscheduler.o : scheduler.h myscheduler.h myscheduler.cpp
	$(CXX) $(CXXFLAGS) myscheduler.cpp

clean : 
	rm -f p1 $(OBJS)
