#ifndef _THIRD_H
#include "third.hpp"
#endif
class AbstractIO{
		private:
				map<string,string> kv;
		protected:
				string filename;
		public:
				string operator[] (string key) {return kv[key];}
				void insert(string k, string v){
						pair<string,string> rp(k,v);
						kv.insert(rp);
				}
				int getSize() { return kv.size();}
				void takeio(){
				/*
				 * Iterates over the map and take inputs from the user
				 * It shows whatever is already in the map as the default input
				 */
						string line;
						map<string,string>::iterator iter = kv.begin();
						cout << "Starting IO....Hit ENTER to take in the default value\n";
						while(iter != kv.end()){
								// iter->first , iter->second
								cout << "Input for " << iter->first << " ( default = " << iter->second << " ) : " ;
								//line = cin.get();
								getline(cin,line);
								//cin >> line;
								/* If user punches ENTER 
								 * Default is taken 
								 * Else user input is read and stored
								 * */
								if( line.empty()) {
										// Enter case
										// no change 
								}
								else {
										// User gave input
										iter->second = line;
								}
								iter++;
						}
				}
				void printer(){
				/*
				 *To print everything in the map
				 */
						map<string,string>::iterator iter = kv.begin();
						for(;iter != kv.end();iter++){
								cout << iter->first << " : " << iter->second << endl;
						}
				}
				void outputfile(){
						cout << "Enter the file name ( default = " << filename << " ) : " ;
						string line;
						getline(cin, line);
						if( line.empty()) {
								// Enter case
								// no change 
						}
						else {
								// User gave input
								filename = line;
						}
						ofstream outfile;
						outfile.open(filename); // Opening
						map<string,string>::iterator iter = kv.begin();
						for(;iter != kv.end();iter++) {
								outfile << iter->first << "=" << iter->second << ";" << endl;
								/*************************
								 * So, This file parsing is done using Boost::qi
								 * I am not using Boost::karma to generate these
								 * I didn't understand how to get it working 
								 * ***********************/
						}
						outfile.close();
				}
};

class Pulsar : public AbstractIO {
		private:
				vector<string> param;
		public:
				Pulsar() {
						/*
						 * All the parameters that will be used in the file
						 * or that are expected in the file 
						 * or that will used in generating FITS file 
						 * will come here. 
						 * Initialising with default values
						 * *************************
						 * Brace yourself!!
						 */
						insert("CHAN_DM","0.0");
						insert("COORD_MD","J2000");
						insert("EQUINOX","2000.0");
						insert("RA","00:00:00.0000");
						insert("DEC","-00:00:00.000");
						filename = "pulsar.cfg";
				}
};


class Observatory : public AbstractIO {
		private:
		public:
				Observatory() {
						/*
						 * All the parameters that will be used in the file
						 * or that are expected in the file 
						 * or that will used in generating FITS file 
						 * will come here. 
						 * Initialising with default values
						 * *************************
						 * Brace yourself!!
						 */
						insert("TELESCOP","GMRT");
						insert("ANT_X","1656446.9142");
	/*
	 *ph->ant_x = 1656446.9142;
     *ph->ant_y = 5798313.9493;
     *ph->ant_z = 2073374.9814;
     *ph->fend = "30 Antenna Interferometer";
     *ph->ibeam = 1;
	 */
						insert("ANT_Y","5798313.9493");
						insert("ANT_Z","2073374.9814");
						insert("FRONTEND","30 Antenna Interferometer");
						insert("IBEAM","1");
						insert("NRCVR","");
						insert("FD_POLN","LIN");
						insert("FD_HAND","1");
						insert("FD_SANG","0.0");
						insert("FD_XYPH","0.0");
						insert("BACKEND","GSB");
						insert("BECONFIG","GSBVbeam");
						insert("BE_PHASE","0");
						insert("BE_DCC","0");
						insert("BE_DELAY","0.0");
						insert("TCYCLE","0.0");
						filename = "observatory.cfg";
				}



};

class Project : public AbstractIO {
		private:
		public:
				Project(){
						/*
						 * All the parameters that will be used in the file
						 * or that are expected in the file 
						 * or that will used in generating FITS file 
						 * will come here. 
						 * Initialising with default values
						 * *************************
						 * Brace yourself!!
						 */
						insert("OBSERVER","REVRESBO");	
						insert("PROJID","InPTA");						
						insert("OBS_MODE","PSR");						
						insert("OBSFREQ","1300.3333");
						insert("OBSBW","120");						
						insert("OBSNCHAN","1024");						
						insert("CAL_MODE","OFF");						
						insert("CAL_FREQ","0.00");						
						insert("CAL_DCYC","1.0");						
						insert("CAL_PHS","0.0");						
						insert("CAL_NPHS","0.");						
						insert("STT_LST","12345");	
						filename = "project.cfg";
				}

};
class Scan : public AbstractIO {
		private:
		public:
				Scan(){
						/*
						 * All the parameters that will be used in the file
						 * or that are expected in the file 
						 * or that will used in generating FITS file 
						 * will come here. 
						 * Initialising with default values
						 * *************************
						 * Brace yourself!!
						 */
						insert("DATE-OBS","2014-01-01T16:20+05:30");
						insert("PNT_ID","J2145-0750"); // TODO
						insert("SRC_NAME","J2145-0750");
						insert("SCANLEN","3000");						
						insert("STT_IMJD","57375");						
						insert("STT_SMJD","7345");						
						insert("STT_OFFS","234535");
						insert("NPOL","2");		
						filename = "scan.cfg";
				}

};
