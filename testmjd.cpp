#ifndef _THIRD_H
#include "third.hpp"
#endif

using namespace std;


int main(int argc, char * argv[]){
		double im, fm;
		cout << "Written to test MJD code.\n";
		if(argc < 2) {
				im = 57975;
				fm = 39087.7129422; 
		}
		else{
				im = stod(argv[1]);
				fm = stod(argv[2]);
		}
		ManageMJD tm(im,fm);
		cout << "Integral MJD " << im << endl;
		cout << "Fractional MJD " << fm << endl;
		cout << tm.getDateTime() << endl;
		return 0;
}
