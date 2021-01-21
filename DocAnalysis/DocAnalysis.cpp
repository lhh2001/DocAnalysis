#include "DocAnalysis.h"
#include "WInFile.h"
#include "WOutFile.h"
#include <iostream>
#include <Windows.h>
#include <queue>

const char* DocAnalysis::DICT_DIR = "dict";
const char* DocAnalysis::DOC_EXTENSION = ".docx";
bool DocAnalysis::USE_TXT_FILE_DIRECTLY = false;

DocAnalysis::DocAnalysis(const char* inFileAName_, const char* inFileBName_)
{
    inFileAName = inFileAName_;
    inFileBName = inFileBName_;
}

DocAnalysis::DocAnalysis(std::string& inFileAName_, std::string& inFileBName_)
{
    inFileAName = inFileAName_;
    inFileBName = inFileBName_;
}

DocAnalysis::~DocAnalysis()
{
    delete dictFile;
    delete stopWordsFile;
    delete dictTrieTree;
    delete stopWordsTrieTree;
}

void DocAnalysis::run()
{
    std::cout << "--�ļ����ƶȱȽ�--" << std::endl;
    
    stopWordsFile = new WInFile((static_cast<std::string>(DICT_DIR) + "/baidu_stopwords.txt").c_str());
    stopWordsTrieTree = new TrieTree<wchar_t>;
    buildTrieTreeFromFile(stopWordsFile, stopWordsTrieTree);
    std::cout << "�Ѽ���ֹͣ�ʿ�" << std::endl;

    dictFile = new WInFile((static_cast<std::string>(DICT_DIR) + "/dict.txt").c_str());
    dictTrieTree = new TrieTree<wchar_t>;
    buildTrieTreeFromFile(dictFile, dictTrieTree);
    std::cout << "�Ѽ��طִʴʵ�" << std::endl << std::endl;

    if (!USE_TXT_FILE_DIRECTLY)
    {
        std::cout << "ת��doc��txt��(��������5-7����)..." << std::endl;
        if (convertDocToTxt(inFileAName, inFileBName))
        {
            std::cout << "ת��doc��txt�ɹ�!" << std::endl;
        }
        else
        {
            std::cout << "ת��doc��txt�쳣!" << std::endl;
            return;
        }
        std::cout << std::endl;
    }

    std::string tmp = inFileAName;

    analyzeFile(inFileAName);
    system(("type " + tmp + '\\' + tmp + "_result.txt").c_str());
    std::cout << std::endl;
    analyzeFile(inFileBName);
    tmp = inFileBName;
    system(("type " + tmp + '\\' + tmp + "_result.txt").c_str());
    std::cout << std::endl;

    std::cout << std::endl << "��ͬ�ַ��� " << sameCharNum << " ��" << std::endl;
    std::cout << "��ͬ�ַ��� " << totCharNum - 2 * sameCharNum << " ��" << std::endl;
}

void DocAnalysis::analyzeFile(std::string& fileName)
{
    std::cout << "���� " << fileName << DOC_EXTENSION << " ��..." << std::endl;

    WInFile* inFile = new WInFile((fileName + '/' + fileName + ".txt").c_str());
    WOutFile* outFile = new WOutFile((fileName + '/' + fileName + "_result.txt").c_str());

    dictTrieTree->clearFreq(); //�����ϴεĴʵ�Ա��ε�Ӱ��

    std::wstring myDoc;
    inFile->readAll(myDoc); //������������

    getCharFreq(myDoc);
    getWordFreq(myDoc);

    printResult(outFile);
    std::cout << "���������� " << fileName + '\\' + fileName + "_result.txt" << std::endl;

    delete inFile;
    delete outFile;
}

bool DocAnalysis::convertDocToTxt(std::string& fileAName, std::string& fileBName) //����true��ʾ�ɹ�
{
    std::string tmp = fileAName + '\\' + fileAName + DOC_EXTENSION + ' ' + fileBName + '\\' + fileBName + DOC_EXTENSION;
    return !system(("powershell -ExecutionPolicy Bypass -F tools\\doc2txt.ps1 " + tmp).c_str());
}

void DocAnalysis::buildTrieTreeFromFile(WInFile* trieTreeWInFile, TrieTree<wchar_t>* myTrieTree)
{
    bool flag = true;
    do
    {
        std::wstring s;
        flag = trieTreeWInFile->readLine(s);
        myTrieTree->insert(s.c_str(), s.size());
    } while (flag);
}

bool DocAnalysis::isWCharValid(wchar_t wc)
{
    if (!(wc & 0x00ff)) //���Ǻ���
    {
        char c = wc >> 8;
        //wc�ǻ���, �ո����Ч�ַ�
        if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')))
        {
            return false;
        }
    }
    return true;
}

void DocAnalysis::getCharFreq(std::wstring& myDoc) //ͳ����ͬ�ַ�
{
    if (!charFreq.size()) //sizeΪ0��ʾ���ڷ�����һ���ļ�
    {
        for (auto wc : myDoc)
        {
            if (!isWCharValid(wc)) //�ַ����Ϸ�������
            {
                continue;
            }
            totCharNum++;
            charFreq[wc]++;
        }
    }
    else
    {
        for (auto wc : myDoc)
        {
            if (!isWCharValid(wc))
            {
                continue;
            }
            totCharNum++;
            auto iter = charFreq.find(wc);
            if (iter != charFreq.end() && iter->second)
            {
                sameCharNum++;
                iter->second--;
            }
        }
    }
}

void DocAnalysis::getWordFreq(std::wstring& myDoc)
{
    dictTrieTree->match(myDoc.c_str(), myDoc.size());
    dictTrieTree->getFreqTable(q);
}

void DocAnalysis::printResult(WOutFile* outFile)
{
    int wordNum = 0; //���������Ŀ
    int charNum = 0; //���������Ŀ
    const int limit = 10; //�����Ŀ����
    std::queue<std::pair<int, std::vector<wchar_t>>> cq; //����ַ��Ķ���

    (*outFile) << "ǰʮ��Ƶ��: \n";
    while (!q.empty())
    {
        if (!stopWordsTrieTree->find(q.top().second))
        {
            if (q.top().second.size() > 1 && wordNum < limit)
            {
                wordNum++;
                (*outFile) << q.top().second << ' ' << q.top().first << "��\n";
            }
            else if (q.top().second.size() == 1 && charNum < limit)
            {
                charNum++;
                cq.push(q.top());
            }
        }
        q.pop();
    }

    (*outFile) << "\nǰʮ��Ƶ��: \n";
    while (!cq.empty())
    {
        (*outFile) << cq.front().second << ' ' << cq.front().first << "��\n";
        cq.pop();
    }
}

void DocAnalysis::changeDocExtension(const char* docExtension_)
{
    if (docExtension_ != ".docx" && docExtension_ != ".doc")
    {
        throw "doc�ļ���׺���淶!(ӦΪ.docx��.doc)";
    }
    DOC_EXTENSION = docExtension_;
}

void DocAnalysis::useTxtFileDirectly()
{
    USE_TXT_FILE_DIRECTLY = true;
}
