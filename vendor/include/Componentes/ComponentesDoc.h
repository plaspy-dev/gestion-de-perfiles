//***************************************************************************
//
// AUTHOR:  JUHU
//
// DESCRIPTION: Interfaz de la clase CComponentesDoc
//
// ORIGINAL: 
//
//***************************************************************************

#pragma once

class __COMPONENTES_EXPORT__ CComponentesDoc : public CDocument
{
	DECLARE_DYNCREATE(CComponentesDoc)
protected: // Crear sólo a partir de serialización
	//***********************************************************************
	// Name:		CComponentesDoc
	// Description:	Default constructor.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		See CDocument.
	//***********************************************************************
	CComponentesDoc();
	//***********************************************************************
	// Name:		CComponentesDoc
	// Description:	Destructor.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		Virtual.
	//***********************************************************************
	virtual ~CComponentesDoc();

// Operaciones
public:
	//***********************************************************************
	// Name:		OnNewDocument
	// Description:	TODO.
	// Parameters:	void
	// Return:		BOOL.	
	// Notes:		Reemplazo virual.
	//***********************************************************************
	virtual BOOL OnNewDocument();
	//***********************************************************************
	// Name:		Serialize
	// Description:	TODO.
	// Parameters:	CArchive&
	// Return:		void.	
	// Notes:		Reemplazo.
	//***********************************************************************
	virtual void Serialize(CArchive& ar);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual BOOL SaveModified();
DECLARE_MESSAGE_MAP()
};
