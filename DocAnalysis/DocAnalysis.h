#pragma once
#include <string>
#include "TrieTree.hpp"

class WInFile;
class WOutFile;

class DocAnalysis
{
private:
	static const char* DICT_DIR;
	static const char* DOC_EXTENSION;
	static bool USE_TXT_FILE_DIRECTLY;

	std::string inFileAName;
	std::string inFileBName;
	std::map<wchar_t, int> charFreq; //字符出现次数
	int sameCharNum = 0; //相同字符个数
	WInFile* dictFile = nullptr;
	WInFile* stopWordsFile = nullptr;
	TrieTree<wchar_t>* dictTrieTree = nullptr;
	TrieTree<wchar_t>* stopWordsTrieTree = nullptr;
	TrieTree<wchar_t>::qtype q;
	
	void buildTrieTreeFromFile(WInFile* trieTreeWInFile, TrieTree<wchar_t>* myTrieTree);
	bool convertDocToTxt(std::string& fileName);
	void getCharFreq(std::wstring& myDoc);
	void getWordFreq(std::wstring& myDoc);
	void printResult(WOutFile* outFile);
	void analyzeFile(std::string& fileName);

public:
	DocAnalysis(const char* inFileAName_, const char* inFileBName_);
	DocAnalysis(std::string& inFileAName_, std::string& inFileBName_);
	~DocAnalysis();
	void run();

	static void changeDocExtension(const char* docExtension_);
	static void useTxtFileDirectly();
};
