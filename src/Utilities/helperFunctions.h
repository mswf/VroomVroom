#pragma once
#ifndef helperfunctions_h
#define helperfunctions_h
#include <random>


enum class LineEndingType
{
	//Mac \r
	CR,
	//Unix \n
	LF,
	//Windows \r\n
	CRLF
};

class HelperFunctions
{
public:
	static void ConvertLineEndings(std::string& text, LineEndingType type);
	static void ReplaceStringInPlace(std::string& subject, const std::string& search, const std::string& replace);
private:
	
};

#endif //helperfunctions_h
