#include <iostream>
#include <sstream>
#include <cctype>
#include <algorithm>
#include "util.h"

using namespace std;
std::string convToLower(std::string src)
{
    std::transform(src.begin(), src.end(), src.begin(), ::tolower);
    return src;
}

/** Complete the code to convert a string containing a rawWord
    to a set of words based on the criteria given in the assignment **/

/** 
 * Splitting string of many words into individual keywords (split at 
 * punctuation, with at least 2 character words)
 */
std::set<std::string> parseStringToWords(string rawWords)
{
	// Convert all words to lower case.
	string lowerWords = convToLower(rawWords);

	set<string> stringSet;
	stringstream words;

	for(unsigned int i = 0; i < lowerWords.size(); i++)
	{
		if(ispunct(lowerWords[i]))
		{
			lowerWords.erase(i, 1);
			lowerWords.insert(i, 1, ' ');
		}
	}

	words << lowerWords;
	string keyWord;

	while(words >> keyWord)
	{
		if(keyWord.size() >= 2)
		{
			stringSet.insert(keyWord);
		}
	}
	return stringSet;
}

/**************************************************
 * COMPLETED - You may use the following functions
 **************************************************/

// Used from http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
// trim from start
std::string &ltrim(std::string &s) {
    s.erase(s.begin(), 
	    std::find_if(s.begin(), 
			 s.end(), 
			 std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

// trim from end
std::string &rtrim(std::string &s) {
    s.erase(
	    std::find_if(s.rbegin(), 
			 s.rend(), 
			 std::not1(std::ptr_fun<int, int>(std::isspace))).base(), 
	    s.end());
    return s;
}

// trim from both ends
std::string &trim(std::string &s) {
    return ltrim(rtrim(s));
}
