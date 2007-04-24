#include "StdAfx.h"
#include "htmlfile.h"

CHtmlFile::CHtmlFile(void)
{
	m_hFile= NULL;
}

CHtmlFile::~CHtmlFile(void)
{
	if( m_hFile) 
		fclose(m_hFile);
}

CHtmlFile::CHtmlFile(CString Archivo)
{
	m_hFile= fopen( Archivo, "w");
}

bool CHtmlFile::SetFile(CString Archivo) {

	if( m_hFile)
		fclose(m_hFile);
	return (m_hFile= fopen( Archivo, "w"))!=NULL;
}

void CHtmlFile::SetTitulo(CString titulo) {

	m_szTitulo= titulo;
}

bool CHtmlFile::VolcarFile( FILE *destino) {

	char a;
	bool found= false;
	while( !feof(destino)) {

		if( fscanf(destino, "%c", &a)==EOF)
			break;
		if(a=='%') {
			if( !found) {
				found= true;
				continue;
			} else fprintf(m_hFile, "%%");
		} else if( found) {
			if( a=='s')
				fprintf(m_hFile, "%s", m_szTitulo);
			else fprintf(m_hFile, "%%%c", a);
			found= false;
			continue;
		}
		fprintf(m_hFile, "%c", a);
	}
	fclose( destino);
	return true;
}

bool CHtmlFile::VolcarFile(CString Archivo) {

	if( !m_hFile || Archivo.IsEmpty())
		return false;
	FILE *header;
	if(!(header= fopen(Archivo, "r")))
		return false;
	return VolcarFile( header);
}

bool CHtmlFile::CreateHeader(CString Titulo, CString Archivo) {

	m_szTitulo= Titulo;
	if( !m_hFile) 
		return false;
	FILE *header;
	if( Archivo.IsEmpty() || !(header= fopen(Archivo, "r")) || !VolcarFile(header))
		return GenericHeader(Titulo);
	return true;
}

bool CHtmlFile::CreateFooter(CString Archivo) {

	if( !m_hFile) 
		return false;
	if( Archivo.IsEmpty())
		return GenericFooter();
	FILE *footer;
	if( !(footer= fopen(Archivo, "r")))
		return GenericFooter();
	return VolcarFile(footer);
}

bool CHtmlFile::GenericHeader(CString Titulo) {

	m_szTitulo= Titulo;
	fprintf(m_hFile, "<html>\n<head>\n");
	fprintf(m_hFile, "<title>%s</title>", Titulo);
	fprintf(m_hFile, "\n</head>\n<body>\n");
	fprintf( m_hFile,"<font  size=\"+1\" color=\"#0000FF\"><center>%s</center></font>", Titulo);
	return true;
}

bool CHtmlFile::GenericFooter() {

	fprintf(m_hFile, "</html>\n</head>\n");
	return true;
}

bool CHtmlFile::WriteEx(const char *Palabra, ...) {

	va_list marker;
    va_start( marker, Palabra);
	if( !m_hFile)
		return false;
	vfprintf( m_hFile, Palabra, marker);
	return true;
}

bool CHtmlFile::Write(CString Palabra) {

	if( !m_hFile)
		return false;
	fprintf(m_hFile,Palabra);
	return true;
}