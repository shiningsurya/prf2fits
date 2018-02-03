#ifndef _THIRD_H
#include "third.hpp"
#endif

using namespace std;


int main(int argc, char * argv[]) {
		if(argc < 2) {
				cout << "Usage : ";
				cout << "<program> TEST/FILE " << endl;
				exit(1);
		}
		IOer io;
		io.LoadFile(argv[1]);	
		// 
		cout << "Number of parameters read from the file: " << io.getNumberParameters() << endl;
		cout << "DATA[key1] : " << io["key1"] << endl;	










		return 0;
}
