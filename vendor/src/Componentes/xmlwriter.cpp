
#include "stdafx.h"
# include "Componentes.h"
#include "xmlwriter.h"

XmlWriter::XmlWriter()
{
	m_pFile = new CFile();
	Init();
}
XmlWriter::~XmlWriter()
{
	delete m_pFile;
	m_pFile = NULL;
}
void XmlWriter::Init()
{
	FUNCTION_STACK_TRACE("FFFFE0F3");
	SetIndentLevel(0);
	m_bFileOpen = FALSE;
	m_bPrototypeWritten = FALSE;
}
BOOL XmlWriter::Open(CString &szFilename)
{
	FUNCTION_STACK_TRACE("FFFFE0F4");
	if (m_bFileOpen)
		return TRUE;

	m_szFilename = szFilename;

	return TRUE;
}
BOOL XmlWriter::WriteString(CString &szData)
{
	FUNCTION_STACK_TRACE("FFFFE0F5");
	if (!m_bFileOpen) // open file
	{
		m_bFileOpen = m_pFile->Open( (LPCTSTR)m_szFilename, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyNone );
	}
	if (!m_bFileOpen)
		return FALSE;

	if (!m_bPrototypeWritten)
	{
		m_bPrototypeWritten = TRUE; // make sure to set this flag to true before we call a reentrant method such like .WritePInstruction

		// write XML prototype, once and only once
		//
		int nOldIndent = GetIndentLevel();
		SetIndentLevel(0);

		CString s;
		XmlElement xml("xml");
		xml.AddAttrib( CString("version"), CString("1.0") );
		xml.AddAttrib( CString("encoding"), CString("UTF-8") );
		xml.WritePInstruction(*this);
		
		SetIndentLevel(nOldIndent);

	}

	// actual write
//	m_pFile->SeekToEnd();
	m_pFile->Write( szData.GetBuffer(0), szData.GetLength() );
//	m_pFile->Flush();

	return TRUE;
}
BOOL XmlWriter::Close()
{
	if (m_pFile && m_bFileOpen)
	{
		m_pFile->Close();
	}
	Init();
	return TRUE;
}



XmlStringWriter::XmlStringWriter()
{
	Init();
}
XmlStringWriter::~XmlStringWriter()
{
}
void XmlStringWriter::Init()
{
	m_szContent.Empty();
}
CString &XmlStringWriter::GetContent()
{
	return m_szContent;
}
BOOL XmlStringWriter::WriteString(CString &szData)
{
	FUNCTION_STACK_TRACE("FFFFE0F6");
	if (!m_bPrototypeWritten)
	{
		m_bPrototypeWritten = TRUE; // make sure to set this flag to true before we call a reentrant method such like .WritePInstruction

		// write XML prototype, once and only once
		//
		int nOldIndent = GetIndentLevel();
		SetIndentLevel(0);

		CString s;
		XmlElement xml("xml");
		xml.AddAttrib( CString("version"), CString("1.0") );
		xml.AddAttrib( CString("encoding"), CString("UTF-8") );
		xml.WritePInstruction(*this);
		
		SetIndentLevel(nOldIndent);

	}

	// actual write
	m_szContent += szData;

	return TRUE;
}
BOOL XmlStringWriter::Close()
{
	return TRUE;
}




void HelperBuildIndent(CString &szSpaces, int nLevel)
{
	szSpaces.Empty();
	for (int i=0;i<nLevel;i++)
		szSpaces += "\t";
}

XmlElement::XmlElement()
{
}
XmlElement::XmlElement(const CString &szName)
{
	SetName(szName);
}
void XmlElement::SetName(/*in*/const CString &szName)
{
	m_szName = szName;
}
void XmlElement::GetName(/*out*/CString &szName)
{
	szName = m_szName;
}
void XmlElement::GetAttribName(int nIndex, /*out*/ CString &szAttribName)
{
	szAttribName.Empty();
	if (nIndex>-1 && nIndex<GetAttribCount())
		szAttribName = m_szAttribNames.GetAt( nIndex );
}
void XmlElement::GetAttribValue(int nIndex, /*out*/ CString &szAttribValue)
{
	szAttribValue.Empty();
	if (nIndex>-1 && nIndex<m_szAttribValues.GetSize())
		szAttribValue = m_szAttribValues.GetAt( nIndex );
}
int XmlElement::GetAttribCount()
{
	return (int) m_szAttribNames.GetSize();
}
BOOL XmlElement::FindAttrib(/*in*/CString &szAttribName)
{
	FUNCTION_STACK_TRACE("FFFFE0F7");
	BOOL bFound = FALSE;
	int i=0;
	int nSize = (int) m_szAttribNames.GetSize();
	while (i<nSize && !bFound)
	{
		bFound = (m_szAttribNames.GetAt(i)==szAttribName);
		i++;
	}
	return bFound;
}
void XmlElement::AddAttrib(/*in*/CString &szAttribName, /*in*/CString &szAttribValue)
{
	FUNCTION_STACK_TRACE("FFFFE0F8");
	BOOL bFound = FALSE;
	int i=0;
	int nSize = (int) m_szAttribNames.GetSize();
	while (i<nSize && !bFound)
	{
		bFound = (m_szAttribNames.GetAt(i)==szAttribName);
		i++;
	}
	if (bFound) // already known
	{
		i--;
		m_szAttribValues.SetAt( i, szAttribValue );
	}
	else
	{
		m_szAttribNames.Add( szAttribName );
		m_szAttribValues.Add( szAttribValue );
	}
}
void XmlElement::Write(XmlWriter &writer, int nDeltaLevel, BOOL bIndent, BOOL bEOL) // for any kind of open tag
{
	FUNCTION_STACK_TRACE("FFFFE0F9");
	writer.SetIndentLevel( writer.GetIndentLevel()+nDeltaLevel );

	CString s;
	if (bIndent)
		HelperBuildIndent(s,writer.GetIndentLevel());
	s += "<";
	s += m_szName;
	int i;
	int nCount = GetAttribCount();
	for (i=0;i<nCount;i++)
	{
		s += " "; // separator between attribute pairs
		s += m_szAttribNames.GetAt( i );
		s += "=\"";
		s += m_szAttribValues.GetAt( i );
		s += "\"";
	}
	s += ">";
	if (bEOL)
		s += "\r\n"; // ENDL
	writer.WriteString( s );
}
void XmlElement::WriteEmpty(XmlWriter &writer, int nDeltaLevel, BOOL bIndent, BOOL bEOL)
{
	FUNCTION_STACK_TRACE("FFFFE0FA");
	writer.SetIndentLevel( writer.GetIndentLevel()+nDeltaLevel );

	CString s;
	if (bIndent)
		HelperBuildIndent(s,writer.GetIndentLevel());
	s += "<";
	s += m_szName;
	int i;
	int nCount = GetAttribCount();
	for (i=0;i<nCount;i++)
	{
		s += " "; // separator between attribute pairs
		s += m_szAttribNames.GetAt( i );
		s += "=\"";
		s += m_szAttribValues.GetAt( i );
		s += "\"";
	}
	s += "/>";
	if (bEOL)
		s += "\r\n"; // ENDL
	writer.WriteString( s );

	writer.SetIndentLevel( writer.GetIndentLevel()-nDeltaLevel );
}
void XmlElement::WritePInstruction(XmlWriter &writer, int nDeltaLevel)
{
	FUNCTION_STACK_TRACE("FFFFE0FB");
	writer.SetIndentLevel( writer.GetIndentLevel()+nDeltaLevel );

	CString s;
	HelperBuildIndent(s,writer.GetIndentLevel());
	s += "<?";
	s += m_szName;
	int i;
	int nCount = GetAttribCount();
	for (i=0;i<nCount;i++)
	{
		s += " "; // separator between attribute pairs
		s += m_szAttribNames.GetAt( i );
		s += "=\"";
		s += m_szAttribValues.GetAt( i );
		s += "\"";
	}
	s += "?>";
	s += "\r\n"; // ENDL
	writer.WriteString( s );
}
void XmlElement::WriteClosingTag(XmlWriter &writer, int nDeltaLevel, BOOL bIndent, BOOL bEOL)
{
	FUNCTION_STACK_TRACE("FFFFE0FC");
	CString s;
	if (bIndent)
		HelperBuildIndent(s,writer.GetIndentLevel());
	s += "</";
	s += m_szName;
	s += ">";
	if (bEOL)
		s += "\r\n"; // ENDL
	writer.WriteString( s );

	writer.SetIndentLevel( writer.GetIndentLevel()+nDeltaLevel );
}

CString GetEscapedXmlString(CString szInput)
{
	CString s;
	if (szInput.GetLength()==0) return s;

	long nLength = szInput.GetLength();
	for (long i=0; i<nLength; i++)
	{
		char c = szInput.GetAt(i);
		if (c=='&')
			s += "&amp;";
		else if (c=='"')
			s += "&quot;";
		else if (c=='<')
			s += "&lt;";
		else
			s += c;
	}

	return UTF8Conversion(s);
}


CString UTF8Conversion(CString &s)
{

	wchar_t *pWideString = new wchar_t[s.GetLength()+1];
	if (!pWideString) return s;

	// local charset to widechar
	//

	int nActualWideLength = ::MultiByteToWideChar(CP_ACP,
			0,
			s.GetBuffer(0),
			s.GetLength(),
			pWideString,
			s.GetLength());

	pWideString[nActualWideLength] = 0;

	// widechar to UTF8
	//		
	char *temps = new char[2*nActualWideLength+5]; // 2*x+5 (why not?)
	if (!temps)
	{
		delete [] pWideString;
		return s;
	}

	int nActualLength = ::WideCharToMultiByte(CP_UTF8,
			0,
			pWideString,
			-1,
			temps,
			2*nActualWideLength+5,
			NULL,
			NULL);

	CString st = temps;

	delete [] temps;

	delete [] pWideString;

	return st;

}