// TemplateView.cpp: implementación de la clase CTemplateView
//

#include "stdafx.h"
#include "Template.h"

#include "TemplateView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTemplateView

IMPLEMENT_DYNCREATE(CTemplateView, CFormView)

BEGIN_MESSAGE_MAP(CTemplateView, CFormView)
	ON_BN_CLICKED(IDC_BCANCELAR, &CTemplateView::OnBnClickedBcancelar)
	ON_MESSAGE(WM_MYSQL, OnMySQLMessage)
	ON_EN_KILLFOCUS(IDC_EPKZ, &CTemplateView::OnEnKillfocusEpkz)
	ON_WM_DESTROY()
	ON_MESSAGE(WM_INSERTPKZ,  OnInsertPKZ)
END_MESSAGE_MAP()

// Construcción o destrucción de CTemplateView

CTemplateView::CTemplateView()
	: CFormView(CTemplateView::IDD)
{
	
}

CTemplateView::~CTemplateView()
{
}

void CTemplateView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EPKZ, m_EPKZ);
	DDX_Control(pDX, IDC_EENCABEZADO, m_EEncabezamiento);
	DDX_Control(pDX, IDC_ETITULO, m_ETitulo);
	DDX_Control(pDX, IDC_ENOMBRES, m_ENombre);
	DDX_Control(pDX, IDC_EAPELLIDO, m_EApellido);
	DDX_Control(pDX, IDC_ESAPELLIDO, m_ESApellido);
	DDX_Control(pDX, IDC_DNACIMIENTO, m_DTNacimiento);
	DDX_Control(pDX, IDC_CSEXO, m_CBSexo);
	DDX_Control(pDX, IDC_CORRESPONDENCIA, m_CBCorrespondencia);
	DDX_Control(pDX, IDC_BCANCELAR, m_BCancelar);
	DDX_Control(pDX, IDC_ENACIONALIDAD, m_ENacionalidad);
}

BOOL CTemplateView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: modificar aquí la clase Window o los estilos cambiando
	//  CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void CTemplateView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	GetDocument()->SetTitle("Personales");
	m_pConector= (CConector *)AfxGetMainWnd()->SendMessage(WM_QUERYCONECTOR);

	m_CBSexo.SetItemData(m_CBSexo.AddString("Masculino"), 1);
	m_CBSexo.SetItemData(m_CBSexo.AddString("Femenino"), 2);

	m_CBCorrespondencia.SetItemData(m_CBCorrespondencia.AddString("Oficina"), 1);
	m_CBCorrespondencia.SetItemData(m_CBCorrespondencia.AddString("Casa"), 2);	
}


// Diagnósticos de CTemplateView

#ifdef _DEBUG
void CTemplateView::AssertValid() const
{
	CFormView::AssertValid();
}

void CTemplateView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

void CTemplateView::OnBnClickedBaceptar()
{
	CString temp;
	if(m_EPKZ.Text.IsEmpty()) {
		SetErrorEx(&m_EPKZ, "Por favor ingrese un PKZ", 2, "usuario");
		return;
	}
	if(m_CBSexo.SelectedIndex==-1) {
		SetErrorEx(&m_CBSexo, "Por favor seleccione un sexo", 2, "usuario");
		return;
	}
	if(m_CBCorrespondencia.SelectedIndex==-1) {
		SetErrorEx(&m_CBCorrespondencia, "Por favor seleccione un sitio de correspondencia", 2, "usuario");
		return;
	}

	CQuery query("usuario");

	query["pkz"]=m_EPKZ.Text;
	query["encabezado"]=m_EEncabezamiento.Text;
	query["titulo"]=m_ETitulo.Text;
	query["nombre"]=m_ENombre.Text;
	query["apellido"]=m_EApellido.Text;
	query["sapellido"]=m_ESApellido.Text;
	CMyTime tiempo;
	m_DTNacimiento.GetTime(tiempo);
	query["nacimiento"]= tiempo.GetFormatoMysql();
	temp.Format("%d", m_CBSexo.SelectedData);
	query["sexo"]=temp;
	query["nacionalidad"]=m_ENacionalidad.Text;
	temp.Format("%d", m_CBCorrespondencia.SelectedData);
	query["correspondencia"]=temp;

	m_pConector->Query(query.GetInsert());
	OnBnClickedBcancelar();
}

void CTemplateView::Clean() {

	m_EEncabezamiento.Text="";
	m_ETitulo.Text="";
	m_ENombre.Text="";
	m_EApellido.Text="";
	m_ESApellido.Text="";
	m_CBSexo.Text="";
	m_ENacionalidad.Text="";
	m_CBCorrespondencia.Text="";
}

void CTemplateView::OnBnClickedBcancelar()
{
	m_EPKZ.Text="";
	SetErrorEx(&m_EPKZ, "Listo");
	Clean();
}

LRESULT CTemplateView::OnMySQLMessage(WPARAM wParam, LPARAM lParam) {
	CMySQLMessage mess(lParam);
	switch(wParam) {
		case QUERY_RESULT:
			if(mess.GetID()==query_pkz) {

				m_EEncabezamiento.Text= mess["encabezado"];
				m_ETitulo.Text=  mess["titulo"];
				m_ENombre.Text= mess["nombre"];
				m_EApellido.Text= mess["apellido"];
				m_ESApellido.Text= mess["sapellido"];
				CMyTime tiempo(mess["nacimiento"]);
				m_DTNacimiento.SetTime(tiempo);
				m_CBSexo.SelectedData= atoi(mess["sexo"]);
				m_ENacionalidad.Text=mess["nacionalidad"];
				m_CBCorrespondencia.SelectedData= atoi(mess["correspondencia"]);
			}
			break;
	}
	return 0;
}

void CTemplateView::OnEnKillfocusEpkz()
{
	CString temp= m_EPKZ.Text;
	if(temp.IsEmpty() || m_szOldPKZ==temp)
		return;
	m_szOldPKZ=temp;
	Clean();
	((CMDIFrameWndXP *) AfxGetMainWnd())->SendMessageBroadCast(WM_INSERTPKZ, 0, (LPARAM)(const char *)temp);

	CQuery query("usuario");

	query["encabezado"]="encabezado";
	query["titulo"]="titulo";
	query["nombre"]="nombre";
	query["apellido"]="apellido";
	query["sapellido"]="sapellido";
	query["nacimiento"]="nacimiento";
	query["sexo"]="sexo";
	query["nacionalidad"]="nacionalidad";
	query["correspondencia"]="correspondencia";

	m_pConector->Query(query.GetSelect("pkz='" + m_EPKZ.Text + "'"), query_pkz, this);
}

void CTemplateView::OnDestroy()
{
	m_EPKZ.Text="";
	CFormView::OnDestroy();
}

BOOL CTemplateView::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN)
		if( pMsg->wParam== VK_RETURN) {

			CWnd *foco= GetFocus();
			if( foco)
				if( !foco->IsKindOf( RUNTIME_CLASS(CButtonXP))) 

					pMsg->wParam= VK_TAB;
		}
		return CFormView::PreTranslateMessage(pMsg);
}

LRESULT CTemplateView::OnInsertPKZ(WPARAM wParam, LPARAM lParam) {

	CString temp=(const char *)lParam;
	m_EPKZ.Text=temp;
	OnEnKillfocusEpkz();
	return 0;
}
