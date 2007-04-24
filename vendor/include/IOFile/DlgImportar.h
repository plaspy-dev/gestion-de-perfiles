#pragma once
#include "afxwin.h"
#include "afxcmn.h"
# include "import.h"
# include "ImportThread.h"
# include "resource.h"

// CDlgImportar dialog

class CDlgImportar : public CDialog
{
	DECLARE_DYNAMIC(CDlgImportar)
	ULONG		m_nTotal;
	int			m_nCurrent;
	ULONG		m_nCompletado;
public:
	CDlgImportar(CWnd* pParent = NULL);   // standard constructor
	virtual	~CDlgImportar();
	CImport			*m_pImport;
// Dialog Data
	enum { IDD = IDD_DIALOGIMPORTAR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
public:
	CImportThread *m_pImportThread;
	CStatic m_SAyuda;
	CProgressCtrl m_PCStatus;
	CButtonXP m_BCancel;
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	virtual BOOL OnInitDialog();
};
