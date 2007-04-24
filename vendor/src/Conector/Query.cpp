# include "stdafx.h"
#include "Query.h"
# include "Conector.h"

CQuery::CQuery(const CString &tabla)
{
	m_szTabla= tabla;
}

CQuery::~CQuery(void)
{
}

void CQuery::SetTabla(const CString &tabla) {

	m_szTabla= tabla;
}

CString CQuery::GetTabla() {

	return m_szTabla;
}


CString& CQuery::operator[]( const CString &campo) {

	int index=-1;
	for( int i=0;i< m_saCampos.GetCount(); i++)
		if(m_saCampos[i]==campo) {
			index=i;
			break;
		}
	if(index==-1) {

		index= m_saCampos.Add(campo);
		m_saValores.Add("");
		m_baQuoted.Add(true);
	} 
	ASSERT(index!=-1);
	return m_saValores[index];
}

void CQuery::AddCampo(const CString &campo, const CString &valor, bool quotes/*=true*/) {

	int index=-1;
	for( int i=0;i< m_saCampos.GetCount(); i++)
		if(m_saCampos[i]==campo) {
			index=i;
			break;
		}
	if(index==-1) {

		index= m_saCampos.Add(campo);
		m_saValores.Add(valor);
		m_baQuoted.Add(quotes);
	}  else {

		m_saValores[index]= valor;
		m_saCampos[index]=campo;
		m_baQuoted[index]=quotes;
	}
}

CString CQuery::GetText(const CString &campo) {

	for( int i=0; i< m_saCampos.GetCount(); i++)
		if(m_saCampos[i]==campo) 
			return m_saValores[i];
	ASSERT(false);
	return "";
}

void CQuery::SetText(const CString &campo, const CString &valor) {

	int index=-1;
	for( int i=0;i< m_saCampos.GetCount(); i++)
		if(m_saCampos[i]==campo) {
			index=i;
			break;
		}
	if(index==-1) {

		m_saCampos.Add(campo);
		m_saValores.Add(valor);
		m_baQuoted.Add(true);
	} else {
		m_saValores[index]= valor;
	}
}

void CQuery::CleanQuote(const CString &campo) {
	int index=-1;
	for( int i=0;i< m_saCampos.GetCount(); i++)
		if(m_saCampos[i]==campo) {
			index=i;
			break;
		}
	ASSERT(index!=-1);
	if(index!=-1)
		m_baQuoted[index]=false;
}

void CQuery::Clean() {

	m_saCampos.RemoveAll();
	m_saValores.RemoveAll();
	m_baQuoted.RemoveAll();
	m_baBinario.RemoveAll();
	m_szBinario="";
}

CString CQuery::GetValor(const CString &valor) {

	if(valor.IsEmpty())
		return "NULL";
	if(valor.CompareNoCase("SYSDATE()")==0)
		return valor;
	CString otro= valor;
	otro.Replace("\\", "\\\\");
	otro.Replace("\"", "\\\"");
	return "\"" + otro +"\"";
}

void CQuery::SetBinario(const CString &campo, const CByteArray &arreglo) {
	
	m_szBinario= campo;
	m_baBinario.Copy(arreglo);
	AddCampo(m_szBinario, "%0binario%");
}

CString CQuery::GetUpdate(const CString condicion) {

	ASSERT(m_saValores.GetCount()>0);
	ASSERT(m_saValores.GetCount()==m_saCampos.GetCount());
	if(m_saValores.IsEmpty())
		return "";
	
	CString query;
	CString valor;

	query="UPDATE " + m_szTabla + " SET ";
	for( int i=0; i<m_saCampos.GetCount(); i++) {

		if( i)
			query+=",";
		valor= m_baQuoted[i] ? GetValor(m_saValores[i]) : m_saValores[i];
		query+= m_saCampos[i]+"="+ valor;
	}

	// almacenamos el numero binario.
	CString binario;
	if(!m_baBinario.IsEmpty()) {
		for(int i=0; i<m_baBinario.GetCount(); i++) {
			switch( m_baBinario[i]) {
				case 0:
					binario+="\\0";
					break;
				case '\\':
					binario+="\\\\";
					break;
				case '\"':
					binario+="\\\"";
					break;
				default:
					binario+= m_baBinario[i];
					break;
			}
		}
	}
	if(!condicion.IsEmpty())
		query+= " WHERE " + condicion;
	query.Replace("%0binario%", binario);
	return query;
}

CString CQuery::GetInsert() {

	ASSERT(m_saValores.GetCount()>0);
	ASSERT(m_saValores.GetCount()==m_saCampos.GetCount());
	if(m_saValores.IsEmpty())
		return "";

	CString query;

	query= "INSERT INTO " + m_szTabla + " (";
	for( int i=0; i<m_saCampos.GetCount(); i++) {
		if( i)
			query+=",";
		query+=m_saCampos.GetAt(i);
	}

	query+=") VALUES (";
	for( int i=0; i<m_saValores.GetCount(); i++) {

		if(i)
			query+=",";
		query+= m_baQuoted[i] ? GetValor(m_saValores[i]) : m_saValores[i];
	}
	// calculamos el binario
	CString binario;
	if(!m_baBinario.IsEmpty()) {
		for(int i=0; i<m_baBinario.GetCount(); i++) {
			switch( m_baBinario[i]) {
				case 0:
					binario+="\\0";
					break;
				case '\\':
					binario+="\\\\";
					break;
				case '\"':
					binario+="\\\"";
					break;
				default:
					binario+= m_baBinario[i];
					break;
			}
		}
	}
	query+=");";
	query.Replace("%0binario%", binario);
	return query;
}

CString	CQuery::GetSelect(const CString condicion/*=""*/, const CString join/*=""*/) {
	
	ASSERT(m_saValores.GetCount()>0);
	ASSERT(m_saValores.GetCount()==m_saCampos.GetCount());
	if(m_saValores.IsEmpty())
		return "";

	CString query="SELECT ";
	CString valor;

	for( int i=0; i<m_saCampos.GetCount(); i++) {
		if(i)
			query+=",";
		if(m_saValores[i]==m_saCampos[i])
			query+=m_saValores[i];
		else query+=m_saValores[i] + " AS '" + m_saCampos[i] + "'";
	}

	query+=" FROM " + m_szTabla + " ";
	query+=join + " ";
	if(!condicion.IsEmpty())
		query+= " WHERE " + condicion;
	return query;
}


// obsoletas funciones obsoletas

CString CQuery::CreateUpdate(CString tabla, CStringArray &Campos, CStringArray &Valores, CString condicion) {

	ASSERT(Campos.GetCount()==Valores.GetCount());

	FUNCTION_STACK_TRACE("FFFFC008");
	CString query;
	CString valor;

	query="UPDATE " + tabla + " SET ";
	for( int i=0; i<Campos.GetCount(); i++) {

		if( i)
			query+=",";
		valor= Valores.GetAt(i);
		valor.Replace("\\", "\\\\");
		valor.Replace("\"", "\\\"");
		query+= Campos.GetAt(i)+"=";
		query+= valor.IsEmpty() ? "NULL" : !valor.CompareNoCase("SYSDATE()") ? valor : "\"" + valor + "\"";
	}
	if(!condicion.IsEmpty())
		query+= " WHERE " + condicion;
	return query;
}

CString CQuery::CreateInsert(CString tabla, CStringArray &valores, CString condicionselect) {

	FUNCTION_STACK_TRACE("FFFFC109");
	CString query;
	CString valor;
	query= "INSERT INTO " + tabla + " SELECT ";
	for( int i=0; i<valores.GetCount(); i++) {

		if(i)
			query+=",";
		valor= valores.GetAt(i);
		valor.Replace("\\", "\\\\");
		valor.Replace("\"", "\\\"");
		query+= valor.IsEmpty() ? "NULL" : !valor.CompareNoCase("SYSDATE()") ? valor : "\"" + valor + "\"";
	}
	query+=" " + condicionselect + ";";
	return query;
}

CString CQuery::CreateInsert(CString tabla, CStringArray &campos,CStringArray &valores) {

	ASSERT(valores.GetCount()==campos.GetCount());
	CString query;
	CString valor;

	query= "INSERT INTO " + tabla + " (";
	for( int i=0; i<campos.GetCount(); i++) {
		if( i)
			query+=",";
		query+=campos.GetAt(i);
	}
	query+=") VALUES (";
	for( int i=0; i<valores.GetCount(); i++) {

		if(i)
			query+=",";
		valor= valores.GetAt(i);
		valor.Replace("\\", "\\\\");
		valor.Replace("\"", "\\\"");
		query+= valor.IsEmpty() ? "NULL" : !valor.CompareNoCase("SYSDATE()") ? valor : "\"" + valor + "\"";
	}
	query+=");";
	return query;
}

CString CQuery::CreateInsert(CString tabla, CStringArray &Valores) {

	FUNCTION_STACK_TRACE("FFFFC009");
	CString query;
	CString valor;

	query= "INSERT INTO " + tabla + " VALUES (";
	for( int i=0; i<Valores.GetCount(); i++) {

		if(i)
			query+=",";
		valor= Valores.GetAt(i);
		valor.Replace("\\", "\\\\");
		valor.Replace("\"", "\\\"");
		query+= valor.IsEmpty() ? "NULL" : !valor.CompareNoCase("SYSDATE()") ? valor : "\"" + valor + "\"";
	}
	query+=");";
	return query;
}

void CQuery::operator+=(const CString &campo) {

	AddCampo(campo, campo);
}
