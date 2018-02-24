#ifndef _THIRD_H
#include "third.hpp"
#endif 

#include "generator.hpp"

using namespace std;

int main(int argc, char * argv[]) {
		if(argc < 2) {
				cout << "Usage : <program> arg\n";
				cout << "+-------+----------------------------------------------+\n";
				cout << "|  ARG  |             Description                      |\n";
				cout << "+-------+----------------------------------------------+\n";
				cout << "|   1   |  Ask everything. Takes some time.            |\n";
				cout << "|   2   |  Minimal IO. Assumes you're in GMRT.         |\n";
				cout << "|   3   |  Dump the default values in cfg files.       |\n";
				cout << "|       |  You will have to edit them manually         |\n";
				cout << "+-------+----------------------------------------------+\n";
				exit(1);
		}
		int opt = stoi(argv[1]);
		Pulsar pul;
		Observatory obs;
		Project proj;
		Scan scan;
		// Semantic action coming up
		if(opt == 1) {
				pul.takeio();
				obs.takeio();
				proj.takeio();
				scan.takeio();
		}
		if(opt == 2){
				// What all are required??
				// NUMCHANS SOURCE RA DEC NPOL
				pul.takeio("RA");
				pul.takeio("DEC");
				scan.takeio("SRC_NAME");
				scan.takeio("NUMCHANS");
				scan.takeio("NPOL");
		}
		// This is default in all the three cases.
		pul.outputfile(opt);
		obs.outputfile(opt);
		proj.outputfile(opt);
		scan.outputfile(opt);
		return 0;
}
