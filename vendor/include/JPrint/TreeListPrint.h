#pragma once

# include "componentes/componentes.h"
# include "PrintTemplate.h"

class  __JPRINT_EXPORT__ CTreeListPrint: public CPrintTemplate
{
// Construction
public:
   CTreeListPrint();
   virtual ~CTreeListPrint();

// Operations
public:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo, bool first=true);

// Implementation
public:
	void SetTreeListCtrl(CTreeListCtrl* pTree);
   int GetRowCount();

protected:
	virtual void PrintBody(CDC* pDC, CPrintInfo* pInfo);
	virtual int GetPageCount(CDC *pDC);

   void DrawRow(CDC* pDC, int nRow, int real);

   double GetTextRatioX(CDC* pDC);
   CRect GetCellRect(int nRow, int nCol);
   CRect GetColumnRect(int nCol);
   CString GetColumnHeading(int col);
   int GetColumnWidth(int col);
	int GetPageColumns();
   int GetPageRows();
   int GetStartRow(int page);
   int GetNextRow(int row);

protected:   
   CTreeListCtrl *m_pTreeList;
   int m_nRowCount;
   int m_nPageCols;
   double m_RatioX;
   int	m_nPageRows;
};
