
// IND_18981.h : main header file for the IND_18981 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CIND18981App:
// See IND_18981.cpp for the implementation of this class
//

class CIND18981App : public CWinApp
{
public:
	CIND18981App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CIND18981App theApp;
