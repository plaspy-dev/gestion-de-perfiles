#include "StdAfx.h"
# include "JPrint.h"
#include "treelistprint.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTreeListPrint construction/destruction

CTreeListPrint::CTreeListPrint()
{	
	m_pTreeList= NULL;
}

CTreeListPrint::~CTreeListPrint()
{
}

/////////////////////////////////////////////////////////////////////////////
// CTreeListPrint printing

BOOL CTreeListPrint::OnPreparePrinting(CPrintInfo* pInfo, bool first)
{
   
	CPrintTemplate::OnPreparePrinting(pInfo, first);
	m_nRowCount = 0;
	m_nPageCols = 0;
	m_RatioX = 0; 
	m_nPageRows = 0;

   if(!pInfo->m_bPreview && first) {
	   
	   CPrintDialog dlg(false);
	   if( m_pTreeList->GetSelectedCount() && pInfo->m_pPD) 
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

int CTreeListPrint::GetPageCount(CDC *pDC) {

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

	ASSERT(m_pTreeList);
	return m_nPageCount;
}

void CTreeListPrint::DrawRow(CDC* pDC, int nRow, int real)
{
   for (int nCol = 0; nCol < m_nPageCols; nCol++)
   {
      CRect rect = GetCellRect(real,nCol);
	  CString strText = m_pTreeList->GetItemTextEx(nRow, nCol);
      DrawText(pDC->m_hDC, strText, -1, rect, DT_LEFT | DT_VCENTER);
   }
}

int CTreeListPrint::GetPageColumns()
{
   ASSERT(m_pTreeList);
   return m_pTreeList->GetColumnCount();
}

CString CTreeListPrint::GetColumnHeading(int col)
{
   ASSERT(m_pTreeList);
   return m_pTreeList->GetColumnText(col);
}

CRect CTreeListPrint::GetColumnRect(int nCol)
{
   ASSERT(nCol >= 0 && nCol < m_nPageCols);
   CSize charSize = m_CharSizeBody;
   CRect rectBody = m_rectBody;
   int nLeft=0, i=0;
   for (nLeft=0, i=0; i < nCol; i++) 
      nLeft += GetColumnWidth(i);
   int nRight = nLeft + GetColumnWidth(i);
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

CRect CTreeListPrint::GetCellRect(int nRow, int nCol)
{
   ASSERT(nCol >= 0 && nCol < m_nPageCols);
   ASSERT(nRow >= 0 && nRow < m_nRowCount);
   CSize charSize = m_CharSizeBody;
   CRect rectBody = m_rectBody;
   CRect rectCol = GetColumnRect(0);
   int nLeft=0, i=0; 
   for (nLeft=0, i=0; i < nCol; i++) 
      nLeft += GetColumnWidth(i);
   int nRight = nLeft + GetColumnWidth(i);   
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

int CTreeListPrint::GetPageRows()
{
   CSize charSize = m_CharSizeBody;
   CRect rectBody = m_rectBody;
   CRect rectCol = GetColumnRect(0);
   return (rectBody.Height() - rectCol.Height())/ charSize.cy;
}

int CTreeListPrint::GetColumnWidth(int col)
{
   ASSERT(m_pTreeList);
   return (int)((m_pTreeList->GetColumnWidth(col))* m_RatioX);
}

int CTreeListPrint::GetRowCount()
{
   ASSERT(m_pTreeList);
   return m_bPrintSelection ? m_pTreeList->GetSelectedCount() : m_pTreeList->GetItemCount();
}

double CTreeListPrint::GetTextRatioX(CDC* pDC)
{
	ASSERT(pDC != NULL);
	ASSERT(m_pTreeList);
	CDC* pCurrentDC = m_pTreeList->GetDC();
	TEXTMETRIC tmSrc;
	pCurrentDC->GetTextMetrics(&tmSrc);
	m_pTreeList->ReleaseDC(pCurrentDC);
	return ((double)m_CharSizeBody.cx) / ((double)tmSrc.tmAveCharWidth);
}

/////////////////////////////////////////////////////////////////////////////
// CTreeListPrint printing

int CTreeListPrint::GetStartRow(int page) {

	if( m_bPrintSelection) {
		int count= (page-1)*m_nPageRows;
		HTREELISTITEM item=NULL;
		while(item= m_pTreeList->GetNextSelectedItem(item)) {
			if(!count)
				break;
			count--;
		}
		return m_pTreeList->GetTreeListItemEx(item);
	} else {
		return (page-1)*m_nPageRows;
	}
}

int CTreeListPrint::GetNextRow(int row) {

	if( m_bPrintSelection)
		return m_pTreeList->GetTreeListItemEx(m_pTreeList->GetNextSelectedItem( m_pTreeList->GetTreeListItemEx(row)));
	else return row+1==m_nRowCount ? -1 : row+1;
}

void CTreeListPrint::PrintBody(CDC* pDC, CPrintInfo* pInfo)
{
   ASSERT(m_pFontBody && m_pFontColumn && pInfo);
   //ASSERT(m_pListView);

#ifdef _DEBUG
   DrawRect(pDC, m_rectBody,RGB(255,255,0));
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

void CTreeListPrint::SetTreeListCtrl(CTreeListCtrl* pTree)
{
   m_pTreeList = pTree;
}

