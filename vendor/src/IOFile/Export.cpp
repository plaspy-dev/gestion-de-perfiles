#include "StdAfx.h"
#include "export.h"
# include "IOFile.h"
# include "DlgExportar.h"
# include "XZip.h"

CExport::CExport(CConector *pConector)
{
	m_pConector= pConector;
	m_dwTipo=0;
	m_nCurrent= 0;
	m_pXML= NULL;
	m_nAgregados=0;
	m_pFile= NULL;
	m_pHtml= NULL;
	m_bVacio= false;
	m_nColumna= -1;

	char temporal[MAX_PATH];

	GetTempPath( MAX_PATH, temporal);
	m_szTempPath= temporal;
	m_szTempPath+="Export\\";
	CreateDirectory(m_szTempPath,NULL);

	m_bNumero= AfxGetApp()->GetProfileInt("iofile", "tonumeros", 0)==1;
}

CExport::~CExport(void)
{
	if( m_pXML)
		delete m_pXML;
	if( m_pHtml)
		delete m_pHtml;
}

void CExport::SetTipoColumna(int columna, int tipo) {

	while(m_uaTipos.GetCount()<=columna)
		m_uaTipos.Add(1);
	m_uaTipos.SetAt(columna, tipo+1);
}

void CExport::SetConector( CConector *pConector) {
	
	m_pConector= pConector;
}

void CExport::Vacio() {

	m_bVacio= true;
	if( m_dwTipo==EXTENCION_HTML) {

		if( !m_pHtml) {

			m_pHtml= new CHtmlFile( m_szFinalFile);
			m_pHtml->CreateHeader( m_pNombres.GetAt(m_pNombres.GetCount()-1), m_szHtmlHeader);
			if( !m_pHtml->VolcarFile(m_szHtmlVoid))
				m_pHtml->Write("<br>No se encontraron resultados<br>");
		}
	}
}

void CExport::SetHtmlHeader(CString Archivo) {

	m_szHtmlHeader= Archivo;
}

void CExport::SetHtmlFooter(CString Archivo) {

	m_szHtmlFooter=Archivo;
}
void CExport::SetHtmlEmpty(CString Archivo) {

	m_szHtmlVoid=Archivo;
}

void CExport::Inicio(CStringArray &valores) {

	m_pAtributos.RemoveAll();
	
	if( m_pTipos.GetAt(m_nCurrent)==TIPO_ODS) {
		for( int i=0; i<valores.GetCount(); i++) {
			if( valores.GetAt(i)=="id") { // removemos todos los id ya que no nos interesan
				valores.RemoveAt(i);
				i--;
			}
		}
		valores.SetAt(44, "min");
		valores.SetAt(39, "ciudadguia");
		valores.SetAt(37, "guianumero");
	}
	for( int i=0; i< valores.GetCount(); i++) 
		m_pAtributos.Add( valores.GetAt(i));

	if( m_dwTipo==EXTENCION_XML || m_dwTipo==EXTENCION_SIT) {

		if( !m_pXML) 
			m_pXML= new XmlWriter();
		m_pXML->Open(m_szTempPath+m_pNombres.GetAt(m_nCurrent)+ ".xml");

		XmlElement wkey;
		wkey.SetName(m_pNombres.GetAt(m_nCurrent));
		wkey.Write(*m_pXML);
	} else if( m_dwTipo==EXTENCION_EXCEL) {

		for( int i=0; i<m_pAtributos.GetCount(); i++) {

			miniexcel(m_nAgregados,i) = m_pAtributos.GetAt(i);
			miniexcel(m_nAgregados,i).setAlignament(ALIGN_CENTER);
		}
		SetTipoColumna(m_pAtributos.GetCount(), 1);
		m_nAgregados++;
	} else if( m_dwTipo==EXTENCION_TXT || m_dwTipo==EXTENCION_DESCONOCIDA) {

		if( !m_pFile)

			m_pFile= fopen( m_szFinalFile, "w");
	} else if( m_dwTipo==EXTENCION_HTML && !m_bVacio) {

		if( !m_pHtml) {

			m_pHtml= new CHtmlFile( m_szFinalFile);
			m_pHtml->CreateHeader( m_pNombres.GetAt(m_pNombres.GetCount()-1), m_szHtmlHeader);
		}
		if( m_pHtml) {

			m_pHtml->Write("<table border=\"1\">\n");
			m_pHtml->Write("\t<tr>\n");
			for( int i=0; i<m_pAtributos.GetCount(); i++)
				m_pHtml->WriteEx("<td><em><strong>%s</strong></em></td>", m_pAtributos.GetAt(i));
			m_pHtml->Write("\t</tr>\n");
		}
	}
	m_bVacio= false;
}

void CExport::Terminar() {

	FILE *f;
	if( m_dwTipo==EXTENCION_XML || m_dwTipo==EXTENCION_SIT) {

		XmlElement wkey;
		wkey.SetName(m_pNombres.GetAt(m_nCurrent));
		wkey.WriteClosingTag(*m_pXML);
		m_pXML->Close();
		CString temp= m_szTempPath+m_pNombres.GetAt(m_nCurrent);
		if( m_pXML) {
			delete m_pXML;
			m_pXML= NULL;
		}
		DWORD valor;
		CCrc32Static::FileCrc32Streams( temp+".xml", valor);
		temp+= ".crc";
		if( f=fopen(temp, "w")) {

			fprintf(f, "%lu", valor);
			fclose(f);
		}
	} else if( m_dwTipo==EXTENCION_EXCEL) { 

		if( m_nCurrent>= m_pNombres.GetCount()-1)
			
			if( f=fopen(m_szFinalFile, "wb")) 

				miniexcel.Write(f);
		m_nAgregados++;
	} else if( m_dwTipo==EXTENCION_TXT || m_dwTipo==EXTENCION_DESCONOCIDA) {
		if( m_nCurrent>=m_pNombres.GetCount()-1)
			
			if( m_pFile)

				fclose(m_pFile);
	} else if( m_dwTipo==EXTENCION_HTML) {
			
		if( !m_bVacio)
			if( m_pHtml) 
				m_pHtml->Write( "</table>\n");
		if( m_nCurrent>=m_pNombres.GetCount()-1)
			
			if( m_pHtml) {

				m_pHtml->CreateFooter( m_szHtmlFooter);
				delete m_pHtml;
				m_pHtml= NULL;
			}
	}
	m_nCurrent++;
}

CString	CExport::FindShared(CString id) {

	CStringArray result;
	CString temp=id;
	if( m_pConector->Find("SELECT nombre FROM shared WHERE id="+id, NULL, &result)) 
		temp= result.GetAt(0);
	return temp;
}

void CExport::ReplaceFromShared(CStringArray &valores, UINT index) {

	CString temp= valores.GetAt(index);
	if( !temp.IsEmpty()) {
		valores.RemoveAt(index);
		valores.InsertAt(index, FindShared(temp)); 
	}
}

void CExport::ReplaceProblema(CStringArray &valores, UINT index) {

	CString temp= valores.GetAt(index);
	if( !temp.IsEmpty()) {
		int pos= temp.Find("?");
		if( pos!=-1) {
			CString query= temp.Mid(0, pos);
			valores.RemoveAt(index);
			temp.Replace(query, FindShared(query));
			valores.InsertAt(index, temp);
		}
	}
}

double CExport::ConvertToNumero(const char *valor) {

	const char *temp=valor;
	if(!*temp)
		return NULL;
	while( *temp) {
		if((*temp)<'0' || (*temp)>'9') // no es un numero
			if((*temp)!='.' && !((*temp)=='-' && (*temp)==(*valor))) // no es un signo
				return NULL;
        temp++;
	}
	return atof(valor);
}

void CExport::Agregar(CStringArray &valores, CString link) {

	int len=valores.GetCount();
	int tipo= m_pTipos.GetAt(m_nCurrent);
	switch( tipo) {			
		case TIPO_MODELO:
			m_nModelos.Add(atol( valores.GetAt(4)));
			m_pModelos.Add( valores.GetAt(0));
			len--;
		case TIPO_MODELOEX:
			{	
				CString marca= NumerosToMarcas( m_pMarcas, m_nMarcas, valores.GetAt(3));
				valores.RemoveAt(3);
				valores.InsertAt(3, marca);
			}
			break;
		case TIPO_MARCA:
			m_nMarcas.Add(atol( valores.GetAt(4)));
			m_pMarcas.Add(valores.GetAt(0));
			len--;
			break;
		case TIPO_ITEM:
			{
				// 6 marca
				CString marca= NumerosToMarcas( m_pMarcas, m_nMarcas, valores.GetAt(6));
				valores.RemoveAt(6);
				valores.InsertAt(6, marca);
				// 7 modelo
				marca= NumerosToMarcas( m_pModelos, m_nModelos, valores.GetAt(7));
				valores.RemoveAt(7);
				valores.InsertAt(7, marca);
			}
			break;
		case TIPO_ODS:
			// todos los ids removidos
			valores.RemoveAt(47);
			valores.RemoveAt(36);
			valores.RemoveAt(27);
			valores.RemoveAt(19);
			valores.RemoveAt(14);
			len= valores.GetCount();
			ReplaceFromShared(valores, 2);
			ReplaceFromShared(valores, 18);
			ReplaceFromShared(valores, 19);
			ReplaceFromShared(valores, 20);
			ReplaceFromShared(valores, 21);
			ReplaceFromShared(valores, 22);
			ReplaceFromShared(valores, 23);
			ReplaceFromShared(valores, 24);
			ReplaceFromShared(valores, 39);
			ReplaceFromShared(valores, 40);
			ReplaceFromShared(valores, 41);
			ReplaceFromShared(valores, 42);
			ReplaceFromShared(valores, 47);
			ReplaceProblema(valores, 17);
			break;
	}

	if( m_dwTipo== EXTENCION_XML || m_dwTipo==EXTENCION_SIT) {
	
		XmlElement wkey;
		CString atributo;
		CString valor;
		wkey.SetName( "data");
		for( int i=0; i<len; i++) {
			atributo= m_pAtributos.GetAt(i);
			valor= valores.GetAt(i);
			wkey.AddAttrib( GetEscapedXmlString(atributo),GetEscapedXmlString(valor));
		}
		wkey.Write( *m_pXML);
		wkey.WriteClosingTag( *m_pXML);
	} else if( m_dwTipo== EXTENCION_EXCEL) {

		double valor=NULL;
		int tipo;
		COleDateTime t1;

		for( int i=0; i<len;i++) {

			tipo= m_uaTipos.GetAt(i);
			if( tipo==1) {
				if( m_bNumero && ((valor=this->ConvertToNumero(valores.GetAt(i)))!=NULL || valores.GetAt(i)=="0")) 
					miniexcel(m_nAgregados, i)= valor;
				else miniexcel(m_nAgregados,i)= valores.GetAt(i);
			} else if( tipo==2) { // entero
				miniexcel(m_nAgregados, i)= atof(valores.GetAt(i));
			} else if( tipo==3) {
				miniexcel(m_nAgregados, i)= atof(CEditFloat::GetNormal(valores.GetAt(i)));
			} else miniexcel(m_nAgregados,i)= valores.GetAt(i);
		}
		m_nAgregados++;
	} else if( m_dwTipo==EXTENCION_TXT || m_dwTipo==EXTENCION_DESCONOCIDA) {

		if( m_pFile) {
			
			for( int i=0; i<len;i++) {

				if(i)
					fprintf(m_pFile, AfxGetApp()->GetProfileString("exportar", "separador", "\t"));
				fprintf(m_pFile, "%s", valores.GetAt(i));
			}
			fprintf(m_pFile, "\n");
		}
	} else if( m_dwTipo==EXTENCION_HTML) {

		if( m_pHtml) {

			bool useimages= AfxGetApp()->GetProfileInt("control", "images", 1)==1;
			m_pHtml->Write("\t<tr>\n");
			for( int i=0; i<len;i++) {

				if(useimages && !i && m_nColumna!=-1) {
					m_pHtml->WriteEx("\t\t<td><a name=\"%s\"></a><a href=\"?ods=%s#%s\"><img src=\"image.gif\"></a>", link.IsEmpty() ? valores.GetAt(m_nColumna): link, link.IsEmpty() ? valores.GetAt(m_nColumna): link,link.IsEmpty() ? valores.GetAt(m_nColumna): link);
					m_pHtml->WriteEx("%s</td>\n", valores.GetAt(i));
				} else if( !useimages && i==m_nColumna)
					m_pHtml->WriteEx("\t\t<td><a name=\"%s\"></a><a href=\"?ods=%s#%s\">%s</a></td>\n", link.IsEmpty() ? valores.GetAt(i) : link, link.IsEmpty() ? valores.GetAt(i) : link, link.IsEmpty() ? valores.GetAt(i) : link, valores.GetAt(i), link.IsEmpty() ? valores.GetAt(i) : link, valores.GetAt(i));
				else m_pHtml->WriteEx("\t\t<td>%s</td>\n", valores.GetAt(i));
			}
			m_pHtml->Write("\t</tr>\n");
		}
	}
}

CString CExport::GetDefaultType(bool custom) {

	if(custom)
		return "xls";
	return CApplicacion::GetAppName();
}

CString CExport::GetFileTypes(bool custom) {

	if( custom)
		return "Archivos de Excel (*.xls)|*.xls|Archivos xml (*.xml)|*.xml|Pagina Web (*.html)|*.html|Archivos de texto (*.txt)|*.txt||";
	return "Archivos SIT (*.sit)|*.sit|Archivos de Excel (*.xls)|*.xls| Archivos xml (*.xml)|*.xml|Pagina Web (*.html)|*.html|Archivos de texto (*.txt)|*.txt||";
}

void CExport::InitExportacion() {

	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_nAgregados=0;
	m_bVacio= false;
	CDlgExportar dlg;
	dlg.m_pExport= this;
	dlg.m_szNombre= &m_pNombres;
	dlg.m_szQuery= &m_pQuerys;
	dlg.DoModal();
}

void CExport::SetTipo( CString Archivo) {

	if( Archivo.Find(".xml")>0)
		m_dwTipo= EXTENCION_XML;
	else if( Archivo.Find(".html")>0 || Archivo.Find(".htm")>0)
		m_dwTipo= EXTENCION_HTML;
	else if( Archivo.Find(".txt")>0)
		m_dwTipo=EXTENCION_TXT;
	else if( Archivo.Find(".sve")>0)
		m_dwTipo=EXTENCION_SIT;
	else if( Archivo.Find(".xls")>0)
		m_dwTipo=EXTENCION_EXCEL;
	else m_dwTipo=EXTENCION_DESCONOCIDA;
	m_szFinalFile= Archivo;
}

void CExport::CleanExportacion(CString Archivo) {

	if( m_dwTipo== EXTENCION_SIT) {

		FILE *f;
		if( f=fopen(m_szTempPath+"index.txt", "w")) {
			for( int i=0; i<m_pNombres.GetCount(); i++)
				fprintf(f, "%s\n", m_pNombres.GetAt(i));
			fclose(f);
		}
		HZIP hZip = NULL;
		hZip = CreateZip((LPVOID)(LPCTSTR)Archivo, 0, ZIP_FILENAME);
		if (!hZip)
			return;
		ZRESULT zr;

		CString nombres;
		CString path;
		// agregamos el index
		nombres= m_szTempPath+"index.txt";
		ZipAdd(hZip, "index.txt", (LPVOID)(LPCTSTR)nombres, 0, ZIP_FILENAME);
		DeleteFile( nombres);
		for( int i=0; i<m_pNombres.GetCount(); i++) {

			nombres= m_pNombres.GetAt(i);
			path= m_szTempPath+nombres+".xml";
			zr = ZipAdd(hZip, nombres+".xml", (LPVOID)(LPCTSTR)path, 0, ZIP_FILENAME);
			if( zr!=ZR_OK)
				break;
			DeleteFile( path);
			path= m_szTempPath+nombres+".crc";
			zr = ZipAdd(hZip, nombres+".crc", (LPVOID)(LPCTSTR)path, 0, ZIP_FILENAME);
			if( zr!=ZR_OK)
				break;
			DeleteFile( path);
		}
		if (hZip)
			CloseZip(hZip);
	}
}

void CExport::CreateMarcas() {

	// modelos 13
	// codigos de reparacion 8
	// sintomas 7
	// marcas 6
	m_pModelos.RemoveAll();
	m_nModelos.RemoveAll();
	m_pMarcas.RemoveAll();
	m_nMarcas.RemoveAll();

	// primero las marcas
	m_pTipos.Add(TIPO_MARCA);
	m_pQuerys.Add( "SELECT nombre,descripcion,nReservado,szReservado,id FROM shared WHERE nReservado=6;");
	m_pNombres.Add("Marcas");
	// luego los modelos
	m_pTipos.Add(TIPO_MODELO);
	m_pQuerys.Add( "SELECT nombre,descripcion,nReservado,szReservado,id FROM shared WHERE nReservado=13;");
	m_pNombres.Add("Modelos");
	// el resto
	m_pTipos.Add(TIPO_MODELOEX);
	m_pQuerys.Add( "SELECT nombre,descripcion,nReservado,szReservado FROM shared WHERE nReservado=7 OR nReservado=8;");
	m_pNombres.Add("codigos");	
}

void CExport::CreateUsuarios() {

	m_pTipos.Add(TIPO_NORMAL);
	m_pQuerys.Add( "SELECT nombre,descripcion,nReservado,szReservado FROM shared WHERE (nReservado=17 OR nReservado=15 OR nReservado=2 OR nReservado=3 OR nReservado=4 OR nReservado=5)");
	m_pNombres.Add("usuarios");
}

void CExport::CreateItems() {

	m_pTipos.Add(TIPO_ITEM);
	// nReservado= marca
	// szReservado= modelo
	m_pQuerys.Add("SELECT nombre,codigo,descripcion,valor,Maximo,Minimo,nReservado,szReservado FROM items");
	m_pNombres.Add("repuestos");
}

void CExport::CreateMiscelaneos() {

	m_pTipos.Add(TIPO_NORMAL);
	m_pQuerys.Add("SELECT nombre,descripcion,nReservado,szReservado FROM shared WHERE (nReservado=14 OR nReservado=12 OR nReservado=11 OR nReservado=10 OR nReservado=9 OR nReservado=1 OR nReservado=17)");
	m_pNombres.Add("miscelaneos");
}

void CExport::CreateConfiguracion() {

	m_pTipos.Add(TIPO_NORMAL);
	m_pQuerys.Add("SELECT nombre,descripcion,nReservado,szReservado FROM shared WHERE nReservado=16;");
	m_pNombres.Add("configuracion");
}

bool CExport::ExportMarcas(CString Archivo) {
	
	if( !m_pConector || !m_pConector->GetDatabasesCount())
		return false;
	SetTipo( Archivo);
	m_nCurrent=0;
	m_pQuerys.RemoveAll();
	m_pNombres.RemoveAll();
	m_pTipos.RemoveAll();
	CreateMarcas();
	InitExportacion();
	CleanExportacion(Archivo);
	return true;
}

bool CExport::ExportODS(CString Archivo) {

	if( !m_pConector || !m_pConector->GetDatabasesCount())
		return false;
	SetTipo( Archivo);
	m_nCurrent=0;
	m_pQuerys.RemoveAll();
	m_pNombres.RemoveAll();
	m_pTipos.RemoveAll();
	m_pTipos.Add(TIPO_ODS);
	m_pQuerys.Add( "SELECT * FROM ods LEFT JOIN descripcion ON (descripcion.id=ods.descripcion) LEFT JOIN empleados ON (empleados.id=ods.empleados) LEFT JOIN fechas ON (fechas.id=ods.fechas) LEFT JOIN movimiento ON (movimiento.id=ods.guia) LEFT JOIN telefono ON (telefono.id=ods.telefono)");
	m_pNombres.Add("ods");
	InitExportacion();
	CleanExportacion(Archivo);
	return true;
}

bool CExport::ExportClientes(CString Archivo) {

	if( !m_pConector || !m_pConector->GetDatabasesCount())
		return false;
	SetTipo( Archivo);
	m_nCurrent=0;
	m_pQuerys.RemoveAll();
	m_pNombres.RemoveAll();
	m_pTipos.RemoveAll();
	m_pTipos.Add(TIPO_NORMAL);
	m_pQuerys.Add( "SELECT * FROM usuario");
	m_pNombres.Add("clientes");
	InitExportacion();
	CleanExportacion(Archivo);
	return true;
}

bool CExport::ExportUsuarios(CString Archivo) {

	if( !m_pConector || !m_pConector->GetDatabasesCount())
		return false;
	SetTipo( Archivo);
	m_nCurrent=0;
	m_pQuerys.RemoveAll();
	m_pNombres.RemoveAll();
	m_pTipos.RemoveAll();
	CreateUsuarios();
	InitExportacion();
	CleanExportacion(Archivo);
	return true;
}

bool CExport::ExportConfiguracion(CString Archivo) {
	if(!m_pConector ||  !m_pConector->GetDatabasesCount())
		return false;
	SetTipo( Archivo);
	m_nCurrent=0;
	m_pQuerys.RemoveAll();
	m_pNombres.RemoveAll();
	m_pTipos.RemoveAll();
	CreateConfiguracion();
	InitExportacion();
	CleanExportacion(Archivo);
	return true;
}

bool CExport::ExportMiscelaneos(CString Archivo) {

	if( !m_pConector || !m_pConector->GetDatabasesCount())
		return false;
	SetTipo( Archivo);
	m_nCurrent=0;
	m_pQuerys.RemoveAll();
	m_pNombres.RemoveAll();
	m_pTipos.RemoveAll();
	CreateMiscelaneos();
	InitExportacion();
	CleanExportacion(Archivo);
	return true;
}

bool CExport::CustomExport( CString Archivo, CString Query, CString nombre) {

	if( !m_pConector || !m_pConector->GetDatabasesCount())
		return false;
	SetTipo( Archivo);
	m_nCurrent=0;
	m_pQuerys.RemoveAll();
	m_pNombres.RemoveAll();
	m_pTipos.RemoveAll();
	m_pQuerys.Add(Query);
	m_pNombres.Add(nombre);
	m_pTipos.Add(TIPO_NORMAL);
	InitExportacion();
	CleanExportacion(Archivo);
	return true;
}

bool CExport::CustomExportLocal( CString Archivo, CString nombre) {

	FILE *f;
	if( !(f=fopen(Archivo, "w")))
		return false;
	fclose(f);
	m_bVacio= false;
	SetTipo( Archivo);
	m_nCurrent=0;
	m_pQuerys.RemoveAll();
	m_pNombres.RemoveAll();
	m_pTipos.RemoveAll();
	m_pNombres.Add(nombre);
	m_pTipos.Add(TIPO_NORMAL);
	m_nAgregados=0;
	return true;
}

bool CExport::ExportAll( CString Archivo) {

	if( !m_pConector || !m_pConector->GetDatabasesCount())
		return false;
	SetTipo( Archivo);
	m_nCurrent=0;
	m_pQuerys.RemoveAll();
	m_pNombres.RemoveAll();
	m_pTipos.RemoveAll();
	CreateUsuarios();
	CreateConfiguracion();
	CreateMiscelaneos();
	CreateMarcas();
	CreateItems();
	InitExportacion();
	CleanExportacion(Archivo);
	return true;
}

bool CExport::ExportItems(CString Archivo) {
	if( !m_pConector || !m_pConector->GetDatabasesCount())
		return false;
	SetTipo( Archivo);
	m_nCurrent=0;
	m_pQuerys.RemoveAll();
	m_pNombres.RemoveAll();
	m_pTipos.RemoveAll();
	CreateMarcas();
	CreateItems();
	InitExportacion();
	CleanExportacion(Archivo);
	return true;
}

bool CExport::ExportFiles(CString archivo, CStringArray &archivos, CStringArray &nombreszip) {

	if( archivos.GetCount() != nombreszip.GetCount())
		return false;
	FILE *f;
	if( f=fopen(m_szTempPath+"index.txt", "w")) { // guardamos el index.txt
		fprintf(f, "consultas\n");
		fclose(f);
	}
	HZIP hZip = NULL;
	hZip = CreateZip((LPVOID)(LPCTSTR)archivo, 0, ZIP_FILENAME);
	if (!hZip)
		return false;
	ZRESULT zr;

	CString nombres;
	CString path;
	// agregamos el index
	nombres= m_szTempPath+"index.txt";
	ZipAdd(hZip, "index.txt", (LPVOID)(LPCTSTR)nombres, 0, ZIP_FILENAME);
	DeleteFile( nombres);

	for( int i=0; i<archivos.GetCount(); i++) {

		nombres= archivos.GetAt(i);
		path= nombreszip.GetAt(i);
		zr = ZipAdd(hZip, path, (LPVOID)(LPCTSTR)nombres, 0, ZIP_FILENAME);
		if( zr!=ZR_OK)
			break;
	}
	if (hZip)
		CloseZip(hZip);
	return true;
}