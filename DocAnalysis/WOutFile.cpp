#include "WOutFile.h"

WOutFile::WOutFile(const char* fileName)
{
	outFile = new std::ofstream(fileName);
}

WOutFile::~WOutFile()
{
	delete outFile;
}

void WOutFile::printWChar(wchar_t wc)
{
    if (wc & 0x00ff)
    {
        outFile->put((wc & 0xff00) >> 8);
        outFile->put(wc & 0x00ff);
    }
    else
    {
        outFile->put(wc >> 8);
    }
}

void WOutFile::printWString(std::wstring s)
{
    for (auto wc : s)
    {
        printWChar(wc);
    }
}

void WOutFile::printWString(std::vector<wchar_t> s)
{
    for (auto wc : s)
    {
        printWChar(wc);
    }
}

std::ostream& WOutFile::operator<<(std::wstring s)
{
    printWString(s);
    return *outFile;
}

std::ostream& WOutFile::operator<<(std::vector<wchar_t> s)
{
    printWString(s);
    return *outFile;
}
