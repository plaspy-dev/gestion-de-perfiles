#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "arrayusuarios.h"
# include "resource.h"
// CFormUser form view

class CFormUser : public CFormView
{
	DECLARE_DYNCREATE(CFormUser)

	CStringArray	m_saMetodos;
	CStringArray	m_saGrupos;
	CStringArray	m_saDescripciones;
	CUIntArray		m_uaIDS;
	CArrayUsuarios	m_pUsuarios;
	CConector		*m_pConector;
	DWORD			m_dwCurrentID;
	void			AddLista(CUsuario *Usuario);
	int				m_nCurrentGroup;
	void			FillLista(int grupo);
	void			Editar(CUsuario *usuario, int grupo=0);
	void			CambiarContrasena(CString usuario, CString newpasswd);
	void			EliminarUsuario(CString usuario);
	void			AgregarUsuario(CUsuario *usuario, CString passwd);
protected:
	CFormUser();           // protected constructor used by dynamic creation
	virtual ~CFormUser();

public:
	enum { IDD = IDD_FORMVIEWUSER };

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()

public:
	CMyListCtrl	m_LCGrupos;
	CMyListCtrl	m_LCUser;
	CButtonXP	m_BNuevo;
	afx_msg void OnBnClickedButtonnuevo();
	afx_msg void OnVer();
	afx_msg void OnEditar();
	afx_msg void OnContrasena();
	afx_msg void OnEliminar();
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	virtual void OnInitialUpdate();
	afx_msg void OnNMDblclkListgrupos(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkListusuarios(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRclickListusuarios(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnNMRclickListgrupos(NMHDR *pNMHDR, LRESULT *pResult);
protected:
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
};


