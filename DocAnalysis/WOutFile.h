#pragma once
#include <fstream>
#include <string>
#include <vector>

class WOutFile
{
private:
	std::ofstream* outFile;
	void printWChar(wchar_t wc);

public:
	WOutFile(const char* fileName);
	~WOutFile();
	void printWString(std::wstring s);
	void printWString(std::vector<wchar_t> s);
	std::ostream& operator<<(std::wstring s);
	std::ostream& operator<<(std::vector<wchar_t> s);
	template<class T>
	std::ostream& operator<<(T t)
	{
		return *outFile << t;
	}
};
