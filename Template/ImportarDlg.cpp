// ImportarDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Template.h"
#include "ImportarDlg.h"
# include "GestionView.h"
# include "IndexarDlg.h"

# define SEPARADOR_COMAS	";" 

// CImportarDlg dialog

IMPLEMENT_DYNAMIC(CImportarDlg, CDialog)

CImportarDlg::CImportarDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CImportarDlg::IDD, pParent)
{
	m_nPKZ=-1;
	m_nLineas=0;
	m_nActual=0;
}

CImportarDlg::~CImportarDlg()
{
	for(int i=0; i<m_pTablas.GetCount(); i++) {
		CQuery *query= (CQuery *)m_pTablas[i]->GetData();
		delete query;
	}
	for(int i=0; i<m_pTablasAlias.GetCount(); i++) {
		CQuery *query= (CQuery *)m_pTablasAlias[i]->GetData();
		delete query;
	}
}

void CImportarDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PPROGRESOREAL, m_PCProgreso);
}


BEGIN_MESSAGE_MAP(CImportarDlg, CDialog)
	ON_MESSAGE(WM_MYSQL, OnMySQLMessage)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CImportarDlg message handlers

BOOL CImportarDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetWindowText(CLanguage::GetTraduccion(IDS_IMPORTANDO));
	m_pConector= (CConector *)AfxGetMainWnd()->SendMessage(WM_QUERYCONECTOR);
	SetTimer(1, 100, NULL);
	return TRUE;
}

LRESULT CImportarDlg::OnMySQLMessage(WPARAM wParam, LPARAM lParam) {

	CMySQLMessage mess(lParam);
	switch(wParam) {
		case QUERY_COUNT2:
			break;
		case QUERY_RESULT:
			if(mess.GetID()==query_tablas) {
				CString tabla= mess[0];
				tabla.MakeLower();
				if(tabla=="shared" || tabla=="busqueda")
					break;
				m_pTablas.Add(tabla , (DWORD)new CQuery(tabla));
				m_pTablasAlias.Add(tabla, (DWORD)new CQuery(tabla));
			}
			break;
		case THREAD_END:
			if(mess.GetID()==query_tablas) {
				if(LoadXML())
					ParseFile();
				else OnCancel();
			}
			break;
	}
	return 0;
}

bool CImportarDlg::LoadXML() {

	XmlReader xml;
	xml.Open(CApplicacion::GetAppPath(CLanguage::GetTraduccion(IDS_ALIAS)));
	CString szName,szValue;
	CString m_szOldName;
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
					for(int i=0; i<m_pTablas.GetCount(); i++) {
						if(m_pTablas[i]->GetNombre()==szValue) {
							actual= (CQuery *)m_pTablas[i]->GetData();
							alias= (CQuery *)m_pTablasAlias[i]->GetData();
							break;
						}
					}
				} else if(szName=="nombre") {
					m_szOldName= szValue;
				} else if(szName=="alias") {
					if(actual && alias) {
						szValue.MakeLower();
						(*alias)[szValue]=m_szOldName;
					}
				}
			break;
		}
	}
	m_PCProgreso.SetPos(1);
	if(m_pTablas.GetCount()==0) {
		szValue.Format(CLanguage::GetTraduccion(IDS_NOSEPUEDECARGARARCHIVO),CApplicacion::GetAppPath(CLanguage::GetTraduccion(IDS_ALIAS)));
		MessageBox(szValue, NULL, MB_ICONERROR);
		return false;
	}
	return true;
}

bool CImportarDlg::FindColumna(const CString &columna) {

	CString temp;
	for(int i=0; i<m_pTablasAlias.GetCount(); i++) {
		CQuery *query= (CQuery *)m_pTablasAlias[i]->GetData();
		temp= (*query)[columna];
		if(!temp.IsEmpty()) {
			m_pCampos.Add(query->GetTabla(), 0, temp);
			return true;
		}
	}
	temp.Format(CLanguage::GetTraduccion(IDS_NOSEENCONTROELCAMPOLLAMADO), columna);
	MessageBox(temp, NULL, MB_ICONERROR);
	return false;
}

bool CImportarDlg::ParseCabeceras(CStdioFile *file) {

	CString cabezera;
	if(!file->ReadString(cabezera))
		return false;
	CString palabra, temp;
	while(!cabezera.IsEmpty()) {
		palabra= GetLeftString(cabezera, AfxGetApp()->GetProfileString("control", "cvs", SEPARADOR_COMAS));
		temp= GetRightString(cabezera, AfxGetApp()->GetProfileString("control", "cvs", SEPARADOR_COMAS));
		// procesamos el encabezado.
		palabra.MakeLower();
		if(!FindColumna(palabra)) { // no encontro la columna.
			return false;
		}
		if(palabra=="pkz")
			m_nPKZ= m_pCampos.GetCount()-1;
		if(temp==cabezera)
			break;
		cabezera= temp;
	}
	if(m_nPKZ==-1) {
		MessageBox(CLanguage::GetTraduccion(IDS_NOSEENCONTROPKZ), NULL, MB_ICONERROR);
		return false;
	}
	return true;
}

CString CImportarDlg::ParseTablaEmpresa(const CString &campo, CString valor) {
	
	if(campo=="tipo") {
		if(!valor.IsEmpty()) {
			CStringArray valores;
			if(m_pConector->Find("SELECT id FROM shared WHERE nReservado=4 AND nombre='" + valor + "'", NULL, &valores)>=0) {
				if(valores.IsEmpty()) { // no existe
					CString formato;
					formato.Format(IDS_TIPODEEMPRESANOENCONTRADO, valor, m_nActual+2);
					if(AfxMessageBox(formato, MB_YESNO | MB_ICONWARNING)==IDYES) {
						CQuery query("shared");
						query["nReservado"]="4";
						query["nombre"]=valor;
						if(m_pConector->Find(query.GetInsert(), NULL, &valores)>0) 
							valor= valores[0];
					}
				} else valor= valores[0];
			}
		}
	}
	return valor;
}

CString CImportarDlg::GetArea(CString area) {
	
	if(!area.IsEmpty()) {
		CStringArray valores;
		if(m_pConector->Find("SELECT id FROM shared WHERE nReservado=3 AND nombre='" + area + "'", NULL, &valores)>=0) {
			if(valores.IsEmpty()) { // no existe
				CString formato;
				formato.Format(IDS_AREANOENCONTRADA, area, m_nActual+2);
				if(AfxMessageBox(formato, MB_YESNO | MB_ICONWARNING)==IDYES) {
					CQuery query("shared");
					query["nReservado"]="3";
					query["nombre"]=area;
					if(m_pConector->Find(query.GetInsert(), NULL, &valores)>0) 
						area= valores[0];
				}
			} else area= valores[0];
		}
	}
	return area;
}

CString CImportarDlg::ParseTablaEducacion(const CString &campo, CString valor) {

	if(campo=="area") 
		return GetArea(valor);
	return valor;
}

CString CImportarDlg::ParseTablaUsuario(const CString &campo, CString valor) {

	if(campo=="sexo") {
		if(!valor.CompareNoCase(CLanguage::GetTraduccion(IDS_NOAPLICA))) {
			valor="0";
		} else if(!valor.CompareNoCase(CLanguage::GetTraduccion(IDS_MASCULINO))) {
			valor="1";
		} else if(!valor.CompareNoCase(CLanguage::GetTraduccion(IDS_FEMENINO))) {
			valor="2";
		} else if(valor!="2" && valor!="1" && valor!="0" && !valor.IsEmpty()) {
			CString formato;
			formato.Format(CLanguage::GetTraduccion(IDS_SEXONOENCONTRADO), 
				m_nActual+2,
				valor,
				CLanguage::GetTraduccion(IDS_FEMENINO),
				CLanguage::GetTraduccion(IDS_MASCULINO),
				CLanguage::GetTraduccion(IDS_NOAPLICA));
			AfxMessageBox(formato);
		}
	} else if(campo=="correspondencia") {

		if(!valor.CompareNoCase(CLanguage::GetTraduccion(IDS_NOAPLICA))) {
			valor="0";
		} else if(!valor.CompareNoCase(CLanguage::GetTraduccion(IDS_OFICINA))) {
			valor="1";
		} else if(!valor.CompareNoCase(CLanguage::GetTraduccion(IDS_CASA))) {
			valor="2";
		} else if(valor!="2" && valor!="1" && valor!="0" && !valor.IsEmpty()) {
			CString formato;
			formato.Format(CLanguage::GetTraduccion(IDS_DESTINONOENCONTRADO), 
				m_nActual+2,
				valor,
				CLanguage::GetTraduccion(IDS_CASA),
				CLanguage::GetTraduccion(IDS_OFICINA),
				CLanguage::GetTraduccion(IDS_NOAPLICA));
			AfxMessageBox(formato);
		}
	}
	return valor;
}

CString CImportarDlg::ParseValor(const CString &tabla, const CString &campo, const CString &valor) {

	if(tabla=="usuario") {
		return ParseTablaUsuario(campo, valor);
	} else if(tabla=="empresa") {
		return ParseTablaEmpresa(campo, valor);
	} else if(tabla=="educacion") {
		return ParseTablaEducacion(campo, valor);
	}
	return valor;
}

bool CImportarDlg::SetQuery(const CString &tabla, const CString &columna, const CString &valor) {

	if(valor.IsEmpty())
		return true;
	for(int i=0; i<m_pTablas.GetCount(); i++) {
		CQuery *query= (CQuery *)m_pTablas[i]->GetData();
		if(query->GetTabla()==tabla) {
			(*query)[columna]= ParseValor(tabla, columna, valor);
			m_pTablas[i]->SetExtra("1");
			return true;
		}
	}
	return false;
}

bool CImportarDlg::ParseQuerys(CString pkz) {

	if(pkz.IsEmpty()) {
		CStringArray valores;
		if(m_pConector->Find("SELECT MAX(MID(pkz,9))+1 FROM usuario",this, &valores)==-1)
			return false;
		if(!valores.IsEmpty())
			pkz= valores[0];
		else pkz= "1";
		pkz= "NO_DAAD_" + pkz;
	}

	CString temp;
	for(int i=0; i<m_pTablas.GetCount(); i++) {
		CQuery *query= (CQuery *)m_pTablas[i]->GetData();
		if(!m_pTablas[i]->GetExtra().IsEmpty()) {

			temp= query->GetUpdate("pkz='" + pkz + "'");
			if(m_pConector->Find(temp)==-1)
				return false;

			(*query)["pkz"]= pkz;
			temp= query->GetInsert();
			temp.Replace("INSERT INTO ", "INSERT IGNORE INTO ");
			if(m_pConector->Find(temp)==-1)
				return false;
		}
		m_pTablas[i]->SetExtra("");
		query->Clean();
	}
	CGestionView::Actualizar(m_pConector, pkz, false);
	return true;
}

bool CImportarDlg::ParseLinea(CStdioFile *file, const CString &old/*=""*/, bool bPrimero/*=false*/) {

	CString cabezera;
	if(!file->ReadString(cabezera))
		return false;
	CString separador=AfxGetApp()->GetProfileString("control", "cvs", SEPARADOR_COMAS);
	
	bool primero=bPrimero;
	for(int i=0; i<cabezera.GetLength(); i++) {
		if(cabezera[i]=='\"') {
			primero=!primero;
			cabezera.Delete(i);
			i--;
		} else if(cabezera[i]==separador) {
			if(primero)
				cabezera.SetAt(i, ' ');
		}
	}
	if(!old.IsEmpty()) {
		m_nLineas--;
		cabezera= old + " " + cabezera;
	}
	if(primero)  // se quedo sin quitar la ultima coma
		return ParseLinea(file, cabezera, primero);
	CString pkz;
	CString palabra, temp;

	for(int i=0; i<m_pCampos.GetCount(); i++) {
		palabra= GetLeftString(cabezera, AfxGetApp()->GetProfileString("control", "cvs", SEPARADOR_COMAS));
		temp= GetRightString(cabezera, AfxGetApp()->GetProfileString("control", "cvs", SEPARADOR_COMAS));
		if(m_nPKZ==i)
			pkz= palabra;
		else 
			VERIFY(SetQuery(m_pCampos[i]->GetNombre(), m_pCampos[i]->GetExtra(), palabra));
		if(temp==cabezera) {
			// ASSERT(i==m_pCampos.GetCount()-1);
			cabezera.Empty();
		} else cabezera=temp;
	}
	if(!ParseQuerys(pkz))
		return false;
	return true;
}

bool CImportarDlg::ParseFile() {

	CWaitCursor wait;
	CStdioFile file;
	if(!file.Open(m_szArchivo, CFile::modeRead | CFile::shareDenyWrite)) {
		OnCancel();
		return false;
	}
	CString temp;
	m_nLineas=0;
	while(file.ReadString(temp)) {
		m_nLineas++;
	}
	file.SeekToBegin();
	if(!ParseCabeceras(&file)) {
		file.Close();
		OnCancel();
		return false;
	}
	m_nLineas--;
	m_nActual=0;
	while(ParseLinea(&file)) {

		m_nActual++;
		m_PCProgreso.SetPos((m_nActual*100)/m_nLineas);
	}
	file.Close();
	if(m_nActual==m_nLineas) {
		temp.Format(CLanguage::GetTraduccion(IDS_IMPORTACIONTERMINADA), m_nLineas);
		MessageBox(temp);
		OnOK();
	} else {
		temp.Format(CLanguage::GetTraduccion(IDS_ERRORENLINEA), m_nActual+2);
		MessageBox(temp, NULL, MB_ICONERROR);
		OnCancel();
	}
	return true;
}

void CImportarDlg::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent==1) {
		KillTimer(1);
		m_pConector->Query("SHOW TABLES;", query_tablas, this);
	}
	CDialog::OnTimer(nIDEvent);
}
