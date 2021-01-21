#include "DocAnalysis.h"

#ifdef _DEBUG
#include <crtdbg.h>
#endif

int main()
{
    #ifdef _DEBUG //Debug模式下的两个文件名
    const char* inFileAName = "1";
    const char* inFileBName = "2";
    #else //Release模式下的两个文件名
    const char* inFileAName = "资本论";
    const char* inFileBName = "共产党宣言";

    DocAnalysis::useTxtFileDirectly(); //直接使用txt文件请执行本函数, 否则将本行代码注释掉
    #endif

    //DocAnalysis::changeDocExtension(".doc"); //设置文件后缀名, 如果为.doc请执行本函数

    DocAnalysis* myDocAnalysis = new DocAnalysis(inFileAName, inFileBName);
    myDocAnalysis->run();
    delete myDocAnalysis;
    
    #ifdef _DEBUG
    _CrtDumpMemoryLeaks(); //检查内存是否泄漏
    #endif
    return 0;
}
