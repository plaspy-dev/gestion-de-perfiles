#include "StdAfx.h"
#include "import.h"
# include "XUnzip.h"
# include "iofile.h"
# include "DlgImportar.h"

CImport::CImport(CConector *pConector)
{
	m_pConector= pConector;

	char temporal[MAX_PATH];
	srand( (unsigned)time( NULL ) );
	GetTempPath( MAX_PATH, temporal);

	m_szTempPath.Format("%sSitImport%d\\",temporal,rand());

	CreateDirectory(m_szTempPath,NULL);
	m_nCurrent=0;
	m_pParent= NULL;
	m_bForce= false;
#ifdef _DEBUG
	m_bForce=true;
#endif
}

CImport::~CImport(void)
{
	Clean();
}

void CImport::SetConector( CConector *pConector) {
	
	m_pConector= pConector;
}

CString CImport::GetFileTypes() {

	return "Archivo sve(*.sve)|*.sve| Todos los archivos (*.*)|*.*||";
}

CString CImport::GetDefaultType() {

	return CApplicacion::GetAppName();
}

CString	CImport::FindShared(CString Query) {

	CStringArray result;
	if( m_pConector->Find(Query, NULL, &result)) 
		return result.GetAt(0);
	return NULL;
}

CString	CImport::FindShared(CString nombre, DWORD nReservado) {

	CString query;
	query.Format("SELECT id FROM shared WHERE nombre=\"%s\" AND nReservado=%d", nombre, nReservado);
	query= FindShared(query);
	if( !query.IsEmpty())
		return query;
	CStringArray valores;
	valores.Add("");
	valores.Add(nombre);
	valores.Add("");
	query.Format("%d", nReservado);
	valores.Add(query);
	valores.Add("");
	return InsertEx(CQuery::CreateInsert("shared", valores));
}

void CImport::ReplaceFromShared(CStringArray &valores, UINT index, DWORD nReservado) {

	CString temp= valores.GetAt(index);
	if( !temp.IsEmpty()) {
		valores.RemoveAt(index);
		valores.InsertAt(index, FindShared(temp, nReservado)); 
	}
}

void CImport::ReplaceProblema(CStringArray &valores, UINT index) {

	CString temp= valores.GetAt(index);
	if( !temp.IsEmpty()) {
		int pos= temp.Find("?");
		if( pos!=-1) {
			CString query= temp.Mid(0, pos);
			valores.RemoveAt(index);
			temp.Replace(query, FindShared(query, 14));
			valores.InsertAt(index, temp);
		}
	}
}

CString	CImport::InsertEx(CString query) {

	m_pConector->Find(query, m_pParent, NULL);
	CStringArray result;
	m_pConector->Find("SELECT LAST_INSERT_ID()", m_pParent, &result);
	return result.GetAt(0);
}

CString	CImport::CrearInsercion(CStringArray &nombres, CStringArray &valores,CString &extra) {

	valores.InsertAt(0, "");
	extra.Empty();
	CString tabla= "shared";

	switch( m_pTipos.GetAt(m_nCurrent)) {
		case TIPO_MARCA:
			m_pMarcas.Add( valores.GetAt(1));
			extra= "SELECT LAST_INSERT_ID();";
			break;
		case TIPO_MODELO:
			m_pModelos.Add( valores.GetAt(1));
			extra= "SELECT LAST_INSERT_ID();";
			break;
		case TIPO_NORMAL:
		case TIPO_MODELOEX:
			break;
		case TIPO_ITEM:
			tabla= "items";
			valores.Add("");
			break;
		case TIPO_CLIENTE:
			valores.RemoveAt(0);
			while(valores.GetCount()<5)
				valores.Add("");
			tabla= CQuery::CreateInsert( "usuario", valores);
			tabla.Replace("INSERT", "INSERT IGNORE");
			return tabla;
			break;
		case TIPO_REPARACION:
			{
				if( valores.GetCount()<5) 
					return NULL;
				ReplaceFromShared(valores, 2, 7);
				ReplaceFromShared(valores, 3, 8);
				CString temp=valores.GetAt(4);
				temp.Remove(' ');

				if( !temp.IsEmpty()) { // item
				
					temp.Format("SELECT id FROM items WHERE codigo=\"%s\"", valores.GetAt(4));
					temp= FindShared(temp);
					if( temp.IsEmpty()) { // nuevo item
						CStringArray nuevo;
						nuevo.Add("");
						nuevo.Add("Desconocido");
						temp= valores.GetAt(4);
						nuevo.Add(valores.GetAt(4));
						nuevo.Add("");
						nuevo.Add("1");
						nuevo.Add("");
						nuevo.Add("");
						nuevo.Add("");
						nuevo.Add("");
						nuevo.Add("1");
						temp= InsertEx(CQuery::CreateInsert("items",nuevo));
					}
					valores.SetAt(4, temp);
				}
				// eliminamos la descripcion
				if( valores.GetCount()>5) {
					temp= valores.GetAt(5);
					valores.RemoveAt(5);
					if( !temp.IsEmpty()) {
						CString nueva;
						nueva.Format("SELECT observacionFinal FROM ods WHERE numero=%s", valores.GetAt(1));
						temp+= " " + FindShared(nueva);
						nueva.Format("UPDATE ods SET observacionFinal=\"%s\" WHERE numero=%s", temp, valores.GetAt(1));
						FindShared(nueva);
					}
				}
				valores.InsertAt(2, "0");
				valores.InsertAt(3, "0");
				temp= CQuery::CreateInsert("partes", valores);
				return temp;
			}
			break;
		case TIPO_ODS:
			{
				if(valores.GetCount()<49)
					return NULL;
				valores.RemoveAt(0);
				// ReplaceFromShared(valores, 2, 1);
				if( valores.GetAt(18).IsEmpty())
					valores.SetAt(18, "Desconocida");
				ReplaceFromShared(valores, 18, 5);
				ReplaceFromShared(valores, 19, 2);
				ReplaceFromShared(valores, 20, 3);
				ReplaceFromShared(valores, 21, 4);
				ReplaceFromShared(valores, 22, 3);
				ReplaceFromShared(valores, 23, 2);
				ReplaceFromShared(valores, 24, 2);
				ReplaceFromShared(valores, 39, 5);
				ReplaceFromShared(valores, 40, 5);
				ReplaceFromShared(valores, 41, 3);
				ReplaceFromShared(valores, 42, 3);
				if( valores.GetAt(47).IsEmpty())
					valores.SetAt(47,"Desconocido");
				ReplaceFromShared(valores, 47, 13);
				ReplaceProblema(valores, 17);

				CString temp;
				CString otro, log;

				// ciudad
				temp.Format("SELECT descripcion FROM shared WHERE id=%s", valores.GetAt(18));
				valores.SetAt(2,FindShared(temp));
				if(valores.GetAt(2).IsEmpty())
					valores.SetAt(2, "Desconocida");
				ReplaceFromShared(valores, 2, 1);

				if( m_pMarcas.IsEmpty()) {
					CStringArray resultado;
					m_pConector->Find("select nombre,szReservado from shared where nReservado=9", NULL, &resultado);
					for( int i=0; i<resultado.GetCount(); i+=2) {
						
						temp= resultado.GetAt(i);
						otro= temp;
						m_pMarcas.Add(temp);
						if( temp.GetAt(temp.GetLength()-1)=='a') 
							temp.SetAt(temp.GetLength()-1, 'o');
						else if( temp.GetAt(temp.GetLength()-1)=='o') 
							temp.SetAt(temp.GetLength()-1, 'a');
						if( temp!=otro) {
							m_pMarcas.Add(temp);
							m_nMarcas.Add(atol(resultado.GetAt(i+1)));
						}
						m_nMarcas.Add(atol(resultado.GetAt(i+1)));
					}
					m_pMarcas.Add(" ");
					m_nMarcas.Add(2);
				}
				temp= valores.GetAt(14);
				log= MarcasToNumerosEx(m_pMarcas, m_nMarcas, temp, otro);
				while(!log.IsEmpty()) {
					
					int h= 0;
					for( int i=0; i<m_nMarcas.GetCount(); i++)
						if( m_nMarcas.GetAt(i)>h)
							h= m_nMarcas.GetAt(i);
					h++;
					otro.Format("INSERT INTO shared VALUES(NULL, \"%s\", NULL, 9, %d)", log, h);
					m_pConector->Find(otro, NULL, NULL);
					m_pMarcas.Add(log);
					m_nMarcas.Add(h);
					otro= log;
					if( log.GetAt(log.GetLength()-1)=='a') 
						log.SetAt(log.GetLength()-1, 'o');
					else if( log.GetAt(log.GetLength()-1)=='o') 
						log.SetAt(log.GetLength()-1, 'a');
					if( log!= otro) {
						m_pMarcas.Add(log);
						m_nMarcas.Add(h);
					}
					log= MarcasToNumerosEx(m_pMarcas, m_nMarcas, temp, otro);
				}
				otro.Remove(',');
				temp= otro;
				char myvalor=0;
				for( int i=0; i<temp.GetLength(); i++) {
					myvalor= temp.GetAt(i)-'0';
					temp.SetAt(i, myvalor ? myvalor : 2);
				}
				int len= temp.GetLength();
				for( int i=0; i<len/2; i++) { // invertimos todo
					char a= temp[i];
					temp.SetAt(i, temp[len-1-i]);
					temp.SetAt(len-1-i, a);
				}
				valores.SetAt(14, temp);

				CStringArray nuevos;
				// comenzamos con la descripcion
				nuevos.Add("");
				for( int i=14; i<18; i++)
					nuevos.Add(valores.GetAt(i));
				valores.SetAt(6,InsertEx(CQuery::CreateInsert("descripcion",nuevos)));
				// empleados
				nuevos.RemoveAll();
				nuevos.Add("");
				for( int i=18; i<25; i++)
					nuevos.Add(valores.GetAt(i));
				if(nuevos.GetAt(2).IsEmpty())
					nuevos.SetAt(2, "1");
				valores.SetAt(5,InsertEx(CQuery::CreateInsert("empleados",nuevos)));

                // fechas
				nuevos.RemoveAll();
				nuevos.Add("");
				for( int i=25; i<33; i++)
					nuevos.Add(valores.GetAt(i));
				if(nuevos.GetAt(2).IsEmpty())
					nuevos.SetAt(2, "SYSDATE()");
				valores.SetAt(9,InsertEx(CQuery::CreateInsert("fechas",nuevos)));

				// envios
				if( !valores.GetAt(7).IsEmpty()) {
					nuevos.RemoveAll();
					nuevos.Add("");
					for( int i=33; i<43; i++)
						nuevos.Add(valores.GetAt(i));
					valores.SetAt(7,InsertEx(CQuery::CreateInsert("movimiento",nuevos)));
				}
				// telefono
				nuevos.RemoveAll();
				nuevos.Add("");
				if( valores.GetAt(45).IsEmpty())
					valores.SetAt(45, " ");
				for( int i=43; i<48; i++)
					nuevos.Add(valores.GetAt(i));
				valores.SetAt(4,InsertEx(CQuery::CreateInsert("telefono",nuevos)));

				nuevos.RemoveAll();
				for( int i=0; i<14; i++)
					nuevos.Add(valores.GetAt(i));
				if(nuevos.GetAt(2).IsEmpty())
					nuevos.SetAt(2, "1");
				return CQuery::CreateInsert("ods", nuevos);
			}
			break;
	}
	return CQuery::CreateInsert( tabla, valores);
}

CString	CImport::CrearActualizacion(CStringArray &nombres, CStringArray &valores,CStringArray &result) {

	CString tabla= "shared";
	switch( m_pTipos.GetAt(m_nCurrent)) {
		case TIPO_MARCA:
			m_nMarcas.Add( atol( result.GetAt(0)));
			m_pMarcas.Add(result.GetAt(1));
			break;
		case TIPO_MODELO:
			if(result.GetCount()<2)
				break;
			m_nModelos.Add(atol( result.GetAt(0)));
			m_pModelos.Add(result.GetAt(1));
			break;
		case TIPO_ITEM:
			tabla= "items";
			break;
		case TIPO_CLIENTE:
			return NULL;
			break;
		case TIPO_REPARACION:
			return NULL;
			break;
		case TIPO_ODS:
			if(result.GetAt(0).IsEmpty())
			{
				m_pConector->Find("DELETE FROM telefono WHERE id="+result.GetAt(5), m_pParent,NULL);
				m_pConector->Find("DELETE FROM empleados WHERE id="+result.GetAt(6), m_pParent,NULL);
				m_pConector->Find("DELETE FROM descripcion WHERE id="+result.GetAt(7), m_pParent,NULL);
				if(!result.GetAt(8).IsEmpty())
					m_pConector->Find("DELETE FROM movimiento WHERE id="+result.GetAt(8), m_pParent,NULL);
				m_pConector->Find("DELETE FROM fechas WHERE id="+result.GetAt(10), m_pParent,NULL);
				m_pConector->Find("DELETE FROM ods WHERE numero="+result.GetAt(1), m_pParent,NULL);
				m_pConector->Find("DELETE FROM partes WHERE ods="+result.GetAt(1), m_pParent,NULL);
				CString temp;
				return CrearInsercion(nombres,valores, temp);
			} 
			m_pConector->Find("DELETE FROM partes WHERE ods="+result.GetAt(1), m_pParent,NULL);
			return NULL;
			break;
		case TIPO_NORMAL:
		case TIPO_MODELOEX:
			break;
		default:
			return "";
	}
	return CQuery::CreateUpdate( tabla, nombres, valores, "id="+ result.GetAt(0));
}

void CImport::Extra( CStringArray &result) {

	switch( m_pTipos.GetAt(m_nCurrent)) {
		case TIPO_MARCA:
			m_nMarcas.Add(atol(result.GetAt(0)));
			break;
		case TIPO_MODELO:
			m_nModelos.Add(atol(result.GetAt(0)));
			break;
		case TIPO_NORMAL:
		case TIPO_ITEM:
		case TIPO_MODELOEX:
			break;
	}
}

CString	CImport::CrearComprobacion(CStringArray &nombres, CStringArray &valores) {

	CString query;
	switch( m_pTipos.GetAt(m_nCurrent)) {
		case TIPO_MODELO:
		case TIPO_MODELOEX:
			query= MarcasToNumeros( m_pMarcas, m_nMarcas, valores.GetAt(3));
			valores.RemoveAt(3);
			valores.InsertAt(3, query);
			query.Format("SELECT id,nombre FROM shared WHERE nReservado=%s AND szReservado=\"%s\" AND nombre=\"%s\"",
				valores.GetAt(2),valores.GetAt(3),valores.GetAt(0));
			break;
		case TIPO_MARCA:
		case TIPO_NORMAL:
			query.Format("SELECT id,nombre FROM shared WHERE nReservado=%s AND nombre=\"%s\"", valores.GetAt(2),valores.GetAt(0));
			break;
		case TIPO_CLIENTE:
			// no compruebe nada
			return NULL;
			break;
		case TIPO_REPARACION:
			return NULL;
			break;
		case TIPO_ODS:
			query.Format("SELECT fechas.reparacion,ods.* FROM ods LEFT JOIN fechas ON (fechas.id=ods.fechas) WHERE numero=\"%s\"", valores.GetAt(0));
			break;
		case TIPO_ITEM:
			query= MarcasToNumeros( m_pMarcas, m_nMarcas, valores.GetAt(6));
			valores.RemoveAt(6);
			valores.InsertAt(6, query);
			query= MarcasToNumeros( m_pModelos, m_nModelos, valores.GetAt(7));
			valores.RemoveAt(7);
			valores.InsertAt(7, query);
			query.Format( "SELECT id FROM items WHERE nReservado=\"%s\" AND nombre=\"%s\"", valores.GetAt(6), valores.GetAt(0));
			break;
		default:
			return "";
	}
	return query;
}

bool CImport::Importar(CString Archivo) {

	if(!ExtractFiles(Archivo)) {

		AfxMessageBox("No se puede Leer el archivo\r\nEl archivo no es compatible o esta corrupto", MB_ICONERROR | MB_OK);
		return false;
	} if( !LoadIndex()) {

		AfxMessageBox("No se puede Leer el archivo\r\nEl archivo no es compatible o esta corrupto", MB_ICONERROR | MB_OK);
		return false;
	}
	if( !ComprobarIntegridad()) {


		if(m_bForce ) {
			if( MessageBox(NULL, "No se puede comprobar la integridad del archivo\r\nEsta seguro de continuar.",NULL, MB_ICONQUESTION | MB_YESNO)==IDNO) {
				return false;
			}
		} else {
			AfxMessageBox("No se puede comprobar la integridad del archivo", MB_ICONERROR);
			return false;
		}
	}
	m_pModelos.RemoveAll();
	m_nModelos.RemoveAll();
	m_pMarcas.RemoveAll();
	m_nMarcas.RemoveAll();
	if( m_pTipos.GetAt(0)==TIPO_CONSULTA) {
		
		CString temp=m_szTempPath;
		m_szTempPath= CApplicacion::GetAppPath();
		// ahora si descomprimimos las consultas donde debe ser
		if(!ExtractFiles(Archivo)) {
			m_szTempPath= temp;
			AfxMessageBox("No se pueden importar las consultas", MB_ICONERROR | MB_OK);
			return false;
		}
		m_szTempPath= temp;
	} else {
		if( !m_pConector) {

			AfxMessageBox("No se pueden importar datos.\r\nDebe ingresar primero con un usuario", MB_ICONERROR | MB_OK);
			return false;
		}
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
		CDlgImportar dlg;
		dlg.m_pImport= this;
		dlg.DoModal();
	}
	return true;
}

void  CImport::CreateDir( CString Parent, CString destino) {

	FUNCTION_STACK_TRACE("0001AAAB");
	int pos=-1;
	CString temp;
	destino.Replace("\\", "/");
	if( Parent.GetAt(Parent.GetLength()-1)=='\\') 
		Parent.Delete(Parent.GetLength()-1);
	while( (pos= destino.Find("/", pos+1))!=-1) {

		temp= "\\";
		temp+= destino.Left( pos);
		CreateDirectory( Parent+ temp, NULL);
	}
}

bool CImport::ExtractFiles(CString Archivo) {

	ZIPENTRY ze;
	memset(&ze, 0, sizeof(ze));
	char ruta[MAX_PATH];

	strcpy( ruta, Archivo);
	m_pNombresEx.RemoveAll();

	HZIP hz = OpenZip(ruta, 0, ZIP_FILENAME);
	if (hz) {

		ZRESULT zr = GetZipItem(hz, -1, &ze);
		if (zr == ZR_OK) {

			int numitems= ze.index;
			for( int i=0; i<numitems; i++) {
	
				GetZipItem(hz, i, &ze);
				sprintf(ruta, "%s%s", m_szTempPath,ze.name);
				m_pNombresEx.Add(ze.name);
				CreateDir( m_szTempPath, ze.name);
				zr= UnzipItem(hz,i, ruta, 0, ZIP_FILENAME);
			}
		}
		CloseZip(hz);
	} else return false;
	return true;
}

bool CImport::LoadIndex() {

	FILE *f;
	m_pNombres.RemoveAll();
	m_nCurrent=0;
	if( f= fopen(m_szTempPath+"index.txt", "r")) {

		char nombres[MAX_PATH];
		CString real;

		while(!feof(f)) {

			if( fgets( nombres, MAX_PATH, f)) {
			
				real= nombres;
				if( !real.IsEmpty()) {
					real.Remove('\r');
					real.Remove('\n');
					m_pNombres.Add(real);
					if( !real.CompareNoCase("Marcas"))
						m_pTipos.Add(TIPO_MARCA);
					else if( !real.CompareNoCase("miscelaneos") || !real.CompareNoCase("configuracion") || !real.CompareNoCase("usuarios"))
						m_pTipos.Add(TIPO_NORMAL);
					else if(!real.CompareNoCase("Modelos"))
						m_pTipos.Add(TIPO_MODELO);
					else if(!real.CompareNoCase("codigos"))
						m_pTipos.Add(TIPO_MODELO);
					else if(!real.CompareNoCase("repuestos"))
						m_pTipos.Add(TIPO_ITEM);
					else if(!real.CompareNoCase("clientes")) 
						m_pTipos.Add(TIPO_CLIENTE);
					else if(!real.CompareNoCase("ods"))
						m_pTipos.Add(TIPO_ODS);
					else if(!real.CompareNoCase("reparacion"))
						m_pTipos.Add(TIPO_REPARACION);
					else if(!real.CompareNoCase("consultas")) 
						m_pTipos.Add(TIPO_CONSULTA);
					else m_pTipos.Add(TIPO_DESCONOCIDA);
				}
			}
		}
		fclose(f);
		return true;
	}
	return false;
}

bool CImport::ComprobarIntegridad() {

	CStdioFile f;
	CString nuevoValor;
	CString real;

	if( m_pTipos.GetAt(0)==TIPO_CONSULTA) {

		CString temp, mycrc;
		for( int i=0; i<m_pNombresEx.GetCount(); i++) {
			temp= m_pNombresEx.GetAt(i);
			if( temp.Right(4)==".xml") {
				mycrc= temp;
				mycrc.Replace(".xml", ".key");

				if(f.Open(m_szTempPath+mycrc, CFile::modeRead)) {

					real=CMD5Checksum::GetMD5(m_szTempPath+temp);
					f.ReadString(nuevoValor);
					f.Close();
					if( real!= nuevoValor) 
						return false;
				} else { // no se pudo abrir el archivo.
					return false;
				}
			}
		}
	} else {
		for( int i=0; i<m_pNombres.GetCount(); i++) {

			if(f.Open(m_szTempPath+ m_pNombres.GetAt(i) + ".key", CFile::modeRead)) {

				real= CMD5Checksum::GetMD5(m_szTempPath+m_pNombres.GetAt(i)+".xml");
				f.ReadString(nuevoValor);
				f.Close();
				if( real!= nuevoValor) 
					return false;
			} else { // no se pudo abrir el archivo
				return false;
			}
		}
	}
	return true;
}

CString CImport::GetTemporalPath( CString ArchivoNuevo) {

	TCHAR szEXEPath[MAX_PATH];

	GetTempPath(MAX_PATH, szEXEPath);
	CString Archivo= szEXEPath;
	Archivo+= CApplicacion::GetAppName() + "\\";
	CreateDirectory( Archivo,NULL);
	Archivo+= ArchivoNuevo;
	return Archivo;
}

CString CImport::GetTemporalFile(int file) {

	CString archivo;
	archivo.Format("temp%d.xml", file);
	return GetTemporalPath(archivo);
}

void CImport::Clean() {

	for( int i=0; i<m_pNombresEx.GetCount(); i++) 
		DeleteFile(m_szTempPath + m_pNombresEx.GetAt(i));
	RemoveDirectory(m_szTempPath);
}
