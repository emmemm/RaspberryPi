# -*- Makefile -*-

OBJS= \
	CRecorder.o \

main: $(OBJS) main.cpp
	$(CXX) main.cpp $(OBJS) -o timing.bin -lbcm2835 -lrt
