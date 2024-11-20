
// OptimumPrajm.h : main header file for the OptimumPrajm application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// COptimumPrajmApp:
// See OptimumPrajm.cpp for the implementation of this class
//

class COptimumPrajmApp : public CWinApp
{
public:
	COptimumPrajmApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	UINT  m_nAppLook;
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern COptimumPrajmApp theApp;
