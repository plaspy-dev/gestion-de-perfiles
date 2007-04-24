// DlgIngreso.cpp : implementation file
//

#include "stdafx.h"
# include "Conector.h"
#include "DlgIngreso3.h"
# include <resource.h>
# include <atlbase.h>

// CDlgIngreso3 dialog

IMPLEMENT_DYNAMIC(CDlgIngreso3, CDialogXP)
CDlgIngreso3::CDlgIngreso3(CWnd* pParent /*=NULL*/)
: CDialogXP(CDlgIngreso3::IDD, pParent)
{
	m_dwGrupo= 0;
	m_dwID=0;
	m_bExiste= false;
	m_bLocalMachine= false;
}

CDlgIngreso3::~CDlgIngreso3()
{
}

void CDlgIngreso3::DoDataExchange(CDataExchange* pDX)
{
	CDialogXP::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDITNOMBRE, m_EUsuario);
	DDX_Control(pDX, IDC_EDITCONTRASENA, m_EContrasena);
	DDX_Control(pDX, IDOK, m_BOK);
	DDX_Control(pDX, IDCANCEL, m_BCancelar);
	DDX_Control(pDX, IDC_STATICAYUDA, m_SAyuda);
	DDX_Control(pDX, IDC_CRECORDAR, m_CRecordar);
}


BEGIN_MESSAGE_MAP(CDlgIngreso3, CDialogXP)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancelar)
END_MESSAGE_MAP()


// CDlgIngreso3 message handlers
bool CDlgIngreso3::validateUser(CString usuario, CString passwd, bool codificado) {

	m_EContrasena.EnableWindow(false);
	m_EUsuario.EnableWindow(false);
	m_BOK.EnableWindow(false);

	m_szNombreUsuario = codificado ?  CUsuario::GetCodificado(usuario) : usuario;

	m_pConector.SetUser(m_szNombreUsuario);
	m_pConector.SetPasswd(passwd);

	m_dwID= 0;
	m_dwGrupo= 0;
	m_szPasswd= passwd;
	m_szNombreReal= usuario;

	m_pConector.ResetState();

	CQuery query("usuario");

	query+="id";
	query+="nombre";
	query+="login";
	query+="grupo";

	m_pConector.Query(query.GetSelect("login='" + usuario + "' AND (estado!=-1 OR estado IS NULL)"), codificado ? query_codificado : query_normal, this);

	return true;
}

void CDlgIngreso3::OnBnClickedCancelar() {

	CDialog::OnCancel();
}

void CDlgIngreso3::OnBnClickedOk()
{
	CString user;
	CString contrasena;
	m_EUsuario.GetWindowText(user);
	if( user.IsEmpty()) {

		m_EUsuario.SetFocus();
		return;
	}
	m_EContrasena.GetWindowText(contrasena);
	if( contrasena.IsEmpty()) {

		m_EContrasena.SetFocus();
		return;
	}
	m_SAyuda.SetWindowText("Validando usuario");
	CWnd *parent= GetParent();
	if(parent)
		parent->SendMessage(WM_SETSTATUS, 0, (LPARAM) (const char *)"Validando Usuario");
	validateUser(user, contrasena);
}

LRESULT CDlgIngreso3::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if( message==WM_MYSQL) {

		CMySQLMessage query(lParam);
		if( wParam==QUERY_RESULT) {

			if(query.GetID()==query_codificado || query.GetID()==query_normal) {

				m_pConector.KillQuery(-1, this);

				m_dwID= atol(query["id"]);
				m_dwGrupo= atol(query["grupo"]);
				m_szNombreUsuario= query["nombre"];

				CMyRegKey::WriteProfileString("", "LastUser", m_szNombreReal);
				CMyRegKey::WriteProfileInt("", "autostart", m_CRecordar.GetCheck());
				if(m_CRecordar.GetCheck())
					CMyRegKey::WriteProfileString("", "lastpasswd", m_szPasswd);

				CWnd *parent= GetParent();
				if(parent)
					parent->SendMessage(WM_SETSTATUS, 0, (LPARAM) (const char *)"Listo");
				if(m_dwGrupo) {
					CQuery permisos("permisos");

					permisos+="clase";
					permisos+="permiso";

					CString temp;
					temp.Format("grupo='%d'", m_dwGrupo);
					m_pConector.Query(permisos.GetSelect(temp), query_permisos, this);
				} else {
					CDialog::OnOK();
				}
				return 0;
			} else if(query.GetID()==query_permisos) {
				m_pPermisos.Add(query["clase"], atoi(query["permiso"]));
			}
		} else if(wParam==QUERY_EMPTY) {

		} else if( wParam==QUERY_ERROR) {

			if( query.GetID()==query_normal) {

				m_SAyuda.SetWindowText(query[0]);
				CWnd *parent= GetParent();
				if(parent)
					parent->SendMessage(WM_SETSTATUS, 2, (LPARAM) (const char *)query[0]);
			} else {
				TRACE(query[0] + "\n");
			}
		} else if( wParam==THREAD_END) {
			if( query.GetID()==query_codificado) {
				validateUser(m_szNombreReal, m_szPasswd, false);
			} else if(query.GetID()==query_normal) {
				if(!m_dwID) {
					m_EContrasena.EnableWindow(true);
					m_EUsuario.EnableWindow(!m_bExiste);
					m_BOK.EnableWindow(true);
					m_EUsuario.SetSel(0, m_EUsuario.GetWindowTextLength());
					m_EContrasena.SetSel(0, m_EContrasena.GetWindowTextLength());
					m_bExiste ? m_EContrasena.SetFocus() : m_EUsuario.SetFocus();
				}
			} else if(query.GetID()==query_permisos) {
				CDialog::OnOK();
			}
		}
	}
	return CDialogXP::WindowProc(message, wParam, lParam);
}

BOOL CDlgIngreso3::OnInitDialog()
{
	CDialogXP::OnInitDialog();
	m_SAyuda.SetWindowText("");
	m_EUsuario.EnableWindow(!m_bExiste);
	VERIFY(m_pConector.Cargar(m_bLocalMachine ? HKEY_LOCAL_MACHINE : HKEY_CURRENT_USER));
	ASSERT(m_pConector.GetDatabasesCount());
	m_pConector.SetMainFrame(this);
	m_EUsuario.SetWindowText(CMyRegKey::GetProfileString("", "LastUser", ""));

	if(CMyRegKey::GetProfileInt("", "autolog")==0) {
		CRect rlabel;
		m_SAyuda.GetWindowRect(rlabel);
		CRect rrecord;
		m_CRecordar.GetWindowRect(rrecord);
		int ancho= rlabel.Height();
		rlabel.bottom= rlabel.top= rrecord.top;
		rlabel.bottom+=ancho;
		ScreenToClient(rlabel);
		m_SAyuda.MoveWindow(rlabel);
	}
	m_CRecordar.ShowWindow(CMyRegKey::GetProfileInt("", "autolog") ? SW_NORMAL : SW_HIDE);
	m_CRecordar.SetCheck(CMyRegKey::GetProfileInt("", "autostart"));

	if(m_CRecordar.GetCheck()) {
		m_EContrasena.SetWindowText(CMyRegKey::GetProfileString("", "lastpasswd"));
		OnBnClickedOk();
	}
	return TRUE;
}
