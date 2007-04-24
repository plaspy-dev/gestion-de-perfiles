#pragma once
# include "resource.h"
#include "newmenu/dialogxp.h"
# include "HyperLink.h"
# include "KCSideBannerWnd.h"
#include "afxwin.h"

class CAboutDlg : public CDialogXP
{
protected:
	CString m_szVersion;
	CString m_szProducto;
	HICON	m_hIcon;
public:
	CAboutDlg(const CString &szVersion,HICON icono,const CString &producto="");

	// Datos del cuadro de diálogo
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Compatibilidad con DDX/DDV
public:
	CHyperLink m_SLink;
	CStatic		m_SVersion;
	virtual BOOL OnInitDialog();
	// Implementación
protected:
	DECLARE_MESSAGE_MAP()
public:
	CKCSideBannerWnd m_banner;
public:
	CStatic m_SPrograma;
	CStatic m_SIcono;
};