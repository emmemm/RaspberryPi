# -*- Makefile -*-

OBJS= \
	CLapWatch.o \

all: isr_pin_both

isr_pin_both: $(OBJS) isr_pin_both.cpp
	$(CXX) isr_pin_both.cpp $(OBJS) -o isr_pin_both -lwiringPi

isr_pin: $(OBJS) isr_pin.cpp
	$(CXX) isr_pin.cpp $(OBJS) -o isr_pin -lwiringPi
