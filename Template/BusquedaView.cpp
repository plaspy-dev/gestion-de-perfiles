// BusquedaView.cpp : implementation file
//

#include "stdafx.h"
#include "Template.h"
#include "BusquedaView.h"
# include "IndexarDlg.h"

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
	DDX_Control(pDX, IDC_EBUSQUEDA, m_EBusqueda);
	DDX_Control(pDX, IDC_BBUSCAR, m_BBuscar);
	DDX_Control(pDX, IDC_LRESULTADOS, m_LCBuscar);
	DDX_Control(pDX, IDC_CAREA, m_CBArea);
	DDX_Control(pDX, IDC_BINDEXAR, m_BIndexar);
	DDX_Control(pDX, IDC_SAVANZADO, m_SAvanzado);
	DDX_Control(pDX, IDC_ENOMBRE, m_ENombre);
	DDX_Control(pDX, IDC_EAPELLIDO, m_EApellido);
	DDX_Control(pDX, IDC_CPAIS, m_CBPais);
	DDX_Control(pDX, IDC_CBCIUDAD, m_CBCiudad);
	DDX_Control(pDX, IDC_EINTITUCION, m_EInstitucion);
	DDX_Control(pDX, IDC_CBUNIVERSIDAD, m_CBUniversidad);
	DDX_Control(pDX, IDC_SBUSQUEDA, m_SBusqueda);
	DDX_Control(pDX, IDC_SRESULTADOS, m_SFin);
	DDX_Control(pDX, IDC_CTIPO, m_CBTipo);
	DDX_Control(pDX, IDC_CTODOS, m_CTodos);
	DDX_Control(pDX, IDC_CTODOSA, m_CTodosA);
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
	ON_BN_CLICKED(IDC_BINDEXAR, &CBusquedaView::OnBnClickedBindexar)
	ON_STN_CLICKED(IDC_SAVANZADO, &CBusquedaView::OnStnClickedSavanzado)
	ON_STN_CLICKED(IDC_SBUSQUEDA, &CBusquedaView::OnStnClickedBusqueda)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_CTODOS, OnChangeTodos)
	ON_BN_CLICKED(IDC_CTODOSA, OnChangeTodosA)
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
CString CBusquedaView::GetCampoReal(CString campo) {

	if(campo=="usuario.sexo") {
		campo.Format("IF(usuario.sexo=1, '%s', IF(usuario.sexo=2, '%s', ''))", CLanguage::GetTraduccion(IDS_MASCULINO), CLanguage::GetTraduccion(IDS_FEMENINO));
	} else if(campo=="usuario.correspondencia") {
		campo.Format("IF(usuario.correspondencia=1, '%s', IF(usuario.correspondencia=2, '%s', ''))", CLanguage::GetTraduccion(IDS_OFICINA), CLanguage::GetTraduccion(IDS_CASA));
	} else if(campo=="empresa.tipo") {
		campo="tipoempresa.nombre";
	} else if(campo=="estudios.parea") {
		campo="estudiosparea.nombre";
	} else if(campo=="estudios.marea") {
		campo="estudiosmarea.nombre";
	} else if(campo=="estudios.darea") {
		campo="estudiosdarea.nombre";
	} else if(campo=="estudios.oarea") {
		campo="estudiosoarea.nombre";
	} else if(campo=="estudiosfuera.parea") {
		campo="estudiospareaf.nombre";
	} else if(campo=="estudiosfuera.marea") {
		campo="estudiosmareaf.nombre";
	} else if(campo=="estudiosfuera.darea") {
		campo="estudiosdareaf.nombre";
	} else if(campo=="estudiosfuera.oarea") {
		campo="estudiosoareaf.nombre";
	} else if(campo=="usuario.fallecido") {
		campo.Format("IF(usuario.fallecido=1, 'X','')");
	} else if(campo=="educacion.becario") {
		campo.Format("IF(educacion.becario=1, 'X','')");
	}
	return campo;
}

void CBusquedaView::OnBuscar(bool avanzado, bool todo/*=false*/) {

	m_pConector->KillQuery(query_resultados, this);
	m_LCBuscar.DeleteAllItems();
	m_LCBuscar.Mensaje=CLanguage::GetTraduccion(IDS_BUSCANDO);
	while(m_LCBuscar.GetColumnCount()) {
		m_LCBuscar.DeleteColumn(0);
	}

	CString left=" LEFT JOIN empresa ON (usuario.pkz=empresa.pkz) \
				 LEFT JOIN residencia ON (usuario.pkz=residencia.pkz) \
				 LEFT JOIN busqueda ON (usuario.pkz=busqueda.pkz)";

	CQuery query("usuario");

	if(todo) {
		
		XmlReader xml;
		xml.Open(CApplicacion::GetAppPath(CLanguage::GetTraduccion(IDS_ALIAS)));
		CString szName,szValue;
		CString szTabla,szCampo;
		bool entro=false;
		CQuery *actual=NULL, *alias=NULL;

		while(xml.ReadString()) {
			switch(xml.GetNodeType()) {
			case NODETYPE_CONTENT:
				xml.GetNodeName(szName);
				xml.GetNodeContent(szValue);
				szValue.Trim();
				if(szValue.IsEmpty())
					break;
				if(szName=="tabla") {
					szTabla= szValue;
				} else if(szName=="nombre") {
					szCampo= szValue;
				} else if(szName=="alias") {
					query[szValue]= GetCampoReal(szTabla + "." + szCampo);
					entro=true;
					if(szTabla!="usuario") {
						CString temporal= "LEFT JOIN "+ szTabla +" ON (usuario.pkz=" + szTabla+ ".pkz)";
						if(left.Find(temporal)==-1)
							left+=" "+temporal +" ";
					}
				}
				break;
			}
		}
		if(!entro) {
			szValue.Format(CLanguage::GetTraduccion(IDS_NOSEPUEDECARGARARCHIVO),CApplicacion::GetAppPath(CLanguage::GetTraduccion(IDS_ALIAS)));
			MessageBox(szValue, NULL, MB_ICONERROR);
			return;
		}
		left+=" LEFT JOIN shared AS tipoempresa ON (empresa.tipo=tipoempresa.id) ";
		left+=" LEFT JOIN shared AS estudiosparea ON (estudios.parea=estudiosparea.id) ";
		left+=" LEFT JOIN shared AS estudiosmarea ON (estudios.marea=estudiosmarea.id) ";
		left+=" LEFT JOIN shared AS estudiosdarea ON (estudios.darea=estudiosdarea.id) ";
		left+=" LEFT JOIN shared AS estudiosoarea ON (estudios.oarea=estudiosoarea.id) ";
		left+=" LEFT JOIN shared AS estudiospareaf ON (estudiosfuera.parea=estudiospareaf.id) ";
		left+=" LEFT JOIN shared AS estudiosmareaf ON (estudiosfuera.marea=estudiosmareaf.id) ";
		left+=" LEFT JOIN shared AS estudiosdareaf ON (estudiosfuera.darea=estudiosdareaf.id) ";
		left+=" LEFT JOIN shared AS estudiosoareaf ON (estudiosfuera.oarea=estudiosoareaf.id) ";

	} else {
		query["pkz"]="usuario.pkz";
		query["nombre"]="usuario.nombre";
		query["apellido"]="usuario.apellido";
		query["sapellido"]="usuario.sapellido";
		query["toficina"]="residencia.telefono";
		query["tempresa"]="empresa.telefono";


		m_LCBuscar.AddColumn(CLanguage::GetTraduccion(IDS_PKZ));
		m_LCBuscar.AddColumn(CLanguage::GetTraduccion(IDS_NOMBRE));
		m_LCBuscar.AddColumn(CLanguage::GetTraduccion(IDS_APELLIDO));
		m_LCBuscar.AddColumn(CLanguage::GetTraduccion(IDS_SAPELLIDO));
		m_LCBuscar.AddColumn(CLanguage::GetTraduccion(IDS_TELEFONOCASA));
		m_LCBuscar.AddColumn(CLanguage::GetTraduccion(IDS_TELEFONOOFICINA));
	}

	CString temp;
	CString orderby;
	CString area;

	if(avanzado) {

		if(!m_EBusqueda.Text.IsEmpty()) {
			temp="MATCH (busqueda.pkz, busqueda.texto) AGAINST ('$$' IN BOOLEAN MODE)";
			CString cadena= m_EBusqueda.Text;
			cadena.Trim();
			if(cadena.Find("\"")==-1) {
				cadena= "+" + cadena;
				cadena.Replace(" ", " +");
			}
			temp.Replace("$$", cadena);
		}
	} else { // busqueda avanzada

		if(!m_ENombre.Text.IsEmpty()) {
			temp+=" ( ";
			temp+="usuario.nombre LIKE '%" + m_ENombre.Text + "%'";
			temp+=" ) ";
		}

		if(!m_EApellido.Text.IsEmpty()) {
			if(!temp.IsEmpty())
				temp+=" AND ";
			temp+=" ( ";
			temp+="usuario.apellido LIKE '%" + m_EApellido.Text + "%'";
			temp+=" OR ";
			temp+="usuario.sapellido LIKE '%" + m_EApellido.Text + "%'";
			temp+=" ) ";
		}

		if(!m_CBPais.Text.IsEmpty() && m_CBPais.SelectedData!=-1) {
			if(!temp.IsEmpty())
				temp+=" AND ";
			temp+=" ( ";
			temp+="usuario.nacionalidad LIKE '%" + m_CBPais.Text + "%'";
			temp+=" OR ";
			temp+="residencia.pais LIKE '%" + m_CBPais.Text + "%'";
			temp+=" OR ";
			temp+="empresa.pais LIKE '%" + m_CBPais.Text + "%'";
			temp+=" ) ";
		}

		if(!m_CBCiudad.Text.IsEmpty() && m_CBCiudad.SelectedData!=-1) {
			if(!temp.IsEmpty())
				temp+=" AND ";
			temp+=" ( ";
			temp+="residencia.ciudad LIKE '%" + m_CBCiudad.Text + "%'";
			temp+=" OR ";
			temp+="empresa.ciudad LIKE '%" + m_CBCiudad.Text + "%'";
			temp+=" ) ";
		}

		if(!m_EInstitucion.Text.IsEmpty()) {
			if(!temp.IsEmpty())
				temp+=" AND ";
			temp+=" ( ";
			temp+="empresa.nombre LIKE '%" + m_EInstitucion.Text + "%'";
			temp+=" OR ";
			temp+="empresa.cargo LIKE '%" + m_EInstitucion.Text + "%'";
			temp+=" OR ";
			temp+="empresa.direccion LIKE '%" + m_EInstitucion.Text + "%'";
			temp+=" ) ";
		}

		if(!m_CBUniversidad.Text.IsEmpty() && m_CBUniversidad.SelectedData!=-1) {

			if(!temp.IsEmpty())
				temp+=" AND ";
			temp+=" ( ";
			temp+="estudios.puniversidad LIKE '%" + m_CBUniversidad.Text + "%'";
			temp+=" OR ";
			temp+="estudios.muniversidad LIKE '%" + m_CBUniversidad.Text + "%'";
			temp+=" OR ";
			temp+="estudios.duniversidad LIKE '%" + m_CBUniversidad.Text + "%'";
			temp+=" OR ";
			temp+="estudios.ouniversidad LIKE '%" + m_CBUniversidad.Text + "%'";
			temp+=" OR ";
			temp+="estudiosfuera.puniversidad LIKE '%" + m_CBUniversidad.Text + "%'";
			temp+=" OR ";
			temp+="estudiosfuera.muniversidad LIKE '%" + m_CBUniversidad.Text + "%'";
			temp+=" OR ";
			temp+="estudiosfuera.duniversidad LIKE '%" + m_CBUniversidad.Text + "%'";
			temp+=" OR ";
			temp+="estudiosfuera.ouniversidad LIKE '%" + m_CBUniversidad.Text + "%'";
			temp+=" ) ";

			if(left.Find("LEFT JOIN estudios ON (usuario.pkz=estudios.pkz)")==-1)
				left+=" LEFT JOIN estudios ON (usuario.pkz=estudios.pkz) ";
			if(left.Find("LEFT JOIN estudiosfuera ON (usuario.pkz=estudiosfuera.pkz)")==-1)
				left+=" LEFT JOIN estudiosfuera ON (usuario.pkz=estudiosfuera.pkz) ";
		}

		if(m_CBTipo.SelectedIndex!=-1 && m_CBTipo.SelectedData!=-1) {
			if(!temp.IsEmpty())
				temp+=" AND ";
			temp+=" ( ";
			CString data;
			data.Format("%d", m_CBTipo.SelectedData);
			temp+="empresa.tipo='" + data + "'";
			temp+=" ) ";
		}

		if(m_CBArea.SelectedIndex!=-1 && m_CBArea.SelectedData!=-1) {

			if(left.Find("LEFT JOIN estudios ON (usuario.pkz=estudios.pkz)")==-1)
				left+=" LEFT JOIN estudios ON (usuario.pkz=estudios.pkz) ";
			if(left.Find("LEFT JOIN estudiosfuera ON (usuario.pkz=estudiosfuera.pkz)")==-1)
				left+=" LEFT JOIN estudiosfuera ON (usuario.pkz=estudiosfuera.pkz) ";

			area=" (estudios.parea=$$ OR estudios.marea=$$ OR estudios.darea=$$ OR estudios.oarea=$$ ";
			area+=" OR estudiosfuera.parea=$$ OR estudiosfuera.marea=$$ OR estudiosfuera.darea=$$ OR estudiosfuera.oarea=$$) ";
			CString data;
			data.Format("%d", m_CBArea.SelectedData);
			area.Replace("$$", data);
			if(!temp.IsEmpty())
				temp+=" AND ";
			temp+=area;
		}
	}
	m_pConector->Query(query.GetSelect(temp + orderby, left), todo ? query_todos : query_resultados, this);
}

void CBusquedaView::OnBnClickedBbuscar()
{
	OnBuscar(!m_SAvanzado.IsCollapsed(), m_CTodos.GetCheck());
}

void CBusquedaView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	GetDocument()->SetTitle(CLanguage::GetTraduccion(IDS_BUSQUEDA));
	m_pConector= (CConector *)AfxGetMainWnd()->SendMessage(WM_QUERYCONECTOR);
	
	m_LCBuscar.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	
	m_LCBuscar.AddColumn(CLanguage::GetTraduccion(IDS_PKZ));
	m_LCBuscar.AddColumn(CLanguage::GetTraduccion(IDS_NOMBRE));
	m_LCBuscar.AddColumn(CLanguage::GetTraduccion(IDS_APELLIDO));
	m_LCBuscar.AddColumn(CLanguage::GetTraduccion(IDS_SAPELLIDO));
	m_LCBuscar.AddColumn(CLanguage::GetTraduccion(IDS_TELEFONOCASA));
	m_LCBuscar.AddColumn(CLanguage::GetTraduccion(IDS_TELEFONOOFICINA));

	m_LCBuscar.SetOrdenable();

	m_CBPais.SetItemData(m_CBPais.AddString(CLanguage::GetTraduccion(IDS_STODOS)), -1);
	m_CBPais.SelectedIndex=0;
	m_CBCiudad.SetItemData(m_CBCiudad.AddString(CLanguage::GetTraduccion(IDS_STODAS)), -1);
	m_CBCiudad.SelectedIndex=0;
	m_CBUniversidad.SetItemData(m_CBUniversidad.AddString(CLanguage::GetTraduccion(IDS_STODAS)), -1);
	m_CBUniversidad.SelectedIndex=0;
	m_CBArea.SetItemData(m_CBArea.AddString(CLanguage::GetTraduccion(IDS_STODAS)), -1);
	m_CBArea.SelectedIndex=0;
	m_CBTipo.SetItemData(m_CBTipo.AddString(CLanguage::GetTraduccion(IDS_STODOS)), -1);
	m_CBTipo.SelectedIndex=0;

	m_LCBuscar.Mensaje=CLanguage::GetTraduccion(IDS_TERMINOBUSQUEDA);

	m_SBusqueda.SetResizeParent(false);
	m_SBusqueda.SetNextSectionCtrl(m_SAvanzado);
	
	m_SAvanzado.SetResizeParent(false);
	m_SAvanzado.SetNextSectionCtrl(m_SFin);

	m_CTodos.SetCheck(AfxGetApp()->GetProfileInt("busqueda", "todos", 0));
	m_CTodosA.SetCheck(AfxGetApp()->GetProfileInt("busqueda", "todosa", 0));

	if(AfxGetApp()->GetProfileInt("buscar", "avanzado", 1)==1)
		m_SAvanzado.Collapse();
	if(AfxGetApp()->GetProfileInt("buscar", "normal", 0)==1)
		m_SBusqueda.Collapse();

	m_pConector->Query("SELECT id, nombre FROM shared WHERE nReservado=3 AND (szReservado IS NULL OR szReservado!=-1)" ,query_areas, this);

	CString consulta= "SELECT nacionalidad AS pais FROM usuario ";
	consulta+=" UNION ";
	consulta+=" SELECT pais FROM residencia ";
	consulta+=" UNION ";
	consulta+=" SELECT pais FROM empresa ";
	consulta+=" GROUP BY pais ";
	
	m_pConector->Query(consulta , query_paises, this);

	consulta= " SELECT ciudad FROM residencia ";
	consulta+=" UNION ";
	consulta+=" SELECT ciudad FROM empresa ";
	consulta+=" GROUP BY ciudad";

	m_pConector->Query(consulta, query_ciudades, this);

	consulta= " SELECT puniversidad as a FROM estudios ";
	consulta+=" UNION ";
	consulta+=" SELECT muniversidad as a FROM estudios ";
	consulta+=" UNION ";
	consulta+=" SELECT duniversidad as a FROM estudios ";
	consulta+=" UNION ";
	consulta+=" SELECT ouniversidad as a FROM estudios ";
	consulta+=" UNION ";
	consulta+=" SELECT puniversidad as a FROM estudiosfuera ";
	consulta+=" UNION ";
	consulta+=" SELECT muniversidad as a FROM estudiosfuera ";
	consulta+=" UNION ";
	consulta+=" SELECT duniversidad as a FROM estudiosfuera ";
	consulta+=" UNION ";
	consulta+=" SELECT ouniversidad as a FROM estudiosfuera ";
	consulta+=" GROUP BY a";

	m_pConector->Query(consulta, query_universidades, this);

	m_pConector->Query("SELECT id, nombre FROM shared WHERE nReservado=4 AND (szReservado IS NULL OR szReservado!=-1)" ,query_tipos, this);
}

LRESULT CBusquedaView::OnMySQLMessage(WPARAM wParam ,LPARAM lParam) {

	CMySQLMessage query(lParam);
	switch(wParam) {
		case QUERY_TITLE:
			if(query.GetID()==query_todos) {
				for(int i=0; i<query.GetColumnCount(); i++) {
					m_LCBuscar.AddColumn(query.GetColumna(i));
				}
			}
			break;
		case QUERY_RESULT:
			if(query.GetID()==query_resultados) {
			
				int pos= m_LCBuscar.AddItem(query["pkz"]);
				m_LCBuscar.Items[pos][1]=query["nombre"];
				m_LCBuscar.Items[pos][2]=query["apellido"];
				m_LCBuscar.Items[pos][3]=query["sapellido"];
				m_LCBuscar.Items[pos][4]=query["toficina"];
				m_LCBuscar.Items[pos][5]=query["tempresa"];
			} else if(query.GetID()==query_todos) {
				int pos= m_LCBuscar.GetItemCount();
				for(int i=0; i<query.GetColumnCount(); i++) {
					m_LCBuscar.Items[pos][i]= query[i];
				}
			} else if(query.GetID()==query_areas) {
				m_CBArea.SetItemData(m_CBArea.AddString(query["nombre"]), atoi(query["id"]));
			} else if(query.GetID()==query_paises) {
				if(!query[0].IsEmpty())
					m_CBPais.AddString(query[0]);
			} else if(query.GetID()==query_ciudades) {
				if(!query[0].IsEmpty())
					m_CBCiudad.AddString(query[0]);
			} else if(query.GetID()==query_universidades) {
				if(!query[0].IsEmpty())
					m_CBUniversidad.AddString(query[0]);
			} else if(query.GetID()==query_tipos) {
				m_CBTipo.SetItemData(m_CBTipo.AddString(query["nombre"]), atoi(query["id"]));
			}
			break;
		case THREAD_END:
			if(query.GetID()==query_resultados || query.GetID()==query_todos ) {
				m_LCBuscar.AutoSizeColumns();
				m_LCBuscar.Mensaje=CLanguage::GetTraduccion(IDS_NORESULTADOS);
			}
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

	menu.AppendMenu(MF_STRING,2000,CLanguage::GetTraduccion(IDS_VERUSUARIO));
	menu.SetDefaultItem(2000, false);
	menu.AppendMenu(MF_SEPARATOR);
	m_LCBuscar.GetMenuContextual(&menu);

	POINT point;
	GetCursorPos( &point );
	menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, AfxGetMainWnd());

	*pResult = 0;
}

void CBusquedaView::OnGuardar() {

	Exportar(&m_LCBuscar, CLanguage::GetTraduccion(IDS_RESULTADOS), true);
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

void CBusquedaView::OnBnClickedBindexar()
{
	OnBuscar(false, m_CTodosA.GetCheck());
}

void CBusquedaView::OnStnClickedSavanzado()
{
	AfxGetApp()->WriteProfileInt("buscar", "avanzado", m_SAvanzado.IsCollapsed());
	CRect rectcliente;
	GetWindowRect(rectcliente);
	OnSize(0 ,rectcliente.Width(), rectcliente.Height());

	if(!m_SAvanzado.IsCollapsed()) {
		m_EBusqueda.SetFocus();
		if(!m_SBusqueda.IsCollapsed()) {
			m_SBusqueda.Collapse();
			OnStnClickedBusqueda();
		}
	} else if(m_SBusqueda.IsCollapsed()) {
		m_SBusqueda.Uncollapse();
		OnStnClickedBusqueda();
	}
}

void CBusquedaView::OnStnClickedBusqueda() {

	AfxGetApp()->WriteProfileInt("buscar", "normal", m_SBusqueda.IsCollapsed());
	CRect rectcliente;
	GetWindowRect(rectcliente);
	OnSize(0 ,rectcliente.Width(), rectcliente.Height());
	if(!m_SBusqueda.IsCollapsed()) {
		m_ENombre.SetFocus();
		if(!m_SAvanzado.IsCollapsed()) {
			m_SAvanzado.Collapse();
			OnStnClickedSavanzado();
		}
	} else if(m_SAvanzado.IsCollapsed()) {
		m_SAvanzado.Uncollapse();
		OnStnClickedSavanzado();
	}
}

void CBusquedaView::OnDestroy()
{
	CFormView::OnDestroy();
	AfxGetApp()->WriteProfileInt("busqueda", "todos", m_CTodos.GetCheck());
	AfxGetApp()->WriteProfileInt("busqueda", "todosa", m_CTodosA.GetCheck());
}

void CBusquedaView::OnChangeTodos() {

	m_CTodosA.SetCheck(m_CTodos.GetCheck());
}

void CBusquedaView::OnChangeTodosA() {

	m_CTodos.SetCheck(m_CTodosA.GetCheck());
}
