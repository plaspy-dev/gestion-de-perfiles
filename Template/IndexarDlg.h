#pragma once
#include "afxcmn.h"


// CIndexarDlg dialog

class CIndexarDlg : public CDialog
{
	DECLARE_DYNAMIC(CIndexarDlg)
	CConector	*m_pConector;
	int			m_nTotal;
	bool		m_bFirst;
	int			m_nMaximo;
	int			m_nCurrent;
	CString		m_szPKZ;
	enum {
		query_tablas=1,
		query_resultados,
	};

protected:
	static CIndexarDlg	*m_pDlg;
	CIndexarDlg(const CString pkz);   // standard constructor
public:
	CIndexarDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CIndexarDlg();

// Dialog Data
	enum { IDD = IDD_INDEXARDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CTextProgressCtrl m_PCProgreso;
	static void SetPKZ(const CString &pkz);
public:
	virtual BOOL OnInitDialog();
	LRESULT OnMySQLMessage(WPARAM wParam, LPARAM lParam);
};
