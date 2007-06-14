// AreasView.cpp : implementation file
//

#include "stdafx.h"
#include "Template.h"
#include "AreasView.h"


// CAreasView

IMPLEMENT_DYNCREATE(CAreasView, CFormView)

CAreasView::CAreasView()
	: CFormView(CAreasView::IDD)
{
	m_dwId=0;
}

CAreasView::~CAreasView()
{
}

void CAreasView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EAREA, m_EArea);
	DDX_Control(pDX, IDC_BAGREGAR, m_BAgregar);
	DDX_Control(pDX, IDC_LAREAS, m_LCAreas);
	DDX_Control(pDX, IDC_BCANCELAR, m_BCancelar);
	DDX_Control(pDX, IDC_SAREA, m_SArea);
}

BEGIN_MESSAGE_MAP(CAreasView, CFormView)
	ON_EN_CHANGE(IDC_EAREA, &CAreasView::OnEnChangeEarea)
	ON_BN_CLICKED(IDC_BAGREGAR, &CAreasView::OnBnClickedBagregar)
	ON_NOTIFY(NM_DBLCLK, IDC_LAREAS, &CAreasView::OnNMDblclkLareas)
	ON_NOTIFY(NM_RCLICK, IDC_LAREAS, &CAreasView::OnNMRclickLareas)
	ON_COMMAND(2000, OnEditar)
	ON_COMMAND(2001, OnEliminar)
	ON_MESSAGE(WM_MYSQL, OnMySQLMessage)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BCANCELAR, &CAreasView::OnBnClickedBcancelar)
END_MESSAGE_MAP()


// CAreasView diagnostics

#ifdef _DEBUG
void CAreasView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CAreasView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CAreasView message handlers

void CAreasView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetDocument()->SetTitle(CLanguage::GetTraduccion(IDS_AREAS));
	m_pConector= (CConector *)AfxGetMainWnd()->SendMessage(WM_QUERYCONECTOR);
	m_LCAreas.AddColumn(CLanguage::GetTraduccion(IDS_AREA));
	m_LCAreas.Mensaje=CLanguage::GetTraduccion(IDS_NOHAYAREAS);
	
	CQuery query("shared");

	query+="id";
	query+="nombre";

	m_pConector->Query(query.GetSelect("nReservado=3 AND (szReservado IS NULL OR szReservado!=-1)"), query_areas, this);
}

void CAreasView::OnEnChangeEarea()
{
	m_BAgregar.EnableWindow(!m_EArea.Text.IsEmpty());
}

void CAreasView::OnBnClickedBagregar()
{
	CQuery query("shared");
	CString temp;

	query["nombre"]= m_EArea.Text;

	if(m_dwId) {
		temp.Format("id=%d", m_dwId);
		for(int i=0; i<m_LCAreas.GetItemCount(); i++) {
			if(m_LCAreas.GetItemData(i)==m_dwId) {
				m_LCAreas.Items[i][0]= m_EArea.Text;
				break;
			}
		}
		m_pConector->Query(query.GetUpdate(temp));
	} else {
		m_LCAreas.AddItem(m_EArea.Text);
		query["nReservado"]="3";
		m_pConector->Query(query.GetInsert(), insert_area);
	}
	OnBnClickedBcancelar();
}

void CAreasView::OnNMDblclkLareas(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if( pNMListView->iItem == -1 || pNMListView->iSubItem == -1) 
		return;
	OnEditar();
	*pResult = 0;
}

void CAreasView::OnNMRclickLareas(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if( pNMListView->iItem == -1 || pNMListView->iSubItem == -1) 

		return;

	CNewMenu menu;
	menu.CreatePopupMenu();

	menu.AppendMenu(MF_STRING,2000,CLanguage::GetTraduccion(IDS_EDITAR));
	menu.AppendMenu(MF_SEPARATOR);
	menu.AppendMenu(MF_STRING,2001,CLanguage::GetTraduccion(IDS_ELIMINAR));
	menu.SetDefaultItem(2000, false);
	menu.AppendMenu(MF_SEPARATOR);
	m_LCAreas.GetMenuContextual(&menu);

	POINT point;
	GetCursorPos( &point );
	menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, AfxGetMainWnd());
	
	*pResult = 0;
}

void CAreasView::OnEditar() {

	int index=m_LCAreas.SelectedIndex;
	if(index==-1) 
		return;
	m_dwId= m_LCAreas.GetItemData(index);
	m_EArea.Text= m_LCAreas.Items[index][0];
	m_BAgregar.SetWindowText(CLanguage::GetTraduccion(IDS_EDITAR));
	SetError(&m_EArea, CLanguage::GetTraduccion(AFX_IDS_IDLEMESSAGE));
}

void CAreasView::OnEliminar() {

	if(m_LCAreas.SelectedIndex==-1)
		return;
	CString formato;
	formato.Format(CLanguage::GetTraduccion(IDS_ESTASEGURODEELIMINAR), m_LCAreas.Items[m_LCAreas.SelectedIndex][0]);
	if(MessageBox(formato, NULL, MB_YESNO |MB_ICONQUESTION)==IDNO)
		return;
	CQuery query("shared");
	query["szReservado"]="-1";
	formato.Format("id=%d", m_LCAreas.GetItemData(m_LCAreas.SelectedIndex));
	m_LCAreas.DeleteItem(m_LCAreas.SelectedIndex);
	m_pConector->Query(query.GetUpdate(formato));
}

LRESULT CAreasView::OnMySQLMessage(WPARAM wParam, LPARAM lParam) {
	
	CMySQLMessage query(lParam);
	switch(wParam)
	{
	case QUERY_RESULT:
		if(query.GetID()==query_areas) {
			m_LCAreas.SetItemData(m_LCAreas.AddItem(query["nombre"]),atoi(query["id"]));
		} else if(query.GetID()==insert_area) {
			for(int i=0; i<m_LCAreas.GetItemCount(); i++) {
				if(!m_LCAreas.GetItemData(i)) {
					m_LCAreas.SetItemData(i, atoi(query[0]));
					break;
				}
			}
		}
		break;
	}
	return 0;
}

void CAreasView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);
	if(::IsWindow(m_LCAreas)) {
		CRect rect;
		m_LCAreas.GetWindowRect(rect);
		ScreenToClient(rect);
		rect.bottom=cy-10;
		rect.right=cx-10;
		m_LCAreas.MoveWindow(rect);
	}
}

void CAreasView::OnBnClickedBcancelar()
{
	m_dwId=0;
	m_EArea.Text="";
	SetError(&m_EArea, CLanguage::GetTraduccion(AFX_IDS_IDLEMESSAGE));
	m_BAgregar.SetWindowText(CLanguage::GetTraduccion(IDS_AGREGAR));
}
