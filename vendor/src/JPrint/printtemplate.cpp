# include "stdafx.h"
#include "PrintTemplate.h"
# include "JPrint.h"


CPrintTemplate::CPrintTemplate(void)
{
	m_footerLines = FOOTER_LINES;
	m_nCellPadding= 0;
}

CPrintTemplate::~CPrintTemplate(void)
{
}

BOOL CPrintTemplate::OnPreparePrinting(CPrintInfo* pInfo, bool first)
{

	m_bPrintSelection= false;
	m_footerLines = FOOTER_LINES;
	// recalculamos el footer
	SetFooter(m_strFooter);
	m_nCellPadding= 0;
	m_rectPaper  = CRect(0,0,0,0);
	m_rectPage   = CRect(0,0,0,0);
	m_rectHeader = CRect(0,0,0,0);
	m_rectFooter = CRect(0,0,0,0);
	m_rectBody   = CRect(0,0,0,0);
	m_pFontHeader = NULL;
	m_pFontFooter = NULL;
	m_pFontColumn = NULL;
	m_pFontBody   = NULL;
	m_CharSizeHeader = CSize(0,0);
	m_CharSizeFooter = CSize(0,0);
	m_CharSizeBody   = CSize(0,0);
	m_pixelpantalla = CSize(0,0);
	m_marginLeft   = MARGIN_LEFT;
	m_marginTop    = MARGIN_TOP;
	m_marginRight  = MARGIN_RIGHT;
	m_marginBottom = MARGIN_BOTTOM;
	m_headerHeight = 0;
	m_footerHeight = 0;
	m_headerLines = HEADER_LINES;
	m_nPageCount = 0;
	m_nCellPadding= AfxGetApp()->GetProfileInt("printer", "cellpadding", 1);
	m_nEspaciado= AfxGetApp()->GetProfileInt("printer", "rowpadding", 1);

	return TRUE;
}

void CPrintTemplate::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{   
	// Create fonts
	ASSERT(pDC != NULL && pInfo != NULL);
	m_pFontHeader = CreateFont(pDC,AfxGetApp()->GetProfileString("printer\\header", "font",_T("Arial")), 
		AfxGetApp()->GetProfileInt("printer\\header","tamano",12), 
		AfxGetApp()->GetProfileInt("printer\\header","weight",FW_BOLD),
		AfxGetApp()->GetProfileInt("printer\\header","italic",false));
	m_pFontFooter = CreateFont(pDC,AfxGetApp()->GetProfileString("printer\\footer", "font",_T("Arial")), 
		AfxGetApp()->GetProfileInt("printer\\footer","tamano",10),
		AfxGetApp()->GetProfileInt("printer\\footer","weight",FW_NORMAL),
		AfxGetApp()->GetProfileInt("printer\\footer","italic",false));
	m_pFontColumn = CreateFont(pDC,AfxGetApp()->GetProfileString("printer\\column", "font",_T("Arial")), 
		AfxGetApp()->GetProfileInt("printer\\column","tamano",9), 
		AfxGetApp()->GetProfileInt("printer\\column","weight",FW_BOLD),
		AfxGetApp()->GetProfileInt("printer\\column","italic",false));
	m_pFontBody   = CreateFont(pDC,AfxGetApp()->GetProfileString("printer\\body", "font", _T("Times New Roman")), 
		AfxGetApp()->GetProfileInt("printer\\body","tamano",10),
		AfxGetApp()->GetProfileInt("printer\\body","weight",FW_NORMAL),
		AfxGetApp()->GetProfileInt("printer\\body","italic",false));

	// Prepare layout 
	m_rectPaper  = GetPaperRect(pDC);
	// convertimos los milimetros a pixeles
	m_nCellPadding= m_rectPaper.right*m_nCellPadding/m_pixelpantalla.cx;
	m_nEspaciado= m_rectPaper.bottom*m_nEspaciado/m_pixelpantalla.cy;

	if( !pInfo->m_bPreview)
		m_bPrintSelection= pInfo->m_pPD->PrintSelection();

	// Calculate character size
	m_CharSizeHeader = GetCharSize(pDC, m_pFontHeader);
	m_CharSizeFooter = GetCharSize(pDC, m_pFontFooter);
	m_CharSizeBody   = GetCharSize(pDC, m_pFontBody);

	m_rectPage   = GetPageRect();
	m_rectHeader = GetHeaderRect();
	m_rectFooter = GetFooterRect();
	m_rectBody = GetBodyRect();

	m_nPageCount= GetPageCount(pDC);

	pInfo->SetMaxPage(m_nPageCount);
	if( !pInfo->m_bPreview)
		AfxGetMainWnd()->PostMessage(WM_STATUSBAR, 0, (LPARAM)m_nPageCount);
}

int CPrintTemplate::GetPageCount(CDC *pDC) {

	return 0;
}

void CPrintTemplate::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
#ifdef _DEBUG
	DrawRect(pDC, m_rectPaper,RGB(0,0,0));
	DrawRect(pDC, m_rectPage,RGB(0,0,255));
#endif
	PrintHeader(pDC, pInfo);
	PrintBody(pDC, pInfo);
	PrintFooter(pDC, pInfo);
	if( !pInfo->m_bPreview)
		AfxGetMainWnd()->PostMessage(WM_STATUSBAR, 1);
}

void CPrintTemplate::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* pInfo)
{
	delete m_pFontHeader;
	delete m_pFontFooter;
	delete m_pFontColumn;
	delete m_pFontBody;
	if( !pInfo->m_bPreview)
		AfxGetMainWnd()->PostMessage(WM_STATUSBAR, 2);
}

/////////////////////////////////////////////////////////////////////////////
// CPrintTemplate diagnostics

#ifdef _DEBUG
void CPrintTemplate::DrawRect(CDC* pDC, CRect rect, COLORREF color)
{
	UNUSED_ALWAYS(pDC);
	UNUSED_ALWAYS(rect);
	UNUSED_ALWAYS(color);

#if 1
	CPen pen;
	pen.CreatePen(PS_SOLID, 3, color);
	CPen* pOldPen = pDC->SelectObject(&pen);
	pDC->Rectangle(rect);
	pDC->SelectObject(pOldPen);
#endif
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPrintTemplate helpers

CRect CPrintTemplate::GetPaperRect(CDC* pDC)
{
	ASSERT(pDC != NULL);
	CSize paperSize = CSize(pDC->GetDeviceCaps(HORZRES), pDC->GetDeviceCaps(VERTRES));
	m_pixelpantalla= CSize(pDC->GetDeviceCaps(HORZSIZE), pDC->GetDeviceCaps(VERTSIZE));
	return CRect(0, 0, paperSize.cx, paperSize.cy);
}

CRect CPrintTemplate::GetPageRect()
{
	ASSERT(m_rectPaper != CRect(0,0,0,0));
	CRect rect = m_rectPaper;

	rect.DeflateRect(m_rectPaper.right*m_marginLeft/m_pixelpantalla.cx, 
		m_rectPaper.bottom*m_marginTop/m_pixelpantalla.cy,
		m_rectPaper.right*m_marginRight/m_pixelpantalla.cx,
		m_rectPaper.bottom*m_marginBottom/m_pixelpantalla.cy);
	return rect;
}

CRect CPrintTemplate::GetHeaderRect()
{
	ASSERT(m_rectPage != CRect(0,0,0,0));
	CRect rect = m_rectPage;
	CSize charSize = m_CharSizeHeader;
	rect.DeflateRect(0, 0, 0, rect.Height() - m_headerLines * charSize.cy);
	return rect;
}

CRect CPrintTemplate::GetFooterRect()
{
	ASSERT(m_rectPage != CRect(0,0,0,0));
	CRect rect = m_rectPage;
	CSize charSize = m_CharSizeFooter;
	rect.DeflateRect(0, rect.Height() - m_footerLines * charSize.cy, 0, 0);
	return rect;
}

CRect CPrintTemplate::GetBodyRect()
{
	ASSERT(m_rectPage != CRect(0,0,0,0));
	ASSERT(m_rectHeader != CRect(0,0,0,0));
	ASSERT(m_rectFooter != CRect(0,0,0,0));
	CRect rectPage = m_rectPage;
	CRect rectHead = m_rectHeader;
	CRect rectFoot = m_rectFooter;
	rectPage.DeflateRect(0, rectHead.Height(), 0, rectFoot.Height());
	return rectPage;
}

CSize CPrintTemplate::GetCharSize(CDC* pDC, CFont* pFont)
{
	ASSERT(pDC != NULL && pFont != NULL);
	CFont *pOldFont = pDC->SelectObject(pFont);
	CSize charSize = pDC->GetTextExtent(_T("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSATUVWXYZ"),52);
	charSize.cx /= 52;
	charSize.cy+=m_nEspaciado;
	charSize.cx+=m_nCellPadding;
	pDC->SelectObject(pOldFont);
	return charSize;
}

CFont* CPrintTemplate::CreateFont(CDC* pDC, CString strName, int nPoints, int lfWeight, bool lfItalic)
{
	ASSERT(pDC);
	CFont* pFont = new CFont();

	VERIFY(pFont->CreateFont(
		-MulDiv(nPoints, pDC->GetDeviceCaps(LOGPIXELSY), 72),                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		lfWeight,                 // nWeight
		lfItalic,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		strName));                 // lpszFacename
	return pFont;
}

/////////////////////////////////////////////////////////////////////////////
// CPrintTemplate printing

void CPrintTemplate::PrintHeader(CDC* pDC, CPrintInfo* pInfo)
{
	ASSERT(m_pFontHeader && pInfo);
	UNUSED_ALWAYS(pInfo);

	CFont *pOldFont = pDC->SelectObject(m_pFontHeader);
#ifdef _DEBUG
	DrawRect(pDC, m_rectHeader,RGB(0,255,0));
#endif

	// Create black brush
	CBrush brush;
	brush.CreateSolidBrush(RGB(0,0,0));
	CBrush* pOldBrush = pDC->SelectObject(&brush);

	// Draw line
	CRect rectLine = m_rectHeader;
	rectLine.top = rectLine.top + m_CharSizeHeader.cy * 3 / 2;
	rectLine.bottom = rectLine.top + HEADER_LINE_SIZE;
	pDC->FillRect(&rectLine, &brush);
	pDC->SelectObject(pOldBrush);
	
	// Draw "Program" and "Document"
	CRect rectHeader = m_rectHeader;
	rectHeader.bottom = rectHeader.top + m_CharSizeHeader.cy;
	pDC->SetBkMode(TRANSPARENT);
	pDC->DrawText(m_strAppName, rectHeader, DT_LEFT | DT_BOTTOM);
	pDC->DrawText(m_strDocName, rectHeader, DT_RIGHT | DT_BOTTOM);   
	pDC->SelectObject(pOldFont);
}

void CPrintTemplate::PrintBody(CDC* pDC, CPrintInfo* pInfo)
{
	ASSERT(m_pFontBody && m_pFontColumn && pInfo);

#ifdef _DEBUG
	DrawRect(pDC, m_rectBody,RGB(255,255,0));
#endif
}

void CPrintTemplate::PrintFooter(CDC* pDC, CPrintInfo* pInfo)
{
	ASSERT(m_pFontFooter != NULL && pInfo != NULL);
	CFont *pOldFont = pDC->SelectObject(m_pFontFooter);

#ifdef _DEBUG
	DrawRect(pDC, m_rectFooter,RGB(0,255,0));
#endif

	// Create black brush
	CBrush brush;
	brush.CreateSolidBrush(RGB(0,0,0));
	CBrush* pOldBrush = pDC->SelectObject(&brush);

	// Draw line
	CRect rectLine = m_rectFooter;
	rectLine.top = rectLine.top + m_CharSizeHeader.cy * 1 / 2;
	rectLine.bottom = rectLine.top + HEADER_LINE_SIZE;
	pDC->FillRect(&rectLine, &brush);
	pDC->SelectObject(pOldBrush);

	// Draw "Pages"
	CString strPages;
	strPages.Format(_T("Página %d de %d"),pInfo->m_nCurPage, m_nPageCount);
	CRect rectPage = m_rectFooter;
	rectPage.top = rectPage.bottom - m_CharSizeFooter.cy;
	pDC->DrawText(strPages, rectPage, DT_RIGHT | DT_BOTTOM);
	pDC->DrawText(m_strFooterIzquierdo, rectPage, DT_LEFT | DT_BOTTOM);
	rectPage.top= rectLine.bottom+HEADER_LINE_SIZE;
	pDC->DrawText(m_strFooter, rectPage, DT_CENTER | DT_VCENTER);

	if( pInfo->m_nCurPage==m_nPageCount && !m_strFinal.IsEmpty()) {

		int final= 2;
		for( int i=0; i<m_strFinal.GetLength(); i++)
			if( m_strFinal[i]=='\n')
				final++;
		rectPage=GetBodyRect();
		rectPage.top= rectPage.bottom-final*m_CharSizeFooter.cy;
		pDC->DrawText(m_strFinal, rectPage, DT_CENTER | DT_VCENTER);
	}

	pDC->SelectObject(pOldFont);
}

void CPrintTemplate::SetAppName(const CString& strName)
{
	m_strAppName = strName;
}

void CPrintTemplate::SetDocTitle(const CString &strName)
{
	m_strDocName = strName;
}

void CPrintTemplate::SetFooter(const CString& strFooter) {
	
	m_strFooter= strFooter;
	if(m_strFooter.IsEmpty())
		return;
	m_footerLines++;
	for( int i=0; i<m_strFooter.GetLength(); i++)
		if( m_strFooter[i]=='\n')
			m_footerLines++;
}

void CPrintTemplate::SetFooterIzq(const CString& strFooter) {
	m_strFooterIzquierdo= strFooter;
}

void CPrintTemplate::SetFinal(const CString& strFooter) {
	m_strFinal= strFooter;
}

void CPrintTemplate::SetMarginTopPix(int topMargin)
{ 
	m_marginTop = topMargin; 
}

void CPrintTemplate::SetMarginLeftPix(int leftMargin)
{
	m_marginLeft = leftMargin; 
}

int CPrintTemplate::GetMarginLeftPix()
{ 
	return m_marginLeft; 
}

int CPrintTemplate::GetMarginTopPix()
{ 
	return m_marginTop; 
}

void CPrintTemplate::SetMarginRightPix(int rightMargin)
{ 
	m_marginRight = rightMargin; 
}

int CPrintTemplate::GetMarginRightPix()
{
	return m_marginRight; 
}

void CPrintTemplate::SetMarginBottomPix(int bottomMargin)
{ 
	m_marginBottom = bottomMargin; 
}

int CPrintTemplate::GetMarginBottomPix()
{ 
	return m_marginBottom; 
}

void CPrintTemplate::SetHeadLines(int lines)
{ 
	m_headerLines = lines;
}

void CPrintTemplate::SetFootLines(int lines)
{ 
	m_footerLines = lines; 
}

int CPrintTemplate::GetFootLines(){ 
	return m_footerLines;
}
