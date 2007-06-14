// TemplateView.cpp: implementación de la clase CTemplateView
//

#include "stdafx.h"
#include "Template.h"
# include <math.h>

#include "TemplateView.h"
# include "GestionView.h"
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
	ON_BN_CLICKED(IDC_BACEPTAR, &CTemplateView::OnBnClickedBaceptar)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DNACIMIENTO, &CTemplateView::OnDtnDatetimechangeDnacimiento)
	ON_WM_SIZE()
	ON_MESSAGE(WM_QUERYUSUARIO, OnQueryUsuario)
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
	DDX_Control(pDX, IDC_ETITULO, m_ETitulo);
	DDX_Control(pDX, IDC_ENOMBRES, m_ENombre);
	DDX_Control(pDX, IDC_EAPELLIDO, m_EApellido);
	DDX_Control(pDX, IDC_ESAPELLIDO, m_ESApellido);
	DDX_Control(pDX, IDC_DNACIMIENTO, m_DTNacimiento);
	DDX_Control(pDX, IDC_CSEXO, m_CBSexo);
	DDX_Control(pDX, IDC_CORRESPONDENCIA, m_CBCorrespondencia);
	DDX_Control(pDX, IDC_BCANCELAR, m_BCancelar);
	DDX_Control(pDX, IDC_ENACIONALIDAD, m_ENacionalidad);
	DDX_Control(pDX, IDC_BACEPTAR, m_BAceptar);
	DDX_Control(pDX, IDC_SANOS, m_SAnos);
	DDX_Control(pDX, IDC_CFALLECIDO, m_BFallecido);
	DDX_Control(pDX, IDC_SFALLECIDO, m_SFallecido);
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

	GetDocument()->SetTitle(CLanguage::GetTraduccion(IDS_PERSONALES));
	m_pConector= (CConector *)AfxGetMainWnd()->SendMessage(WM_QUERYCONECTOR);
	m_pUsuario= (CUsuario *)AfxGetMainWnd()->SendMessage(WM_QUERYUSUARIO);

	m_CBSexo.SetItemData(m_CBSexo.AddString(CLanguage::GetTraduccion(IDS_NOAPLICA)), 0);
	m_CBSexo.SetItemData(m_CBSexo.AddString(CLanguage::GetTraduccion(IDS_MASCULINO)), 1);
	m_CBSexo.SetItemData(m_CBSexo.AddString(CLanguage::GetTraduccion(IDS_FEMENINO)), 2);

	m_CBCorrespondencia.SetItemData(m_CBCorrespondencia.AddString(CLanguage::GetTraduccion(IDS_NOAPLICA)), 0);
	m_CBCorrespondencia.SetItemData(m_CBCorrespondencia.AddString(CLanguage::GetTraduccion(IDS_OFICINA)), 1);
	m_CBCorrespondencia.SetItemData(m_CBCorrespondencia.AddString(CLanguage::GetTraduccion(IDS_CASA)), 2);

	CMyTime tiempo(true);
	tiempo.m_status= COleDateTime::error;
	m_DTNacimiento.SetTime(tiempo);

	m_SFallecido.SetFontColor(RGB(255,0,0));
	m_SFallecido.ShowWindow(SW_HIDE);
	m_BAceptar.ShowWindow(m_pUsuario->GetGrupo()==1 ? SW_NORMAL : SW_HIDE);

	m_EPKZ.InitAutoWidth();
	m_ETitulo.InitAutoWidth();
	m_ENombre.InitAutoWidth();
	m_EApellido.InitAutoWidth();
	m_ESApellido.InitAutoWidth();
	m_ENacionalidad.InitAutoWidth();
}

LRESULT CTemplateView::OnQueryUsuario(WPARAM wParam, LPARAM lParam) {

	m_pUsuario= (CUsuario *)lParam;
	m_BAceptar.ShowWindow(m_pUsuario->GetGrupo()==1 ? SW_NORMAL : SW_HIDE);
	return 0;
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
		SetErrorEx(&m_EPKZ, CLanguage::GetTraduccion(IDS_INSERTPKZ), 2, "usuario");
		return;
	}
	if(m_CBSexo.SelectedIndex==-1) {
		SetErrorEx(&m_CBSexo, CLanguage::GetTraduccion(IDS_SELECCIONSEXO), 2, "usuario");
		return;
	}
	if(m_CBCorrespondencia.SelectedIndex==-1) {
		SetErrorEx(&m_CBCorrespondencia, CLanguage::GetTraduccion(IDS_SELECCIONCORRESPONDENCIA), 2, "usuario");
		return;
	}

	CQuery query("usuario");

	if(m_szCurrentPKZ.IsEmpty())
		query["pkz"]=m_EPKZ.Text;
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
	temp.Format("%d",m_BFallecido.GetCheck());
	query["fallecido"]=temp;

	m_pConector->Query(m_szCurrentPKZ.IsEmpty() ? query.GetInsert() : query.GetUpdate("pkz='"+ m_szCurrentPKZ+ "'"));

	CGestionView::Actualizar(m_pConector, m_EPKZ.Text);
	OnBnClickedBcancelar();
}

void CTemplateView::Clean() {

	m_szCurrentPKZ="";
	m_ETitulo.Text="";
	m_ENombre.Text="";
	m_EApellido.Text="";
	m_ESApellido.Text="";
	m_CBSexo.Text="";
	m_ENacionalidad.Text="";
	m_CBCorrespondencia.Text="";
	m_BFallecido.SetCheck(0);
	CMyTime tiempo(true);
	tiempo.m_status= COleDateTime::error;
	m_DTNacimiento.SetTime(tiempo);
	m_SFallecido.ShowWindow(SW_HIDE);
	OnDtnDatetimechangeDnacimiento(NULL, NULL);
}

void CTemplateView::OnBnClickedBcancelar()
{
	m_szOldPKZ="";
	m_EPKZ.Text="";
	SetErrorEx(&m_EPKZ, CLanguage::GetTraduccion(AFX_IDS_IDLEMESSAGE));
	Clean();
}

LRESULT CTemplateView::OnMySQLMessage(WPARAM wParam, LPARAM lParam) {
	CMySQLMessage mess(lParam);
	switch(wParam) {
		case QUERY_RESULT:
			if(mess.GetID()==query_pkz) {

				m_szCurrentPKZ= m_EPKZ.Text;
				m_ETitulo.Text=  mess["titulo"];
				m_ENombre.Text= mess["nombre"];
				m_EApellido.Text= mess["apellido"];
				m_ESApellido.Text= mess["sapellido"];
				CMyTime tiempo(mess["nacimiento"]);
				m_DTNacimiento.SetTime(tiempo);
				m_CBSexo.SelectedData= atoi(mess["sexo"]);
				m_ENacionalidad.Text=mess["nacionalidad"];
				m_CBCorrespondencia.SelectedData= atoi(mess["correspondencia"]);
				m_BFallecido.SetCheck(atoi(mess["fallecido"]));

				m_SFallecido.ShowWindow(m_BFallecido.GetCheck() ? SW_SHOW : SW_HIDE);
				((CMDIFrameWndXP *) AfxGetMainWnd())->SendMessageBroadCast(WM_SHOWFALLECIDO, m_BFallecido.GetCheck());

				OnDtnDatetimechangeDnacimiento(NULL, NULL);
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

	CQuery query("usuario");

	query["titulo"]="titulo";
	query["nombre"]="nombre";
	query["apellido"]="apellido";
	query["sapellido"]="sapellido";
	query["nacimiento"]="nacimiento";
	query["sexo"]="sexo";
	query["nacionalidad"]="nacionalidad";
	query["correspondencia"]="correspondencia";
	query+="fallecido";

	m_pConector->Query(query.GetSelect("pkz='" + m_EPKZ.Text + "'"), query_pkz, this);
	((CMDIFrameWndXP *) AfxGetMainWnd())->SendMessageBroadCast(WM_INSERTPKZ, 0, (LPARAM)(const char *)m_EPKZ.Text);
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

void CTemplateView::OnDtnDatetimechangeDnacimiento(NMHDR *pNMHDR, LRESULT *pResult)
{
	CMyTime tiempo;
	m_DTNacimiento.GetTime(tiempo);
	if(tiempo.EsValido()) {
		CMyTime actual(true);
		COleDateTimeSpan span=(COleDateTime)actual-(COleDateTime)tiempo;
		CString edad;
		int total=span.GetTotalDays();
		int anos= (int)floor((float)total/365);
		total= span.GetTotalDays()-(int)floor((float)anos/4);
		anos= (int)floor((float)total/365);
		edad.Format(CLanguage::GetTraduccion(IDS_FORMATOANOS),anos);
		m_SAnos.SetWindowText(edad);
	} else {
		m_SAnos.SetWindowText("");
	}
	if(pResult)
		*pResult = 0;
}

void CTemplateView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);
	if(::IsWindow(m_SFallecido)) {
		CRect rect;
		m_SFallecido.GetWindowRect(rect);
		m_SFallecido.MoveWindow(0,0,cx, rect.Height());
	}
}
