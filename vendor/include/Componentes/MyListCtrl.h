#pragma once

# include "newmenu/newmenu.h"
#include "ComboBoxXP.h"
# include "MasEdit.h"
#include "SortHeaderCtrl.h"
//***************************************************************************
//
// AUTHOR:  JUHU
//
// DESCRIPTION: Una lista de control avanzada.
//
// ORIGINAL: 
//
//***************************************************************************

typedef class CListCtrlXP CMyListCtrl;

class  __COMPONENTES_EXPORT__ CListCtrlXP : public CListCtrl
{
	DECLARE_DYNCREATE(CListCtrlXP);
	// Construction
public:
	//***********************************************************************
	// Name:		CListCtrlXP
	// Description:	Constructor.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	CListCtrlXP();
	//***********************************************************************
	// Name:		CListCtrlXP
	// Description:	Destructor.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	virtual ~CListCtrlXP();

// Attributes
public:
	enum {
		tipocol_texto=0,
		tipocol_numero=1,
		tipocol_double=2,
		tipocol_fechahora=3,
		tipocol_hora=5
	};
	enum {
		tipo_edit_combo=1,
		tipo_edit_edit,
		tipo_edit_datetime,
	};

protected:
	CXWinVersion		m_WVersion;
	CPtrArray				m_pArrayControles;
	CUIntArray				m_pArrayColumnas;
	CNewMenu				*m_hMenuContextual;
	bool					m_bDropDown;
	int		m_nMaxRow;
	int		m_nMaxCol;
	int		m_MaxRowsPerView;
	bool	m_bOrdenable;
	int		m_nNoSort;
	bool	m_bEditable;
	bool	m_bScrolling;
	bool	m_bCanClean;
	bool	m_bCanShowMenu;
	CUIntArray				m_nColumType;
	int		m_nCol;
	CPoint	m_pPoint;
	int		m_nRow;
	CWnd	*m_pParent;
	CString		m_szVacia;
	CSortHeaderCtrl *m_header;

// Operations
public:
	//***********************************************************************
	// Name:		SetSelectedIndex
	// Description:	Coloca el foco en el elemento seleccionado.
	// Parameters:	int index.
	// Return:		void.	
	// Notes:		None.
	//***********************************************************************
	void	SetSelectedIndex(int index);
	//***********************************************************************
	// Name:		SetText
	// Description:	Coloca el texto en una ubicacion especifica.
	// Parameters:	int row, int col,const CString &mensaje.
	// Return:		void.	
	// Notes:		None.
	//***********************************************************************
	void	SetText(int row, int col,const CString &mensaje);
	//***********************************************************************
	// Name:		SetMessageLista
	// Description:	Mensaje por defecto cuando la lista esta vacia.
	// Parameters:	const CString &message.
	// Return:		void.	
	// Notes:		None.
	//***********************************************************************
	void	SetMessageLista(const CString &message);
	//***********************************************************************
	// Name:		ResetTipos
	// Description:	Deja en default (texto) todos los tipos de la lista.
	// Parameters:	None.
	// Return:		void.	
	// Notes:		None.
	//***********************************************************************
	void	ResetTipos();
	//***********************************************************************
	// Name:		SetType	
	// Description:	Define el tipo de control(fecha, numero , texto, etc).
	// Parameters:	int columna, int tipo.
	// Return:		void.	
	// Notes:		None.
	//***********************************************************************
	void	SetType(int columna, int tipo);
	//***********************************************************************
	// Name:		SetCanClean
	// Description:	Boton derecho y limpiar todo.
	// Parameters:	bool canclean=true.
	// Return:		void.	
	// Notes:		None.
	//***********************************************************************
	void	SetCanClean(bool canclean=true);
	//***********************************************************************
	// Name:		SetNoSort
	// Description:	No ordene la lista.
	// Parameters:	int nosort=-1.
	// Return:		void.	
	// Notes:		None.
	//***********************************************************************
	void	SetNoSort( int nosort=-1) {
		m_nNoSort= nosort;
	};
	//***********************************************************************
	// Name:		SetParent
	// Description:	.
	// Parameters:	CWnd *parent.
	// Return:		void.	
	// Notes:		None.
	//***********************************************************************
	void	SetParent(CWnd *parent);
	//***********************************************************************
	// Name:		OnSetText
	// Description:	.
	// Parameters:	None.
	// Return:		void.	
	// Notes:		None.
	//***********************************************************************
	void	OnSetText(bool notify=true);
	//***********************************************************************
	// Name:		SetEditable
	// Description:	Permitir editar la lista.
	// Parameters:	bool editable=true.
	// Return:		void.	
	// Notes:		None.
	//***********************************************************************
	void	SetEditable(bool editable=true);
	//***********************************************************************
	// Name:		LoadFromXML
	// Description:	Carge una lista de un archivo XML.
	// Parameters:	CString Archivo.
	// Return:		void.	
	// Notes:		None.
	//***********************************************************************
	void	LoadFromXML( CString Archivo);
	//***********************************************************************
	// Name:		SetOrdenable
	// Description:	Haga que la lista este ordenada.
	// Parameters:	bool ordenable=true.
	// Return:		void.	
	// Notes:		None.
	//***********************************************************************
	void	SetOrdenable( bool ordenable=true);
	//***********************************************************************
	// Name:		SetDateTimePicker
	// Description:	Datetime picker para las listas.
	// Parameters:	unsigned int col.
	// Return:		void.	
	// Notes:		None.
	//***********************************************************************
	CDateTimeCtrl *SetDateTimePicker( unsigned int col, bool shownone=false);
	//***********************************************************************
	// Name:		SetCombo
	// Description:	Combo box para listas.
	// Parameters:	unsigned int col.
	// Return:		void.	
	// Notes:		None.
	//***********************************************************************
	CComboBoxXP	*SetCombo( unsigned int col);
	//***********************************************************************
	// Name:		SetEdit
	// Description:	Haga una columna editable.
	// Parameters:	unsigned int col.
	// Return:		void.	
	// Notes:		None.
	//***********************************************************************
	CEdit *SetEdit( unsigned int col, int tipo=CListCtrlXP::tipocol_texto);
	//***********************************************************************
	// Name:		EditCampo
	// Description:	Haga un campo editable.
	// Parameters:	unsigned int col, unsigned int row.
	// Return:		void.	
	// Notes:		None.
	//***********************************************************************
	void	EditCampo( unsigned int col, unsigned int row);
	//***********************************************************************
	// Name:		GetCurrentPos		
	// Description:	Obtenga la posicion actual.
	// Parameters:	int &col, int &row.
	// Return:		void.	
	// Notes:		None.
	//***********************************************************************
	void	GetCurrentPos(int &col, int &row);
	//***********************************************************************
	// Name:		StoreState
	// Description:	Guardar el tamaño de las columnas.
	// Parameters:	CString nombre.
	// Return:		void.	
	// Notes:		None.
	//***********************************************************************
	void	StoreState(CString nombre);
	//***********************************************************************
	// Name:		RestoreState
	// Description:	Restaurar valores iniciales.
	// Parameters:	CString nombre.
	// Return:		void.	
	// Notes:		None.
	//***********************************************************************
	void	RestoreState(CString nombre);
	//***********************************************************************
	// Name:		AutoSizeColumns
	// Description:	Haga que las columnas crescan deacuerdo con su contenido.
	// Parameters:	int  delta=0.
	// Return:		void.	
	// Notes:		None.
	//***********************************************************************
	void	AutoSizeColumns(int  delta=0);
	//***********************************************************************
	// Name:		ResetPos
	// Description:	Volver al inicio con el foco.
	// Parameters:	None.
	// Return:		void.	
	// Notes:		None.
	//***********************************************************************
	void	ResetPos();
	//***********************************************************************
	// Name:		OnDrawItem
	// Description:	.
	// Parameters:	int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct.
	// Return:		void.	
	// Notes:		None.
	//***********************************************************************
	void	OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	//***********************************************************************
	// Name:		RepaintSelectedItems
	// Description:	.
	// Parameters:	None.
	// Return:		void.	
	// Notes:		None.
	//***********************************************************************
	void	RepaintSelectedItems();
	//***********************************************************************
	// Name:		SetCanShowMenu
	// Description:	Mostrar el menu boton derecho.
	// Parameters:	bool can=true.
	// Return:		bool.	
	// Notes:		Obsoleta.
	//***********************************************************************
	bool	SetCanShowMenu(bool can=true);
	//***********************************************************************
	// Name:		CopyToClipBoard
	// Description:	Copia el contenido de la lista al ClipBoard (Seleccionado).
	// Parameters:	CString &valor.
	// Return:		bool.	
	// Notes:		None.
	//***********************************************************************
	bool	CopyToClipBoard(CString &valor);
	//***********************************************************************
	// Name:		GetSelectedIndex
	// Description:	Devuelve la posicion del item seleccionado.
	// Parameters:	None.
	// Return:		int.	
	// Notes:		None.
	//***********************************************************************
	int		GetSelectedIndex();
	//***********************************************************************
	// Name:		GetColumnType
	// Description:	Devuelve el entero correspondiente con el tipo de columna.
	// Parameters:	int columna.
	// Return:		int.	
	// Notes:		tipocol_numero=1,
	//				tipocol_double=2,
	//				tipocol_fechahora=3,
	//				tipocol_hora=5.
	//***********************************************************************
	int		GetColumnType(int columna);
	//***********************************************************************
	// Name:		GetTipoControl
	// Description:	Devuelve el tipo de control que esta dentro de la lista.
	// Parameters:	None.
	// Return:		int.	
	// Notes:		None.
	//***********************************************************************
	int		GetTipoControl(unsigned int col);
	//***********************************************************************
	// Name:		AddColumn
	// Description:	Añade una columna nueva.
	// Parameters:	CString texto, int tipo=0.
	// Return:		int.	
	// Notes:		None.
	//***********************************************************************
	int		AddColumn(CString texto, int tipo=CListCtrlXP::tipocol_texto);
	//***********************************************************************
	// Name:		GetColumnCount
	// Description:	Obtiene el numero de columnas.
	// Parameters:	None.
	// Return:		int.	
	// Notes:		None.
	//***********************************************************************
	int		GetColumnCount() const;
	//***********************************************************************
	// Name:		GetNoSort
	// Description:	Obtiene el estado de Sort.
	// Parameters:	None.
	// Return:		int.	
	// Notes:		None.
	//***********************************************************************
	int		GetNoSort();
	//***********************************************************************
	// Name:		AddItem
	// Description:	Añade un item DAh. 
	// Parameters:	LPCTSTR lpszItem.
	// Return:		int.	
	// Notes:		None.
	//***********************************************************************
	int		AddItem(LPCTSTR lpszItem);
	//***********************************************************************
	// Name:		InsertItem
	// Description:	DAh.
	// Parameters:	int nItem,LPCTSTR lpszItem.
	// Return:		int.	
	// Notes:		None.
	//***********************************************************************
	int		InsertItem(int nItem,LPCTSTR lpszItem);
	//***********************************************************************
	// Name:		GetText
	// Description:	Obtiene el texto de un item en particular.
	// Parameters:	int row, int col.
	// Return:		CString.	
	// Notes:		None.
	//***********************************************************************
	CString GetText(int row, int col);
	//***********************************************************************
	// Name:		GetMensaje
	// Description:	Obtiene el mensaje cuando la lista esta vacia.
	// Parameters:	None.
	// Return:		CString.	
	// Notes:		None.
	//***********************************************************************
	CString GetMensaje();
	CWnd	*GetControl( int col);
	CNewMenu *GetMenuContextual();
	CNewMenu *GetMenuContextual(CNewMenu *menu);
	virtual int		InsertItem(int nItem,LPCTSTR lpszItem,int nImage);
	virtual BOOL	PreTranslateMessage(MSG* pMsg);
	virtual void	DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void	OnLvnInsertitem(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void	OnLvnDeleteitem(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void	OnKillFocus(CWnd* pNewWnd);
	afx_msg void	OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void	OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void	OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void	OnSetFocus(CWnd* pOldWnd);
	afx_msg void	OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void	OnLvnColumnclick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void	OnLvnDeleteallitems(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void	OnPaint();
	afx_msg void	OnSelAll();
	afx_msg LRESULT OnClear(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCopy(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCopyColumn(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCopyValue(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRealUpdateCombo(WPARAM wParam, LPARAM lParam);
	virtual BOOL SetItemText(int nItem,int nSubItem,LPCTSTR lpszText);


protected:
	void	HideAll();
	void	ShowControl();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	bool	GetSelItem(int &x, int &y);
	BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	afx_msg void OnHeaderClicked(NMHDR* pNMHDR, LRESULT* pResult);
	//sorting attributes
	int		m_nSortedColumn;
	BOOL	m_bSortAscending;
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void PreSubclassWindow();
	LRESULT	Notify( DWORD dwMessage, int nItem, int nCol );

public:
	// Text
	__declspec(property(get=GetText,put=SetText))	CString	Items[][];
	__declspec(property(get=GetText,put=SetText))	CString	Text[][];
	// Mensaje
	__declspec(property(get=GetMensaje,put=SetMessageLista))	CString	Mensaje;
	// selected index
	__declspec(property(get=GetSelectedIndex,put=SetSelectedIndex))	int SelectedIndex;

DECLARE_MESSAGE_MAP()
};


