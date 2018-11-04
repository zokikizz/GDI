
// GDITemplate.h : main header file for the GDITemplate application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CGDITemplateApp:
// See GDITemplate.cpp for the implementation of this class
//

class CGDITemplateApp : public CWinAppEx
{
public:
	CGDITemplateApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CGDITemplateApp theApp;
