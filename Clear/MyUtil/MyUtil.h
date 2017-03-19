#ifndef MYUTIL_H
#define MYUTIL_H
#pragma once

#include <fstream>

class MyUtil
{
public:
	MyUtil();
	~MyUtil();
	
	static void WriteString(std::string info);
	static void WriteData(std::string info,int data);
	static void WriteDataFloat(std::string info, float data, bool isEndl);
	static void WstringToCharArry(char(&result)[50], const std::wstring& ws);
private:
	static void Init();
	static bool mIsInit;
public:
	static std::ofstream mFos;
	static bool mIsWriteDebugInfo;
};

#endif