#include <cstdlib>			//	std::exit
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

#include <unordered_map>
#include <set>
#include <utility>			//	std::pair

void tokenize (std::vector<std::string>& tokens, const std::string& input, const std::string& regexp_delim) {

	std::regex re(regexp_delim);
	std::sregex_token_iterator  sre_token_iter(std::begin(input), std::end(input), re, -1);
	std::sregex_token_iterator  finish;

	while (sre_token_iter != finish) {
		std::string t (*sre_token_iter);
		if (t.length()>0) {
			tokens.emplace_back(t);
		}

		sre_token_iter++;
	}
}

std::vector<std::string> tokenize (const std::string& input, const std::string& regexp_delim) {
	std::vector<std::string> tokens;
	tokenize (tokens, input, regexp_delim);
	return tokens;
}

void print_usage(char* self) {
	std::cout << "Usage: " << self << " <input_file.txt> <output_file.txt>" << std::endl;
}

int main(int argc, char** argv) try {


    std::unordered_map <std::string, size_t>  freq_counter;

    auto comparator = [] (const std::pair<std::string, size_t> & l, 
    	                  const std::pair<std::string, size_t> & r) 
    {
    	if (l.second == r.second) 
    		return l.first.compare(r.first) < 0;
    	else 
    		return l.second > r.second;
    };
    std::set <std::pair<std::string, size_t>, decltype(comparator)> 	freq_orderer(comparator);

	if (argc<3) {
		print_usage(argv[0]);
		std::exit(EXIT_SUCCESS);
	}

    std::ifstream  input_fs(argv[1]);
    std::ofstream  output_fs(argv[2]);

    if (input_fs.fail()) {
        throw std::runtime_error ("***ERROR*** Could not read file `"+std::string(argv[1])+"'. Aborting.");
    }

    if (output_fs.fail()) {
        throw std::runtime_error ("***ERROR*** Could not open file `"+std::string(argv[1])+"' for writing. Aborting.");
    }

    //	Reading file line-by-line, and store tokens into a 
    //	single (possibly large) vector of tokens
    std::vector<std::string>	words;

    for(std::string line; getline(input_fs, line); ) {
    	tokenize(words, line, "[^a-zA-Z]");
    }

    //	Count frequencies of tokens (conveniently changed to lower case)
    for (auto w: words) {
    	std::transform(std::begin(w), std::end(w), std::begin(w), ::tolower);
    	freq_counter[w]++;
    }

    //	Give proper ordering to the pairs <word, count>
    for (auto & [word, count]: freq_counter) {
    	freq_orderer.insert ({word, count});
    }

    //	Saving to a file
    for (auto & [word, count]: freq_orderer) {
    	output_fs << count << " " << word << std::endl;
    }
} 
catch (std::runtime_error& e) {
	std::cerr << e.what() << std::endl;
	std::exit(EXIT_FAILURE);
}