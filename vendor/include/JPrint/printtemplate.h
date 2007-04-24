#pragma once

#define MARGIN_LEFT			20
#define MARGIN_TOP			15
#define MARGIN_RIGHT		20
#define MARGIN_BOTTOM		15
#define HEADER_LINES		2
#define FOOTER_LINES		2
#define HEADER_LINE_SIZE	5
#define FOOTER_LINE_SIZE	5

class __JPRINT_EXPORT__ CPrintTemplate
{
public:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo, bool first=true);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
public:
	CPrintTemplate(void);
public:
	~CPrintTemplate(void);

public:
	void SetDocTitle(const CString& strName);
	void SetAppName(const CString& strName);
	void SetFooter(const CString& strFooter);
	void SetFooterIzq(const CString& strFooter);
	void SetFinal(const CString& strFooter);
	void SetMarginLeftPix(int leftMargin);
	int GetMarginLeftPix();
	void SetMarginTopPix(int topMargin);
	int GetMarginTopPix();
	void SetMarginRightPix(int rightMargin);
	int GetMarginRightPix();
	void SetMarginBottomPix(int bottomMargin);
	int GetMarginBottomPix();
	void SetHeadLines(int lines);
	void SetFootLines(int lines);
	int GetFootLines();

protected:
#ifdef _DEBUG
	void DrawRect(CDC* pDC, CRect rect, COLORREF color);
#endif
	virtual void PrintFooter(CDC* pDC, CPrintInfo* pInfo);
	virtual void PrintBody(CDC* pDC, CPrintInfo* pInfo);
	virtual void PrintHeader(CDC* pDC, CPrintInfo* pInfo);
	virtual int GetPageCount(CDC *pDC);

	static CFont* CreateFont(CDC* pDC, CString strName = "", int nPoints = 0, int lfWeight = FW_NORMAL, bool lfItalic = false);
	CRect GetBodyRect();
	CRect GetPaperRect(CDC* pDC);
	CRect GetPageRect();
	CRect GetHeaderRect();
	CRect GetFooterRect();
	int m_nCellPadding;
	bool m_bPrintSelection;
	CSize GetCharSize(CDC* pDC, CFont* pFont);

protected:
	CString m_strAppName;
	CString m_strDocName;
	CString m_strFooter;
	CString m_strFooterIzquierdo;
	CString m_strFinal;
	CRect m_rectHeader;
	CRect m_rectFooter;
	CRect m_rectBody;
	CRect m_rectPage;
	CRect m_rectPaper;
	CFont* m_pFontHeader;
	CFont* m_pFontFooter;
	CFont* m_pFontColumn;
	CFont* m_pFontBody;
	CSize m_CharSizeHeader;
	CSize m_CharSizeFooter;
	CSize m_CharSizeBody;
	CSize m_pixelpantalla;
	int m_marginLeft;
	int m_marginTop;
	int m_marginRight;
	int m_marginBottom;
	int m_headerHeight;
	int m_headerLines;
	int m_footerHeight;
	int m_footerLines;
	int m_nPageCount;
	int m_nEspaciado;
};
