// ListDemoViewPrint.h : interface of the CListDemoViewPrint class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

# include "componentes/componentes.h"
# include "PrintTemplate.h"

class  __JPRINT_EXPORT__ CListDemoViewPrint : public CPrintTemplate
{
// Construction
public:
   CListDemoViewPrint();
   virtual ~CListDemoViewPrint();

// Operations
public:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo, bool first=true);

// Implementation
public:
	void SetListCtrl(CMyListCtrl* pList);
   int GetRowCount();

protected:

	virtual void PrintBody(CDC* pDC, CPrintInfo* pInfo);
	virtual int GetPageCount(CDC *pDC);

   void DrawRow(CDC* pDC, int nRow, int real);
   double GetTextRatioX(CDC* pDC);
   CRect GetCellRect(int nRow, int nCol);
   CRect GetColumnRect(int nCol);
   CString GetColumnHeading(int col);
   int GetStartRow(int page);
   inline int GetNextRow(int row);
   int GetColumnWidth(int col);
	int GetPageColumns();
   int GetPageRows();

protected:
   CMyListCtrl *m_pListCtrl;
   int m_nRowCount;
   int m_nPageCols;
   double m_RatioX;
   int	m_nPageRows;
   int m_nColumns[100]; 
};

