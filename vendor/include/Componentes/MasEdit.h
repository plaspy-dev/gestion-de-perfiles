//***************************************************************************
//
// AUTHOR:  JUHU
//
// DESCRIPTION: Edit con restricciones _dfd_
//
// ORIGINAL: 
//
//***************************************************************************
#ifndef __CmEdit_h__
#define __CmEdit_h__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "EditXP.h"

class CmEditChar
{
// Construction
public:
	//***********************************************************************
	// Name:		CmEditChar
	// Description:	Constructor.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
    CmEditChar() { m_bStaticChar = false; }
	//***********************************************************************
	// Name:		CmEditChar
	// Description:	Constructor.
	// Parameters:	CString strChars.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
    CmEditChar(CString strChars)
    {
        m_strValids = strChars;
        m_bStaticChar = false;
    }
	//***********************************************************************
	// Name:		CmEditChar
	// Description:	Constructor.
	// Parameters:	CString strChars, bool bStaticChar.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
    CmEditChar(CString strChars, bool bStaticChar)
    {
        m_strValids = strChars;
        m_bStaticChar = bStaticChar;
    }
    
    CString m_strValids;
    CString m_strData;
    bool    m_bStaticChar;
};

#define  MAX_CHARS  256

//***************************************************************************
//
// AUTHOR:  JUHU
//
// DESCRIPTION: Edit con restricciones _dfd_
//
// ORIGINAL: 
//
//***************************************************************************
class  __COMPONENTES_EXPORT__ CmEdit : public CEditXP
{
	    DECLARE_DYNCREATE(CmEdit)
// Construction
public:
	//***********************************************************************
	// Name:		CmEdit
	// Description:	Constructor.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	CmEdit();
	//***********************************************************************
	// Name:		CmEdit
	// Description:	Destructor.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	virtual ~CmEdit();

// Attributes
public:
    bool    Init(CString strMask, CString strInitialData = "");
    bool    SetPromptChar(CString strPromptChar);
	bool	m_bRangos;
	
protected:
	bool            m_bInit;
    CString         m_strPrompt;
	CString			m_strMask;
    CmEditChar*     m_Char[MAX_CHARS];
    int             m_iNumChars;
    int             m_iCurrentChar;
    bool            m_bShiftDown;
	bool			m_bUseMask;


// Operaciones
public:
	//***********************************************************************
	// Name:		GetPromptChar
	// Description:	Devuelve el signo con el cual se marca los campos especiales.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	CString GetPromptChar();
	//***********************************************************************
	// Name:		GetMask
	// Description:	Obtiene la mascara o el cojunto de simbolos permitidos.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	CString GetMask();
	//***********************************************************************
	// Name:		IsValidData
	// Description:	Devulve true si la informacion ingresada es valida.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	bool	IsValidData();
	//***********************************************************************
	// Name:		SetData
	// Description:	Inserta data en el campo.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
    void    SetData(const CString& data);
	//***********************************************************************
	// Name:		GetData
	// Description:	Obtiene la data ingresada por el usuario.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
    CString GetData();
	//***********************************************************************
	// Name:		SetDataEx
	// Description:	Permite insertar data teniendo en cuenta las reglas hasta
	//				se ingrese la data permitida.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	void	SetDataEx(CString data);

protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    bool    Parse(const CString& strMask);
	bool	ParseRangos(CString& strMask);
    bool    SetValidChars(const CString& strMask);
    CString GetRange(CString szLow, CString szHigh);
    //CString GetText(bool bDataOnly = false);
    //CString GetData() { return GetText(true); }
    int     FindNextChar(int iStartPos, bool bStatic, bool bForward);

	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg LRESULT OnPaste(WPARAM Wparam, LPARAM LParam);
	afx_msg LRESULT OnCut(WPARAM Wparam, LPARAM LParam);

DECLARE_MESSAGE_MAP()
};

void AFXAPI DDX_CmEdit(CDataExchange* pDX, int nIDC, CmEdit& rControl, CString& data);

#endif
