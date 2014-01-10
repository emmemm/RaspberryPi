# -*- Makefile -*-

all: timeval_resolution show_bit_status show_edge_timings

timeval_resolution: timeval_resolution.c
	$(CC) timeval_resolution.c -o timeval_resolution 

show_bit_status: show_bit_status.c
	$(CC) show_bit_status.c -o show_bit_status -lbcm2835

show_edge_timings: show_edge_timings.c
	$(CC) show_edge_timings.c -o show_edge_timings -lbcm2835