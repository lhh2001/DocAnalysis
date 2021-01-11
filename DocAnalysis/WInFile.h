#pragma once
#include <fstream>
#include <string>

class WInFile
{
private:
	std::ifstream* inFile;
	static const unsigned char UNSIGNED_EOF = 0xff; //0xff为EOF的unsinged形式
	wchar_t convertUCharToWChar(unsigned char c);

public:
	WInFile(const char* fileName);
	~WInFile();
	bool readLine(std::wstring& s);
	void readAll(std::wstring& s);
};