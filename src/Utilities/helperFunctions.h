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
		static void InsertIntoBuffer(char* buffer, int& currentIndex, const T& inValue);
		template<typename T>
		static void ReadFromBuffer(char* buffer, int& currentIndex, T& outValue);
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
void HelperFunctions::InsertIntoBuffer(char* buffer, int& currentIndex, const T& inValue)
{
	int typeSize = sizeof(T);
	for (int i = 0; i < typeSize; ++i)
	{
		buffer[currentIndex + i] = ((char*)&inValue)[i];
	}

	currentIndex += typeSize;
}

template <typename T>
void HelperFunctions::ReadFromBuffer(char* buffer, int& currentIndex, T& outValue)
{
	//copies the value into outValue
	//outValue = T((T)buffer[currentIndex]);
	//int typeSize = sizeof(T);
	//currentIndex += typeSize;

	//outValue = T(*(T*)buffer[currentIndex]);

	//reference to memory instead
	//outValue = (T)buffer[currentIndex];

	int typeSize = sizeof(T);

	for (int i = 0; i < typeSize; ++i)
	{
		((char*)&outValue)[i] = buffer[currentIndex + i];
	}
	currentIndex += typeSize;
}

#endif //helperfunctions_h
