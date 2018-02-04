# Make file 
# PRF2FITS
SHELL := /bin/bash
CC =  g++
CPPINC   = -Icfitsio/include 
CPPFLAGS = -std=c++11
LDFLAGS  = -Lcfitsio/lib -lcfitsio 
PWD     := $(shell pwd)

ioer : testior.cpp 
		$(CC) $? $(CPPINC) $(CPPFLAGS) $(LDFLAGS) -o $@

cfgen : testgenerator.cpp 
		$(CC) $? $(CPPINC) $(CPPFLAGS) $(LDFLAGS) -o $@

clean :
	-rm -f testio testgen prf2fits 

prf2fits : third.cpp 
	$(CC) $? $(CPPINC) $(CPPFLAGS) $(LDFLAGS) -o $@

all: prf2fits cfgen
	$(CC) $? $(CPPINC) $(CPPFLAGS) $(LDFLAGS) -o $@

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

.PHONY: clean cfitsio
