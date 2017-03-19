#include"MyUtil.h"
#include <ctime> 
#include <comdef.h>  

std::ofstream MyUtil::mFos;
bool MyUtil::mIsWriteDebugInfo = true;
bool MyUtil::mIsInit = false;
MyUtil::MyUtil()
{

}
MyUtil::~MyUtil()
{

}

void MyUtil::Init()
{
	mIsInit = true;
	mIsWriteDebugInfo = true;
	mFos.open("debugInfo.txt", std::ios::trunc);
	mFos << std::endl << "----------------------------------------" << std::endl;
	mFos.close();
}
void MyUtil::WriteString(std::string info)
{
	if (mIsWriteDebugInfo)
	{
		if (!mIsInit)
			Init();
		mFos.open("debugInfo.txt", std::ios::app);
		mFos << info.c_str() << std::endl;
		mFos.close();
	}
}
void MyUtil::WriteData(std::string info, int data)
{
	if (mIsWriteDebugInfo)
	{
		if (!mIsInit)
			Init();
		mFos.open("debugInfo.txt", std::ios::app);
		mFos << info.c_str() << " = " << data << std::endl;
		mFos.close();
	}
}
void MyUtil::WriteDataFloat(std::string info, float data, bool isEndl)
{
	if (mIsWriteDebugInfo)
	{
		if (!mIsInit)
			Init();
		mFos.open("debugInfo.txt", std::ios::app);
		mFos << info.c_str() << " = " << data << " ";
		if (isEndl)
			mFos << std::endl;
		mFos.close();
	}
}
void MyUtil::WstringToCharArry(char(&result)[50], const std::wstring& ws)
{
	int len = ws.size();
	if (sizeof(result - 1) <len)
		return;
	_bstr_t t = ws.c_str();
	char* pchar = (char*)t;
	std::string temstr = pchar;
	strcpy_s(result, temstr.c_str());
	result[len] = '\0';
	pchar = NULL;
	delete pchar;
}