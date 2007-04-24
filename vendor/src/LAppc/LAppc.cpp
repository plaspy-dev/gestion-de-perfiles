// LAppc.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "LAppc.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CLAppcApp, CWinApp)
END_MESSAGE_MAP()


CLAppcApp::CLAppcApp()
{
}

CLAppcApp theApp;

BOOL CLAppcApp::InitInstance()
{
	CWinApp::InitInstance();
	int id= CApplicacion::GetID();
	if(id==0) {
		ASSERT(FALSE);
#ifdef  _DEBUG
		return TRUE;
#else 
		return FALSE;
#endif
	}
#ifndef _DEBUG
	if(!RegisterFile(CApplicacion::GetAppPath("lappc.dll")) 
		|| !RegisterFile(CApplicacion::GetAppPath("appt.dll"))) {

		CString texto;
		texto.Format("%s Esta corrupto.\nPor favor ReInstale %s o Informe al Administrador del sistema.", CApplicacion::GetAppName(), CApplicacion::GetAppName());
		AfxMessageBox(texto, MB_ICONERROR);
		return false;
	}
#endif

	return TRUE;
}

bool RegisterFile(CString file, CString *szMD5, bool force) {

	CMyRegKey myKey, keycu;
	CString app= CApplicacion::GetAppName();

	myKey.MyOpen(HKEY_LOCAL_MACHINE, CApplicacion::GetAppKey());
	keycu.MyOpen(HKEY_CURRENT_USER, CApplicacion::GetAppKey());

	file.MakeLower();
	CString md5=CMD5Checksum::GetMD5Ex(file);
	CStdioFile archivo;

	if( szMD5)
		*szMD5= md5;

	CString filecrc= file;
	filecrc.Replace(filecrc.Right(4), ".key");

	if(myKey.GetInt("control", "forcecrc")==210582)
		force=true;
	if(md5.IsEmpty()) // no se encontro el archivo
		return force;
	if( keycu.GetString("llaves", md5)==file) { // verificamos que este en el registro
		if(!force) 
			DeleteFile(filecrc); // borramos el archivo
		return true;
	}
	if( myKey.GetString("llaves", md5)==file) { // verificamos que este en el registro
		keycu.WriteString("llaves", md5, file);
		if(!force) 
			DeleteFile(filecrc); // borramos el archivo
		return true;
	}
	if(!archivo.Open(filecrc, CFile::modeRead))  {// no existe el archivo crc
		if( force) { // lo creamos
			if(archivo.Open(filecrc, CFile::modeCreate | CFile::modeWrite)) {
				archivo.WriteString(md5);
				archivo.Close();
			}
		}
		return force; // omitido completamente
	}
	CString tempmd5;
	if(archivo.GetLength()>0 && archivo.ReadString(tempmd5)) {
		archivo.Close();
		if(tempmd5 != md5) { // si aun asi son diferentes
			if( force) // lo borramos si lo estamos forzando
				DeleteFile(filecrc);
			return force;
		}
		if( keycu.WriteString("llaves", md5, file) && myKey.WriteString("llaves", md5, file)) {
			if( !force) 
				DeleteFile(filecrc);
			return true;
		}
	} else archivo.Close();
	return false;
}
