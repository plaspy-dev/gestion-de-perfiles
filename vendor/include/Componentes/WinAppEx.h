#pragma once


class __COMPONENTES_EXPORT__ CWinAppEx : public CWinApp
{
	// solo por compatibilidad con versiones anteriores
	LPARAM	m_mlparamold;

	DECLARE_DYNCREATE( CWinAppEx )

protected:
	CWinAppEx( LPCTSTR lpszAppName = NULL );
	virtual ~CWinAppEx( void );
	
public:	
	BOOL PostInstanceMessage( WPARAM wParam, LPARAM lParam );		
	BOOL EnableTokenPrivilege( LPCTSTR lpszSystemName, BOOL bEnable = TRUE );
	
public:
	AFX_INLINE void RegisterShellFileTypes( BOOL bCompat = FALSE );
	AFX_INLINE void UnregisterShellFileTypes( void );

public:
	virtual BOOL InitInstance( LPCTSTR lpszUID = NULL );
	virtual BOOL PreTranslateMessage( LPMSG pMsg );
	virtual BOOL OnAnotherInstanceMessage( LPMSG pMsg) ;
	afx_msg void OnFilePrintSetup();

protected:
	BOOL FindAnotherInstance( LPCTSTR lpszUID );
	void RegisterShellFileTypesEx( BOOL bCompat = FALSE, BOOL bRegister = TRUE );
	virtual void OnCommandLine(CString szCommandLine);
	virtual void OnShowApp();
			
protected:
	DECLARE_MESSAGE_MAP()

protected:	
	UINT	m_uMsgCheckInst;
};

AFX_INLINE CWinAppEx* GetApp( void )
	{ return STATIC_DOWNCAST( CWinAppEx, AfxGetApp() ); }

AFX_INLINE void CWinAppEx::RegisterShellFileTypes( BOOL bCompat /*FALSE*/ )
	{ RegisterShellFileTypesEx( bCompat, TRUE ); }

AFX_INLINE void CWinAppEx::UnregisterShellFileTypes( void) 
	{ RegisterShellFileTypesEx( FALSE, FALSE ); }
