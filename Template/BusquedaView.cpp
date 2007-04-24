// BusquedaView.cpp : implementation file
//

#include "stdafx.h"
#include "Template.h"
#include "BusquedaView.h"


// CBusquedaView

IMPLEMENT_DYNCREATE(CBusquedaView, CFormView)

CBusquedaView::CBusquedaView()
	: CFormView(CBusquedaView::IDD)
{
	m_pListPrint=NULL;
}

CBusquedaView::~CBusquedaView()
{
	if( m_pListPrint) 
		delete m_pListPrint;
}

void CBusquedaView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ENOMBRE, m_ENombre);
	DDX_Control(pDX, IDC_BBUSCAR, m_BBuscar);
	DDX_Control(pDX, IDC_LRESULTADOS, m_LCBuscar);
}

BEGIN_MESSAGE_MAP(CBusquedaView, CFormView)
	ON_BN_CLICKED(IDC_BBUSCAR, &CBusquedaView::OnBnClickedBbuscar)
	ON_MESSAGE(WM_MYSQL, OnMySQLMessage)
	ON_WM_SIZE()
	ON_NOTIFY(NM_DBLCLK, IDC_LRESULTADOS, &CBusquedaView::OnNMDblclkLresultados)
	ON_NOTIFY(NM_RCLICK, IDC_LRESULTADOS, &CBusquedaView::OnNMRclickLresultados)
	ON_COMMAND(2000,OnVer)
	ON_COMMAND(ID_FILE_SAVE, OnGuardar)
	ON_COMMAND(ID_FILE_SAVE_AS, OnGuardar)
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
END_MESSAGE_MAP()


// CBusquedaView diagnostics

#ifdef _DEBUG
void CBusquedaView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CBusquedaView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CBusquedaView message handlers

void CBusquedaView::OnBnClickedBbuscar()
{
	if(m_ENombre.Text.IsEmpty()) {
		SetErrorEx(&m_ENombre, "Por favor ingrese un termino de busqueda", 2, "busqueda");
		return ;
	}

	m_pConector->KillQuery(1, this);
	m_LCBuscar.DeleteAllItems();
	m_LCBuscar.Mensaje="Buscando";

	CQuery query("usuario");

	query+="pkz";
	query+="nombre";
	query+="apellido";
	query+="sapellido";

	CString temp;
	temp="pkz LIKE '%$$%' OR pkz LIKE '%$$%' OR pkz LIKE '%$$%' OR pkz LIKE '%$$%'";
	temp.Replace("$$", m_ENombre.Text);

	m_pConector->Query(query.GetSelect(temp), 1, this);
}

void CBusquedaView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	GetDocument()->SetTitle("Busqueda");
	m_pConector= (CConector *)AfxGetMainWnd()->SendMessage(WM_QUERYCONECTOR);
	
	m_LCBuscar.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	
	m_LCBuscar.AddColumn("PKZ");
	m_LCBuscar.AddColumn("Nombre");
	m_LCBuscar.AddColumn("Apellido");
	m_LCBuscar.AddColumn("Segundo apellido");

	m_LCBuscar.Mensaje="Ingrese un termino de busqueda";
}

LRESULT CBusquedaView::OnMySQLMessage(WPARAM wParam ,LPARAM lParam) {

	CMySQLMessage query(lParam);
	switch(wParam) {
		case QUERY_RESULT:
			if(query.GetID()==1) {
			
				int pos= m_LCBuscar.AddItem(query["pkz"]);
				m_LCBuscar.Items[pos][1]=query["nombre"];
				m_LCBuscar.Items[pos][2]=query["apellido"];
				m_LCBuscar.Items[pos][3]=query["sapellido"];
			}
			break;
		case THREAD_END:
			m_LCBuscar.AutoSizeColumns();
			m_LCBuscar.Mensaje="No se encontraron resultados";
			break;
	}
	return 0;
}
void CBusquedaView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);
	if(::IsWindow(m_LCBuscar)) {
		CRect rect;
		m_LCBuscar.GetWindowRect(rect);
		ScreenToClient(rect);
		rect.bottom= cy-10;
		rect.right=cx-10;
		m_LCBuscar.MoveWindow(rect);
	}
}

void CBusquedaView::OnVer() {

	int index= m_LCBuscar.SelectedIndex;
	if(index==-1) 
		return;
	CString temp= m_LCBuscar.Items[index][0];
	AfxGetMainWnd()->SendMessage(WM_TREENAVEGADOR, 100,	(LPARAM)(const char *)"CTemplateView");
	((CMDIFrameWndXP *) AfxGetMainWnd())->SendMessageBroadCast(WM_INSERTPKZ, 0, (LPARAM)(const char *)temp);
}

void CBusquedaView::OnNMDblclkLresultados(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if( pNMListView->iItem == -1 || pNMListView->iSubItem == -1) 
		return;
	OnVer();
	*pResult = 0;
}

void CBusquedaView::OnNMRclickLresultados(NMHDR *pNMHDR, LRESULT *pResult)
{
	
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if( pNMListView->iItem == -1 || pNMListView->iSubItem == -1) 

		return;

	CNewMenu menu;
	menu.CreatePopupMenu();

	menu.AppendMenu(MF_STRING,2000,"Ver &Usuario");
	menu.SetDefaultItem(2000, false);
	menu.AppendMenu(MF_SEPARATOR);
	m_LCBuscar.GetMenuContextual(&menu);

	POINT point;
	GetCursorPos( &point );
	menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, AfxGetMainWnd());

	*pResult = 0;
}

void CBusquedaView::OnGuardar() {

	Exportar(&m_LCBuscar, "Resultados", true);
}

BOOL CBusquedaView::OnPreparePrinting(CPrintInfo* pInfo)
{

	if( m_pListPrint)
		delete m_pListPrint;
	m_pListPrint= new CListDemoViewPrint();

	m_pListPrint->SetListCtrl(&m_LCBuscar);
	m_pListPrint->SetAppName(CApplicacion::GetAppName() + " - " + CApplicacion::GetCompany());
	m_pListPrint->SetDocTitle(GetDocument()->GetTitle());

	CMyTime tiempo(*(CTime *)AfxGetMainWnd()->SendMessage(WM_QUERYTIME));

	m_pListPrint->SetFooter(tiempo.GetFechaHora());
	m_pListPrint->OnPreparePrinting(pInfo);

	m_pListPrint->SetMarginBottomPix(AfxGetApp()->GetProfileInt("printer", "cy", 20));
	m_pListPrint->SetMarginLeftPix(AfxGetApp()->GetProfileInt("printer", "x", 20));
	m_pListPrint->SetMarginRightPix(AfxGetApp()->GetProfileInt("printer", "cx", 20));
	m_pListPrint->SetMarginTopPix(AfxGetApp()->GetProfileInt("printer", "y", 20));

	return DoPreparePrinting(pInfo);
}

void CBusquedaView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	m_pListPrint->OnBeginPrinting(pDC, pInfo);
}

void CBusquedaView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	m_pListPrint->OnPrint(pDC, pInfo);
	CFormView::OnPrint(pDC, pInfo);
}

void CBusquedaView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	m_pListPrint->OnEndPrinting(pDC, pInfo);
	delete m_pListPrint;
	m_pListPrint= NULL;
}
