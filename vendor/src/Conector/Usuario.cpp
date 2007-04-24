#include "StdAfx.h"
# include "conector.h"
#include "usuario.h"

CUsuario::CUsuario(void)
{
	FUNCTION_STACK_TRACE("FFFFC02B");
	m_dwID= 0;
	m_dwGrupo=0;
	m_dwData= 0;
}

CUsuario::~CUsuario(void)
{
}

CUsuario::CUsuario(CString Nombre, DWORD id, DWORD grupo) {

	FUNCTION_STACK_TRACE("FFFFC02C");
	SetUser(Nombre);
	SetId(id);
	SetGrupo(grupo);
}

DWORD CUsuario::GetGrupo() {

	FUNCTION_STACK_TRACE("FFFFC02D");
	return m_dwGrupo;
}

void CUsuario::SetDescripcion(CString descripcion) {

	m_szDescripcion= descripcion;
}

CString	CUsuario::GetDescripcion() {

	return m_szDescripcion;
}

void CUsuario::SetGrupo(DWORD grupos) {

	FUNCTION_STACK_TRACE("FFFFC030");
	m_dwGrupo= grupos;
}

CString	CUsuario::GetGrupos() {

	FUNCTION_STACK_TRACE("FFFFC031");
	return GetGrupos(m_dwGrupo);
}

CString	CUsuario::GetGrupos(DWORD grupo) {

	FUNCTION_STACK_TRACE("FFFFC032");
	CString temp, total;
	DWORD ntotal= grupo;
	DWORD iterador=1;
	while( ntotal) {

		if( ntotal & iterador) { // found

			temp.Format( "%d", iterador);
			if( !total.IsEmpty())
				total+=",";
			total+= temp;
			ntotal-= iterador;
		}
		if( ntotal < iterador) // nos aseguramos que no entre en un bucle infinito
			break;
		iterador*=2;
	}
	return total;
}

DWORD	CUsuario::GetGrupos(CString grupo) {

	FUNCTION_STACK_TRACE("FFFFC033");
	CString temp;
	DWORD result=0;

	for( int i=0; i<grupo.GetLength(); i++) {

		if( grupo[i]==',') { // evalue el numero

			result|= atol(temp);
			temp.Empty();
		} else temp+= grupo[i];
	}
	if( !temp.IsEmpty()) // para evaluar el ultimo numero
		result|= atol(temp);
	return result;
}

void CUsuario::SetUser(CString usuario) {

	FUNCTION_STACK_TRACE("FFFFC034");
	m_szNombre= usuario;
	m_szCodificado= GetCodificado(usuario);
}

void CUsuario::SetId(DWORD	id) {

	FUNCTION_STACK_TRACE("FFFFC035");
	m_dwID= id;
}

DWORD CUsuario::GetID() {

	FUNCTION_STACK_TRACE("FFFFC036");
	return m_dwID;
}

CString CUsuario::GetNombre() {

	FUNCTION_STACK_TRACE("FFFFC037");
	return m_szNombre;
}

bool CUsuario::Pertenece(DWORD idGrupo) {

	FUNCTION_STACK_TRACE("FFFFC038");
	return (m_dwGrupo & idGrupo);
}

int CUsuario::GetNombresGrupos(CStringArray *nombres, CUIntArray *Codigos, CStringArray *metodo,CStringArray *descripciones) {

	FUNCTION_STACK_TRACE("FFFFC03A");
	ASSERT(nombres);
	XmlReader reader;
	
	CString szValue, szName;

	reader.Open(CApplicacion::GetAppPath("extras\\grupos.xml"));
	while(reader.ReadString()) {

		switch( reader.GetNodeType()) {
			case NODETYPE_CONTENT:
						reader.GetNodeName(szName);
						reader.GetNodeContent(szValue);
						szValue.TrimLeft("\t ");
						if(szValue.IsEmpty())
							break;
						if( !szName.CompareNoCase("nombre")) {
							nombres->Add(szValue);
							Codigos->Add(0);
							if( descripciones)
								descripciones->Add("");
							if( metodo)
								metodo->Add("");
						} else if( !szName.CompareNoCase("descripcion")) {
							if( descripciones)
								descripciones->SetAt(nombres->GetCount()-1, szValue);
						} else if(!szName.CompareNoCase("codigo")) 
							Codigos->SetAt(nombres->GetCount()-1, atol(szValue));
						else if( !szName.CompareNoCase("metodo")) {
							if( metodo)
								metodo->SetAt(nombres->GetCount()-1, szValue);
						}
						break;
		}
	}
	reader.Close();
	return nombres->GetCount();
}

void CUsuario::SetData(DWORD data) {
	
	m_dwData= data;
}

DWORD CUsuario::GetData() {

	return m_dwData;
}

CString CUsuario::GetCodificado(CString Usuario) {

	CMD5Checksum m_pCoding;
	CString prefijo=CMyRegKey::GetProfileString("control", "prefijo");

	char user[MAX_PATH];
	CString usuario= Usuario;

	usuario.MakeLower();
	strcpy(user, usuario);
	usuario= prefijo+m_pCoding.GetMD5((BYTE *)user, strlen(user));
	usuario= usuario.Mid(0,15);

	return usuario;
}

CString	CUsuario::GetCodificado() {
	if(m_szCodificado.IsEmpty())
		m_szCodificado= GetCodificado(m_szNombre);
	return m_szCodificado;
}