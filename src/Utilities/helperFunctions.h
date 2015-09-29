#pragma once
#ifndef helperfunctions_h
#define helperfunctions_h
#include <random>




class HelperFunctions
{
public:
	static void ConvertLineEndings(std::string text);
	static void ReplaceStringInPlace(std::string subject, const std::string& search, const std::string& replace);
private:

	enum class LineEndingType
	{
		//Mac \r
		CR,
		//Unix \n
		LF,
		//Windows \r\n
		CRLF
	};
};

#endif //helperfunctions_h
