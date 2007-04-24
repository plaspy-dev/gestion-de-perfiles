#pragma once

class CItems
{
private:
	CItems		*m_pSig;
public:
	CString		m_szNombre;
	DWORD		m_dwData;
	float		m_fPrecio;
	CString		m_szDescripcion;
	bool		LoadDataFromXML(CString Archivo);
	void		AddItem( CString &nombre, DWORD &data, float &Precio, CString &Descripcion);
	CItems		*FindItem( CString &nombre);
	CItems(void);
	~CItems(void);
};
