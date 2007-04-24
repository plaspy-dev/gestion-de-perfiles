#pragma once
//***************************************************************************
//
// AUTHOR:  JUHU
//
// DESCRIPTION: Con esta clase se puede crear un separador que permite
// habilitar o desabilitar opciones en algun conjunto de
// controles.
//
// ORIGINAL: 
//
//***************************************************************************

#include "Separator.h"
#include <set>
using namespace std;

class __COMPONENTES_EXPORT__ CCheckBoxSeparator : public CButton
{
// Construction
public:
	//***********************************************************************
	// Name:		CCheckBoxSeparator
	// Description:	Default constructor.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	CCheckBoxSeparator();
	//***********************************************************************
	// Name:		CCheckBoxSeparator
	// Description:	Destructor.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		virtual.
	//***********************************************************************
    virtual ~CCheckBoxSeparator();

// Attributes
protected:
    CFont m_font;
    CSeparator m_ctrlSeparator;
	bool		m_bEnable;
    set<int> m_ControlledIDs;

// Operations
public:
	//***********************************************************************
	// Name:		SetNextSectionCtrl
	// Description:	Rango de controles habilitados, empieza en el mismo y 
	//				termina en el que se le indique..
	// Parameters:	CWnd&.
	// Return:		void.	
	// Notes:		none.
	//***********************************************************************
    void SetNextSectionCtrl(CWnd& wnd);
	//***********************************************************************
	// Name:		SetNextSectionId
	// Description:	Rango de controles habilitados, empieza en el mismo y 
	//				termina en el id que se le indique.
	// Parameters:	int.
	// Return:		void.	
	// Notes:		none.
	//***********************************************************************
	void SetNextSectionId(int nNextId);
	//***********************************************************************
	// Name:		AddControlledDlgItem
	// Description:	Dentro de un rango de controles deshabilitados habilita 
	//				alguno como exepcion.
	// Parameters:	int.
	// Return:		void.	
	// Notes:		none.
	//***********************************************************************
    void AddControlledDlgItem(int);
	//***********************************************************************
	// Name:		UpdateControlledDlgItems
	// Description:	Actualiza el estado de los controles.
	// Parameters:	void.
	// Return:		void.	
	// Notes:		none.
	//***********************************************************************
    void UpdateControlledDlgItems();
	//***********************************************************************
	// Name:		SetEnable
	// Description:	Invierte el comportamiento del checkBox.
	// Parameters:	bool.
	// Return:		void.	
	// Notes:		Default parameter true.
	//***********************************************************************
	void SetEnable(bool enable=true);

protected:
    void OnUpdateContents();

public:
	afx_msg BOOL OnClicked();
    afx_msg void OnMove(int x, int y);
    afx_msg LRESULT OnSetText(WPARAM wParam, LPARAM lParam);
protected:
    virtual void PreSubclassWindow();

DECLARE_MESSAGE_MAP()
};
