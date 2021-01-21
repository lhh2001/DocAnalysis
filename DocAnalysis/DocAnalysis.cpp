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
    std::cout << "--文件相似度比较--" << std::endl;
    
    stopWordsFile = new WInFile((static_cast<std::string>(DICT_DIR) + "/baidu_stopwords.txt").c_str());
    stopWordsTrieTree = new TrieTree<wchar_t>;
    buildTrieTreeFromFile(stopWordsFile, stopWordsTrieTree);
    std::cout << "已加载停止词库" << std::endl;

    dictFile = new WInFile((static_cast<std::string>(DICT_DIR) + "/dict.txt").c_str());
    dictTrieTree = new TrieTree<wchar_t>;
    buildTrieTreeFromFile(dictFile, dictTrieTree);
    std::cout << "已加载分词词典" << std::endl << std::endl;

    if (!USE_TXT_FILE_DIRECTLY)
    {
        std::cout << "转换doc到txt中(本过程需5-7秒钟)..." << std::endl;
        if (convertDocToTxt(inFileAName, inFileBName))
        {
            std::cout << "转换doc到txt成功!" << std::endl;
        }
        else
        {
            std::cout << "转换doc到txt异常!" << std::endl;
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

    std::cout << std::endl << "相同字符有 " << sameCharNum << " 个" << std::endl;
    std::cout << "不同字符有 " << totCharNum - 2 * sameCharNum << " 个" << std::endl;
}

void DocAnalysis::analyzeFile(std::string& fileName)
{
    std::cout << "分析 " << fileName << DOC_EXTENSION << " 中..." << std::endl;

    WInFile* inFile = new WInFile((fileName + '/' + fileName + ".txt").c_str());
    WOutFile* outFile = new WOutFile((fileName + '/' + fileName + "_result.txt").c_str());

    dictTrieTree->clearFreq(); //消除上次的词典对本次的影响

    std::wstring myDoc;
    inFile->readAll(myDoc); //读入所有文字

    getCharFreq(myDoc);
    getWordFreq(myDoc);

    printResult(outFile);
    std::cout << "已输出结果到 " << fileName + '\\' + fileName + "_result.txt" << std::endl;

    delete inFile;
    delete outFile;
}

bool DocAnalysis::convertDocToTxt(std::string& fileAName, std::string& fileBName) //返回true表示成功
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
    if (!(wc & 0x00ff)) //不是汉字
    {
        char c = wc >> 8;
        //wc是换行, 空格等无效字符
        if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')))
        {
            return false;
        }
    }
    return true;
}

void DocAnalysis::getCharFreq(std::wstring& myDoc) //统计相同字符
{
    if (!charFreq.size()) //size为0表示正在分析第一个文件
    {
        for (auto wc : myDoc)
        {
            if (!isWCharValid(wc)) //字符不合法，跳过
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
    int wordNum = 0; //已输出词数目
    int charNum = 0; //已输出字数目
    const int limit = 10; //输出数目限制
    std::queue<std::pair<int, std::vector<wchar_t>>> cq; //存放字符的队列

    (*outFile) << "前十高频词: \n";
    while (!q.empty())
    {
        if (!stopWordsTrieTree->find(q.top().second))
        {
            if (q.top().second.size() > 1 && wordNum < limit)
            {
                wordNum++;
                (*outFile) << q.top().second << ' ' << q.top().first << "次\n";
            }
            else if (q.top().second.size() == 1 && charNum < limit)
            {
                charNum++;
                cq.push(q.top());
            }
        }
        q.pop();
    }

    (*outFile) << "\n前十高频字: \n";
    while (!cq.empty())
    {
        (*outFile) << cq.front().second << ' ' << cq.front().first << "次\n";
        cq.pop();
    }
}

void DocAnalysis::changeDocExtension(const char* docExtension_)
{
    if (docExtension_ != ".docx" && docExtension_ != ".doc")
    {
        throw "doc文件后缀不规范!(应为.docx或.doc)";
    }
    DOC_EXTENSION = docExtension_;
}

void DocAnalysis::useTxtFileDirectly()
{
    USE_TXT_FILE_DIRECTLY = true;
}
