#include "DocAnalysis.h"
#include <crtdbg.h>

int main()
{
    const char* inFileAName = "1";
    const char* inFileBName = "2";

    //DocAnalysis::changeDocExtension(".doc"); //设置文件后缀名, 如果为.doc请执行本函数
    //DocAnalysis::useTxtFileDirectly(); //直接使用txt文件请执行本函数, 否则将本行代码注释掉

    DocAnalysis* myDocAnalysis = new DocAnalysis(inFileAName, inFileBName);
    myDocAnalysis->run();
    delete myDocAnalysis;
    
    _CrtDumpMemoryLeaks(); //检查内存是否泄漏
    return 0;
}
