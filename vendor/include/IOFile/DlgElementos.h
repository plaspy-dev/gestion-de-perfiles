#pragma once
#include "afxwin.h"
#include "afxcmn.h"
# include "resource.h"

// CDlgElementos dialog

class  __IOFILE_EXPORT__ CDlgElementos : public CDialog
{
	DECLARE_DYNAMIC(CDlgElementos)

public:
	CDlgElementos(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgElementos();

// Dialog Data
	enum { IDD = IDD_DIALOGELEMENTOS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CStringArray	*m_szValores;
	CButtonXP		m_BOK;
	CButtonXP		m_BCancelar;
	CEditXP			m_EElementos;
	CString			m_szFinal;
	CStringArray	*m_szDescripciones;
	static bool		IsValidData(CString data, CStringArray *valores);
	static bool		Validate(CString data,CString &Final, CStringArray *valores, int &inicio, int &fin);
	CString			m_szTitulo;
	void				CheckAll(CString valor);
protected:
	virtual void OnOK();
public:
	virtual BOOL OnInitDialog();
	CMyListCtrl m_LCElementos;
	void Agregar(CString nuevo);
	void OnAgregar();
	void OnEliminar();
	void OnSelAll();
	bool Findtext(CString text, CString find, int &inicio, int &fin);
	afx_msg void OnNMDblclkListelemento(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRclickListelemento(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMLClickListElemento(NMHDR *pNMHDR, LRESULT *pResult);
};
