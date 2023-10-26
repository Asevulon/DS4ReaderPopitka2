
// DS4ReaderPopitka2.h: главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CDS4ReaderPopitka2App:
// Сведения о реализации этого класса: DS4ReaderPopitka2.cpp
//

class CDS4ReaderPopitka2App : public CWinApp
{
public:
	CDS4ReaderPopitka2App();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CDS4ReaderPopitka2App theApp;
