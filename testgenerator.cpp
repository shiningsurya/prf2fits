#ifndef _THIRD_H
#include "third.hpp"
#endif 

#include "generator.hpp"

using namespace std;

int main(int argc, char * argv[]) {
		if(argc < 2) {
				cout << "Usage : <program> \n";
				exit(1);
		}
		Pulsar pul;
		Observatory obs;
		Project proj;
		Scan scan;
		pul.outputfile();
		obs.outputfile();
		proj.outputfile();
		scan.outputfile();

		return 0;
}
