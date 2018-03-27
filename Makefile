# Make file 
# PRF2FITS
SHELL := /bin/bash
CC =  g++
CPPINC   = -Icfitsio/include -I. -Iboost/ 
CPPFLAGS = -std=c++11 -fpermissive -w -g # Shamelessly suppressing all the warnings so that
								         # compilaton is quiet 
# Uncomment these lines and add path to CFITSIO header file 
# to CPPINC and path to shared CFITSIO library
# CPPINC += 
# CPPFLAGS +=

LDFLAGS  = -Lcfitsio/lib -lcfitsio 
PWD     := $(shell pwd)

ioer : testior.cpp 
		$(CC) $? $(CPPINC) $(CPPFLAGS) $(LDFLAGS) -o $@

cfgen : testgenerator.cpp generator.hpp 
		$(CC) testgenerator.cpp $(CPPINC) $(CPPFLAGS) $(LDFLAGS) -o $@

doc : doc/manual.tex
	pdflatex doc/manual.tex --output-directory=doc/

clean :
	-rm -f testio testgen prf2fits cfgen 

prf2fits : third.cpp pfits.hpp mjder.hpp 
	$(CC) third.cpp $(CPPINC) $(CPPFLAGS) $(LDFLAGS) -lboost_program_options -o $@

all:
	$(MAKE) -f Makefile prf2fits 
	$(MAKE) -f Makefile cfgen

mjder: testmjd.cpp mjder.hpp
	$(CC) testmjd.cpp $(CPPINC) $(CPPFLAGS) $(LDFLAGS) -o $@ 

.PHONY: clean doc
