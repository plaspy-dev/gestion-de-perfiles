#pragma once

class CFile; // forward declaration

__COMPONENTES_EXPORT__ CString FromUTF8Conversion(CString &s);
__COMPONENTES_EXPORT__ CString GetUnescapedXmlString(CString szInput);

enum NODETYPE
{
	NODETYPE_NA = 0,
	NODETYPE_BEGINELEMENT = 1,
	NODETYPE_ENDELEMENT = 2,
	NODETYPE_ATTRIB = 3,
	NODETYPE_CONTENT = 4,
	NODETYPE_PI = 5,
	NODETYPE_COMMENT = 6,
	NODETYPE_CDATA = 7
};

#define IDS_EMPTYELEMENTNAME	"Nombre de elemento vacio"
#define IDS_BADBEGINNODESYMBOL "Simbolo '<' Malo"
#define IDS_NOBEGINNODESYMBOLINEOL "Simbolo '<' no es permitido al final de la linea"
#define IDS_GENERICSYNTAXERROR	"Error de sintaxis"
#define IDS_MISSINGATTRIBNAME	"No se encuentra nombre de atributo antes de '='"
#define IDS_MISSINGEQUALSYMBOL  "No se encuentra '=' antes del nombre de atributo"
#define IDS_NOEQUALSYMBOLINEOL "hay un '=' en el final de la linea"
#define IDS_BADATTRIBUTEVALUESYNTAX "no hay una comilla al final de la linea"


//***************************************************************************
//
// AUTHOR:  JUHU
//
// DESCRIPTION: XML Reader (see XML Writer).
//
// ORIGINAL: 
//
//***************************************************************************
class __COMPONENTES_EXPORT__ XmlReader
{
	// Construction
public:
	//***********************************************************************
	// Name:		XmlReader
	// Description:	Constructor.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	XmlReader();
	//***********************************************************************
	// Name:		XmlReader
	// Description:	Destructor.
	// Parameters:	None.
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	virtual ~XmlReader();

// Attributes
protected:
	CStdioFile	m_File; // general file members
	BOOL		m_bFileOpen;
	CString		m_szFilename;
	BOOL		m_bShowMsgBoxOnError; // TRUE if message boxes are allowed to display while parsing
	CString		m_szLastError; // filled with error description if ParseContent() returns FALSE

	CString		m_szContent; // internal use : parser buffer
	long		m_nCursor, m_nbLines; // internal cursors (horizontal and vertical directions)
	NODETYPE	m_nCurNodeType; // returns where is the parser on at the moment
	CString		m_szCurNodeName, m_szCurNodeContent; // returns the current node value, and the current node content value
	CString		m_szCurAttribName, m_szCurAttribValue; // returns the current attrib name/value pair
	CString		m_szCurPInstruction; // returns the current PInstruction (for instance ?xml, !DOCTYPE, ...)
	BOOL		m_bCurInsideComment; // TRUE if the parser is inside a comment ( <!-- ... -->)
	BOOL		m_bCurInsideCDATA; // TRUE if hte parser is inside a CDATA secrtion ( <![CDATA[[ ... ]]> )
	BOOL		m_bCurInsideNode; // TRUE if the parser is inside a node begin tag
	BOOL		m_bCurInsideAttrib; // TRUE if m_szCurAttribName is valid and m_szCurAttribValue is pending
	BOOL		m_bCurInsideContent; // TRUE if the parser is inside content

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

	virtual BOOL ReadString();

	CString &GetFilename() { return m_szFilename; } // file related
	BOOL MustBeClosed() { return m_bFileOpen; }

	void ShowMsgBoxOnError(BOOL bShow=TRUE);
	

	NODETYPE GetNodeType(); // DOM related
	void GetNodeName(/*out*/CString &szName);
	void GetAttrib(/*out*/CString &szName, /*out*/CString &szValue);
	void GetNodeContent(/*out*/CString &szContent);

	long GetCurrentLine();
	void GetPInstruction(/*out*/CString &szName);

	void GetLastError(/*out*/CString &szErrorDescription); // if any
	CString FindElemento( CString Elemento, CString Atributo, CString Valor, CString NuevoAtributo);

	// Internal
	//
protected:
	BOOL ParseContent();

};