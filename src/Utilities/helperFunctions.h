#ifndef helperfunctions_h
#define helperfunctions_h

#include "../Utilities/typedef.h"

enum class ImageFileFormat
{
	PNG,
	BMP,
	TGA
};

struct tm;

class HelperFunctions
{
	public:
		static std::string ConvertLineEndings(std::string text);
		static void ReplaceStringInPlace(std::string& subject, const std::string& search, const std::string& replace);
		static std::string ReadFile(std::string file);
		static std::string ReadFile(const char* file);
		static void WritePixels( const char* filename, ImageFileFormat format, uint8* pixels, int width, int height, int components );
		static std::string VoidPtrToString(void* data, const int size);
		static bool FileExists(const char* file);
		static tm GetTime();
		static string GetTimeString();
		//everything below here is untested
		static int16 VoidPtrToInt16(void* data, const int size);
		static int32 VoidPtrToInt32(void* data, const int size);
		static int64 VoidPtrToInt64(void* data, const int size);
		static float VoidPtrToFloat(void* data, const int size);
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
