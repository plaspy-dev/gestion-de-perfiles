#pragma once
#include "afxwin.h"
#include "afxcmn.h"
# include "Export.h"
# include "resource.h"

// CDlgExportar dialog

class CDlgExportar : public CDialog
{
	DECLARE_DYNAMIC(CDlgExportar)
public:
	CDlgExportar(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgExportar();
// Dialog Data
	enum { IDD = IDD_DIALOGEXPORT };
	CExport		*m_pExport;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
	ULONG			m_nTotal;
	ULONG			m_nRealizados;

public:
	CStringArray	*m_szQuery;
	int				m_nCurrent;
	CStringArray	*m_szNombre;

	CStatic m_SAyuda;
	CProgressCtrl m_PCEstatus;
	CButtonXP m_BCancel;
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	virtual BOOL OnInitDialog();
};
