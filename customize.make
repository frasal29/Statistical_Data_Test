

ifeq ($(OS),Windows_NT)
	CCOMPILER = gcc
	LINKLIB = -lgsl -lgslcblas -lm
else
	CCOMPILER = cc
	LINKLIB = -lgsl -lgslcblas -lm
endif


