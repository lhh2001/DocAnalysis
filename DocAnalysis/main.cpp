#include "DocAnalysis.h"

#ifdef _DEBUG
#include <crtdbg.h>
#endif

int main()
{
    #ifdef _DEBUG //Debugģʽ�µ������ļ���
    const char* inFileAName = "1";
    const char* inFileBName = "2";
    #else //Releaseģʽ�µ������ļ���
    const char* inFileAName = "�ʱ���";
    const char* inFileBName = "����������";

    DocAnalysis::useTxtFileDirectly(); //ֱ��ʹ��txt�ļ���ִ�б�����, ���򽫱��д���ע�͵�
    #endif

    //DocAnalysis::changeDocExtension(".doc"); //�����ļ���׺��, ���Ϊ.doc��ִ�б�����

    DocAnalysis* myDocAnalysis = new DocAnalysis(inFileAName, inFileBName);
    myDocAnalysis->run();
    delete myDocAnalysis;
    
    #ifdef _DEBUG
    _CrtDumpMemoryLeaks(); //����ڴ��Ƿ�й©
    #endif
    return 0;
}
