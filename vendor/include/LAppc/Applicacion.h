#pragma once

class __LAPPC_EXPORT__ CApplicacion
{
protected:
	static HINSTANCE m_pInstancia;
	static HBITMAP m_pBitmap;
	static CString m_szExeName;
	static CString m_szAppName;
	static CString m_szLongName;
	static CString m_szCompany;
	static CString m_szAppKey;
	static int		m_nID;
	static CString	m_szURL;
	// desde el dll
	static CString m_szAppPath;
public:
	CApplicacion(void);
	~CApplicacion(void);
	static HBITMAP GetBitmap();
	static CString GetExeName();
	static CString GetAppName();
	static CString GetLongName();
	static CString GetCompany();
	static CString GetAppKey();
	static int GetID();
	static CString GetURL();

	// calculados
	static CString GetAppPath();
	static CString GetAppPath(CString archivo);

	// set
	static void SetExeName(CString nombre);
	static void SetAppKey(CString key);
};
