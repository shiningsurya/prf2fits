all:
	g++ class.cpp third.cpp -o third -lcfitsio -I$(ASTROSOFT)/include -L$(ASTROSOFT)/lib -lboost_algorithm -std=c++11
clean:
	rm -f *o hello testio testgen
third:
	g++ third.cpp -o third -lcfitsio -I$(ASTROSOFT)/include -L$(ASTROSOFT)/lib -lboost_algorithm -std=c++11 -lboost_program_options
second_bw:
	gcc secondgmrt_bw.c -o secbw -lcfitsio -I$(ASTROSOFT)/include -L$(ASTROSOFT)/lib 
testio:
	g++ testioer.cpp -o testio -lcfitsio -I$(ASTROSOFT)/include -L$(ASTROSOFT)/lib -std=c++11
testgen:
	g++ testgenerator.cpp -o testgen -I$(ASTROSOFT)/include -L$(ASTROSOFT)/lib -lcfitsio  -std=c++11
