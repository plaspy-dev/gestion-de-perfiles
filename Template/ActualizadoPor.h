#pragma once
#include <componentes/Componentes.h>
# include "resource.h"

class CActualizadoPor : public CDialogXP
{
	DECLARE_DYNAMIC(CActualizadoPor)
public:
	CActualizadoPor(CWnd* pParent = NULL);   // standard constructor
	virtual ~CActualizadoPor();
	enum { IDD = IDD_DACTUALIZADOPOR};
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
public:
	CString	m_szUsuario;
	virtual BOOL OnInitDialog();
	CEditXP		m_EActualizado;
	CButtonXP	m_BAceptar;
	void OnBnClickedAceptar();
	void OnBnClickedCancelar();
};
