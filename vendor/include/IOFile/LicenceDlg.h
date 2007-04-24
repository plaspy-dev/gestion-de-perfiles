#pragma once
#include "afxwin.h"
# include "componentes/newmenu/dialogxp.h"
# include "componentes/buttonxp.h"
# include "componentes/editxp.h"

// Cuadro de diálogo de CLicenceDlg

class CLicenceDlg : public CDialogXP
{
	DECLARE_DYNAMIC(CLicenceDlg)

public:
	CLicenceDlg(CWnd* pParent = NULL);   // Constructor estándar
	virtual ~CLicenceDlg();
	CStringArray		m_saSeriales;
	int					m_nCurrentPos;

// Datos del cuadro de diálogo
	enum { IDD = IDD_DLICENCIA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Compatibilidad con DDX o DDV

	DECLARE_MESSAGE_MAP()
public:
	CEditXP m_ESemilla;
	CEditXP m_ESerial;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CButtonXP m_BAceptar;
	CButtonXP m_BCancelar;
	virtual BOOL OnInitDialog();
public:
	CButtonXP m_BProbar;
public:
	CStatic m_STitulo;
public:
	afx_msg void OnBnClickedBprobar();
public:
	CHyperLink m_SLicencia;
public:
	CBitmapPicture m_SLogo;
public:
	CButtonXP m_BNext;
public:
	afx_msg void OnBnClickedBnext();
};
