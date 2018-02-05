#ifndef _THIRD_H
#include "third.hpp"
#endif
#include<pfits.hpp>
#include<boost/program_options/cmdline.hpp>
#include<boost/program_options/config.hpp>
#include<boost/program_options/environment_iterator.hpp>
#include<boost/program_options/option.hpp>
#include<boost/program_options/options_description.hpp>
#include<boost/program_options/parsers.hpp>
#include<boost/program_options/positional_options.hpp>
#include<boost/program_options/variables_map.hpp>


using  namespace std;
namespace po = boost::program_options;

int main(int argc, char * argv[]){
		string observatory, pulsar, observation, project, scan; // file io
		string input, out;
		po::variables_map vm;
		// PO
		po::options_description desc("Options");
		desc.add_options()
		("help,h","Prints help")
		("bug-in-code","Prints contact info")
		("observatory,o",po::value<string>(&observatory)->default_value("observatory.cfg")->required(),"Observatory cfg file")
		("pulsar,n",po::value<string>(&pulsar)->default_value("pulsar.cfg")->required(),"Pulsar cfg file")
		("project,p",po::value<string>(&project)->default_value("project.cfg")->required(),"Project cfg file")
		("scan,s",po::value<string>(&scan)->default_value("scan.cfg")->required(),"Scan cfg file")
		("input",po::value<string>(&input), "Input PROF file\n"
											   "\tYou can directly give the input prof file as the one and only positional argument here.\n" 
											   "You dont have to type --input or -i." 
		 )
		("out,f",po::value<string>(&out),"FITS file\n"
										  "\tMake sure that is no FITS with the same name there.\n"
										 "CFITSIO routines cause error when we create a FITS file \n"
										 "with the same name as one already there."
		 )
		;
		po::positional_options_description p;
		p.add("input",-1);
		po::store(po::command_line_parser(argc,argv).options(desc).positional(p).run(), vm);
		// argument parsing is done..
		if(vm.count("help") || argc < 2) {
				cout << desc << endl;
				return true;

		}
		if(vm.count("bug-in-code")){
				cout << "If you found some bugs in the code\nOr, would like to discuss the code with me.\n" ;
				cout << "You can reach me via email on :\n" ;
				cout << "ep14btech11008 [at] iith [dot] ac [dot] in\n";
				cout << "Suryrao Bethapudi\n";
				return true;
		}
		if(!vm.count("input")) {
				cerr << "No input file given\n";
				cerr << "Exiting...\n";
				return false;
		}
		po::notify(vm); // only raises any errors encountered. 
		// Creating a FITS file
		FITS myfits(vm["input"].as<string>(), pulsar, observatory, project, scan);
		// Now, I will show you a magic
		/***********************************
		 * This magic is called OOP trick.
		 * You just call and it does it's work.
		 * *********************************
		 * I am like a conductor of an orchetra 
		 * and these FITS are my compositions. 
		 * *********************************/
		try{
				myfits.sanityCheck();
		} catch(const char * key) {
				cerr << "Key " << key << " not found to be consistent...\n";
				cerr << "Please fix it\n";
				return false;
		}
		myfits.createFITS(out.c_str());
		myfits.primaryHeader();
		myfits.PutHistoryTable();
		myfits.PutSubint();
		myfits.closeFITS();		
		return 0;
}
