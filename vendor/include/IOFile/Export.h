#pragma once
#include "mexcel.h"
# include "htmlfile.h"
# include "conector/conector.h"

using namespace miniexcel;

class __IOFILE_EXPORT__ CExport
{
	bool			m_bVacio;
	DWORD			m_dwTipo;
	void			SetTipo( CString Archivo);
	XmlWriter		*m_pXML;
	CHtmlFile		*m_pHtml;
	CMiniExcel		miniexcel;
	int				m_nCurrent;
	ULONG			m_nAgregados;
	FILE			*m_pFile;
	CStringArray	m_pAtributos;
	CString			m_szTempPath;
	CString			m_szFinalFile;
	CStringArray	m_pQuerys;
	CStringArray	m_pNombres;
	CUIntArray		m_pTipos;
	CStringArray	m_pModelos;
	CUIntArray		m_nModelos;
	CStringArray	m_pMarcas;
	CUIntArray		m_nMarcas;
	CString			m_szHtmlHeader;
	CString			m_szHtmlFooter;
	CString			m_szHtmlVoid;
	bool			m_bNumero;
	void			InitExportacion();
	CUIntArray		m_uaTipos;
	// generador de querys
	void	CreateMarcas();
	void	CreateUsuarios();
	void	CreateItems();
	void	CreateConfiguracion();
	void	CreateMiscelaneos();
	void	CleanExportacion(CString Archivo);
	CString	FindShared(CString id);
	void	ReplaceFromShared(CStringArray &valores, UINT index);
	void	ReplaceProblema(CStringArray &valores, UINT index);
public:

	CConector			*m_pConector;
	CExport(CConector *pConector= NULL);
	~CExport(void);
	CHtmlFile *GetHtmlFile() {
		return m_pHtml;
	}
	void SetTipoColumna(int columna, int tipo);
	static double ConvertToNumero(const char *valor);
	int			m_nColumna;
	void SetConector( CConector *pConector);
	CString GetFileTypes(bool custom=false);
	CString GetDefaultType(bool custom=false);
	bool ExportMarcas(CString Archivo);
	void	SetHtmlHeader(CString Archivo);
	void	SetHtmlFooter(CString Archivo);
	void	SetHtmlEmpty(CString Archivo);
	bool ExportUsuarios(CString Archivo);
	bool ExportConfiguracion(CString Archivo);
	bool ExportItems(CString Archivo);
	bool ExportMiscelaneos( CString Archivo);
	bool ExportClientes(CString Archivo);
	bool ExportODS(CString Archivo);
	bool ExportAll( CString Archivo);
	bool CustomExport( CString Archivo, CString Query,CString nombre);
	bool CustomExportLocal( CString Archivo, CString nombre);
	
	void	Terminar();
	void	Agregar(CStringArray &valores, CString link="");
	void	Inicio(CStringArray &valores);
	void	Vacio();
	bool	ExportFiles(CString archivo, CStringArray &archivos, CStringArray &nombreszip);
};
