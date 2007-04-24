// DlgIngreso.cpp : implementation file
//

#include "stdafx.h"
# include "Conector.h"
#include "DlgIngreso.h"
# include <resource.h>
# include <atlbase.h>

// CDlgIngreso dialog

IMPLEMENT_DYNAMIC(CDlgIngreso, CDialogXP)
CDlgIngreso::CDlgIngreso(CWnd* pParent /*=NULL*/)
	: CDialogXP(CDlgIngreso::IDD, pParent)
{
	m_dwGrupo= 0;
	m_dwID=0;
	m_dwCac= 0;
	m_bExiste= false;
	m_bLocalMachine= false;
}

CDlgIngreso::~CDlgIngreso()
{
}

void CDlgIngreso::DoDataExchange(CDataExchange* pDX)
{
	CDialogXP::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDITNOMBRE, m_EUsuario);
	DDX_Control(pDX, IDC_EDITCONTRASENA, m_EContrasena);
	DDX_Control(pDX, IDOK, m_BOK);
	DDX_Control(pDX, IDCANCEL, m_BCancelar);
	DDX_Control(pDX, IDC_STATICAYUDA, m_SAyuda);
	DDX_Control(pDX, IDC_CRECORDAR, m_CRecordar);
}


BEGIN_MESSAGE_MAP(CDlgIngreso, CDialogXP)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancelar)
END_MESSAGE_MAP()


// CDlgIngreso message handlers
bool CDlgIngreso::validateUser(CString usuario, CString passwd, bool codificado) {

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

	m_pConector.Query("SELECT id,szReservado,descripcion FROM shared WHERE nReservado=2 AND szReservado!=\"-1\" AND nombre=\"" + usuario + "\"", codificado ? 1 : 2, this);

	return true;
}

void CDlgIngreso::OnBnClickedCancelar() {

	CDialog::OnCancel();
}

void CDlgIngreso::OnBnClickedOk()
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

LRESULT CDlgIngreso::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if( message==WM_MYSQL) {

		MySQLMessage	*message= (MySQLMessage *) lParam;
		if( wParam==QUERY_RESULT) {

			m_pConector.KillQuery(-1, this);
			m_dwID= atol(message->GetAt(0));
			m_dwGrupo= atol(message->GetAt(1));
			m_dwCac= atol(message->GetAt(2));
			m_szExtra= message->GetAt(2);
			CMyRegKey::WriteProfileString("", "LastUser", m_szNombreReal);
			CMyRegKey::WriteProfileInt("", "autostart", m_CRecordar.GetCheck());
			if(m_CRecordar.GetCheck())
				CMyRegKey::WriteProfileString("", "lastpasswd", m_szPasswd);

			CWnd *parent= GetParent();
			if(parent)
				parent->SendMessage(WM_SETSTATUS, 0, (LPARAM) (const char *)"Listo");
			CDialog::OnOK();
			return 0;
		} else if(wParam==QUERY_EMPTY) {

		} else if( wParam==QUERY_ERROR) {

			if( message->GetID()==2) {

				m_SAyuda.SetWindowText(message->GetAt(0));
				CWnd *parent= GetParent();
				if(parent)
					parent->SendMessage(WM_SETSTATUS, 2, (LPARAM) (const char *)message->GetAt(0));
			} else {
//#ifdef _DEBUG
//				AfxGetMainWnd()->SendMessage(WM_SETSTATUS, 2, (LPARAM) (const char *)message->pMessage.GetAt(0));
//#endif
			}
		} else if( wParam==THREAD_END) {
			CMySQLMessage *hilo=(CMySQLMessage *)lParam;
			if( hilo->GetID()==1) {
				validateUser(m_szNombreReal, m_szPasswd, false);
			} else {
				m_EContrasena.EnableWindow(true);
				m_EUsuario.EnableWindow(!m_bExiste);
				m_BOK.EnableWindow(true);
				m_EUsuario.SetSel(0, m_EUsuario.GetWindowTextLength());
				m_EContrasena.SetSel(0, m_EContrasena.GetWindowTextLength());
				m_bExiste ? m_EContrasena.SetFocus() : m_EUsuario.SetFocus();
			}
		}
	}
	return CDialogXP::WindowProc(message, wParam, lParam);
}

BOOL CDlgIngreso::OnInitDialog()
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
