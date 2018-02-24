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

ioer : testior.cpp 
		$(CC) $? $(CPPINC) $(CPPFLAGS) $(LDFLAGS) -o $@

cfgen : testgenerator.cpp 
		$(CC) $? $(CPPINC) $(CPPFLAGS) $(LDFLAGS) -o $@

doc : doc/manual.tex
	pdflatex doc/manual.tex --output-directory=doc/

clean :
	-rm -f testio testgen prf2fits cfgen 

prf2fits : third.cpp 
	$(CC) $? $(CPPINC) $(CPPFLAGS) $(LDFLAGS) -lboost_program_options -o $@

all:
	$(MAKE) -f Makefile prf2fits 
	$(MAKE) -f Makefile cfgen

cfitsio: 
		@echo "Installing CFITSIO..."
		@chmod +x $(PWD)/cfitsio/configure
		@$(PWD)/cfitsio/configure --prefix=$(PWD)/cfitsio/ CFLAGS=-fPIC FFLAGS=-fPIC  
		@$(MAKE) -C cfitsio -f Makefile shared 
		@$(MAKE) -C cfitsio -f Makefile install  
		@$(MAKE) -C cfitsio -f Makefile clean

install:
	$(MAKE) cfitsio 
	@echo "Building PRF2FITS..."
	$(MAKE) all

testmjd: testmjd.cpp 
	$(CC) $? $(CPPINC)  $(CPPFLAGS) $(LDFLAGS) -o $@
.PHONY: clean cfitsio doc
