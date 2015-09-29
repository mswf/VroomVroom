#include "standardIncludes.h"
#include "helperFunctions.h"

void HelperFunctions::ConvertLineEndings(std::string text)
{
#if __APPLE__
	LineEndingType type = LineEndingType::CR;
#elif _WIN32
	LineEndingType type = LineEndingType::CRLF;
#elif __linux__
	LineEndingType type = LineEndingType::LF;
#endif
	switch (type)
	{
		//Mac \r
		case LineEndingType::CR:
		{
			ReplaceStringInPlace(text, "\r\n", "\r");
			ReplaceStringInPlace(text, "\n", "\r");
			break;
		}
		//Unix \n
		case LineEndingType::LF:
		{
			ReplaceStringInPlace(text, "\r\n", "\n");
			ReplaceStringInPlace(text, "\r", "\n");
			break;
		}
		//Windows \r\n
		case LineEndingType::CRLF:
		{
			//Because I don't want to spend time making a RegEx
			const string pls = "AIDUNNOLOLOLOLXOXOYKI";
			ReplaceStringInPlace(text, "\r\n", pls);
			ReplaceStringInPlace(text, "\r", pls);
			ReplaceStringInPlace(text, "\n", pls);
			ReplaceStringInPlace(text, pls, "\r\n");
			break;
		}
		default:
		{
			assert(false);
			break;
		}
	}
}

void HelperFunctions::ReplaceStringInPlace(std::string text, const std::string& find,	const std::string& replace) 
{
	size_t pos = 0;
	while ((pos = text.find(find, pos)) != std::string::npos) 
	{
		text.replace(pos, find.length(), replace);
		pos += replace.length();
	}
}