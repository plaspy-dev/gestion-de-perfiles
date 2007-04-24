// mEdit.cpp : implementation file
//

# include "stdafx.h"
# include "Componentes.h"
# include "MasEdit.h"
# include <resource.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CmEdit

void AFXAPI DDX_CmEdit(CDataExchange* pDX, int nIDC, CmEdit& rControl, CString& data)
{
    DDX_Control(pDX, nIDC, (CWnd&)rControl);
    if (!pDX->m_bSaveAndValidate)
    {
        rControl.SetData(data);
    }
    else
    {
        data = rControl.GetData();
    }
}

IMPLEMENT_DYNCREATE(CmEdit, CEditXP)

CmEdit::CmEdit()
  : m_bInit(false),
    m_strPrompt("_"),
    m_iNumChars(0),
    m_iCurrentChar(0),
    m_bShiftDown(false),
	m_bUseMask(false)
{
	FUNCTION_STACK_TRACE("FFFFE064");
    for (int i = 0; i < MAX_CHARS; i++)
        m_Char[i] = NULL;
	m_bRangos= false;
    //SubclassWindow(pParent->m_hWnd);
    //pParent->SubclassWindow(this->m_hWnd);
}

CmEdit::~CmEdit()
{
	FUNCTION_STACK_TRACE("FFFFE065");
    for (int i = 0; i < MAX_CHARS; i++)
        if (m_Char[i] != NULL)
        {
            delete m_Char[i];
            m_Char[i] = NULL;
        }
}


BEGIN_MESSAGE_MAP(CmEdit, CEditXP)
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SETFOCUS()
	ON_WM_LBUTTONUP()
	ON_WM_KEYUP()
	ON_WM_LBUTTONDBLCLK()
	ON_MESSAGE(WM_PASTE, OnPaste)
	ON_MESSAGE(WM_CUT, OnCut)
	ON_MESSAGE(WM_CLEAR, OnCut)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CmEdit message handlers

int CmEdit::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	FUNCTION_STACK_TRACE("FFFFE066");
	if (CEditXP::OnCreate(lpCreateStruct) == -1)
		return -1;
    return 0;
}

void CmEdit::OnDestroy() 
{
	FUNCTION_STACK_TRACE("FFFFE067");
	CEditXP::OnDestroy();
}

BOOL CmEdit::PreCreateWindow(CREATESTRUCT& cs) 
{
	FUNCTION_STACK_TRACE("FFFFE067");
    cs.style |= WS_TABSTOP | WS_VISIBLE | ES_AUTOHSCROLL;
    // cs.dwExStyle |= WS_EX_OVERLAPPEDWINDOW;
	return CEditXP::PreCreateWindow(cs);
}

void CmEdit::SetData(const CString& data)
{
	FUNCTION_STACK_TRACE("FFFFE068");
    CString strWindowText;
    CString strData;
    int     iNextDataChar = 0;
    
    for (int i = 1; i <= m_iNumChars; i++)
    {
        if (m_Char[i - 1]->m_bStaticChar)
        {
            strWindowText += m_Char[i - 1]->m_strValids;
        }
        else
        {
            if (iNextDataChar < data.GetLength())
            {
                strData = data[iNextDataChar++];
                if (m_Char[i - 1]->m_strValids.Find(strData[0]) < 0)
                    strData = m_strPrompt;
            }
            else
            {
                strData = m_strPrompt;
            }
            
            strWindowText += strData[0];
        }
    }
	
    SetWindowText(strWindowText);
}

CString CmEdit::GetData()
{
	FUNCTION_STACK_TRACE("FFFFE069");
    CString strWindowText;
    GetWindowText(strWindowText);
    CString strData;
    if(!m_bUseMask) 
		return NULL;
    for (int i = 1; i <= m_iNumChars; i++)
    {
        if (!m_Char[i - 1]->m_bStaticChar)
        {
            if (strWindowText.GetLength() >= i)
            {
                if (strWindowText[i - 1] != m_strPrompt)
                    strData += strWindowText[i - 1];
            }
            else
                strData += m_strPrompt;
        }
    }

    return strData;
}

void CmEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	FUNCTION_STACK_TRACE("FFFFE06A");
	if( !m_bUseMask || GetStyle()&ES_READONLY) {

		CEditXP::OnChar(nChar, nRepCnt, nFlags);
		return;
	}
    switch (nChar)
    {
	case VK_RETURN:
    case VK_TAB:
		CEditXP::OnChar(nChar, nRepCnt, nFlags);
                //(((CWnd*)GetParent())->GetNextDlgTabItem(this, m_bShiftDown))->SetFocus();
        break;
    case VK_BACK:
                if (m_iCurrentChar > 0)
                {
                    int iNewNext = FindNextChar(m_iCurrentChar - 1, false, false);
                    if (iNewNext >= 0)
                    {
                        SetSel(iNewNext, iNewNext + 1);
                        ReplaceSel(m_strPrompt);
                        m_iCurrentChar = iNewNext;
                        SetSel(m_iCurrentChar, m_iCurrentChar);
                    }
                }
                break;

    default:    
                int x, y, z;
                GetSel(x,y);
                if (x != y)
                {
                    CString szReplaceText = "";
                    for (z = x; z < y; z++)
                    {
                        if (m_Char[z] && m_Char[z]->m_bStaticChar)
                        {
                            szReplaceText += m_Char[z]->m_strValids;
                        }
                        else
                        {
                            szReplaceText += m_strPrompt;
                        }
                    }
                    ReplaceSel(szReplaceText);
                    m_iCurrentChar = FindNextChar(x, false, true);
                    SetSel(m_iCurrentChar, m_iCurrentChar);
                }
                if (m_iCurrentChar>= 0 && m_iCurrentChar < m_iNumChars)
                {
					if ( m_Char[m_iCurrentChar]) {

						bool valido= (m_Char[m_iCurrentChar]->m_strValids.Find((TCHAR)nChar) >= 0);
						if( !valido) {

							if( isupper(nChar)) 
								nChar= tolower(nChar);
							else if( islower(nChar)) 
								nChar= toupper(nChar);
							valido= (m_Char[m_iCurrentChar]->m_strValids.Find((TCHAR)nChar) >= 0);
						}
						if( valido) {
							int iNewNext = FindNextChar(m_iCurrentChar + 1, false, true);
							SetSel(m_iCurrentChar, m_iCurrentChar + 1);
							ReplaceSel((CString)(char)nChar);
							if (iNewNext >= 0)
					            m_iCurrentChar = iNewNext;
			                else
	                            m_iCurrentChar++;
							SetSel(m_iCurrentChar, m_iCurrentChar);
						} else if( !m_Char[m_iCurrentChar]->m_bStaticChar) {

							int iNextChar=0;
							bool bInside= false;
							CString temporal;
							for (int i = 0; i < m_strMask.GetLength(); i++)
							{
								switch (m_strMask[i])
								{
								case '[':   
									bInside = true;
									break;
								case ']':
									bInside = false;
									iNextChar++;
									break;
								default:    
									if (bInside) {
										if( iNextChar== m_iCurrentChar)
											temporal+= m_strMask[i];
									} else iNextChar++;
									break;
								}
								if( iNextChar>m_iCurrentChar)
									break;
							}
							if( !temporal.IsEmpty()) {
								SetToolTip("Intente: " + temporal,"Cáracter no valido");
							}
						}
					}
				}
			/*	if( m_iNumChars==m_iCurrentChar)
					GetParent()->GetNextDlgTabItem(this, m_bShiftDown ? TRUE : FALSE)->SetFocus();*/
                break;
    }

    //CEditXP::OnChar(nChar, nRepCnt, nFlags);
}

void CmEdit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	
	FUNCTION_STACK_TRACE("FFFFE06B");
    switch (nChar)
    {
    case VK_SHIFT:
                m_bShiftDown = false;
                break;
    
    default:
                break;
    }
	CEditXP::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CmEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	FUNCTION_STACK_TRACE("FFFFE06C");
    int iNext = 0;
    CWnd* pNextItem = NULL;

	m_Tip.Hide();
	if(!m_bUseMask) {
	
		CEditXP::OnKeyDown(nChar, nRepCnt, nFlags);
		return;
	}
	
    switch (nChar)
    {
    // Tab key
	case VK_RETURN:
    case VK_TAB:
		CEditXP::OnKeyDown(nChar, nRepCnt, nFlags);
		break;              
    // Shift key
    case VK_SHIFT:
                m_bShiftDown = true;
                break;
        
    // Left arrow key
    case VK_LEFT:
                iNext = FindNextChar(m_iCurrentChar - 1, false, false);
                if (iNext >= 0)
                {
                    m_iCurrentChar = iNext;
                    SetSel(m_iCurrentChar, m_iCurrentChar);
                }
                break;
    
    // Right arrow key
    case VK_RIGHT:
                iNext = FindNextChar(m_iCurrentChar + 1, false, true);
                if (iNext >= 0)
                    m_iCurrentChar = iNext;
                else
                {
                    m_iCurrentChar = FindNextChar(m_iNumChars - 1, false, false) + 1;
                }
                SetSel(m_iCurrentChar, m_iCurrentChar);
                break;

    // Home key
    case VK_HOME:
                if (m_bShiftDown)
                {
                    SetSel(0, m_iCurrentChar);
                }
                else
                {
                    m_iCurrentChar = FindNextChar(0, false, true);
                    SetSel(m_iCurrentChar, m_iCurrentChar);
                }
                break;

    // End key
    case VK_END:
                if (m_bShiftDown)
                {
                    int iLast = FindNextChar(m_iNumChars - 1, false, false) + 1;
                    SetSel(m_iCurrentChar, iLast);
                }
                else
                {
                    m_iCurrentChar = FindNextChar(m_iNumChars - 1, false, false) + 1;
                    SetSel(m_iCurrentChar, m_iCurrentChar);
                }
                break;

    // Insert key
    case VK_INSERT:

                break;

    // Delete key
    case VK_DELETE:
                int x, y, z;
                GetSel(x,y);
                if (x != y)
                {
                    CString szReplaceText = "";
                    for (z = x; z < y; z++)
                    {
                        if (m_Char[z] && m_Char[z]->m_bStaticChar)
                        {
                            szReplaceText += m_Char[z]->m_strValids;
                        }
                        else
                        {
                            szReplaceText += m_strPrompt;
                        }
                    }
                    ReplaceSel(szReplaceText);
                    m_iCurrentChar = FindNextChar(x, false, true);
                    SetSel(m_iCurrentChar, m_iCurrentChar);
                }
                else if (m_iCurrentChar <= FindNextChar(m_iNumChars - 1, false, false))
                {
                    SetSel(m_iCurrentChar, m_iCurrentChar + 1);
                    ReplaceSel(m_strPrompt);
                    iNext = FindNextChar(m_iCurrentChar + 1, false, true);
                    if (iNext >= 0)
                        m_iCurrentChar = iNext;
                    else
                        m_iCurrentChar++;
                    SetSel(m_iCurrentChar, m_iCurrentChar);
                }
                break;

    default:
                break;

    }
    //CEditXP::OnKeyDown(nChar, nRepCnt, nFlags);
}

CString CmEdit::GetMask() {

	FUNCTION_STACK_TRACE("FFFFE06D");
	return m_strMask;
}

bool CmEdit::ParseRangos(CString& strMask) {

	FUNCTION_STACK_TRACE("FFFFE06E");
	if(strMask.Find("{")==-1) 
		return false;
	if( strMask.Find("}")==-1)
		return false;
	strMask.Remove('{');
	strMask.Remove('}');
	SetLimitText(GetMaxLen( strMask));
	return true;
}

bool CmEdit::IsValidData() {

	FUNCTION_STACK_TRACE("FFFFE06F");
	CString temp;
	GetWindowText(temp);
	if( m_bUseMask) 

		return temp.Find( m_strPrompt)==-1;
	else if( m_bRangos) 

		return IsValid( m_strMask, temp);
	return true;
}

void CmEdit::SetDataEx(CString data) {

	if( m_bUseMask) {
		if( data.GetLength()==m_iNumChars) {
			CString strWindowText;
			for( int i=0; i<m_iNumChars; i++) {
				if( m_Char[i]) {
					if( m_Char[i]->m_bStaticChar) {
						strWindowText += m_Char[i]->m_strValids;
					} else if( m_Char[i]->m_strValids.Find(data[i])!=-1) {
						strWindowText+=data[i];
					} else {
						SetLimitText(0);
						m_bUseMask= false;
						strWindowText=data;
						break;
					}
				}
			}
			data= strWindowText;
		} else {
			SetLimitText(0);
			m_bUseMask=false;
		}
	}
    SetWindowText(data);
}

bool CmEdit::Init(CString strMask, CString strInitialData)
{
	FUNCTION_STACK_TRACE("FFFFE070");
    m_bInit = false;
	m_bRangos= ParseRangos(strMask);
	if( !m_bRangos) {
    
		if (!Parse(strMask))
		{
			SetWindowText("Mascara No valida.");
	        // EnableWindow(FALSE);
		    return false;
		}
	    if (!SetValidChars(strMask))
		{
			SetWindowText("Caracteres no Validos.");
	        // EnableWindow(FALSE);
		    return false;                
		}
		SetData(strInitialData);
	}
	m_strMask= strMask;
    EnableWindow(TRUE);  // this line is important!
    m_bInit = true;
	if( m_bRangos) {
		m_bUseMask= false;
		SetLimitText(0);
	} else m_bUseMask= !strMask.IsEmpty();
    return true;
}

int CmEdit::FindNextChar(int iStartPos, bool bStatic, bool bForward)
{
	FUNCTION_STACK_TRACE("FFFFE071");
    int iIndex = 0;
    int iReturnVal = -1;
    
    if (bForward)
    {
        for (int iIndex = iStartPos; iIndex < m_iNumChars; iIndex++)
        {
            if (m_Char[iIndex] && m_Char[iIndex]->m_bStaticChar == bStatic)
            {
                iReturnVal = iIndex;
                break;
            }
        }
    }
    else
    {
        for (int iIndex = iStartPos; iIndex >= 0; iIndex--)
        {
            if (m_Char[iIndex] && m_Char[iIndex]->m_bStaticChar == bStatic)
            {
                iReturnVal = iIndex;
                break;
            }
        }
    }

    return iReturnVal;
}

bool CmEdit::SetValidChars(const CString& strMask)
{
	FUNCTION_STACK_TRACE("FFFFE072");
    CString strChars;
    CString strRangeLow; 
    bool    bInside       = false;
    bool    bInsideEscape = false;
    bool    bInsideRange  = false;
    int     iNextChar     = 0;
    int     i;
    
    // Clear valid chars
    for (i = 0; i < MAX_CHARS; i++)
        if (m_Char[i] != NULL)
        {
            delete m_Char[i];
            m_Char[i] = NULL;
        }

    // Walk through mask string
    for (i = 0; i < strMask.GetLength(); i++)
    {
        switch (strMask[i])
        {
        case '[':   bInside = true;
                    break;            //done

        case ']':   bInside = false;
                    m_Char[iNextChar++] = new CmEditChar(strChars, false);
                    strChars = "";
                    break;

        case '-':   if (bInside)
                    {
                        if (bInsideEscape)
                        {
                            strChars += "-";
                            bInsideEscape = false;  //done
                        }
                        else
                        {
                            if (strChars.GetLength() == 0)
                                return false;
                            bInsideRange = true;
                            strRangeLow = strChars.Right(1);
                            strChars = strChars.Left(strChars.GetLength() - 1); //done
                        }
                    }
                    else
                    {
                        m_Char[iNextChar++] = new CmEditChar("-", true); //done
                    }
                    break;

        case '\\':  if (bInside)
                    {
                        if (!bInsideEscape)
                        {
                            bInsideEscape = true;  //done
                        }
                        else
                        {
                            if (bInsideRange)
                            {
                                strChars += GetRange(strRangeLow, "\\");
                                strRangeLow = "";
                                bInsideRange = false;  //done
                            }
                            else
                            {
                                strChars += "\\";
                                bInsideEscape = false;  //done
                            }
                        }
                    }
                    else
                    {
                        m_Char[iNextChar++] = new CmEditChar("\\", true); //done
                    }
                    break;

        default:    if (bInside)
                    {
                        if (bInsideEscape)
                        {
                            bInsideEscape = false;        //done
                        }

                        if (bInsideRange)
                        {
                            strChars += GetRange(strRangeLow,(CString) strMask[i]);
                            strRangeLow = "";
                            bInsideRange = false;   //done
                        }
                        else
                        {
                            strChars += strMask[i]; //done
                        }
                    }
                    else
                    {
                        m_Char[iNextChar++] = new CmEditChar((CString) strMask[i], true); //done
                    }
                    break;
        }
    }

    m_iNumChars = iNextChar;
    SetLimitText(m_iNumChars);
    
    return true;
}

CString CmEdit::GetRange(CString szLow, CString szHigh)
{
	FUNCTION_STACK_TRACE("FFFFE073");
    CString szReturn = "";

    if (szLow.GetLength() == 1 && szHigh.GetLength() == 1)
    {
        char first = szLow[0];
        char last  = szHigh[0];

        for (int i = first; i <= last; i++)
            szReturn += (CString)(char)i;
    }

    return szReturn;
}

bool CmEdit::Parse(const CString& strMask)
{
	FUNCTION_STACK_TRACE("FFFFE074");
    bool bValid = true;
    int iBracketState = 0;
    //int iDashState = 0;
    
    for (int i = 1; i <= strMask.GetLength(); i++)
    {
        switch (strMask[i - 1])
        {
        case '[':   iBracketState++; break;

        case ']':   iBracketState--; break;

        //case '-':   if (iBracketState > 0)
        //                iDashState = 1;
        //            break;

        default:    //if (iBracketState > 0) && iDashState == 1)
                    //    iDashState = 0;
                    break;
        }

        // Make sure the [ ] are in sync
        if (iBracketState < 0 || iBracketState > 1)
            bValid = false;

        // Make sure there is a character after a '-' inside the [ ]
        //if (iBracketState == 0 && iDashState == 1)
        //    bValid = false;
    }

    return (iBracketState == 0 /*&& iDashState == 0*/ && bValid) ? true : false;
}

bool CmEdit::SetPromptChar(CString strPromptChar)
{
	FUNCTION_STACK_TRACE("FFFFE075");
    if (strPromptChar.GetLength() != 1)
        return false;

    m_strPrompt = strPromptChar;
    return true;
}

CString CmEdit::GetPromptChar() {

	FUNCTION_STACK_TRACE("FFFFE076");
	return m_strPrompt;
}

void CmEdit::OnSetFocus(CWnd* pOldWnd) 
{
	FUNCTION_STACK_TRACE("FFFFE077");
	if( pOldWnd)

		CEditXP::OnSetFocus(pOldWnd);
	CString temp;
	GetWindowText( temp);
	SetSel(0, temp.GetLength());
}

void CmEdit::OnLButtonUp(UINT nFlags, CPoint point) 
{
	FUNCTION_STACK_TRACE("FFFFE078");
	int x, y;
    GetSel(x, y);

    if (x == y)
    {
        m_iCurrentChar = x;
        SetSel(m_iCurrentChar, m_iCurrentChar);
    }
	CEditXP::OnLButtonUp(nFlags, point);
}

void CmEdit::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	FUNCTION_STACK_TRACE("FFFFE079");
	OnSetFocus( NULL);
}

LRESULT CmEdit::OnCut(WPARAM Wparam, LPARAM LParam) {

	return 0;
}

LRESULT CmEdit::OnPaste(WPARAM Wparam, LPARAM LParam) 
{
	FUNCTION_STACK_TRACE("FFFFE07B");
	if( GetStyle() & ES_READONLY)
		return 0;

	CString buffer;
	COleDataObject	obj;

	if (obj.AttachClipboard()) 
      {
		if (obj.IsDataAvailable(CF_TEXT)) 
         {
			HGLOBAL hmem = obj.GetGlobalData(CF_TEXT);
			CMemFile sf((BYTE*) ::GlobalLock(hmem), ::GlobalSize(hmem));
			CString buffer;

			LPSTR str = buffer.GetBufferSetLength(::GlobalSize(hmem));
			sf.Read(str, ::GlobalSize(hmem));
			::GlobalUnlock(hmem);

			buffer.ReleaseBuffer();
			buffer.FreeExtra();

			if( !m_bUseMask || GetStyle()&ES_READONLY) {
				if( GetStyle() & ES_NUMBER) {
					CString out;
					for( int i=0; i<buffer.GetLength(); i++)
						if( buffer[i]>='0' && buffer[i]<='9')
							out+=buffer[i];
						else break;
						if(!out.IsEmpty())
							ReplaceSel(out, true);
				} else {
					Text= buffer;
					SetSel(buffer.GetLength(), buffer.GetLength(), false);
				}
			} else {
				for( int i=0; i<buffer.GetLength(); i++) {
					OnChar(buffer[i], 1,79);
				}
			}
			return 0;
         }
      }
   return 0;
}
