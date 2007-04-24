#include "StdAfx.h"
#include "importthread.h"

CImportThread::CImportThread(void)
{
	m_pParent= NULL;
	m_pImport= NULL;
}

CImportThread::~CImportThread(void)
{
}

bool CImportThread::Init() {

	ASSERT( m_pImport);
	ASSERT( m_pParent);
	CStringArray valores;
	CStringArray nombres;
	CString		nodo;

	for( int i=0; i<m_pImport->m_pNombres.GetCount(); i++) {

		XmlReader	pXML;
		m_pImport->m_nCurrent= i;
		FILE *f;
		bool txt=false;
		f= fopen(m_pImport->m_szTempPath+m_pImport->m_pNombres.GetAt(i)+ ".xml","r");
		if(!f) {
			txt=true;
			f= fopen(m_pImport->m_szTempPath+m_pImport->m_pNombres.GetAt(i)+ ".txt","r");
		}
		if( f) {
			ULONG total=0;
			while(!feof(f)) {
				if(fgetc(f)=='\n')
					total++;
			}
			fclose(f);
			if( m_pParent)
				m_pParent->SendMessage( WM_IMPORTFILE, 0, txt ? total : ((total/2)-2));
		} else return false;
		
		if( txt) {

			f= fopen(m_pImport->m_szTempPath+m_pImport->m_pNombres.GetAt(i)+ ".txt","r");
			char c;
			CString szValue;
			ULONG total=0;
			if( AfxGetApp()->GetProfileInt("control", "lock", 0))
				m_pImport->m_pConector->Find("LOCK TABLES shared WRITE,usuario WRITE,partes WRITE,items WRITE,descripcion WRITE,empleados WRITE,fechas WRITE,movimiento WRITE,telefono WRITE,ods WRITE;", m_pParent, NULL);

			while(!feof(f)) {
				c= fgetc(f);
				switch( c) {
					case '\t':
						valores.Add(szValue);
						szValue.Empty();
						break;
					case '\n': // salto de linea
						{
							valores.Add(szValue);
							szValue.Empty();
							CStringArray result;
							CString extra;
							total++;

							if( m_pParent)
								m_pParent->SendMessage( WM_IMPORTFILE, 1, total);
							extra= m_pImport->CrearComprobacion( result, valores);
							if( !extra.IsEmpty() && m_pImport->m_pConector->Find( extra, m_pParent, &result)) { // lo encontro
								extra= m_pImport->CrearActualizacion(result, valores, result);
								if( !extra.IsEmpty())
									m_pImport->m_pConector->Find( extra, m_pParent,NULL);
							} else { // insertar
								szValue= m_pImport->CrearInsercion( result,valores, extra);
								if( !szValue.IsEmpty()) {
									m_pImport->m_pConector->Find( szValue, m_pParent, NULL);
									if( !extra.IsEmpty()) {

										m_pImport->m_pConector->Find( extra, m_pParent, &result);
										m_pImport->Extra( result);
									}
									szValue.Empty();
								}
							}
							valores.RemoveAll();
						}
					case '\r':
						break;
					default:
						szValue+=c;
						break;
				}
			}
			if( AfxGetApp()->GetProfileInt("control", "lock", 0))
				m_pImport->m_pConector->Find("UNLOCK TABLES;", m_pParent, NULL);
			fclose(f);
		} else {
			pXML.Open( m_pImport->m_szTempPath+m_pImport->m_pNombres.GetAt(i)+ ".xml");
			if(AfxGetApp()->GetProfileInt("control","lock", 0))
				m_pImport->m_pConector->Find("LOCK TABLES usuario WRITE;", m_pParent, NULL);
			while( pXML.ReadString()) {

				switch( pXML.GetNodeType()) {
					case NODETYPE_BEGINELEMENT:
					case NODETYPE_ENDELEMENT:
					{
						if( !nodo.CompareNoCase("data") && valores.GetCount()) {
							CStringArray result;
							CString extra;

							if( m_pParent)
								m_pParent->SendMessage( WM_IMPORTFILE, 1, ((pXML.GetCurrentLine()/2)-2));
							extra= m_pImport->CrearComprobacion( nombres, valores);

							if( !extra.IsEmpty() && m_pImport->m_pConector->Find( extra, m_pParent, &result)) { // lo encontro
								if( result.GetCount() && !result.GetAt(0).IsEmpty()) {
									extra= m_pImport->CrearActualizacion(nombres, valores, result);
									if( !extra.IsEmpty())
										m_pImport->m_pConector->Find( extra, m_pParent,NULL);
								}
							} else { // insertar
								m_pImport->m_pConector->Find( m_pImport->CrearInsercion( nombres,valores, extra), m_pParent, NULL);
								if( !extra.IsEmpty()) {
									m_pImport->m_pConector->Find( extra, m_pParent, &result);
									m_pImport->Extra( result);
								}
							}
						}
						valores.RemoveAll();
						nombres.RemoveAll();
						pXML.GetNodeName(nodo);
					}
					break;
					case NODETYPE_ATTRIB:
					{
						CString szName, szValue;
						pXML.GetAttrib(szName, szValue);
						valores.Add( GetUnescapedXmlString(szValue));
						nombres.Add( GetUnescapedXmlString(szName));
					}
				}		
			}
			pXML.Close();
			if(AfxGetApp()->GetProfileInt("control","lock", 0))
				m_pImport->m_pConector->Find("UNLOCK TABLES;", m_pParent, NULL);
		}
	}
	return CThreadAuto::Init();
}