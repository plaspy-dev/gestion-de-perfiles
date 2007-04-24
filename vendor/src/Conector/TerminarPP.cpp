// TerminarPP.cpp : implementation file
//

#include "stdafx.h"
#include "Conector.h"
#include "TerminarPP.h"
# include "TablasSheet.h"
# include <atlbase.h>
// CTerminarPP dialog

IMPLEMENT_DYNAMIC(CTerminarPP, CPropertyPage)

CTerminarPP::CTerminarPP()
	: CPropertyPage(CTerminarPP::IDD)
{
	m_psp.dwFlags|= PSP_DEFAULT|PSP_USEHEADERTITLE|PSP_USEHEADERSUBTITLE;
	m_psp.pszHeaderTitle= "Gracias";
	m_psp.pszHeaderSubTitle="Ya puede comenzar a usar la aplicación.";
}

CTerminarPP::~CTerminarPP()
{
}

void CTerminarPP::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CUSAR, m_CConectar);
	DDX_Control(pDX, IDC_CCONECTAR, m_CConectando);
	DDX_Control(pDX, IDC_CCONFIGURAR, m_CConfigurando);
	DDX_Control(pDX, IDC_CUSUARIO, m_CUsuario);
	DDX_Control(pDX, IDC_CGUARDAR, m_CGuardar);
	DDX_Control(pDX, IDC_SACCIONES, m_SAcciones);
	DDX_Control(pDX, IDC_SMENSAJE, m_SMensaje);
	DDX_Control(pDX, IDC_SFINALIZAR, m_SFinalizar);
}


BEGIN_MESSAGE_MAP(CTerminarPP, CPropertyPage)
	ON_MESSAGE(WM_MYSQL, OnMySQLMessage)
END_MESSAGE_MAP()


// CTerminarPP message handlers

BOOL CTerminarPP::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	CTablasSheet *parent= (CTablasSheet *)GetParent();
	m_CConectar.ShowWindow(parent->GetTipo()==2 ? SW_NORMAL : SW_HIDE);
	m_CConectar.SetCheck(1);
	CString texto;
	texto.Format("Gracias por usar %s.\rPara terminar el proceso haga clic en finalizar.", CApplicacion::GetAppName());
	m_SFinalizar.SetWindowText(texto);
	texto.Format("Creando usuario de %s.", CApplicacion::GetAppName());
	m_CUsuario.SetWindowText(texto);
	m_bSilent=false;
	return TRUE;
}

BOOL CTerminarPP::OnSetActive()
{
	CPropertySheet* parent = (CPropertySheet*)GetParent();
	if(parent) 
		parent->SetWizardButtons(PSWIZB_FINISH |PSWIZB_BACK);
	return CPropertyPage::OnSetActive();
}

BOOL CTerminarPP::OnWizardFinish()
{
	CWaitCursor wait;
	CTablasSheet *parent= (CTablasSheet *)GetParent();
	if(!parent) {
		MessageBox("No se ha podido completar la acción.");
		return false;
	}

	if(parent->GetTipo()!=1) { // omitimos toda la configuracion.

		// paso 1 confirmar la conexion con mysql.
		m_SAcciones.ShowWindow(SW_NORMAL);
		m_CConectando.ShowWindow(SW_NORMAL);
		m_CConectando.SetCheck(0);
		m_SMensaje.SetWindowText(parent->GetTipo()==0 ? "Creando instancia de la base de datos." : "Conectando a la base de datos.");
		CConector conector;
		conector.SetMainFrame(this);
		conector.AddDataBase("", parent->m_szUsuario, parent->m_szContrasena, parent->m_szServidor, atoi(parent->m_szPuerto), parent->GetTipo()==0);
		if(!conector.TestConection(this)) {
			return false;
		}
		m_CConectando.SetCheck(1);

		// paso 2 crear la base de datos y las tablas
		m_SMensaje.SetWindowText("Configurando la base de datos.");
		m_CConfigurando.ShowWindow(SW_NORMAL);
		m_CConfigurando.SetCheck(0);
		CString query;
		query.Format("CREATE DATABASE IF NOT EXISTS %s;", parent->m_szDB);
		if(conector.Find(query,this,NULL)==-1) 
			return false;
		query.Format("USE %s;", parent->m_szDB);
		if(conector.Find(query,this,NULL)==-1) 
			return false;

		// obtener el archivo
		CString  app=CApplicacion::GetAppPath("extras\\esquema.sql");
		
		CStdioFile file;
		if(!file.Open(app, CFile::modeRead)) {

			m_SMensaje.SetWindowText("No se puede encontrar el archivo '"+app+ "'.\rLa reinstalación de la aplicación puede solucionar el problema.");
			MessageBox("No se puede encontrar el archivo '"+app+ "'.\rLa reinstalación de la aplicación puede solucionar el problema.");
			return false;
		}
		query.Empty();
		int pos;
		while(file.ReadString(app)) {

			m_bSilent= app.Find("/**/")!=-1;

			pos= app.Find("--");
			if( pos!=-1) 
				app= app.Left(pos);
			app.Trim(" \t\r");
			if(app.IsEmpty())
				continue;
			pos=app.Find("//");		
			query+=app;
			if(app.Find(";")!=-1) {
				if(!query.IsEmpty()) {
					if(conector.Find(query, this, NULL)==-1) {
						if(!m_bSilent) {
							file.Close();
							return false;
						}
					}
				}
				query.Empty();
			}
		}
		file.Close();
		m_CConfigurando.SetCheck(1);

		// paso 3 configurar el usuario de la apliacacion
		m_CUsuario.ShowWindow(SW_NORMAL);
		m_CUsuario.SetCheck(0);
		m_SMensaje.SetWindowText("Creando el usuario de " + CApplicacion::GetAppName() + ".");

		if( parent->m_bUsuarioSVE && parent->GetTipo()==2) {
		
			CStringArray valores;
			query.Format("SELECT id FROM shared WHERE nombre='%s' AND nReservado=2",parent->m_szUsuarioSVE);
			if(conector.Find(query, this, &valores)==-1)
				return false;
			if(valores.IsEmpty())
				query.Format("INSERT INTO shared VALUES(NULL,'%s',NULL,2,'1');", parent->m_szUsuarioSVE);
			else query.Format("UPDATE shared SET szReservado='1' WHERE id='%s'", valores.GetAt(0));
			if(conector.Find(query, this, NULL)==-1)
				return false;

			app= CUsuario::GetCodificado(parent->m_szUsuarioSVE);

			query.Format("GRANT ALL PRIVILEGES ON *.* TO '%s' IDENTIFIED BY '%s' WITH GRANT OPTION;",app, parent->m_szContrasenaSVE);
			if(conector.Find(query, this, NULL)==-1)
				return false;
		}
		m_CUsuario.SetCheck(1);
	
		// paso 4 guardar el conector.
		m_CGuardar.ShowWindow(SW_NORMAL);
		m_CGuardar.SetCheck(0);
		m_SMensaje.SetWindowText("Guardando la configuración.");

		if(parent->m_bUsuarioSVE && parent->GetTipo()==2) {
			CRegKey key;
			if(	key.Open(HKEY_CURRENT_USER, CApplicacion::GetAppKey())!=ERROR_SUCCESS) {
				m_SMensaje.SetWindowText("No se puede guardar la configuración.");
				MessageBox("No se puede guardar la configuración.");
				return false;
			}
			key.SetStringValue("LastUser", parent->m_szUsuarioSVE);
			key.Close();
		}
	}
	if( m_CConectar.GetCheck()) {

		CRegKey key;
		if(	key.Open(HKEY_CURRENT_USER, CApplicacion::GetAppKey())==ERROR_SUCCESS) {
			if(key.RecurseDeleteKey("conector")!=ERROR_SUCCESS)
				key.DeleteSubKey("conector");
		} else {
			m_SMensaje.SetWindowText("No se puede guardar la configuración.");
			MessageBox("No se puede guardar la configuración.");
			return false;
		}
		if(key.Create(HKEY_CURRENT_USER, CApplicacion::GetAppKey() + "\\conector\\servidor0")!=ERROR_SUCCESS) {

			m_SMensaje.SetWindowText("No se puede guardar la configuración.");
			MessageBox("No se puede guardar la configuración.");
			return false;
		}
		key.SetStringValue("servidor", parent->m_szServidor);
		key.SetStringValue("db", parent->m_szDB);
		key.SetDWORDValue("puerto", atoi(parent->m_szPuerto));
		key.SetDWORDValue("incrustado", parent->GetTipo()==0 ? 1 : 0);
		key.Close();
	}
	m_CGuardar.SetCheck(1);
	m_SMensaje.SetWindowText("Configuración terminada.");
	MessageBox("Configuración Terminada.");
	return CPropertyPage::OnWizardFinish();
}

LRESULT CTerminarPP::OnMySQLMessage(WPARAM wParam, LPARAM lParam) {
	MySQLMessage *mess= (MySQLMessage *)lParam;
	if(wParam==QUERY_ERROR && m_bSilent==false) {
		m_SMensaje.SetWindowText(mess->GetAt(0));
		if(mess->GetColumnCount()>1)
			MessageBox(mess->GetAt(1), mess->GetAt(0));
		else MessageBox(mess->GetAt(0));
	}
	return 0;
}