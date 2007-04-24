#pragma once
#include "afxwin.h"
# include <Componentes/Componentes.h>
# include "resource.h"

// CDlgWebContrasena dialog

class CDlgWebContrasena : public CDialog
{
	DECLARE_DYNAMIC(CDlgWebContrasena)
	CString m_szUsuario;
	CString m_szServidor;
	CString m_szID;
public:
	CDlgWebContrasena(const CString &servidor, const CString &usuario, const CString &id, CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgWebContrasena();

// Dialog Data
	enum { IDD = IDD_DWEBPASSWORD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnBnClickedCancel();
public:
	CHTMLCtrl m_EWebBrowser;
public:
	virtual BOOL OnInitDialog();
protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
};
