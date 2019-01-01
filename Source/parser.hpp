namespace parser {
		bool parse(std::string& one, std::map<std::string, std::string>& dat) {
				std::size_t first_semicolon = one.find_first_of(";");
				std::size_t first_equal = one.find_first_of("=");
				// so everything from [0,first_equal-1] --> key
				// [first_equal+1,first_semicolon-1] --> value
				std::string alpha = one.substr(0,first_equal);
				std::string beta = one.substr(first_equal+1, first_semicolon-first_equal-1);
				if(alpha.size() == 0 || beta.size() == 0) return false;
				dat.insert(std::pair<std::string, std::string>(alpha, beta));
				return true;
		}
}
