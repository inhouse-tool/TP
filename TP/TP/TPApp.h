// TPApp.h : main header file for the TP application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"

class CTPApp : public CWinApp
{
protected:
	virtual	BOOL	InitInstance( void );

	afx_msg	void	OnInfoOnWeb( void );
	DECLARE_MESSAGE_MAP()
};

extern	CTPApp	theApp;
