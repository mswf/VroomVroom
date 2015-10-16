#ifndef helperfunctions_h
#define helperfunctions_h

#include "../typedef.h"

class HelperFunctions
{
	public:
		static std::string ConvertLineEndings(std::string text);
		static void ReplaceStringInPlace(std::string& subject, const std::string& search, const std::string& replace);
		static std::string ReadFile(std::string file);
		static std::string ReadFile(const char* file);
		static std::string VoidPtrToString(void* data, const int size);

		//everything below here is untested
		static int16 VoidPtrToInt16(void* data, const int size);
		static int32 VoidPtrToInt32(void* data, const int size);
		static int64 VoidPtrToInt64(void* data, const int size);
		static float VoidPtrToFloat(void* data, const int size);
		template<typename T>
		static void InsertIntoBuffer(char* buffer, const int index, const T& value);
		template<typename T>
		static void ReadFromBuffer(char* buffer, const int index, T& value);
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

template<typename T>
void HelperFunctions::InsertIntoBuffer(char* buffer, const int index, const T& value)
{
	int typeSize = sizeof(T);
	for (int i = 0; i < typeSize; ++i)
	{
		buffer[index + i] = ((char*)&value)[i];
	}
}

template <typename T>
void HelperFunctions::ReadFromBuffer(char* buffer, const int index, T& value)
{
	value =  *(T*)buffer[index];
}

#endif //helperfunctions_h
