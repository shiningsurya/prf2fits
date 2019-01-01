#ifndef _THIRD_H
#include "third.hpp"
#endif
#include<Source/parser.hpp>
using namespace std;

// http://webcache.googleusercontent.com/search?q=cache:-4caWAcY3EgJ:boost-spirit.com/home/articles/qi-example/parsing-a-list-of-key-value-pairs-using-spirit-qi/+&cd=1&hl=en&ct=clnk&gl=in&client=ubuntu 

class IOer {
		// Performs file IO
		// Abstract class which does configuration file IO
		private:
				std::map<std::string, std::string> data;
				// ^ holds the data
				string filename;
				void updater(string& line)  {
						if(!parser::parse(line,data)){
								cerr << "Unable to parse file " << filename << endl;
								cerr << "Failed on line " << line << endl;
						}
				}
		public:
				/***********************
				 *
				 * Requiring any file to be parsed by this class to
				 * be KEY VALUE format
				 * ********************/
				void LoadFile(string fn){
						ifstream file;
						string line;
						int numlines = 0;
						file.open(fn,ios::in);
						if(file.is_open()){
								// while loop to do file io
								while( getline(file, line) ) {
										// write file io here
										updater(line);
										numlines++;
								}
								file.close();
						}
						filename = fn; // copying
						if(data.size() != numlines) {
								cerr << "Check file " << filename << endl;
								cerr << "Sanity check failed\n" ;
						}
				}
				int getNumberParameters(){ return data.size(); }
				string GetValue(string key){
						return data[key]; // I didn't think C++ would be this cool
				}
				void walkThrough() {
						for(auto it = data.begin(); it != data.end(); it++) {
								cout << it->first << ":" << it->second << ";" << std::endl;
						}
				}
				string operator[] (string key) {
						//iter = data.begin();
						//while(iter != data.end()){
								//if(iter->first == key) return iter->second;
								//else iter++;
						//}
						//cerr << "Key " << key << " not found. \n";
						try {
								return data.at(key);
						}
						catch (const out_of_range &oor) {
								cout << filename << " doesn't have " << key << " in it.\n";
								cout << "Please check it and re-run.\n";
						}
				}
				string getFileName(){ return filename; }
};
