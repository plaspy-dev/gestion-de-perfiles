//***************************************************************************
//
// AUTHOR:  JUHU
//
// DESCRIPTION: Conglomerado de funciones para sacar CRC de CString, File, 
//				Streaming.
//
// ORIGINAL: 
//
//***************************************************************************
#ifndef _CRC32STATIC_H_
#define _CRC32STATIC_H_

# include <windns.h>

class __LAPPC_EXPORT__ CCrc32Static
{
public:
	//***********************************************************************
	// Name:		CCrc32Static
	// Description:	Default constructor.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	CCrc32Static();
	//***********************************************************************
	// Name:		CCrc32Static
	// Description:	Destructor.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	virtual ~CCrc32Static();

// Operaciones
public:
	//***********************************************************************
	// Name:		StringCrc32
	// Description:	Obitiene el CRC de una cadena.
	// Parameters:	LPCTSTR (Cadena a la que se le aplica), DWORD (CRC)
	// Return:		DWORW (Codigo de error).	
	// Notes:		static.
	//***********************************************************************
	static DWORD StringCrc32(LPCTSTR szString, DWORD &dwCrc32);
	//***********************************************************************
	// Name:		StringCrc32
	// Description:	Obitiene el CRC de un archivo.
	// Parameters:	LPCTSTR (Paht del sis. archivos), DWORD (CRC)
	// Return:		DWORW (Codigo de error).	
	// Notes:		static FOpen... try FileCrc32Win32.
	//***********************************************************************
	static DWORD FileCrc32Streams(LPCTSTR szFilename, DWORD &dwCrc32);
	//***********************************************************************
	// Name:		StringCrc32
	// Description:	Obitiene el CRC de un archivo.
	// Parameters:	LPCTSTR (Paht del sis. archivos), DWORD (CRC)
	// Return:		DWORW (Codigo de error).	
	// Notes:		static CFile try FileCrc32Assembly.
	//***********************************************************************
	static DWORD FileCrc32Win32(LPCTSTR szFilename, DWORD &dwCrc32);
	//***********************************************************************
	// Name:		StringCrc32
	// Description:	Obitiene el CRC de un archivo (en RAM).
	// Parameters:	LPCTSTR (Paht del sis. archivos), DWORD (CRC)
	// Return:		DWORW (Codigo de error).	
	// Notes:		static Trabaja el archivo en la RAM.
	//***********************************************************************
	static DWORD FileCrc32Filemap(LPCTSTR szFilename, DWORD &dwCrc32);
	//***********************************************************************
	// Name:		StringCrc32
	// Description:	Obitiene el CRC de un archivo.
	// Parameters:	LPCTSTR (Paht del sis. archivos), DWORD (CRC)
	// Return:		DWORW (Codigo de error).	
	// Notes:		static try new life.
	//***********************************************************************
	static DWORD FileCrc32Assembly(LPCTSTR szFilename, DWORD &dwCrc32);

protected:
	static bool GetFileSizeQW(const HANDLE hFile, QWORD &qwSize);
	static inline void CalcCrc32(const BYTE byte, DWORD &dwCrc32);
	static DWORD s_arrdwCrc32Table[256];
};

#endif
