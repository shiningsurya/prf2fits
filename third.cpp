#ifndef _THIRD_H
#include "third.hpp"
#endif
#include<pfits.hpp>
#include<unistd.h>
#include <getopt.h>

using  namespace std;

void help() {
		cout << " ---------- PRF2FITS ---------- " << endl;
		cout << "	-h Prints help " << endl;
		cout << "	-o Observatory cfg file " << endl;
		cout << "	-n Pulsar cfg file " << endl;
		cout << "	-p Project cfg file " << endl;
		cout << "	-s Scan cfg file " << endl;
		cout << "	-a Pulsar ephemeris file " << endl;
		cout << "	-f Output FITS filename " << endl;
		cout << "	-i Input prof file " << endl;
		cout << " ---- Github: @shiningsurya --- " << endl;
}

int main(int argc, char * argv[]){
		if(argc < 2) {
				help();
				exit(0);
		}
		string pulsar("pulsar.cfg"), observatory("observatory.cfg"), project("project.cfg"), scan("scan.cfg"), par("par.cfg"); 
		string input, out;
		int parfile = 0, opt;
		while( (opt = getopt(argc, argv, "h::o:n:p:s:a:f:i:")) != -1) {
				switch(opt) {
						case 'h':
								help();
								exit(0);
						case 'n':
								pulsar.assign(optarg);	
								break;
						case 'o':
								observatory.assign(optarg);
								break;
						case 'p':
								project.assign(optarg);
								break;
						case 's':
								scan.assign(optarg);
								break;
						case 'a':
								parfile = 1;
								par.assign(optarg);
								break;
						case 'f':
								out.assign(optarg);
								break;
						case 'i':
								input.assign(optarg);
								break;
				}
		}
		/////// DEBUG
		/*
		 *cout << pulsar << endl << observatory << endl << project << endl << scan << endl;
		 *cout << input << endl << out;
		 *exit(1);
		 */
		/////// DEBUG
		// Creating a FITS file
		FITS myfits(input, pulsar, observatory, project, scan);
		// Now, I will show you a magic
		/***********************************
		 * This magic is called OOP trick.
		 * You just call and it does it's work.
		 * *********************************
		 * I am like a conductor of an orchetra 
		 * and these FITS are my compositions. 
		 * *********************************/
		myfits.sanityCheck();
		myfits.createFITS(out.c_str());
		myfits.primaryHeader();
		myfits.PutHistoryTable();
		if(parfile) myfits.PutPar(par);
		myfits.PutSubint();
		myfits.closeFITS();		
		return 0;
}
