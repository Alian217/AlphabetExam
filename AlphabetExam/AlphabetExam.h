
// AlphabetExam.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CAlphabetExamApp:
// �йش����ʵ�֣������ AlphabetExam.cpp
//

class CAlphabetExamApp : public CWinApp
{
public:
	CAlphabetExamApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CAlphabetExamApp theApp;