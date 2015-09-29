#include "standardIncludes.h"
#include "helperFunctions.h"

void HelperFunctions::ConvertLineEndings(std::string& text, LineEndingType type)
{
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
		ReplaceStringInPlace(text, "\r\n", "AIDUNNOLOLOLOL");
		ReplaceStringInPlace(text, "\r", "AIDUNNOLOLOLOL");
		ReplaceStringInPlace(text, "\n", "AIDUNNOLOLOLOL");
		ReplaceStringInPlace(text, "AIDUNNOLOLOLOL", "\r\n");
		break;
	default:
		assert(false);
		break;
	}
}

void HelperFunctions::ReplaceStringInPlace(std::string& text, const std::string& find,	const std::string& replace) 
{
	size_t pos = 0;
	while ((pos = text.find(find, pos)) != std::string::npos) 
	{
		text.replace(pos, find.length(), replace);
		pos += replace.length();
	}
}