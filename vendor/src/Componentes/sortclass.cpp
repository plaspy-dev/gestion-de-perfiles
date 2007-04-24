# include "stdafx.h"
# include "SortClass.h"
# include "EditFloat.h"
# include "ItemList.h"
# include "vlistctrl.h"

CSortClass::CSortClass(CMyListCtrl * _pWnd, const int _iCol)
{
	ASSERT(_pWnd);

	pWnd = _pWnd->IsKindOf(RUNTIME_CLASS(CVListCtrl)) ? NULL : _pWnd;
	vpWnd = _pWnd->IsKindOf(RUNTIME_CLASS(CVListCtrl)) ? (CVListCtrl *) _pWnd : NULL;

	int max = pWnd ? pWnd->GetItemCount() : vpWnd->GetItemCount();
	DWORD dw;
	CString txt;

	for (int t = 0; t < max; t++)
	{
		dw = pWnd ? pWnd->GetItemData(t) : vpWnd->GetItemData(t); // save current data to restore it later
		txt = pWnd ? pWnd->GetItemText(t, _iCol) : vpWnd->GetItemText(t, _iCol); 
		pWnd ? pWnd->SetItemData(t, (DWORD) new CSortItem(dw, txt)) : vpWnd->SetItemData(t, (DWORD) new CSortItem(dw, txt));
	}
}

CSortClass::~CSortClass()
{
	int max = pWnd ? pWnd->GetItemCount() : vpWnd->GetItemCount();
	CSortItem * pItem;
	CString temp;
	int nosort= pWnd ? pWnd->GetNoSort() : vpWnd->GetNoSort();

	for (int t = 0; t < max; t++)
	{
		pItem = (CSortItem *) (pWnd ? pWnd->GetItemData(t) : vpWnd->GetItemData(t));
		ASSERT(pItem);
		pWnd ? pWnd->SetItemData(t, pItem->dw) : vpWnd->SetItemData(t, pItem->dw);
		delete pItem;
		if( nosort!=-1) {
			temp.Format("%d", t+1);
			pWnd ? pWnd->SetItemText(t, nosort, temp) : vpWnd->SetItemText(t, nosort, temp);
		}
	}
}

int CompareQ(void *param, const void *item1, const void *item2) {

	
	CItemList *pItem1= *(CItemList **)item1;
	CItemList *pItem2= *(CItemList **)item2;

	return CSortClass::Compare(pItem1->m_dwData, pItem2->m_dwData, (LPARAM)param);
}

void CSortClass::Sort(bool _bAsc, int tipo)
{
	long lParamSort = tipo;

	// if lParamSort positive - ascending sort order, negative - descending
	if (!_bAsc)
		lParamSort *= -1;
	if(vpWnd) {
		CPtrArray *items= vpWnd->GetItems();
		qsort_s(items->GetData(),items->GetCount(), sizeof(void *), CompareQ, (void *)lParamSort);
		vpWnd->Invalidate();
		return;
	}
	pWnd->SortItems(Compare, lParamSort);
}


int CALLBACK CSortClass::Compare(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CSortItem * item1 = (CSortItem *) lParam1;
	CSortItem * item2 = (CSortItem *) lParam2;
	ASSERT(item1 && item2);

	// restore data type and sort order from lParamSort
	// if lParamSort positive - ascending sort order, negative - descending
	short   sOrder = lParamSort < 0 ? -1 : 1; 
	int dType  = (lParamSort * sOrder);
	if( item1->txt.IsEmpty() || item2->txt.IsEmpty())
		dType= 0; // texto

	// declare typed buffers
	COleDateTime t1, t2;

	switch (dType)
	{
	case 2: // numero
		{
			return (atol(item1->txt) - atol(item2->txt))*sOrder;
		}
	case 3: // double
		{
			return (atof(CEditFloat::GetNormal(item1->txt)) < atof(CEditFloat::GetNormal(item2->txt)) ? -1 : 1)*sOrder;
		}
	case 6:
	case 4: // fecha
		{
			if (t1.ParseDateTime(item1->txt) && t2.ParseDateTime(item2->txt))
				return (t1 < t2 ? -1 : 1 )*sOrder;
			else
				return 0;
		}
	default: // texto
		return item1->txt.CompareNoCase(item2->txt)*sOrder;
	}
}

CSortClass::CSortItem::CSortItem(const DWORD _dw, const CString & _txt)
{
	dw  = _dw;
	txt = _txt;
}
