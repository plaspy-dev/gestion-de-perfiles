#pragma once

class CFile; // forward declaration
//***************************************************************************
//
// AUTHOR:  JUHU
//
// DESCRIPTION: XML Writer (see XML Reader).
//
// ORIGINAL: 
//
//***************************************************************************
class  __COMPONENTES_EXPORT__ XmlWriter
{
	// Construction
public:
	//***********************************************************************
	// Name:		XmlWriter
	// Description:	Constructor.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	XmlWriter();
	//***********************************************************************
	// Name:		XmlReader
	// Description:	Destructor.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	virtual ~XmlWriter();

// Attributes
protected:
	CFile *m_pFile;
	BOOL m_bFileOpen, m_bPrototypeWritten;
	CString m_szFilename;
	int m_nLevel;

// Operations
public:
	//***********************************************************************
	// Name:		Init
	// Description:	file related.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	void Init();
	//***********************************************************************
	// Name:		Open
	// Description:	Open file.
	// Parameters:	CString &szFilename.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	BOOL Open(CString &szFilename);
	//***********************************************************************
	// Name:		Close
	// Description:	Cerrar archivo.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	BOOL Close();

	virtual BOOL WriteString(CString &szData);

	// Accessors
	void SetIndentLevel(int n) { m_nLevel = n; }
	int GetIndentLevel() { return m_nLevel; }
	CString &GetFilename() { return m_szFilename; }

	BOOL MustBeClosed() { return m_bFileOpen; }

};
//***************************************************************************
//
// AUTHOR:  JUHU
//
// DESCRIPTION: XmlStringWriter, see XmlWriter.
//
// ORIGINAL: 
//
//***************************************************************************
class __COMPONENTES_EXPORT__ XmlStringWriter : public XmlWriter
{
	// Construction
public:
	//***********************************************************************
	// Name:		XmlStringWriter
	// Description:	Constructor.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	XmlStringWriter();
	//***********************************************************************
	// Name:		XmlStringWriter
	// Description:	Constructor.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	virtual ~XmlStringWriter();

// Attributes
protected:
	CString m_szContent;

// Operations
public:
	//***********************************************************************
	// Name:		Init
	// Description:	Iniciar.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	void Init();
	//***********************************************************************
	// Name:		&GetContent
	// Description:	Obtener contenido.
	// Parameters:	None.
	// Return:		CString.
	// Notes:		None.
	//***********************************************************************
	CString &GetContent();
	//***********************************************************************
	// Name:		Close
	// Description:	Cerrar Cadena.
	// Parameters:	None.
	// Return:		BOOL.	
	// Notes:		None.
	//***********************************************************************
	BOOL Close();

	virtual BOOL WriteString(CString &szData);

};

class __COMPONENTES_EXPORT__ XmlElement
{
// Construction
public:
	//***********************************************************************
	// Name:		XmlElement
	// Description:	Constructor.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	XmlElement();
	//***********************************************************************
	// Name:		XmlElement
	// Description:	Constructor.
	// Parameters:	const CString &szName.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	XmlElement(const CString &szName);

// Attributes
protected:
	CString m_szName;
	CStringArray m_szAttribNames, m_szAttribValues;

	// Accessors
public:
	//***********************************************************************
	// Name:		SetName
	// Description:	Nobre del conjunto de datos.
	// Parameters:	const CString &szName.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	void SetName(/*in*/const CString &szName);
	//***********************************************************************
	// Name:		GetName
	// Description:	Nobre del conjunto de datos.
	// Parameters:	const CString &szName.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	void GetName(/*out*/CString &szName);
	//***********************************************************************
	// Name:		GetAttribName
	// Description:	Nombre del atributo en la pos nIndex.
	// Parameters:	int nIndex, /*out*/ CString &szAttribName.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	void GetAttribName(int nIndex, /*out*/ CString &szAttribName);
	//***********************************************************************
	// Name:		GetAttribValue
	// Description:	Nombre del atributo en la pos nIndex.
	// Parameters:	int nIndex, /*out*/ CString &szAttribName.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	void GetAttribValue(int nIndex, /*out*/ CString &szAttribValue);
	//***********************************************************************
	// Name:		GetAttribCount
	// Description:	Numero de atributos.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	int GetAttribCount();
	//***********************************************************************
	// Name:		FindAttrib
	// Description:	Buscar atributo.
	// Parameters:	/*in*/CString &szAttribName.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	BOOL FindAttrib(/*in*/CString &szAttribName);
	//***********************************************************************
	// Name:		AddAttrib
	// Description:	Añadir atributo atributo.
	// Parameters:	(/*in*/CString &szName, /*in*/CString &szValue.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	void AddAttrib(/*in*/CString &szName, /*in*/CString &szValue);

	void Write(XmlWriter &writer, int nDeltaLevel=0, BOOL bIndent=TRUE, BOOL bEOL=TRUE); // for any kind of open tag
	void WriteEmpty(XmlWriter &writer, int nDeltaLevel=0, BOOL bIndent=TRUE, BOOL bEOL=TRUE); // for any kind of open tag
	void WritePInstruction(XmlWriter &writer, int nDeltaLevel=0); // used mainly for <?xml ... ?>
	void WriteClosingTag(XmlWriter &writer, int nDeltaLevel=0, BOOL bIndent=TRUE, BOOL bEOL=TRUE); // for tag closure

};

__COMPONENTES_EXPORT__ CString UTF8Conversion(CString &s);
__COMPONENTES_EXPORT__ CString GetEscapedXmlString(CString szInput);


