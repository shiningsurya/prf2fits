# Make file 
# PRF2FITS
SHELL := /bin/bash
CC =  g++
CPPINC   = -Icfitsio/include -I. 
CPPFLAGS = -std=c++11 -fpermissive -w # Shamelessly suppressing all the warnings so that
								      # compilaton is quiet 
LDFLAGS  = -Lcfitsio/lib -lcfitsio 
PWD     := $(shell pwd)

ioer : testior.cpp 
		$(CC) $? $(CPPINC) $(CPPFLAGS) $(LDFLAGS) -o $@

cfgen : testgenerator.cpp 
		$(CC) $? $(CPPINC) $(CPPFLAGS) $(LDFLAGS) -o $@

clean :
	-rm -f testio testgen prf2fits 

prf2fits : third.cpp 
	$(CC) $? $(CPPINC) $(CPPFLAGS) $(LDFLAGS) -lboost_program_options -o $@

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
