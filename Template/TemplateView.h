// TemplateView.h: interfaz de la clase CTemplateView
//


#pragma once
#include "afxwin.h"
#include "afxdtctl.h"


class CTemplateView : public CFormView
{
protected: // Crear sólo a partir de serialización
	CTemplateView();
	DECLARE_DYNCREATE(CTemplateView)
	CConector *m_pConector;
	LRESULT OnMySQLMessage(WPARAM wParam, LPARAM lParam);
	CString		m_szOldPKZ;

public:
	enum{ IDD = IDD_TEMPLATE_FORM };
	enum {
		query_pkz=1,
	};

// Operaciones
public:

// Reemplazos
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Compatibilidad con DDX/DDV
	virtual void OnInitialUpdate(); // Se llama la primera vez después de la construcción

// Implementación
public:
	virtual ~CTemplateView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Funciones de asignación de mensajes generadas
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBaceptar();
	afx_msg void OnBnClickedBcancelar();
	CDateTimeCtrl m_DTNacimiento;

	CEditXP m_EPKZ;
	CEditXP m_EEncabezamiento;
	CEditXP m_ETitulo;
	CEditXP m_ENombre;
	CEditXP m_EApellido;
	CEditXP m_ESApellido;
	CComboBoxXP m_CBSexo;
	CComboBoxXP m_CBCorrespondencia;
	afx_msg void OnEnKillfocusEpkz();
	void Clean();
	CButtonXP m_BCancelar;
	afx_msg void OnDestroy();
public:
	CEditXP m_ENacionalidad;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	LRESULT OnInsertPKZ(WPARAM wParam, LPARAM lParam);
};


