#pragma once
# include "conector/conector.h"

class __IOFILE_EXPORT__ CImport
{	
	DWORD			m_dwTipo;
	
	CStringArray	m_pModelos;
	CUIntArray		m_nModelos;
	CStringArray	m_pMarcas;
	CUIntArray		m_nMarcas;
	CStringArray	m_pNombresEx;

	void			InitExportacion();
	bool			ExtractFiles(CString Archivo);
	bool			LoadIndex();
	bool			ComprobarIntegridad();
	void			Clean();
	CString			FindShared(CString Query);
	CString			FindShared(CString nombre, DWORD nReservado);
	void			ReplaceFromShared(CStringArray &valores, UINT index, DWORD nReservado);
	void			ReplaceProblema(CStringArray &valores, UINT index);
	CString			InsertEx(CString query);
public:

	bool			m_bForce;
	CWnd			*m_pParent;
	CString			m_szTempPath;
	CStringArray	m_pNombres;
	CUIntArray		m_pTipos;

	int				m_nCurrent;
	CString			CrearInsercion(CStringArray &nombres, CStringArray &valores, CString &extra);
	CString			CrearActualizacion(CStringArray &nombres, CStringArray &valores, CStringArray &result);
	CString			CrearComprobacion(CStringArray &nombres, CStringArray &valores);
	void			Extra( CStringArray &result);

	CConector			*m_pConector;
	CImport(CConector *pConector= NULL);
	~CImport(void);
	void SetConector( CConector *pConector);
	CString GetFileTypes();
	CString GetDefaultType();
	bool Importar(CString Archivo);
	static void  CreateDir( CString Parent, CString destino);
	static CString GetTemporalPath( CString ArchivoNuevo);
	static CString GetTemporalFile(int file);
};
