// ListDemoView.cpp : implementation of the CListDemoViewPrint class
//

#include "stdafx.h"
# include "JPrint.h"
#include "ListDemoViewPrint.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CListDemoViewPrint construction/destruction

CListDemoViewPrint::CListDemoViewPrint()
{	
   m_pListCtrl = NULL;
}

CListDemoViewPrint::~CListDemoViewPrint()
{
}

/////////////////////////////////////////////////////////////////////////////
// CListDemoViewPrint printing

BOOL CListDemoViewPrint::OnPreparePrinting(CPrintInfo* pInfo, bool first)
{

	CPrintTemplate::OnPreparePrinting(pInfo, first);
	m_nRowCount = 0;
	m_nPageCols = 0;
	m_RatioX = 0; 
	m_nPageRows = 0;
	ZeroMemory(m_nColumns,sizeof(m_nColumns));

   if(!pInfo->m_bPreview && first) {
	   
	   CPrintDialog dlg(false);
	   if( m_pListCtrl->GetSelectedCount() && pInfo->m_pPD)
			pInfo->m_pPD->m_pd.Flags&= ~PD_NOSELECTION;
	   if(dlg.GetDefaults()) {
		   HDC hDC= dlg.GetPrinterDC();
		   if( hDC) {
				CDC dc;
				dc.Attach(hDC);
				pInfo->SetMinPage(1);
				OnBeginPrinting(&dc, pInfo);
				dc.Detach();
				OnPreparePrinting(pInfo, false);
		   }
	   }
   }
	return TRUE;
}

int CListDemoViewPrint::GetPageCount(CDC *pDC) {

	m_RatioX = GetTextRatioX(pDC);
	m_nRowCount = GetRowCount();
	m_nPageCols = GetPageColumns();
	m_nPageRows = GetPageRows();
	int mas=0;
	if( !m_strFinal.IsEmpty()) {

		mas=2;
		for( int i=0; i<m_strFinal.GetLength(); i++)
			if(m_strFinal[i]=='\n')
				mas++;
	}
	m_nRowCount+= mas;
	m_nPageCount = (m_nRowCount + m_nPageRows - 1) / m_nPageRows;
	m_nRowCount-= mas;
	ASSERT(m_pListCtrl);
	m_pListCtrl->GetColumnOrderArray(m_nColumns);
	return m_nPageCount;
}
/////////////////////////////////////////////////////////////////////////////
// CListDemoViewPrint helpers

void CListDemoViewPrint::DrawRow(CDC* pDC, int nRow, int real)
{
   for (int nCol = 0; nCol < m_nPageCols; nCol++)
   {
      CRect rect = GetCellRect(real,nCol);
      CString strText = m_pListCtrl->GetItemText(nRow, m_nColumns[nCol]);
      DrawText(pDC->m_hDC, strText, -1, rect, DT_LEFT | DT_VCENTER);
   }
}

int CListDemoViewPrint::GetPageColumns()
{
   ASSERT(m_pListCtrl);
   CHeaderCtrl* pHeaderCtrl = m_pListCtrl->GetHeaderCtrl();
   ASSERT(pHeaderCtrl != NULL);
   return pHeaderCtrl->GetItemCount();
}

CString CListDemoViewPrint::GetColumnHeading(int col)
{
   ASSERT(m_pListCtrl);
   CHeaderCtrl* pHeaderCtrl = m_pListCtrl->GetHeaderCtrl();
   ASSERT(pHeaderCtrl != NULL);
   char Buffer[1024];

   HDITEM  phi;
   phi.mask = HDI_TEXT;
   phi.cchTextMax = sizeof(Buffer);
   phi.pszText = Buffer;
   pHeaderCtrl->GetItem(m_nColumns[col], &phi);
   return CString(Buffer);
}

CRect CListDemoViewPrint::GetColumnRect(int nCol)
{
   ASSERT(nCol >= 0 && nCol < m_nPageCols);
   CSize charSize = m_CharSizeBody;
   CRect rectBody = m_rectBody;
   int nLeft=0, i=0;
   for (nLeft=0, i=0; i < nCol; i++) 
      nLeft += GetColumnWidth(m_nColumns[i]);
   int nRight = nLeft + GetColumnWidth(m_nColumns[i]);
   CRect rect(
      rectBody.left + nLeft, 
      rectBody.top, 
      rectBody.left + nRight, 
      rectBody.top + charSize.cy);
   if (rect.right > rectBody.right)
      rect.right = rectBody.right;
   rect.left += (int)(m_RatioX);
   return rect;
}

CRect CListDemoViewPrint::GetCellRect(int nRow, int nCol)
{
   ASSERT(nCol >= 0 && nCol < m_nPageCols);
   CSize charSize = m_CharSizeBody;
   CRect rectBody = m_rectBody;
   CRect rectCol = GetColumnRect(0);
   int nLeft=0, i=0; 
   for (nLeft=0, i=0; i < nCol; i++) 
      nLeft += GetColumnWidth(m_nColumns[i]);
   int nRight = nLeft + GetColumnWidth(m_nColumns[i]);   
   int nPageRow =  nRow % m_nPageRows;
   ASSERT(nPageRow <= m_nPageRows);
   CRect rect(
      rectBody.left + nLeft, 
      rectBody.top + rectCol.Height() + charSize.cy * nPageRow, 
      rectBody.left + nRight, 
      rectBody.top + rectCol.Height() + charSize.cy * (nPageRow + 1));
   if (rect.right > rectBody.right)
      rect.right = rectBody.right;
   rect.left += (int)(m_RatioX);
   return rect;
}

int CListDemoViewPrint::GetPageRows()
{
   CSize charSize = m_CharSizeBody;
   CRect rectBody = m_rectBody;
   CRect rectCol = GetColumnRect(0);
   return (rectBody.Height() - rectCol.Height())/ charSize.cy;
}

int CListDemoViewPrint::GetColumnWidth(int col)
{
   ASSERT(m_pListCtrl);
   CHeaderCtrl* pHeaderCtrl = m_pListCtrl->GetHeaderCtrl();
   ASSERT(pHeaderCtrl != NULL);
   HDITEM  hi;
   hi.mask = HDI_WIDTH;
   pHeaderCtrl->GetItem(col, &hi);
   return (int)((hi.cxy)* m_RatioX);
}

double CListDemoViewPrint::GetTextRatioX(CDC* pDC)
{
   ASSERT(pDC != NULL);
   ASSERT(m_pListCtrl);
   CDC* pCurrentDC = m_pListCtrl->GetDC();
   TEXTMETRIC tmSrc;
   pCurrentDC->GetTextMetrics(&tmSrc);
   m_pListCtrl->ReleaseDC(pCurrentDC);
   return ((double)m_CharSizeBody.cx) / ((double)tmSrc.tmAveCharWidth);
}

int CListDemoViewPrint::GetRowCount()
{
   ASSERT(m_pListCtrl);
   return m_bPrintSelection ? m_pListCtrl->GetSelectedCount() : m_pListCtrl->GetItemCount();
}

/////////////////////////////////////////////////////////////////////////////
// CListDemoViewPrint printing

int CListDemoViewPrint::GetStartRow(int page) {

	if( m_bPrintSelection) {
		int count= (page-1)*m_nPageRows;
		int index=-1;
		while((index=m_pListCtrl->GetNextItem(index, LVIS_SELECTED | LVNI_ALL))!=-1) {
			if(!count)
				break;
			count--;
		}
		return index;
	} else {
		return (page-1)*m_nPageRows;
	}
}

int CListDemoViewPrint::GetNextRow(int row) {

	if( m_bPrintSelection)
		return m_pListCtrl->GetNextItem(row, LVIS_SELECTED);
	else return row+1==m_nRowCount ? -1 : row+1;
}

void CListDemoViewPrint::PrintBody(CDC* pDC, CPrintInfo* pInfo)
{
	CPrintTemplate::PrintBody(pDC, pInfo);
   //ASSERT(m_pListView);

#ifdef _DEBUG
   for (int j=0; j < m_nPageCols; j++)
      DrawRect(pDC, GetColumnRect(j),RGB(128,64,0));
#endif

   // Print column headings
   CFont *pOldFont = pDC->SelectObject(m_pFontColumn); 

   for (int i = 0; i < m_nPageCols; i++)
      DrawText(pDC->m_hDC, GetColumnHeading(i), -1, GetColumnRect(i), DT_LEFT | DT_VCENTER);

   // Start and end row
   pDC->SelectObject(m_pFontBody);

   int nStartRow = GetStartRow(pInfo->m_nCurPage);
   // int nEndRow = nStartRow + m_nPageRows;
   int index=nStartRow;
   int count= 0;
   if( index!=-1) {
	do {
		DrawRow(pDC, index, count);
		count++;
		if( count==m_nPageRows)
			break;
	} while( (index=GetNextRow(index))!=-1);
   }
   pDC->SelectObject(pOldFont);
}

void CListDemoViewPrint::SetListCtrl(CMyListCtrl* pList)
{
   ASSERT(pList != NULL);
   m_pListCtrl = pList;
}
