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

	enum {
		query_areas=1,
		query_resultados,
		query_paises,
		query_ciudades,
		query_universidades,
		query_tipos,
		query_todos,
	};
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
	CEditXP m_EBusqueda;
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
public:
	CComboBoxXP m_CBArea;
public:
	CButtonXP m_BIndexar;
public:
	afx_msg void OnBnClickedBindexar();
public:
	CSectionSeparator	m_SAvanzado;
	CSectionSeparator	m_SBusqueda;
	CStaticSeparator	m_SFin;
public:
	CEditXP m_ENombre;
public:
	CEditXP m_EApellido;
public:
	CComboBoxXP m_CBPais;
public:
	CComboBoxXP m_CBCiudad;
public:
	CEditXP m_EInstitucion;
public:
	CComboBoxXP m_CBUniversidad;
	CComboBoxXP m_CBTipo;
	CButton		m_CTodos;
	CButton		m_CTodosA;
public:
	afx_msg void OnStnClickedSavanzado();
	afx_msg void OnStnClickedBusqueda();
	void OnBuscar(bool avanzado, bool todo=false);
public:
	afx_msg void OnDestroy();
	afx_msg void OnChangeTodos();
	afx_msg void OnChangeTodosA();
	CString GetCampoReal(CString campo);
public:
	afx_msg void OnBnClickedBcancelar();
public:
	CButtonXP m_BCancelar;
public:
	CButtonXP m_BCancelar2;
	void OnVerResidencia();
	void OnVerOficina();
	void OnVerEducacion();
	void OnVerSeguimiento();
	void OnVerGestion();
};


