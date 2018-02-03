#ifndef _THIRD_H
//#include "third.hpp"
#endif
#include<boost/program_options/cmdline.hpp>
#include<boost/program_options/config.hpp>
#include<boost/program_options/environment_iterator.hpp>
#include<boost/program_options/errors.hpp>
#include<boost/program_options/option.hpp>
#include<boost/program_options/options_description.hpp>
#include<boost/program_options/parsers.hpp>
#include<boost/program_options/positional_options.hpp>
#include<boost/program_options/value_semantic.hpp>
#include<boost/program_options/variables_map.hpp>


using  namespace std;
namespace po = boost::program_options;

int main(int argc, char * argv[]){
  //FITS myfits;
  string observatory, pulsar, observation, project, scan; // file io


  // PO
  po::options_description desc("Options");
  desc.add_options()("help","Prints help")("bug-in-code","Prints contact info");
  desc.add_options()("observatory",po::value<string> observatory,"Observatory cfg file");
  desc.add_options()("pulsar",po::value<string> pulsar,"Pulsar cfg file");
  desc.add_options()("observation",po::value<string> &observation,"Observation cfg file");
  desc.add_options()("project",po::value<string> &project,"Project cfg file");
  desc.add_options()("scan",po::value<string> &scan,"Scan cfg file");
  return 0;
}
