// TPDlg.h : header file
//

#pragma once

#include "WTimer.h"

#define	NUMOFFREQ	2000

class CGraphWnd : public CWnd
{
protected:
	afx_msg	void	OnPaint( void );
	DECLARE_MESSAGE_MAP()
};

class CTPDlg : public CDialog
{
public:
	CTPDlg( CWnd* pParent = nullptr );

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TP_DIALOG };
#endif

protected:
		HICON	m_hIcon;
		HACCEL	m_hAccel;

		int	m_nTime;
		UINT	m_nPeriod;
		int	m_iTimerKind;
		int	m_iNotify;
		int	m_nMinTest;
		bool	m_bTesting;

		int	m_anFreq    [NUMOFFREQ];
		int	m_anFreqLast[NUMOFFREQ];

		QWORD	m_qwFreq;
		QWORD	m_qwFirst,
			m_qwLast,
			m_qwMin,
			m_qwMax,
			m_qwSum,
			m_nFreq;
		int	m_iCore;
		QWORD	m_qwRate;

	    FILETIME	m_ftIdle,
			m_ftKernel,
			m_ftUser;

		CString	m_strFreq,
			m_strError;

		CWTimer	m_timer;

	    CGraphWnd	m_wndGraph;
		
	virtual	BOOL	OnInitDialog( void );
	virtual	BOOL	DestroyWindow( void );
	virtual	BOOL	PreTranslateMessage( MSG* pMsg );

	afx_msg	void	OnSysCommand( UINT nID, LPARAM lParam );
	afx_msg	void	OnTimer( UINT_PTR nIDEvent );
	afx_msg	void	OnButtonCopy( void );
	afx_msg	void	OnButtonStart( void );
        afx_msg	void	OnButtonStop( void );
        afx_msg	void	OnSelectTimer( UINT uID );
	afx_msg LRESULT	OnPaintGraph( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT	OnWTimer( WPARAM wParam, LPARAM lParam );
	afx_msg	void	OnEditCopy( void );
	afx_msg	void	OnEditPaste( void );
	DECLARE_MESSAGE_MAP()

		void	StartTest( void );
		void	StopTest( void );
		void	StartTimers( void );
		void	StopTimers( void );

		void	ClearFrequency( void );
		void	CountFrequency( void );
		void	ShowFrequency( void );
		void	CopyFrequency( void );
		void	PasteFrequency( void );
		bool	TakeValue( CString strLine, CString strTag, QWORD& qwValue );
	static	void	CALLBACK
			OnTimerCall( HWND hWnd, UINT uMsg, UINT_PTR nIDEvent, DWORD uElapse );
};
