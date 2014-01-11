# -*- Makefile -*-

OBJS= \
	CRecorder.o \

all: timing timingw

timing: $(OBJS) main.cpp
	$(CXX) main.cpp $(OBJS) -o timing -lbcm2835 -lrt

timingw: $(OBJS) main.cpp
	$(CXX) -DWIRING_PI main.cpp $(OBJS) -o timingw -lwiringPi
