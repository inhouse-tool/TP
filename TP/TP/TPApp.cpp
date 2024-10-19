// TP.cpp : Defines the class behaviors for the application.
//

#include "pch.h"
#include "TPApp.h"
#include "TPDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CTPApp	theApp;

BOOL
CTPApp::InitInstance( void )
{
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof( InitCtrls );
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx( &InitCtrls );

	CWinApp::InitInstance();

	CTPDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();

	return	FALSE;
}
///////////////////////////////////////////////////////////////////////////////////////
// Message Handlers

BEGIN_MESSAGE_MAP( CTPApp, CWinApp )
	ON_COMMAND( ID_HELP_INDEX, OnInfoOnWeb )
END_MESSAGE_MAP()

void
CTPApp::OnInfoOnWeb( void )
{
	CString	strPath = _T("https://github.com/inhouse-tool/TP");
	ShellExecute( NULL, _T("open"), strPath, NULL, NULL, SW_SHOWNORMAL );
}
