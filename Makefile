# Make file 
# PRF2FITS
SHELL := /bin/bash
CC =  g++
CPPINC   = -Icfitsio/include -I. 
CPPFLAGS = -std=c++11 -fpermissive -w -g # Shamelessly suppressing all the warnings so that
								         # compilaton is quiet 
# Uncomment these lines and add path to CFITSIO header file 
# to CPPINC and path to shared CFITSIO library
# CPPINC += 
# CPPFLAGS +=
LDFLAGS  = -Lcfitsio/lib -lcfitsio 
PWD     := $(shell pwd)
SRC     := Source/
TST     := Test/

ioer : $(SRC)testioer.cpp 
		$(CC) $? $(CPPINC) $(CPPFLAGS) $(LDFLAGS) -o $@

cfgen : $(SRC)testgenerator.cpp $(SRC)generator.hpp 
		$(CC) $(SRC)testgenerator.cpp $(CPPINC) $(CPPFLAGS) $(LDFLAGS) -o $@

doc : $(PWD)doc/manual.tex
	pdflatex $(PWD)doc/manual.tex --output-directory=doc/

clean :
	-rm -f testio testgen prf2fits cfgen ioer

test: 
	-cp prf2fits $(TST)
	-cd $(TST)
	-./prf2fits -i test.prof -f testnew.fits
	md5sum -c fits.md5

prf2fits : 
	$(CC) $(SRC)third.cpp $(CPPINC) $(CPPFLAGS) $(LDFLAGS) -o $@

all:
		prf2fits cfgen

mjder: $(SRC)testmjd.cpp $(SRC)mjder.hpp
	$(CC) $(SRC)testmjd.cpp $(CPPINC) $(CPPFLAGS) $(LDFLAGS) -o $@ 

.PHONY: clean doc
