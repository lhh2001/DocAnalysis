#include "WInFile.h"

WInFile::WInFile(const char* fileName)
{
	inFile = new std::ifstream(fileName);
}

WInFile::~WInFile()
{
	delete inFile;
}

wchar_t WInFile::convertUCharToWChar(unsigned char c)
{
    wchar_t wc = 0;
    if (c & 0x80) //Ê×Î»Îª1
    {
        wc |= static_cast<wchar_t>(c) << 8;
        c = inFile->get();
        wc |= static_cast<wchar_t>(c);
    }
    else
    {
        wc = static_cast<wchar_t>(c) << 8;
    }
    return wc;
}

bool WInFile::readLine(std::wstring& s)
{
    unsigned char c = inFile->get();
    if (c == UNSIGNED_EOF)
    {
        return false;
    }
    while (c != static_cast<unsigned char>('\n'))
    {
        wchar_t wc = convertUCharToWChar(c);
        s += wc;
        c = inFile->get();
    }
    return true;
}

void WInFile::readAll(std::wstring& s)
{
    unsigned char c = inFile->get();
    while (c != UNSIGNED_EOF)
    {
        wchar_t wc = convertUCharToWChar(c);
        s += wc;
        c = inFile->get();
    }
}
