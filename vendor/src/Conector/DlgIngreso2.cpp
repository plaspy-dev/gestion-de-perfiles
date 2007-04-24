// DlgIngreso.cpp : implementation file
//

#include "stdafx.h"
# include "Conector.h"
#include "DlgIngreso2.h"
# include <resource.h>
# include <atlbase.h>
# include "DlgWebContrasena.h"

// CDlgIngreso2 dialog

IMPLEMENT_DYNAMIC(CDlgIngreso2, CDialogXP)
CDlgIngreso2::CDlgIngreso2(CWnd* pParent /*=NULL*/)
	: CDialogXP(CDlgIngreso2::IDD, pParent)
{
	m_dwGrupo= 0;
	m_dwID=0;
	m_dwCac= 0;
	m_bExiste= false;
	m_bLocalMachine= false;
}

CDlgIngreso2::~CDlgIngreso2()
{
}

void CDlgIngreso2::DoDataExchange(CDataExchange* pDX)
{
	CDialogXP::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDITNOMBRE, m_EUsuario);
	DDX_Control(pDX, IDC_EDITCONTRASENA, m_EContrasena);
	DDX_Control(pDX, IDOK, m_BOK);
	DDX_Control(pDX, IDCANCEL, m_BCancelar);
	DDX_Control(pDX, IDC_STATICAYUDA, m_SAyuda);
	DDX_Control(pDX, IDC_CRECORDAR, m_CRecordar);
}


BEGIN_MESSAGE_MAP(CDlgIngreso2, CDialogXP)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancelar)
END_MESSAGE_MAP()


// CDlgIngreso2 message handlers
bool CDlgIngreso2::validateUser(CString usuario, CString passwd, bool codificado) {

	m_EContrasena.EnableWindow(false);
	m_EUsuario.EnableWindow(false);
	m_BOK.EnableWindow(false);

	m_szNombreUsuario = codificado ?  CUsuario::GetCodificado(usuario) : usuario;

	m_pConector.SetUser(m_szNombreUsuario);
	m_pConector.SetPasswd(passwd);

	m_dwID= 0;
	m_dwGrupo= 0;
	m_szPasswd= passwd;
	m_dwCac= 0;
    m_szNombreReal= usuario;

	m_pConector.ResetState();

	CQuery query("usuarios");

	query+="id";
	query+="grupo";
	query+="descripcion";
	query+="nombre";
	query+="vigencia";
	query+="expira";
	query+="ucambio";
	query+="login";
	query+="autonumerico";

	m_pConector.Query(query.GetSelect("(estado!=-1 OR estado IS NULL) AND login='"+ usuario + "'"), codificado ? 1 : 2, this);

	return true;
}

void CDlgIngreso2::OnBnClickedCancelar() {

	CDialog::OnCancel();
}

void CDlgIngreso2::OnBnClickedOk()
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

LRESULT CDlgIngreso2::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if( message==WM_MYSQL) {

		CMySQLMessage message(lParam);

		if( wParam==QUERY_RESULT) {

			if(message.GetID()==1 || message.GetID()==2) {

				CStringArray valores;

				if(m_pConector.Find("SELECT SYSDATE()", this, &valores)!=-1) {
					
					CMyTime actual(valores[0]);
					CString valor= message["vigencia"]; // actualizamos la fecha para que tenga acceso hasta este dia.
					valor.Replace("00:00:00", "23:59:59");
					CMyTime tiempo(valor);

					if(!message["vigencia"].IsEmpty() && actual>tiempo) { // su cuenta a expirado

						m_pConector.KillQuery(-1, this);
						MessageBox("Su cuenta de usuario ha expirado.\r\nPor favor consulte al administrador del sistema.",CApplicacion::GetAppName(),MB_ICONERROR);
						m_SAyuda.SetWindowText("Su cuenta de usuario ha expirado");
						CWnd *parent= GetParent();
						if(parent)
							parent->SendMessage(WM_SETSTATUS, 3, (LPARAM) (const char *)"Su cuenta de usuario ha expirado.Por favor consulte al administrador del sistema.");
						Reset();
						return 0;
					}
					CQuery query("usuariosh");
					query+="dia";
					query+="entrada";
					query+="salida";

					if(m_pConector.Find(query.GetSelect("usuario='" +message["id"] +"'"), this,&valores)>0) { // se encontro horario.
						ASSERT(valores.GetCount()%3==0);
						CUsuario2 usuario;

						while(!valores.IsEmpty()) {
							usuario.AddHorario(atoi(valores[0]), atoi(valores[1]), atoi(valores[2]));

							valores.RemoveAt(0);
							valores.RemoveAt(0);
							valores.RemoveAt(0);
						}
						if(!usuario.CanEntry(actual)) {

							m_pConector.KillQuery(-1, this);
							MessageBox("No tiene acceso al sistema en este horario.",CApplicacion::GetAppName(), MB_ICONWARNING);
							m_SAyuda.SetWindowText("No tiene acceso al sistema en este horario.");
							CWnd *parent= GetParent();
							if(parent)
								parent->SendMessage(WM_SETSTATUS, 3, (LPARAM) (const char *)"No tiene acceso al sistema en este horario.");
							Reset();
							return 0;
						}
					}
					if(message["ucambio"].IsEmpty() || atoi(message["expira"])>0) {

						tiempo= message["ucambio"];
						COleDateTimeSpan span;
						if(!message["ucambio"].IsEmpty())
							span= actual-tiempo;
						if(message["ucambio"].IsEmpty() || span.GetDays()>=atoi(message["expira"])) {
							// aqui cambiamos la contraseña por medio de la IP
							m_pConector.KillQuery(-1, this);
							AFX_MANAGE_STATE(AfxGetStaticModuleState());
							CDlgWebContrasena dlg(m_pConector.GetDatabaseAt(0)->GetHost(), m_szNombreReal, message["id"]);
							if(dlg.DoModal()==IDOK) {

								m_SAyuda.SetWindowText("Su contraseña a sido cambiada exitosamente.");
								CWnd *parent= GetParent();
								if(parent)
									parent->SendMessage(WM_SETSTATUS, 0, (LPARAM) (const char *)"Su contraseña a sido cambiada exitosamente.");
								Reset();
								m_EContrasena.Text="";
								m_EContrasena.SetFocus();
								return 0;
							} else {
								MessageBox("Su contraseña ha expirado.");
								m_SAyuda.SetWindowText("Su contraseña ha expirado.");
								CWnd *parent= GetParent();
								if(parent)
									parent->SendMessage(WM_SETSTATUS, 3, (LPARAM) (const char *)"Su contraseña ha expirado.");
								Reset();
								return 0;
							}
						}
					}
				}
				m_pConector.KillQuery(-1, this);
				
				m_szNombreReal= m_szNombreUsuario;
				m_dwID=atoi(message["id"]);
				m_dwGrupo= atol(message["grupo"]);
				m_dwCac= atol(message["descripcion"]);
				m_szExtra= message["descripcion"];
				m_szNombreUsuario= message["nombre"];
				m_szNombreReal= message["login"];
				m_nAutoNumerico= atoi(message["autonumerico"]);

				// de aqui en adelante ya entro
				// actualizamos el ultimo acceso.
				CQuery query("usuarios");
				query["acceso"]="SYSDATE()";
				m_pConector.Find(query.GetUpdate("id='" +message["id"] + "'"));

				CMyRegKey::WriteProfileString("", "LastUser", m_EUsuario.Text);
				CMyRegKey::WriteProfileInt("", "autostart", m_CRecordar.GetCheck());
				if(m_CRecordar.GetCheck())
					CMyRegKey::WriteProfileString("", "lastpasswd", m_szPasswd);

				CWnd *parent= GetParent();
				if(parent)
					parent->SendMessage(WM_SETSTATUS, 0, (LPARAM) (const char *)"Listo");
				CDialog::OnOK();
				return 0;
			}
		} else if(wParam==QUERY_EMPTY) {

		} else if( wParam==QUERY_ERROR) {

			if( message.GetID()==2) {

				m_SAyuda.SetWindowText(message.GetAt(0));
				CWnd *parent= GetParent();
				if(parent)
					parent->SendMessage(WM_SETSTATUS, 2, (LPARAM) (const char *)message.GetAt(0));
			} else {
//#ifdef _DEBUG
//				AfxGetMainWnd()->SendMessage(WM_SETSTATUS, 2, (LPARAM) (const char *)message->pMessage.GetAt(0));
//#endif
			}
		} else if( wParam==THREAD_END) {
			if( message.GetID()==1) {
				validateUser(m_szNombreReal, m_szPasswd, false);
			} else {
				Reset();
			}
		}
	}
	return CDialogXP::WindowProc(message, wParam, lParam);
}

void CDlgIngreso2::Reset() {

	m_EContrasena.EnableWindow(true);
	m_EUsuario.EnableWindow(!m_bExiste);
	m_BOK.EnableWindow(true);
	m_EUsuario.SetSel(0, m_EUsuario.GetWindowTextLength());
	m_EContrasena.Text="";
	m_pConector.CloseConections();
	m_bExiste ? m_EContrasena.SetFocus() : m_EUsuario.SetFocus();
}

BOOL CDlgIngreso2::OnInitDialog()
{
	CDialogXP::OnInitDialog();
	m_SAyuda.SetWindowText("");
	m_EUsuario.EnableWindow(!m_bExiste);
	VERIFY(m_pConector.Cargar(m_bLocalMachine ? HKEY_LOCAL_MACHINE : HKEY_CURRENT_USER));
	ASSERT(m_pConector.GetDatabasesCount());
	m_pConector.SetMainFrame(NULL);
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
