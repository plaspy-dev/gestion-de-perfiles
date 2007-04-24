// logomdi.cpp : implementation file
//

#include "stdafx.h"
# include "Componentes.h"
#include "logomdi.h"
# include <ResourceDLL.h>
# include "resource.h"
# include "EnBitmap.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/**************************************/
// CLogoMdi
CLogoMdi::CLogoMdi()  : m_sizeClient(0,0)
{
  	m_LogoFont.CreateFont(36, 0, 0, 0, FW_BOLD, 1, FALSE, FALSE,
	  ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
	  FIXED_PITCH | FF_ROMAN, "Times New Roman");

	m_hBitmap=NULL;
	m_nAncho=0;
	m_nAlto=0;
	m_dwBkColor=-1;
	sLogoString = CApplicacion::GetCompany() + "\r" + CApplicacion::GetLongName();
	m_nLineas= 1;
	
	for( int i=0; i<sLogoString.GetLength(); i++) {
		if(sLogoString[i]=='\r')
			m_nLineas++;
	}
	m_pFlash=NULL;

	CResourceDLL dll;
	m_hBitmap= CEnBitmap::LoadImageResource(IDR_FONDO, "foto",AfxGetResourceHandle());
	ASSERT(m_hBitmap);
	if(m_hBitmap)
		::GetObject(m_hBitmap, sizeof(BITMAP), &m_bmInfo);
}

/**************************************/
CLogoMdi::~CLogoMdi()
{
	m_LogoFont.DeleteObject();
	if(m_pFlash)
		delete m_pFlash;
	if(m_hBitmap) 
		::DeleteObject(m_hBitmap);
}
/**************************************/
BEGIN_MESSAGE_MAP(CLogoMdi, CWnd)
    ON_WM_ERASEBKGND()
    ON_WM_SIZE()
END_MESSAGE_MAP()

/**************************************/
void CLogoMdi::SetFlash(CString archivo, int ancho/*=0*/, int alto/*=0*/, DWORD bkColor) {

	m_szArchivo= archivo;
	m_nAlto= alto;
	m_nAncho= ancho;
	m_dwBkColor= bkColor;
	BuildFlash(true);
}

void CLogoMdi::BuildFlash(bool load/*=false*/) {

	CWaitCursor wait;
	if(m_szArchivo.IsEmpty())
		return;
	int mancho=m_sizeClient.cx/2;
	int malto= m_sizeClient.cy/2;

	CRect rect(mancho-m_nAncho/2,
		malto-m_nAlto/2,
		m_nAncho, m_nAlto);
	rect.right= rect.left+m_nAncho;
	rect.bottom= rect.top+m_nAlto;

	if(!m_pFlash) {
		m_pFlash=new CShockwaveflash1();
		if(!m_pFlash->Create("", "", WS_CHILD | WS_VISIBLE, rect, this, 1050)) {
			delete m_pFlash;
			m_pFlash=NULL;
			return;
		}
	} else {
		m_pFlash->MoveWindow(rect);
	}
	if(m_dwBkColor!=-1) {
		m_pFlash->put_BackgroundColor(RGB(
			GetBValue(m_dwBkColor),
			GetGValue(m_dwBkColor),
			GetRValue(m_dwBkColor))
			);
	}
	if(load)
		m_pFlash->put_Movie(m_szArchivo);
}

void CLogoMdi::Pause() {

	if(m_pFlash) {
		m_pFlash->ShowWindow(SW_HIDE);
	}
}
void CLogoMdi::Start() {

	if(m_pFlash) {
		m_pFlash->ShowWindow(SW_NORMAL);
	}
}
// CLogoMdi message handlers
BOOL CLogoMdi::OnEraseBkgnd(CDC* pDC)
{
	// This is where the real work is done printing the logo in the corner..

	CRect rect,m_rDataBox;
	GetClientRect(&rect);
	CopyRect(&m_rDataBox,&rect);


	if(m_hBitmap) {
		CDC dcMem;
		VERIFY( dcMem.CreateCompatibleDC(pDC));

		HBITMAP* pBmpOld = (HBITMAP*) ::SelectObject(dcMem.m_hDC, m_hBitmap);
		pDC->SetStretchBltMode(COLORONCOLOR);

		pDC->StretchBlt(rect.left, rect.top, rect.Width(), rect.Height(), 
			&dcMem, 0, 0, m_bmInfo.bmWidth-1, m_bmInfo.bmHeight-1,
			SRCCOPY);
		::SelectObject(dcMem.m_hDC, pBmpOld);
	} else {
		CBrush NewBrush(GetSysColor(COLOR_DESKTOP));
		pDC->SetBrushOrg(0,0) ;
		CBrush* pOldBrush = (CBrush*)pDC->SelectObject(&NewBrush);   
		pDC->PatBlt(rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top ,PATCOPY);
		pDC->SelectObject(pOldBrush);
	}
	
	TEXTMETRIC tm;
	pDC->GetTextMetrics(&tm);
	CFont* oldFont = pDC->SelectObject(&m_LogoFont);
	CSize sz = pDC->GetTextExtent(sLogoString, sLogoString.GetLength());
	// GetTextExtent calculates the size of the displayed logo
	// which depends on the device context....
	// Calculate the box size by subtracting the text width and height from the
	// window size.  Also subtract 20% of the average character size to keep the
	// logo from printing into the borders...
	m_rDataBox.left = m_rDataBox.right  - sz.cx - tm.tmAveCharWidth/2;
	m_rDataBox.left-=10;
	m_rDataBox.right-=10;
	m_rDataBox.top  = m_rDataBox.bottom - m_nLineas*(sz.cy - tm.tmHeight/5);
	m_rDataBox.top-=20;
	m_rDataBox.bottom-=10;
	pDC->SetBkMode(TRANSPARENT);
	// shift logo box right, and print black...
	COLORREF oldColor = pDC->SetTextColor(RGB(0,0,0));
	pDC->DrawText(sLogoString, sLogoString.GetLength(), &m_rDataBox, DT_VCENTER | /*DT_SINGLELINE | */DT_RIGHT);
	// shift logo box left and print white
	m_rDataBox.left -= (m_nLineas+1)*tm.tmAveCharWidth/5;
	pDC->SetTextColor(RGB(255,255,255));
	pDC->DrawText(sLogoString, sLogoString.GetLength(), &m_rDataBox, DT_VCENTER | /*DT_SINGLELINE | */DT_RIGHT);
	// Restore original location and print in the button face color
	m_rDataBox.left += tm.tmAveCharWidth/5;
	pDC->SetTextColor(GetSysColor(COLOR_BTNFACE));
	pDC->DrawText(sLogoString, sLogoString.GetLength(), &m_rDataBox, DT_VCENTER | /*DT_SINGLELINE | */DT_RIGHT);
	// restore the original properties and release resources...
	pDC->SelectObject(oldFont);
	pDC->SetTextColor(oldColor);   

    return TRUE;
}      
/**************************************/
void CLogoMdi::OnSize(UINT nType, int cx, int cy)
{
    Default() ;    // Same as DefWindowProc(), or CWnd::OnSize();
    // if the app is just starting up, save the window
    // dimensions and get out
    if ((m_sizeClient.cx == 0) && (m_sizeClient.cy==0)) {
        m_sizeClient.cx = cx;
        m_sizeClient.cy = cy;
        return ;
    }
    // if the size hasn't changed, break and pass to default
    if ((m_sizeClient.cx == cx) && ( m_sizeClient.cy == cy)) 
        return ;
    // window size has changed so save new dimensions and force
    // entire background to redraw, including icon backgrounds
    m_sizeClient.cx = cx;
    m_sizeClient.cy = cy;
	BuildFlash();

    RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_ERASENOW|RDW_ALLCHILDREN);
    return ;
}
/**************************************/
