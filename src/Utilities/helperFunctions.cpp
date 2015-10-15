#include "helperFunctions.h"
#include "standardIncludes.h"
#include <iostream>
#include <fstream>
#include <sstream>

std::string HelperFunctions::ConvertLineEndings(std::string text)
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

	return text;
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

std::string HelperFunctions::ReadFile(std::string file)
{
	return ReadFile(file.c_str());
}

std::string HelperFunctions::ReadFile(const char* file)
{
	std::ifstream fileStream(file);
	if (!fileStream)
	{
		std::cerr << "File could not be opened." << std::endl;
		assert(false);
	}
	std::stringstream fileData;
	fileData << fileStream.rdbuf();  //Loads the entire string into a string stream.
	fileStream.close();
	return ConvertLineEndings(fileData.str());
}

std::string HelperFunctions::VoidPtrToString(void* data, const int size)
{
	return string(static_cast<char*>(data), size);
}

int16 HelperFunctions::VoidPtrToInt16(void* data, const int size)
{
	assert(sizeof(int16) == size);
	return *static_cast<int16*>(data);
}

int32 HelperFunctions::VoidPtrToInt32(void* data, const int size)
{
	assert(sizeof(int32) == size);
	return *static_cast<int32*>(data);
}

int64 HelperFunctions::VoidPtrToInt64(void* data, const int size)
{
	assert(sizeof(int64) == size);
	return *static_cast<int64*>(data);
}

float HelperFunctions::VoidPtrToFloat(void* data, const int size)
{
	assert(sizeof(float) == size);
	return *static_cast<float*>(data);
}