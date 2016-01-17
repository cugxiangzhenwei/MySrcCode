#ifndef _PY_H_
#define _PY_H_

#include<string>
using namespace std;
/********************以下是头文件********************************/

/* 名称: Py.h
 * 实现功能: 获取拼音
 * 版本: 1.00
 * 说明: 本版本由网上获得的一个Dephile程序"翻译"而来
 * 翻译者: bohut
 * 完成时间 : 2003.07.08
 */
#pragma once
#define PYMUSICCOUNT   405

class CPy  
{
public:
	CPy(){}
	virtual ~CPy(){}
 string MakeSpellCode(string strText, int nMode);
};

#endif //_PY_H_