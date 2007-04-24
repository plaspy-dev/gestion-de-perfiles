#pragma once

# include "Tools.h"
# include "editxp.h"
# include "ListBoxXP.h"

class CComboBoxXP;

//***************************************************************************
//
// AUTHOR:  JUHU
//
// DESCRIPTION: 
//
// ORIGINAL: 
//
//***************************************************************************
class CComboEditXP : public CEditXP
{
// Operations
public:
	//***********************************************************************
	// Name:		GetComboBox
	// Description:	Default constructor.
	// Parameters:	None.
	// Return:		CComboBoxXP&.	
	// Notes:		none
	//***********************************************************************
    CComboBoxXP& GetComboBox ();

protected:
    afx_msg void OnKillFocus(CWnd* pNewWnd);
    afx_msg void OnMouseMove(UINT, CPoint);
    afx_msg void OnMouseOut();

    
public:
	afx_msg void OnEnChange();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	DECLARE_MESSAGE_MAP()
};

//***************************************************************************
//
// AUTHOR:  JUHU
//
// DESCRIPTION: 
//
// ORIGINAL: 
//
//***************************************************************************
class  __COMPONENTES_EXPORT__ CComboBoxXP : public CComboBox
{
	DECLARE_DYNCREATE(CComboBoxXP);
public:
	//***********************************************************************
	// Name:		CComboBoxXP
	// Description:	Default constructor.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		
	//***********************************************************************
    CComboBoxXP ();

// Attributes
protected:
	CXWinVersion	m_WVersion;
    int		m_nVisibleLines;
    int		m_nDefaultHeight;
    int		m_nEditHeight;
	bool	m_bValidateScroll;
	bool	m_bRealAutocomplete;
	bool	m_bAutoComplete;
	bool	m_bShowCombo;
	bool	m_bEnable;
	int		m_nSelOK;
    CComboEditXP m_Edit;
    CMouseMgr	m_MouseMgr;
	CXInfoTip	m_pToolTip;
	CListBoxXP	m_listbox;

	// nuevo con la barra de progreso
	CTextProgressCtrl m_wndTextProgress;
	int		m_nMax;
	int		m_nCurrent;
	int		m_nStep;

// Operations
public:
	//***********************************************************************
	// Name:		Create
	// Description:	TODO.
	// Parameters:	DWORD, const RECT&, CWnd* pParentWnd, UINT nID.
	// Return:		BOOL.	
	// Notes:		none.
	//***********************************************************************
    BOOL Create (DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
	//***********************************************************************
	// Name:		IsWindowEnabled
	// Description:	Esta funcion devuelve true si el comboBox esta activo.
	// Parameters:	none.
	// Return:		BOOL.	
	// Notes:		none.
	//***********************************************************************
    BOOL IsWindowEnabled();
	//***********************************************************************
	// Name:		SetVisibleLines
	// Description:	Resume las lineas mostradas al usuario, es util cuando el 
	//				comboBox tiene mas lineas de las que puede mostrar o 
	//				cuando se necesita variar el valor por defecto.
	// Parameters:	none.
	// Return:		void.	
	// Notes:		none.
	//***********************************************************************
    void SetVisibleLines (int nLines);
	//***********************************************************************
	// Name:		ValidateScroll
	// Description:	Permite saltar al usuario entre controles cuando el 
	//				tamaño de la ventana no es suficiente para mostrar todo
	//				el formulario.
	// Parameters:	bool.
	// Return:		void.	
	// Notes:		el parametro por defecto tiene el valor true.
	//***********************************************************************
	void ValidateScroll(bool validar=true);
	//***********************************************************************
	// Name:		EnableWindow
	// Description:	Habilita o desabilita el control para edicion del usuario.
	// Parameters:	bool.
	// Return:		bool.	
	// Notes:		el parametro por defecto tiene el valor true.
	//***********************************************************************
	BOOL EnableWindow(BOOL bEnable=TRUE);
	//***********************************************************************
	// Name:		SetAutoComplete
	// Description:	Cuando el usuario puede editar el comboBox se hablita el
	//				autoCompletar
	// Parameters:	bool.
	// Return:		void.	
	// Notes:		none
	//***********************************************************************
	void	SetAutoComplete(bool bAutocompletar);
	//***********************************************************************
	// Name:		SetSelData
	// Description:	Devuelve true si se encuetra la data solicitada.
	// Parameters:	DWORD, bool(true).
	// Return:		bool.	
	// Notes:		Compatibilidad.
	//***********************************************************************
	bool	SetSelData(DWORD data, bool first= true);
	//***********************************************************************
	// Obsoleta usar GetCurSel(); o SelectedIndex;
	// Name:		GetCurSelEx
	// Description:	Obtiene el index de la seleccion actual.
	// Parameters:	void.
	// Return:		int.	
	// Notes:		
	//***********************************************************************
	int		GetCurSelEx();
	//***********************************************************************
	// Name:		SetSelDataEx
	// Description:	Devuelve la pocision en donde se encuetra la data solicitada.
	// Parameters:	void.
	// Return:		int.	
	// Notes:		
	//***********************************************************************
	int		SetSelDataEx(DWORD data, bool first= true);
	//***********************************************************************
	// Name:		LoadDataFromXML
	// Description:	Carga la informacion de un archivo XML.
	// Parameters:	CString, bool.
	// Return:		void.	
	// Notes:		el parametro coincide un alguna direccion en el sis. de
	//				archivos, es posible mantener la informacion actual o limiar
	//				(reset) la informacion que contiene el comboBox
	//***********************************************************************
	void	LoadDataFromXML(CString Archivo, bool reset=true);
	//***********************************************************************
	// Name:		LoadDataFromXMLEx
	// Description:	Carga la informacion de un archivo XML.
	// Parameters:	CString, bool.
	// Return:		void.	
	// Notes:		Otro fomato a (LoadDataFromXML)
	//***********************************************************************
	void	LoadDataFromXMLEx( CString Archivo);
	//***********************************************************************
	// Name:		LoadDataFromXML
	// Description:	Carga la informacion de un archivo XML.
	// Parameters:	CString, bool.
	// Return:		void.	
	// Notes:		Otros indices dentro del archivo XML.
	//***********************************************************************
	void	LoadDataFromXML( CString Archivo, CString Atributo, CString Valor, bool esacto=true);
	//***********************************************************************
	// Name:		*GetEdit()
	// Description:	Obtiene el edit del comboBox .
	// Parameters:	void.
	// Return:		CComboEditXP.	
	// Notes:		Util para saber si el comboBox tiene el foco.
	//***********************************************************************
	CComboEditXP *GetEdit() {return &m_Edit;};

	// funciones del progress control
	void SetRange(int minimo, int maximo);
	void SetPos(int pos);
	void SetStep(int step);
	int Step();
	int	GetPos() const;
	int GetMaximo() const;
	CTextProgressCtrl *GetProgressCtrl();
	void HideProgress();
	void ResetContent(bool cleantext=true);
	void UpdateSeleccion();

// Implementation
public:
    virtual void PreSubclassWindow();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
	void SetWindowText(LPCTSTR lpszString);

protected:
	afx_msg void OnCbnEditupdate();
	afx_msg virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
    afx_msg UINT OnGetDlgCode();
    afx_msg void OnWindowPosChanging(WINDOWPOS*);
    afx_msg void OnWindowPosChanged(WINDOWPOS*);
    afx_msg void OnPaint();
    afx_msg void OnSetFocus(CWnd* pOldWnd);
    afx_msg void OnKillFocus(CWnd* pNewWnd);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnMouseOut();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
    
public:
	// Text
	__declspec(property(get=GetText,put=SetText))		CString	Text;
	void	SetText(const CString &texto);
	CString	GetText();

	// Mensaje
	__declspec(property(get=GetMensaje,put=SetMensaje))		CString	Mensaje;
	void	SetMensaje(const CString &mensaje);
	CString  GetMensaje();

	// SelectedIndex
	__declspec(property(get=GetSelectedIndex,put=SetSelectedIndex))						int	SelectedIndex;
	void	SetSelectedIndex(int index);
	int GetSelectedIndex();

	// SelectedData
	__declspec(property(get=GetSelectedData,put=SetSelectedData))						DWORD	SelectedData;
	void	SetSelectedData(DWORD data);
	DWORD GetSelectedData();

	// texto de la barra
	__declspec(property(get=GetProgressText,put=SetProgressText))		CString	ProgressText;
	void	SetProgressText(const CString &mensaje);
	CString  GetProgressText();

	friend CComboEditXP;
	DECLARE_MESSAGE_MAP()
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
public:
	virtual BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult);
};


///////////////////////////////////////////////////////////////////////////////
inline CComboBoxXP& CComboEditXP::GetComboBox ()
{
    return *((CComboBoxXP*)GetParent());
}
