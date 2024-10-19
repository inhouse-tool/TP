// TPDlg.cpp : implementation file
//

#include "pch.h"
#include "TPApp.h"
#include "TPDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifdef	UNICODE
#define	atoi		_wtoi
#endif//UNICODE

#define	WM_WTIMER	(WM_APP+0)
#define	WM_PAINT_GRAPH	(WM_APP+1)

#define	TID_COUNT_FREQ	1
#define	TID_SHOW_FREQ	2
#define	TID_TEST	3

#define	TIME_RESOLUTION	1000	// [us]

///////////////////////////////////////////////////////////////////////////////////////
// Message Handlers ( of the Graph window )

BEGIN_MESSAGE_MAP( CGraphWnd, CWnd )
	ON_WM_PAINT()
END_MESSAGE_MAP()

void
CGraphWnd::OnPaint( void )
{
	PAINTSTRUCT ps;
	CDC*	pDC = BeginPaint( &ps );

	GetParent()->SendMessage( WM_PAINT_GRAPH, (WPARAM)&ps.rcPaint, (LPARAM)pDC );

	EndPaint( &ps );
}

///////////////////////////////////////////////////////////////////////////////////////
// Constructor

CTPDlg::CTPDlg( CWnd* pParent )
	: CDialog( IDD_TP_DIALOG, pParent )
{
	m_hIcon = AfxGetApp()->LoadIcon( IDR_MAINFRAME );
	m_hAccel = LoadAccelerators( AfxGetApp()->m_hInstance, MAKEINTRESOURCE( IDR_ACCELERATOR ) );

	m_nTime = 0;
	m_nPeriod = 0;
	m_iTimerKind = 0;
	m_iNotify = 0;
	m_nMinTest = 0;
	m_bTesting = false;

	m_ftIdle   = { 0 };
	m_ftKernel = { 0 };
	m_ftUser   = { 0 };
	m_qwRate   = 0;

	ClearFrequency();

	m_iCore = -1;

	m_timer.SetOwnerWnd( this );
}

///////////////////////////////////////////////////////////////////////////////////////
// Overridden Functions

BOOL
CTPDlg::OnInitDialog( void )
{
	CDialog::OnInitDialog();

	SetIcon( m_hIcon, TRUE );
	SetIcon( m_hIcon, FALSE );

	GetDlgItem( IDC_EDIT_TIME   )->SetWindowText( _T("15") );
	GetDlgItem( IDC_EDIT_PERIOD )->SetWindowText( _T("0") );
	GetDlgItem( IDC_EDIT_TEST   )->SetWindowText( _T("0") );

	((CButton*)GetDlgItem( IDC_RADIO_SETTIMER ))->SetCheck( BST_CHECKED );
	OnSelectTimer( IDC_RADIO_SETTIMER );
	((CComboBox*)GetDlgItem( IDC_COMBO_NOTIFY ))->SetCurSel( 0 );

	((CSpinButtonCtrl*)GetDlgItem( IDC_SPIN_TIME )  )->SetRange( 1, _countof( m_anFreq )*2 );
	((CSpinButtonCtrl*)GetDlgItem( IDC_SPIN_PERIOD ))->SetRange( 0, 16 );
	((CSpinButtonCtrl*)GetDlgItem( IDC_SPIN_TEST )  )->SetRange( 0, 60*24 );

	GetDlgItem( IDC_BUTTON_START )->EnableWindow( TRUE );
	GetDlgItem( IDC_BUTTON_STOP  )->EnableWindow( FALSE );
	GetDlgItem( IDC_BUTTON_COPY  )->EnableWindow( FALSE );
	GotoDlgCtrl( GetDlgItem( IDC_BUTTON_START ) );

	m_wndGraph.SubclassDlgItem( IDC_STATIC_GRAPH, this );

	CMenu* pSysMenu = GetSystemMenu( FALSE );
	if	( pSysMenu ){
		pSysMenu->InsertMenu( 5, MF_BYPOSITION, MF_SEPARATOR );
		pSysMenu->InsertMenu( 6, MF_BYPOSITION, ID_HELP_INDEX, _T("View info on &website\tF1") );
	}

	SetThreadPriority( AfxGetApp()->m_hThread, THREAD_PRIORITY_BELOW_NORMAL );

	return	FALSE;
}

BOOL
CTPDlg::DestroyWindow( void )
{
	StopTimers();

	return	CDialog::DestroyWindow();
}

BOOL
CTPDlg::PreTranslateMessage( MSG* pMsg )
{
	if	( TranslateAccelerator( GetSafeHwnd(), m_hAccel, pMsg ) )
		return	TRUE;
	else
		return	CDialog::PreTranslateMessage( pMsg );
}

///////////////////////////////////////////////////////////////////////////////////////
// Message Handlers

#include <mmsystem.h>			// for timeBeginPeriod, PlaySound
#pragma comment( lib, "winmm.lib" )

BEGIN_MESSAGE_MAP( CTPDlg, CDialog )
	ON_WM_SYSCOMMAND()
	ON_WM_TIMER()
	ON_BN_CLICKED( IDC_BUTTON_COPY,  OnButtonCopy )
	ON_BN_CLICKED( IDC_BUTTON_START, OnButtonStart )
	ON_BN_CLICKED( IDC_BUTTON_STOP,  OnButtonStop )
	ON_CONTROL_RANGE( BN_CLICKED, IDC_RADIO_SETTIMER, IDC_RADIO_WAITABLETIMER, OnSelectTimer )
	ON_MESSAGE( WM_PAINT_GRAPH, OnPaintGraph )
	ON_MESSAGE( WM_WTIMER,      OnWTimer )
	ON_COMMAND( ID_EDIT_COPY,  OnEditCopy )
	ON_COMMAND( ID_EDIT_PASTE, OnEditPaste )
END_MESSAGE_MAP()

void
CTPDlg::OnSysCommand( UINT nID, LPARAM lParam )
{
	if	( nID >= ID_APP_ABOUT &&
		  nID <= ID_DEFAULT_HELP )
		PostMessage( WM_COMMAND, nID, 0 );
	else
		CDialog::OnSysCommand( nID, lParam );
}

void
CTPDlg::OnTimer( UINT_PTR nIDEvent )
{
	if	( nIDEvent == TID_COUNT_FREQ )
		CountFrequency();
	else if	( nIDEvent == TID_SHOW_FREQ )
		ShowFrequency();
	else if	( nIDEvent == TID_TEST )
		StopTest();
}

void
CTPDlg::OnSelectTimer( UINT uID )
{
	BOOL	bEnable = ( uID == IDC_RADIO_WAITABLETIMER )? TRUE: FALSE;

	GetDlgItem( IDC_STATIC_NOTIFY )->EnableWindow( bEnable );
	GetDlgItem( IDC_COMBO_NOTIFY  )->EnableWindow( bEnable );
}

void
CTPDlg::OnButtonCopy( void )
{
	CopyFrequency();
}

void
CTPDlg::OnButtonStart( void )
{
	StartTest();
}

void
CTPDlg::OnButtonStop( void )
{
	m_nMinTest = 0;
	StopTest();
}

LRESULT
CTPDlg::OnPaintGraph( WPARAM wParam, LPARAM lParam )
{
	// Get rectangles.

	CDC*	pDC = (CDC*)lParam;
	CRect	rectUpdate = *(RECT*)wParam;
	CRect	rectGraph;
	GetDlgItem( IDC_STATIC_GRAPH )->GetClientRect( rectGraph );
	int	ox = rectUpdate.left;
	int	oy = rectUpdate.bottom;
	int	tx = rectUpdate.right;
	int	ty = rectUpdate.top;
	int	xMarker =  m_nTime * ( 1000/TIME_RESOLUTION );

	// Draw background.
	{
		CRect	rect = rectGraph;
		rect &= rectUpdate;
		pDC->FillSolidRect( &rect, RGB( 32, 32, 32 ) );

		CPen	penGrid( PS_SOLID, 1, RGB( 100, 100, 100 ) );
		pDC->SelectObject( &penGrid );
		for	( int x = 10; x < tx; x += 10 ){
			if	( x < ox || x > tx )
				continue;
			pDC->MoveTo( x, oy-1 );
			pDC->LineTo( x, ty );
		}
		for	( int y = rectGraph.bottom; y > 0; y -= 10 ){
			if	( y > oy || y < ty )
				continue;
			pDC->MoveTo( ox, y );
			pDC->LineTo( tx, y );
		}
	}

	// Draw historgram lines.

	CPen	penRed(    PS_SOLID, 1, RGB( 255,   0,   0 ) );
	CPen	penYellow( PS_SOLID, 1, RGB( 255, 255,   0 ) );
	CPen	penMarker( PS_SOLID, 1, RGB(   0,   0, 255 ) );
	pDC->SelectObject( &penRed );
	for	( int x = ox; x < tx; x++ ){
		if	( x >= _countof( m_anFreq ) )
			break;

		int	y = oy-m_anFreq[x];
		if	( y < 0 )
			y = 0;

		if	( x == xMarker )
			pDC->SelectObject( &penYellow );
		pDC->MoveTo( x, oy );
		pDC->LineTo( x,  y );
		if	( x == xMarker ){
			pDC->SelectObject( &penMarker );
			pDC->LineTo( x, ty );
			pDC->SelectObject( &penRed );
		}
	}

	return	0;
}

LRESULT
CTPDlg::OnWTimer( WPARAM wParam, LPARAM lParam )
{
	OnTimer( (UINT_PTR)wParam );

	return	0;
}

void
CTPDlg::OnEditCopy( void )
{
	CopyFrequency();
}

void
CTPDlg::OnEditPaste( void )
{
	PasteFrequency();
}

///////////////////////////////////////////////////////////////////////////////////////
// Specific Functions

void
CTPDlg::StartTest( void )
{
	// Stop previous timers.

	StopTimers();

	// Clear the previous results.

	ClearFrequency();

	// Prepare GUI items.

	m_wndGraph.Invalidate( TRUE );
	GetDlgItem( IDC_STATIC_RESULT )->SetWindowText( _T("") );

	for	( UINT uID = IDC_EDIT_TIME; uID <= IDC_STATIC_TEST2; uID++ )
		GetDlgItem( uID )->EnableWindow( FALSE );
	GetDlgItem( IDC_BUTTON_COPY  )->EnableWindow( FALSE );
	GetDlgItem( IDC_BUTTON_START )->EnableWindow( FALSE );
	GetDlgItem( IDC_BUTTON_STOP  )->EnableWindow( TRUE );
	GotoDlgCtrl( GetDlgItem( IDC_BUTTON_STOP ) );

	// Get current factors.

	CString	str;
	GetDlgItem( IDC_EDIT_TIME )->GetWindowText( str );
	m_nTime = atoi( str.GetBuffer() );

	GetDlgItem( IDC_EDIT_PERIOD )->GetWindowText( str );
	m_nPeriod = atoi( str.GetBuffer() );

	m_iTimerKind =	( ((CButton*)GetDlgItem( IDC_RADIO_SETTIMER )     )->GetCheck() == BST_CHECKED )? 1:
			( ((CButton*)GetDlgItem( IDC_RADIO_WAITABLETIMER ))->GetCheck() == BST_CHECKED )? 2: 0;

	m_iNotify = ((CComboBox*)GetDlgItem( IDC_COMBO_NOTIFY ))->GetCurSel();
	int	nTimerMsg = ( m_iNotify > 0 )? WM_WTIMER: WM_TIMER;

	m_timer.SetNotifyMsg( nTimerMsg );

	// Start the timers.

	GetDlgItem( IDC_EDIT_TEST )->GetWindowText( str );
	m_nMinTest = atoi( str.GetBuffer() );
	if	( m_nMinTest ){
		m_wndGraph.Invalidate( TRUE );
		m_timer.SetTimer( TID_TEST, m_nMinTest*1000*60, NULL );
		CTimeSpan	ts( 0, 0, m_nMinTest,  0 );
		CTime		t = CTime::GetCurrentTime();
		t += ts;
		str = t.Format( _T("Test until %H:%M:%S") );
		GetDlgItem( IDC_STATIC_RESULT )->SetWindowText( str );
		GetDlgItem( IDC_STATIC_CORE   )->SetWindowText( _T("") );
	}

	StartTimers();

	GetSystemTimes( &m_ftIdle, &m_ftKernel, &m_ftUser );

	m_bTesting = true;
}

void
CTPDlg::StopTest( void )
{
	m_bTesting = false;

	{
		FILETIME	ftIdle, ftKernel, ftUser;
		GetSystemTimes( &ftIdle, &ftKernel, &ftUser );

		ULARGE_INTEGER	liIdle, liKernel, liUser;
		liIdle   = *(ULARGE_INTEGER*)&ftIdle;
		liKernel = *(ULARGE_INTEGER*)&ftKernel;
		liUser   = *(ULARGE_INTEGER*)&ftUser;

		ULARGE_INTEGER	liIdle0, liKernel0, liUser0;
		liIdle0   = *(ULARGE_INTEGER*)&m_ftIdle;
		liKernel0 = *(ULARGE_INTEGER*)&m_ftKernel;
		liUser0   = *(ULARGE_INTEGER*)&m_ftUser;
		liIdle.QuadPart   -= liIdle0.QuadPart;
		liKernel.QuadPart -= liKernel0.QuadPart;
		liUser.QuadPart   -= liUser0.QuadPart;

#if	0
		// Rate = ( U+K-I ) / ( U+K ): Including 'net' Kernel time
		m_qwRate = liUser.QuadPart + liKernel.QuadPart - liIdle.QuadPart;
		m_qwRate *= 100*1000;
		m_qwRate /= liKernel.QuadPart + liUser.QuadPart;
#else
		// Rate = U / ( U+K ): 'Net' User time only ( like Task Manager shows )
		m_qwRate = liUser.QuadPart;
		m_qwRate *= 100*1000;
		m_qwRate /= ( liUser.QuadPart + liKernel.QuadPart );
#endif
	}
	CString	strRate;
	strRate.Format( _T("CPU: %lld.%03lld%%"), m_qwRate/1000, m_qwRate%1000 );

	// Stop the timers.

	StopTimers();
	m_timer.KillTimer( TID_TEST );

	// Restore GUI items.

	for	( UINT uID = IDC_EDIT_TIME; uID <= IDC_STATIC_TEST2; uID++ )
		GetDlgItem( uID )->EnableWindow( TRUE );
	GetDlgItem( IDC_BUTTON_COPY  )->EnableWindow( TRUE );
	GetDlgItem( IDC_BUTTON_START )->EnableWindow( TRUE );
	GetDlgItem( IDC_BUTTON_STOP  )->EnableWindow( FALSE );
	GotoDlgCtrl( GetDlgItem( IDC_BUTTON_START ) );
	GetDlgItem( IDC_STATIC_CORE )->SetWindowText( strRate );

	int	iKind = ( ((CButton*)GetDlgItem( IDC_RADIO_SETTIMER )     )->GetCheck() == BST_CHECKED )? 1:
			( ((CButton*)GetDlgItem( IDC_RADIO_WAITABLETIMER ))->GetCheck() == BST_CHECKED )? 2: 0;
	if	( iKind == 1 )
		OnSelectTimer( IDC_RADIO_SETTIMER );
	else if	( iKind == 2 )
		OnSelectTimer( IDC_RADIO_WAITABLETIMER );

	ShowFrequency();
	m_wndGraph.Invalidate( TRUE );

	if	( m_nMinTest )
		PlaySound( _T("C:\\Windows\\Media\\notify.wav"), NULL, SND_FILENAME | SND_ASYNC );
}

void
CTPDlg::StartTimers( void )
{
	if	( m_nPeriod )
		timeBeginPeriod( m_nPeriod );

	if	( m_iTimerKind == 1 )
		SetTimer( TID_COUNT_FREQ, m_nTime, NULL );
	else if	( m_iTimerKind == 2 )
		m_timer.SetTimer( TID_COUNT_FREQ, m_nTime, ( m_iNotify == 2 )? OnTimerCall: NULL );

	if	( !m_nMinTest )
		m_timer.SetTimer( TID_SHOW_FREQ, 1000, NULL );
}

void
CTPDlg::StopTimers( void )
{
	if	( m_nPeriod )
		timeEndPeriod( m_nPeriod );

	if	( m_iTimerKind == 1 )
		KillTimer( TID_COUNT_FREQ );
	else if	( m_iTimerKind == 2 )
		m_timer.KillTimer( TID_COUNT_FREQ );

	m_timer.KillTimer( TID_SHOW_FREQ );
}

void
CTPDlg::ClearFrequency( void )
{
	LARGE_INTEGER	liFreq;
	QueryPerformanceFrequency( &liFreq );
	m_qwFreq = liFreq.QuadPart;

	m_qwLast = m_qwSum = m_qwMax = 0;
	m_qwMin = (QWORD)-1;
	m_nFreq = 0;
	m_iCore = -1;
	for	( int i = 0; i < _countof( m_anFreq ); i++ ){
		m_anFreq[i] = 0;
		m_anFreqLast[i] = 0;
	}
}

void
CTPDlg::CountFrequency( void )
{
	if	( !m_bTesting )
		return;

	LARGE_INTEGER	liCount;
	QueryPerformanceCounter( &liCount );

	if	( m_qwLast ){
		QWORD	qwElapsed = liCount.QuadPart - m_qwLast;
		qwElapsed *= 1000000;
		qwElapsed /= m_qwFreq;

		if	( qwElapsed < m_qwMin )
			m_qwMin = qwElapsed;
		if	( qwElapsed > m_qwMax )
			m_qwMax = qwElapsed;
		m_qwSum += qwElapsed;

		int	ix = (int)( (qwElapsed+(TIME_RESOLUTION/2))/TIME_RESOLUTION );
		if	( ix < _countof( m_anFreq ) )
			m_anFreq[ix]++;

		m_nFreq++;
	}

	if	( m_iTimerKind == 1 )
		m_iCore = (int)GetCurrentProcessorNumber();
	else if	( m_iTimerKind == 2 )
		m_iCore = m_timer.GetCore( TID_COUNT_FREQ );

	m_qwLast = liCount.QuadPart;
}

void
CTPDlg::ShowFrequency( void )
{
	if	( !m_nFreq )
		return;

	CString	str;
	QWORD	qwAve = m_qwSum/m_nFreq;
	str.Format( _T("Min: %lld.%03lld / Ave: %lld.%03lld / Max: %lld.%03lld"),
			m_qwMin/1000, m_qwMin%1000, qwAve/1000, qwAve%1000, m_qwMax/1000, m_qwMax%1000 );
	if	( str != m_strFreq ){
		m_strFreq = str;
		GetDlgItem( IDC_STATIC_RESULT )->SetWindowText( str );
	}

	if	( m_bTesting ){
		str.Format( _T("Core %d"), m_iCore );
		GetDlgItem( IDC_STATIC_CORE )->SetWindowText( str );
	}

	CRect	rectGraph;
	m_wndGraph.GetClientRect( &rectGraph );
	int	yMax = rectGraph.Height();

	for	( int i = 0; i < _countof( m_anFreq ); i++ ){
		if	( m_anFreq[i] != m_anFreqLast[i] ){
			if	( m_anFreqLast[i] < yMax ){
				rectGraph.left  = i;
				rectGraph.right = i+1;
				m_wndGraph.InvalidateRect( &rectGraph, FALSE );
			}
			m_anFreqLast[i] = m_anFreq[i];
		}
	}
}

void
CTPDlg::CopyFrequency( void )
{
	if	( !m_nFreq )
		return;

	CStringA	str, strLines;

	// Write the result as the first line.

	{
		CString	strT;
		if	( m_iTimerKind == 1 ){
			str.Format( "SetTimer( %d )", m_nTime );
			strLines += str;
		}
		if	( m_iTimerKind == 2 ){
			str.Format( "SetWaitableTimer( %d )", m_nTime );
			strLines += str;
			strLines += " notified by ";
			CComboBox*	pCombo = (CComboBox*)GetDlgItem( IDC_COMBO_NOTIFY );
			pCombo->GetLBText( pCombo->GetCurSel(), strT );
			str = strT;
			strLines += str;
		}
		if	( m_nPeriod ){
			str.Format( "with timeBeginPeriod( %d )", m_nPeriod );
			strLines += "\t" + str;
		}
		if	( m_nMinTest ){
			strLines += "\t";
			strT.Format( _T("tested %d min."), m_nMinTest );
			str = strT;
			strLines += str;
		}
		strLines += "\t";
		GetDlgItem( IDC_STATIC_RESULT )->GetWindowText( strT );
		str = strT;
		strLines += str + "\t";

		str.Format( "CPU: %lld.%03lld%%", m_qwRate/1000, m_qwRate%1000 );
		strLines += str + "\r\n";
	}

	// Write the data for a scatter chart in Excel.

	int	n = _countof( m_anFreq );
	for	( INT_PTR i = 0; i <= _countof( m_anFreq ); i++ )
		if	( m_anFreq[i] ){
			str.Format( "%lld\t%d\r\n", i, m_anFreq[i] );
			strLines += str;
		}
	strLines += "\r\n";

	// Copy them into the clipboard.

	if	( OpenClipboard() ){
		EmptyClipboard();
		HGLOBAL	hMem = GlobalAlloc( GMEM_MOVEABLE, strLines.GetLength() );
		if	( hMem ){
			void*	pMem = GlobalLock( hMem );
			if	( pMem )
				memcpy( pMem, strLines.GetBuffer(), strLines.GetLength() );
			GlobalUnlock( hMem );
			SetClipboardData( CF_TEXT, hMem );
		}
		CloseClipboard();
	}
}

void
CTPDlg::PasteFrequency( void )
{
	if	( OpenClipboard() ){
		HGLOBAL	hMem = GetClipboardData( CF_TEXT );
		if	( hMem ){
			ClearFrequency();

			void*	pMem = GlobalLock( hMem );
			CString	strLines( (char*)pMem );

			QWORD	qwAve = 0;
			int	y = 0;
			CString	str, strLine = strLines.Tokenize( _T("\r\n"), y );
			do{
				int	x = 0;
				str = strLine.Tokenize( _T("\t"), x );
				if	( str.Left( 9 ) == _T("SetTimer(") ){
					m_iTimerKind = 1;
					str.Delete( 0, 9 );
				}
				else if	( str.Left( 17 ) == _T("SetWaitableTimer(") ){
					m_iTimerKind = 2;
					str.Delete( 0, 17 );
				}
				else
					break;
				m_nTime = atoi( str.GetBuffer() );
				CString	strValue;
				strValue.Format( _T("%d"), m_nTime );
				GetDlgItem( IDC_EDIT_TIME )->SetWindowText( strValue );
				if	( m_iTimerKind == 1 ){
					((CButton*)GetDlgItem( IDC_RADIO_SETTIMER )      )->SetCheck( BST_CHECKED );
					((CButton*)GetDlgItem( IDC_RADIO_WAITABLETIMER ) )->SetCheck( BST_UNCHECKED );
					GetDlgItem( IDC_STATIC_NOTIFY )->EnableWindow( FALSE );
					GetDlgItem( IDC_COMBO_NOTIFY  )->EnableWindow( FALSE );
				}
				else if	( m_iTimerKind == 2 ){
					((CButton*)GetDlgItem( IDC_RADIO_SETTIMER )      )->SetCheck( BST_UNCHECKED );
					((CButton*)GetDlgItem( IDC_RADIO_WAITABLETIMER ) )->SetCheck( BST_CHECKED );
					GetDlgItem( IDC_STATIC_NOTIFY )->EnableWindow( TRUE );
					GetDlgItem( IDC_COMBO_NOTIFY  )->EnableWindow( TRUE );

					CComboBox*	pCombo = (CComboBox*)GetDlgItem( IDC_COMBO_NOTIFY );
					int	i = str.Find( _T("notified by ") );
					if	( i < 0 )
						break;
					str.Delete( 0, i+12 );
					if	( str == _T("WM_TIMER") )
						pCombo->SetCurSel( 0 );
					else if	( str == _T("WM_APP") )
						pCombo->SetCurSel( 1 );
					else if	( str == _T("Callback") )
						pCombo->SetCurSel( 2 );
					else
						break;
				}

				str = strLine.Tokenize( _T("\t"), x );

				if	( str.Find( _T("with timeBeginPeriod(") ) >= 0 ){
					str.Delete( 0, 21 );
					m_nPeriod = atoi( str.GetBuffer() );
					str = strLine.Tokenize( _T("\t"), x );
				}
				else
					m_nPeriod = 0;
				strValue.Format( _T("%d"), m_nPeriod );
				GetDlgItem( IDC_EDIT_PERIOD )->SetWindowText( strValue );

				if	( str.Left( 7 ) == _T("tested ") ){
					str.Delete( 0, 7 );
					m_nMinTest = atoi( str.GetBuffer() );
					str = strLine.Tokenize( _T("\t"), x );
				}
				strValue.Format( _T("%d"), m_nMinTest );
				GetDlgItem( IDC_EDIT_TEST )->SetWindowText( strValue );

				if	( TakeValue( str, _T("Min: "), m_qwMin ) &&
					  TakeValue( str, _T("Ave: "),   qwAve ) &&
					  TakeValue( str, _T("Max: "), m_qwMax ) ){
					GetDlgItem( IDC_STATIC_RESULT )->SetWindowText( str );

				}
				else
					break;

				str = strLine.Tokenize( _T("\t"), x );
				if	( TakeValue( str, _T("CPU: "), m_qwRate ) ){
					GetDlgItem( IDC_STATIC_CORE )->SetWindowText( str );
					str.Empty();
				}
			}while	( false );

			if	( !str.IsEmpty() ){
				CString	strError;
				strError.Format( _T("Unknown word '%s'"), str.GetBuffer() );
				MessageBox( strError, _T("Paste error") );
			}
			else{
				m_nFreq = 0;
				while	( true ){
					strLine = strLines.Tokenize( _T("\r\n"), y );
					if	( strLine.IsEmpty() )
						break;
					int	x = 0;
					str = strLine.Tokenize( _T("\t"), x );
					int	i = atoi( str.GetBuffer() );
					str = strLine.Tokenize( _T("\t"), x );
					m_anFreq[i] = atoi( str.GetBuffer() );
					m_nFreq += m_anFreq[i];
				}
				m_qwSum = qwAve*m_nFreq;
				m_wndGraph.Invalidate( TRUE );
			}

			GlobalUnlock( hMem );
		}
		CloseClipboard();
	}
}

bool
CTPDlg::TakeValue( CString strLine, CString strTag, QWORD& qwValue )
{
	int	x = strLine.Find( strTag );
	if	( x < 0 )
		return	false;

	strLine.Delete( 0, x + strTag.GetLength() );
	qwValue = atoi( strLine );

	x = strLine.Find( '.' );
	strLine.Delete( 0, x+1 );
	qwValue *= 1000;
	qwValue += atoi( strLine.GetBuffer() );
	
	return	true;
}

void
CTPDlg::OnTimerCall( HWND hWnd, UINT uMsg, UINT_PTR nIDEvent, DWORD uElapse )
{
	CTPDlg*	pThis = (CTPDlg*)theApp.m_pMainWnd;
	pThis->OnTimer( nIDEvent );
}
