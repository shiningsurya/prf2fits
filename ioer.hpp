#ifndef _THIRD_H
#include "third.hpp"
#endif
#include<boost/spirit/include/qi.hpp>
#include <boost/fusion/include/std_pair.hpp>
using namespace std;
//using namespace qi:

// http://webcache.googleusercontent.com/search?q=cache:-4caWAcY3EgJ:boost-spirit.com/home/articles/qi-example/parsing-a-list-of-key-value-pairs-using-spirit-qi/+&cd=1&hl=en&ct=clnk&gl=in&client=ubuntu 
/*
 *struct keys_and_values
 *: qi::grammar<string::iterator, map<string, string>>
 *{
 *        keys_and_values()
 *                : keys_and_values::base_type(query)
 *        {
 *                query =  ppair >> *((qi::lit(';') | '&') >> ppair);
 *                ppair  =  key >> -('=' >> value);
 *                key   =  qi::char_("a-zA-Z_") >> *qi::char_("a-zA-Z_0-9");
 *                value = +qi::char_("a-zA-Z_0-9");
 *        }
 *        qi::rule<string::iterator, map<string, string>> query;
 *        qi::rule<string::iterator, pair<string, string>> ppair;
 *        qi::rule<string::iterator, string> key, value;
 *};
 */

namespace client
{
		namespace qi = boost::spirit::qi;
		typedef std::map<std::string, std::string> pairs_type;
		template <typename Iterator>
				struct key_value_sequence 
				: qi::grammar<Iterator, pairs_type()>
				{
						key_value_sequence()
								: key_value_sequence::base_type(query)
						{
								query =  pair >> *((qi::lit(';') | '&') >> pair);
								pair  =  key >> -('=' >> value);
								key   =  qi::char_("a-zA-Z_") >> *qi::char_("a-zA-Z_0-9 -");
								value = +qi::char_("a-zA-Z_0-9 +-:. ");
						}

						qi::rule<Iterator, pairs_type()> query;
						qi::rule<Iterator, std::pair<std::string, std::string>()> pair;
						qi::rule<Iterator, std::string()> key, value;
				};
}

/*
 *template <typename Iterator>
 *struct keys_and_values
 *: qi::grammar<Iterator, std::map<std::string, std::string>>
 *{
 *        keys_and_values()
 *                : keys_and_values::base_type(query)
 *        {
 *                query =  ppair >> *((qi::lit(';') | '&') >> ppair);
 *                ppair  =  key >> -('=' >> value);
 *                key   =  qi::char_("a-zA-Z_") >> *qi::char_("a-zA-Z_0-9");
 *                value = +qi::char_("a-zA-Z_0-9");
 *        }
 *        qi::rule<Iterator, std::map<std::string, std::string>> query;
 *        qi::rule<Iterator, std::pair<std::string, std::string>> ppair;
 *        qi::rule<Iterator, std::string> key, value;
 *};
 */

namespace qi = boost::spirit::qi;
class IOer {
		// Performs file IO
		// Abstract class which does configuration file IO
		private:
				client::key_value_sequence<string::iterator> pparse; // This is my parser.
				client::pairs_type data;
				client::pairs_type::iterator iter;
				string filename;
				void updater(string& line)  {
						//vector<string> res; // I can't seem to not use this
						//boost::split(res,line,[](char c){return c == ' ';}); // tokenizing
						string::iterator it = line.begin();
						string::iterator end = line.end();
						if(!qi::parse(it,end,pparse,data)){
								cerr << "Unable to parse file " << filename << endl;
								cerr << "Failed on line " << line << endl;
						}
						else{
								/*
								 * // YaY!! 
								 * // parsing worked
								 * //data.insert(m);	
								 *std::cout << "-------------------------------- \n";
								 *std::cout << "Parsing succeeded, found entries:\n";
								 *client::pairs_type::iterator end = data.end();
								 *for (client::pairs_type::iterator it = data.begin(); it != end; ++it)
								 *{
								 *        std::cout << (*it).first;
								 *        if (!(*it).second.empty())
								 *                std::cout << " does this really work " << (*it).second;
								 */
								/*
								 *        std::cout << std::endl;
								 *}
								 *std::cout << "---------------------------------\n";
								 */
						}
						// debugging vaste
						/*
						 *bool result = qi::parse(it,end,pparse,m);
						 *if(it != end || !result){
						 *        cerr << "Unable to parse file " << filename << endl;
						 *        cerr << "Failed on line " << line << endl;
						 */
						/*
						 *
						 *qi::phrase_parse(it, line.end(), 
						 *  // Grammar
						 *string_ >> char_('=') >> string_ ,
						 *ascii::space, res);
						 *if(it != end()){
						 *        cerr << "Unable to parse " << filename << ". Fix it before running\n" ;
						 *}
						 *pair<string,string> tp(res[0],res[1]); // creating a temp pair
						 *data.insert(tp); // inserting in map
						 */
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
				string operator[] (string key) {
						iter = data.begin();
						while(iter != data.end()){
								if(iter->first == key) return iter->second;
								else iter++;
						}
						cerr << "Key " << key << " not found. \n";
				}
				string getFileName(){ return filename; }
};
