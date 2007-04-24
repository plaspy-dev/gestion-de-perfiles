#pragma once
#include "afxwin.h"
#include "afxcmn.h"
# include <JPrint/JPrint.h>



// CBusquedaView form view

class CBusquedaView : public CFormView
{
	DECLARE_DYNCREATE(CBusquedaView)
	CConector *m_pConector;
	CListDemoViewPrint *m_pListPrint;

protected:
	CBusquedaView();
	virtual ~CBusquedaView();

public:
	enum { IDD = IDD_BUSQUEDAVIEW };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()
public:
	CEditXP m_ENombre;
public:
	CButtonXP m_BBuscar;
public:
	CMyListCtrl m_LCBuscar;
public:
	afx_msg void OnBnClickedBbuscar();
public:
	virtual void OnInitialUpdate();
	LRESULT OnMySQLMessage(WPARAM wParam ,LPARAM lParam);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
public:
	afx_msg void OnNMDblclkLresultados(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnNMRclickLresultados(NMHDR *pNMHDR, LRESULT *pResult);
	void OnVer();
	void OnGuardar();
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
};


