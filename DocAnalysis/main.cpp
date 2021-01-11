#include "DocAnalysis.h"
#include <crtdbg.h>

int main()
{
    const char* inFileAName = "1";
    const char* inFileBName = "2";

    //DocAnalysis::changeDocExtension(".doc"); //�����ļ���׺��, ���Ϊ.doc��ִ�б�����
    //DocAnalysis::useTxtFileDirectly(); //ֱ��ʹ��txt�ļ���ִ�б�����, ���򽫱��д���ע�͵�

    DocAnalysis* myDocAnalysis = new DocAnalysis(inFileAName, inFileBName);
    myDocAnalysis->run();
    delete myDocAnalysis;
    
    _CrtDumpMemoryLeaks(); //����ڴ��Ƿ�й©
    return 0;
}
