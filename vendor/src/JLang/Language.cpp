# include "stdafx.h"
#include "Language.h"
# include <Componentes/Componentes.h>

HINSTANCE CLanguage::m_hInstDefault=NULL;
HINSTANCE CLanguage::m_hInstActual=NULL;
DWORD CLanguage::m_dwIdioma=0;
CString CLanguage::m_szNombre;

CLanguage::CLanguage(void)
{
}

CLanguage::~CLanguage(void)
{
}

void CLanguage::Clean() {

	if(m_hInstActual && m_hInstActual!=m_hInstDefault)
		FreeLibrary(m_hInstActual);
	if(m_hInstDefault)
		FreeLibrary(m_hInstDefault);
}

void CLanguage::SetLanguage(DWORD language/*=0*/) {

	AfxGetApp()->WriteProfileInt("control", "idioma", language);
}

void CLanguage::InitLanguage() {

	LoadLanguage(AfxGetApp()->GetProfileInt("control", "idioma", 0));
}

DWORD CLanguage::GetLanguage() {

	return m_dwIdioma==0 ? LANG_SPANISH : m_dwIdioma;
}

CString CLanguage::GetTraduccion(UINT id) {
	CString texto;
	if(!texto.LoadString(id)) {
#ifdef  _DEBUG
		texto.Format("No se encontro '%d'",id);
		SetErrorEx(NULL, texto, 2, "traduccion");
#endif
	}
	return texto;
}

CString CLanguage::GetTraduccion(const CString &cadena) {

	CString resultado;
	if(m_szNombre.IsEmpty()) {
		resultado= cadena;
	} else {
		XmlReader reader;
		reader.Open(CApplicacion::GetAppPath(m_szNombre + ".xml"));
		CString tempcadena= cadena;
		tempcadena.Replace("\\n", "\\\\n");
		tempcadena.Replace("\\r", "\\\\r");
		resultado= reader.FindElemento("texto", "original", tempcadena, "traduccion");
		resultado.Replace("\\\\n", "\\n");
		resultado.Replace("\\\\r", "\\r");
		reader.Close();
	}
#ifdef  _DEBUG
	if(resultado.IsEmpty())
		SetErrorEx(NULL, "No se encontro '" + cadena + "'", 2, "traduccion");
#endif
	return resultado.IsEmpty() ? cadena : resultado;
}

void CLanguage::LoadLanguage(DWORD language/*=0*/) {

	CWaitCursor wait;
	if(!m_hInstDefault)
		m_hInstDefault= AfxGetInstanceHandle();
	if(language==0) {
		LCID lcid= GetThreadLocale();
		lcid &= 0xFF;
		language= lcid;
	}
	if(m_hInstActual && m_hInstDefault!=m_hInstDefault)
		FreeLibrary(m_hInstActual);
	switch(language) {
		case LANG_GERMAN:
			AfxGetApp()->WriteProfileString("ayuda", "ruta", "Fenster\\");
			m_szNombre=CApplicacion::GetAppName() + "DEU";
			break;
		/*case LANG_FRENCH:
			AfxGetApp()->WriteProfileString("ayuda", "ruta", "Aide\\");
			m_szNombre=CApplicacion::GetAppName() + "FRA";
			break;*/
		case LANG_ENGLISH:
			AfxGetApp()->WriteProfileString("ayuda", "ruta", "Help\\");
			m_szNombre=CApplicacion::GetAppName() + "ENU";
			break;
		case LANG_SPANISH:
			AfxGetApp()->WriteProfileString("ayuda", "ruta", "Ayuda\\");
			m_szNombre.Empty();
			return;
			break;
		default: // idioma español.
			// establecemos la ruta inicial de la ayuda
			AfxGetApp()->WriteProfileString("ayuda", "ruta", "Help\\");
			m_szNombre=CApplicacion::GetAppName() + "ENU";
			break;
	}
	m_hInstActual = LoadLibrary(m_szNombre+ ".dll");
	if(!m_hInstActual) {
		SetLanguage();
		return;
	}
	m_dwIdioma= language;
	AfxSetResourceHandle(m_hInstActual);
	CNewMenu *pMenuCurrent = (CNewMenu *)AfxGetMainWnd()->GetMenu();
	CNewMenu *m_pMenuNew = new CNewMenu();

	if(pMenuCurrent->m_hMenu != ((CMDIFrameWndXP *) AfxGetMainWnd())->m_hMenuDefault)
	{
		pMenuCurrent->DestroyMenu();
		delete pMenuCurrent;
	}
	m_pMenuNew->LoadMenu(128);
	AfxGetMainWnd()->SetMenu(m_pMenuNew);

	CString strBuffer;
	strBuffer.LoadString(AFX_IDS_IDLEMESSAGE);
	// toca buscar el toolbar.
	SetError(NULL, strBuffer);

	((CMDIFrameWndXP *) AfxGetMainWnd())->m_hAccelTable = NULL;

	if(!((CMDIFrameWndXP *) AfxGetMainWnd())->LoadAccelTable(MAKEINTRESOURCE(128))) 
		AfxMessageBox("Error cargando la tabla de aceleradores");
}
