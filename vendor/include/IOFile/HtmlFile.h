#pragma once

class __IOFILE_EXPORT__ CHtmlFile
{
private:
	FILE *m_hFile;
	CString m_szTitulo;
protected:
	bool VolcarFile( FILE *destino);
public:
	CHtmlFile(CString Archivo);
	CHtmlFile(void);
	~CHtmlFile(void);
	
	bool VolcarFile(CString Archivo);
	bool SetFile(CString Archivo);
	bool CreateHeader(CString Titulo,CString Archivo="");
	bool CreateFooter(CString Archivo="");
	bool WriteEx(const char *Palabra, ...);
	bool Write(CString Palabra);
	bool GenericHeader(CString Titulo);
	bool GenericFooter();
	void	SetTitulo(CString titulo);
};
