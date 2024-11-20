
// Monapuzzle.h : main header file for the Monapuzzle application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CMonapuzzleApp:
// See Monapuzzle.cpp for the implementation of this class
//

class CMonapuzzleApp : public CWinApp
{
public:
	CMonapuzzleApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMonapuzzleApp theApp;
