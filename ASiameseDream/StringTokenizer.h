#ifndef _STR_TOKENIZER_H_
#define _STR_TOKENIZER_H_

#include <string>
#include <vector>

class StringTokenizer {
public:

	static const std::string WHITESPACES;
	
	static void tokenize(const std::string& str, std::vector<std::string>& tokens, const std::string& delimiters = " ")
	{
		// Skip delimiters at beginning.
		std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
		// Find first "non-delimiter".
		std::string::size_type pos = str.find_first_of(delimiters, lastPos);

		while (std::string::npos != pos || std::string::npos != lastPos)
		{
			// Found a token, add it to the vector.
			tokens.push_back(str.substr(lastPos, pos - lastPos));
			// Skip delimiters.  Note the "not_of"
			lastPos = str.find_first_not_of(delimiters, pos);
			// Find next "non-delimiter"
			pos = str.find_first_of(delimiters, lastPos);
		}
	}
};

const std::string StringTokenizer::WHITESPACES = " \t\n\v\f\r";
#endif