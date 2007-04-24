
#include "stdafx.h"
# include "Componentes.h"
#include "xmlreader.h"



XmlReader::XmlReader()
{
	Init();
}

XmlReader::~XmlReader()
{
}

CString XmlReader::FindElemento( CString Elemento, CString Atributo, CString Valor, CString NuevoAtributo) {

	FUNCTION_STACK_TRACE("FFFFE0EF");
	bool found= false;

	while( ReadString()) {

		switch( GetNodeType()) {
			case NODETYPE_ATTRIB:
				{
				CString szName, szValue;
				GetAttrib(szName, szValue);
				szName = GetUnescapedXmlString(szName);
				szValue = GetUnescapedXmlString(szValue);

				CString szCurrentElement;
				GetNodeName(szCurrentElement);
				if( szCurrentElement.CompareNoCase( Elemento)==0) {

					if(szName.CompareNoCase(Atributo)==0) {

						if(!szValue.CompareNoCase(Valor))

							found= true;
					} else if( found) 

						if( !szName.CompareNoCase(NuevoAtributo))

							return szValue;
				}
				}
		}
	}
	return NULL;
}

void XmlReader::Init()
{
	FUNCTION_STACK_TRACE("FFFFE0F0");

	m_bFileOpen = FALSE;
	ShowMsgBoxOnError(FALSE);
}

BOOL XmlReader::Open(CString &szFilename)
{
	FUNCTION_STACK_TRACE("FFFFE0F1");

	if (m_bFileOpen)
		return TRUE;

	m_szFilename = szFilename;

	return TRUE;
}

BOOL XmlReader::ReadString()
{
	FUNCTION_STACK_TRACE("FFFFE0F2");

	if (!m_bFileOpen) // open file for reading
	{
		m_bFileOpen = m_File.Open( (LPCTSTR)m_szFilename, CFile::modeRead | CFile::shareDenyNone );
		m_nCursor = -1;
		m_nbLines = 0;
		m_bCurInsideNode = m_bCurInsideComment = m_bCurInsideAttrib = m_bCurInsideContent = m_bCurInsideCDATA = FALSE;
		m_szLastError.Empty();
		m_szCurNodeName.Empty();
		m_szCurNodeContent.Empty();
		m_szCurAttribName.Empty();
		m_szCurAttribValue.Empty();
		m_szCurPInstruction.Empty();
	}
	if (!m_bFileOpen)
		return FALSE;

	BOOL bResult = TRUE;

	if (m_nCursor==-1)
	{
		m_szContent.Empty();
		bResult = m_File.ReadString(m_szContent);
		CString utf;
		utf.Format("%c%c%c", 0xEF,0xBB,0xBF);
		m_szContent.Replace("ï»¿", "");
		m_szContent.Replace(utf, "");
		m_nCursor = 0;
		m_nbLines++;
	}

	if (bResult && !ParseContent())
	{
		CString s;
		s.Format("Error analizando en linea %d : (%s)", m_nbLines, m_szLastError.GetBuffer(0) );
		m_szLastError = s;

		if (m_bShowMsgBoxOnError)
			AfxMessageBox(s);

		bResult = FALSE;
	}

	return bResult;
}
BOOL XmlReader::Close()
{
	FUNCTION_STACK_TRACE("FFFFE0F1");
	if (m_bFileOpen)
		m_File.Close();
	Init();

	return TRUE;
}



void XmlReader::ShowMsgBoxOnError(BOOL bShow)
{
	m_bShowMsgBoxOnError = bShow;
}


NODETYPE XmlReader::GetNodeType()
{
	return m_nCurNodeType;
}

void XmlReader::GetNodeName(/*out*/CString &szName)
{
	szName = m_szCurNodeName;
}

void XmlReader::GetAttrib(/*out*/CString &szName, /*out*/CString &szValue)
{
	szName = m_szCurAttribName;
	szValue = m_szCurAttribValue;
}

void XmlReader::GetNodeContent(/*out*/CString &szContent)
{
	szContent = m_szCurNodeContent;
}


void XmlReader::GetPInstruction(/*out*/CString &szName)
{
	szName = m_szCurPInstruction;
}

long XmlReader::GetCurrentLine()
{
	return m_nbLines;
}


void XmlReader::GetLastError(/*out*/CString &szErrorDescription) // if any
{
	szErrorDescription = m_szLastError;
}


BOOL XmlReader::ParseContent()
{
	FUNCTION_STACK_TRACE("FFFFE0F2");
	m_nCurNodeType = NODETYPE_NA;

	long i = m_nCursor;
	long imax = m_szContent.GetLength()-1;

	if (i>imax)
	{
		m_nCursor = -1; // force next string to be read from file
		return TRUE;
	}

	CString szTemp = m_szContent.GetBuffer(0)+i;

	// pass spaces if we are inside a <...> and not yet processing an attribute value
	while ( (i<=imax) && 
			(m_bCurInsideNode && !m_bCurInsideAttrib) && 
			(m_szContent.GetAt(i)==' ' || m_szContent.GetAt(i)==0x0A || m_szContent.GetAt(i)==0x0D) )
		i++;

	if (i>imax)
	{
		m_nCursor = -1; // force next string to be read from file
		return TRUE;
	}


	// are we inside a comment ?
	if (m_bCurInsideComment)
	{
		while ( (i<=imax-2) && 
			!(m_szContent.GetAt(i)=='-' && m_szContent.GetAt(i+1)=='-' && m_szContent.GetAt(i+2)=='>') )
			i++;

		if (i<=imax-2) // found an end-comment
		{
			m_nCurNodeType = NODETYPE_NA; // tell user we have nothing to provide him with
			m_nCursor = i+2+1;

			// after '-->' we are automatically within a content
			m_bCurInsideNode = m_bCurInsideAttrib = m_bCurInsideComment = m_bCurInsideCDATA = FALSE;
			m_bCurInsideContent = TRUE;
			m_szCurNodeContent.Empty();

			return TRUE;
		}
		else // we still are inside an comment
		{
			m_nCurNodeType = NODETYPE_COMMENT;
			m_nCursor = imax+1; // force next string to be read
			return TRUE;
		}
	}


	// are we inside a CDATA section ?
	if (m_bCurInsideCDATA)
	{
		while ( (i<=imax-2) && 
			!(m_szContent.GetAt(i)==']' && m_szContent.GetAt(i+1)==']' && m_szContent.GetAt(i+2)=='>') )
			i++;

		if (i<=imax-2) // found an end-comment
		{
			m_nCurNodeType = NODETYPE_NA; // tell user we have nothing to provide him with
			m_nCursor = i+2+1;

			// after ']]>' we are automatically within a content
			m_bCurInsideNode = m_bCurInsideAttrib = m_bCurInsideComment = m_bCurInsideCDATA = FALSE;
			m_bCurInsideContent = TRUE;
			m_szCurNodeContent.Empty();

			return TRUE;
		}
		else // we still are inside an CDATA section
		{
			m_nCurNodeType = NODETYPE_CDATA;
			m_nCursor = imax+1; // force next string to be read
			return TRUE;
		}
	}


	if (m_bCurInsideAttrib) // extracting the attrib value, possibly in multiple passes
	{
		if ( m_szCurAttribValue.IsEmpty() )
		{
			// pass EOL
			while ( (i<=imax) && (m_szContent.GetAt(i)==' ' || m_szContent.GetAt(i)==0x0A || m_szContent.GetAt(i)==0x0D) )
				i++;

			if (i>imax)
			{
				m_nCurNodeType = NODETYPE_NA;
				m_nCursor = i;
				return TRUE;
			}

			char quotechar = m_szContent.GetAt(i++);

			m_szCurAttribValue += quotechar; // start with something whatsoever!
			// in fact, we don't check the quotechar is an actual quotechar, ie " or '

			m_nCurNodeType = NODETYPE_NA;
			m_nCursor = i;
			return TRUE;
		}
		else
		{

			long ibegin = i;

			// pass until we find spaces or EOL or >
			while ( (i<=imax) && m_szContent.GetAt(i)!='\"'
							  && m_szContent.GetAt(i)!='\''
							  && m_szContent.GetAt(i)!=0x0A
							  && m_szContent.GetAt(i)!=0x0D
							  && m_szContent.GetAt(i)!='>') 
				i++;

			// TODO : properly manage the case of a multiple-line attrib-value
			// (we should in this case return a N/A nodetype as long as we haven't
			// encountered the ending quotechar, while buffering all the chars in
			// the szAttribValue member).

			long iend = i;

			m_szCurAttribValue += m_szContent.Mid(ibegin, iend-ibegin);

			if (i>imax)
			{ // don't forget to add the EOL as well
				m_szCurAttribValue += "\r\n";

				m_nCurNodeType = NODETYPE_NA;
				m_nCursor = i;
				return TRUE;
			}

			// and remove the prefixed quote char
			while ( !m_szCurAttribValue.IsEmpty() && 
					(m_szCurAttribValue.GetAt(0)=='\"' || m_szCurAttribValue.GetAt(0)=='\'') )
			{
				m_szCurAttribValue = m_szCurAttribValue.Right( m_szCurAttribValue.GetLength()-1 );
			}

			m_nCurNodeType = NODETYPE_ATTRIB;
			m_bCurInsideAttrib = FALSE;

			if ( m_szContent.GetAt(i)!='>' )
				i++; // pass ending quote char

			m_nCursor = i;
			return TRUE;
		}

	} // end if m_bCurInsideAttrib==TRUE


	if (m_bCurInsideContent)
	{
		long ibegin = i;

		// pass until we find spaces or EOL or >
		while ( (i<=imax) && m_szContent.GetAt(i)!=0x0A
						  && m_szContent.GetAt(i)!=0x0D
						  && m_szContent.GetAt(i)!='<')
			i++;

		long iend = i;

		if ( (i<=imax) && m_szContent.GetAt(i)=='<')
			m_bCurInsideContent = FALSE;

		m_szCurNodeContent = m_szContent.Mid(ibegin, iend-ibegin);
		if (m_szCurNodeContent.IsEmpty())
			m_nCurNodeType = NODETYPE_NA;	
		else
			m_nCurNodeType = NODETYPE_CONTENT;

		m_nCursor = i;
		return TRUE;

	} // end if (m_bCurInsideContent)

	//
	char c = m_szContent.GetAt(i);

	// a node ?
	if (c=='<')
	{
		if (m_bCurInsideNode) // error, we were already inside one
		{
			m_szLastError = IDS_BADBEGINNODESYMBOL;
			return FALSE;
		}

		m_bCurInsideNode = TRUE;
		m_bCurInsideAttrib = m_bCurInsideContent = m_bCurInsideComment = m_bCurInsideCDATA = FALSE;

		i++;

		// pass spaces
		while ( m_szContent.GetAt(i)==' ' || m_szContent.GetAt(i)==0x0A || m_szContent.GetAt(i)==0x0D)
			i++;

		if (i>imax)
		{
			m_szLastError = IDS_NOBEGINNODESYMBOLINEOL;
			return FALSE;
		}

		// here we have either a node name, a PI, or a begin comment
		if (imax-i>=2) // is it a begin comment ? ( <!-- )
		{
			if ( m_szContent.GetAt(i+0)=='!' &&
				 m_szContent.GetAt(i+1)=='-' &&
				 m_szContent.GetAt(i+2)=='-')
			{
				m_nCurNodeType = NODETYPE_COMMENT;
				m_bCurInsideComment = TRUE;

				i+=3; // go to actual comment content

				m_nCursor = i;
				return TRUE;
			}
		}

		if (imax-i>=7) // is it a begin cdatasection ? ( <![CDATA[ )
		{
			if ( m_szContent.GetAt(i+0)=='!' &&
				 m_szContent.GetAt(i+1)=='[' &&
				 m_szContent.GetAt(i+2)=='C' &&
				 m_szContent.GetAt(i+3)=='D' &&
				 m_szContent.GetAt(i+4)=='A' &&
				 m_szContent.GetAt(i+5)=='T' &&
				 m_szContent.GetAt(i+6)=='A' &&
				 m_szContent.GetAt(i+7)=='[')
			{
				m_nCurNodeType = NODETYPE_CDATA;
				m_bCurInsideCDATA = TRUE;

				i+=8; // go to actual cdata section content

				m_nCursor = i;
				return TRUE;
			}
		}


		// the node name begins at position i
		long ibegin = i;

		// pass until we find spaces or EOL or >
		while ( (i<=imax) && m_szContent.GetAt(i)!=' ' 
						  && m_szContent.GetAt(i)!=0x0A
						  && m_szContent.GetAt(i)!=0x0D
						  && (m_szContent.GetAt(i)!='/' || (i==ibegin)) // don't forget empty elements (for instance <br/>)
						  && m_szContent.GetAt(i)!='>')
			i++;

		long iend = i;
	
		m_szCurNodeName = m_szContent.Mid(ibegin, iend-ibegin);
		if (m_szCurNodeName.IsEmpty())
		{
			m_szLastError = IDS_EMPTYELEMENTNAME;
			return FALSE;
		}


		if (m_szCurNodeName.GetAt(0)=='?' || m_szCurNodeName.GetAt(0)=='!')
		{
			m_nCurNodeType = NODETYPE_PI;
			m_szCurPInstruction = m_szCurNodeName;
			m_szCurNodeName.Empty(); // erase the PI instruction so it does not appear as a node name
		}
		else if (m_szCurNodeName.GetAt(0)=='/')
		{
			m_nCurNodeType = NODETYPE_ENDELEMENT;
			m_szCurNodeName = m_szCurNodeName.Right( m_szCurNodeName.GetLength()-1 ); // remove /
		}
		else
			m_nCurNodeType = NODETYPE_BEGINELEMENT;

		m_nCursor = i;
		return TRUE;

	}
	else // >, or ?, or content or attribute
	{
		if (c=='?')
		{
			m_nCurNodeType = NODETYPE_NA;
			m_nCursor = i+1;
			return TRUE;
		}
		else if (c=='/')
		{
			i++;

			// pass node name
			long ibegin = i;

			// pass until we find spaces or EOL or >
			while ( (i<=imax) && m_szContent.GetAt(i)!=' ' 
							  && m_szContent.GetAt(i)!=0x0A
							  && m_szContent.GetAt(i)!=0x0D
							  && m_szContent.GetAt(i)!='>')
				i++;

			long iend = i;
		
			m_nCurNodeType = NODETYPE_ENDELEMENT;

			m_nCursor = i;
			return TRUE;
		}
		else if (c=='>')
		{
			m_bCurInsideNode = m_bCurInsideAttrib = FALSE;
			m_bCurInsideContent = TRUE;
			m_szCurNodeContent.Empty();
			m_nCurNodeType = NODETYPE_NA;
			m_nCursor = i+1;
			return TRUE;
		}

		if (m_bCurInsideNode) // attributes
		{

			if (!m_bCurInsideAttrib)
			{
				if (c=='=')
				{
					m_nCurNodeType = NODETYPE_NA;
					m_bCurInsideAttrib = TRUE; // enable extraction of the associated attribute value

					i++; // pass '=' symbol

					m_nCursor = i;
					return TRUE;
				}

				// get attribute name
				long ibegin = i;

				// pass until we find spaces or EOL or >
				while ( (i<=imax) && m_szContent.GetAt(i)!=' '
								  && m_szContent.GetAt(i)!=0x0A
								  && m_szContent.GetAt(i)!=0x0D
								  && m_szContent.GetAt(i)!='='
								  && m_szContent.GetAt(i)!='>') // check against > is just for safety
					i++;

				long iend = i;

				m_szCurAttribName = m_szContent.Mid(ibegin, iend-ibegin);
				if (m_szCurAttribName.IsEmpty())
				{
					m_szLastError = IDS_MISSINGATTRIBNAME;
					return FALSE;
				}

				m_szCurAttribValue.Empty(); // make sure the attrib value is empty for the moment
				m_nCurNodeType = NODETYPE_NA;

				m_nCursor = i;
				return TRUE;
			}

		}
	}

	// this code never executes
	m_szLastError = IDS_GENERICSYNTAXERROR;
	return FALSE;
}

CString FromUTF8Conversion(CString &s) // UTF8 ==> local charset
{
	wchar_t *pWideString = new wchar_t[s.GetLength()+1];
	if (!pWideString) return s;

	// UTF8 to widechar
	//

	int nActualWideLength = ::MultiByteToWideChar(CP_UTF8,
			0,
			s.GetBuffer(0),
			s.GetLength(),
			pWideString,
			s.GetLength());

	pWideString[nActualWideLength] = 0;

	// widechar to local charset
	//		
	char *temps = new char[nActualWideLength+1];
	if (!temps)
	{
		delete [] pWideString;
		return s;
	}

	int nActualLength = ::WideCharToMultiByte(CP_ACP,
			0,
			pWideString,
			-1,
			temps,
			nActualWideLength+1,
			NULL,
			NULL);

	CString st = temps;

	delete [] temps;

	delete [] pWideString;

	return st;
}

CString GetUnescapedXmlString(CString szInput) // Xml escape chars (&quot; ==> ", &lt; ==> <, &amp; ==> &);
{
	CString s;
	if (szInput.GetLength()==0) 
		return s;

	s = FromUTF8Conversion(szInput);

	s.Replace("&lt;", "<");
	s.Replace("&quot;","\"");
	s.Replace("&amp;", "&");
	/*int nLt;
	while ( (nLt=s.Find("&lt;",0))>-1 )
		s = s.Left(nLt) + "<" + s.Right( s.GetLength()-(nLt+strlen("&lt;")) );
	int nQuot;
	while ( (nQuot=s.Find("&quot;",0))>-1 )
		s = s.Left(nQuot) + "\"" + s.Right( s.GetLength()-(nQuot+strlen("&quot;")) );
	int nAmp;
	while ( (nAmp=s.Find("&amp;",0))>-1 )
		s = s.Left(nAmp) + "&" + s.Right( s.GetLength()-(nAmp+strlen("&amp;")) );*/
	return s;
}
