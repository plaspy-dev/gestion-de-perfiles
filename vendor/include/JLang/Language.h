#pragma once

class __JLANG_EXPORT__ CLanguage
{
public:
	CLanguage(void);
public:
	~CLanguage(void);
protected:
	static HINSTANCE m_hInstDefault;
	static HINSTANCE m_hInstActual;
	static CString	 m_szNombre;
	static DWORD	 m_dwIdioma;
public:
	static void Clean();
	static void LoadLanguage(DWORD language=0);
	static void SetLanguage(DWORD language=0);
	static void InitLanguage();
	static DWORD GetLanguage();
	static CString GetTraduccion(UINT id);
	static CString GetTraduccion(const CString &cadena);
};
