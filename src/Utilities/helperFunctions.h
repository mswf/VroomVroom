#pragma once
#ifndef helperfunctions_h
#define helperfunctions_h
#include <random>




class HelperFunctions
{
	public:
		static std::string ConvertLineEndings(std::string text);
		static void ReplaceStringInPlace(std::string& subject, const std::string& search, const std::string& replace);
		static std::string ReadFile(std::string file);
		static std::string ReadFile(const char* file);
		static std::string VoidPtrToString(void* data, const int size);
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
