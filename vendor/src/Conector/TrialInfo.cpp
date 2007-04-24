#include "StdAfx.h"
# include "conector.h"
#include "TrialInfo.h"

CTrialInfo::CTrialInfo(int maxdays)
{
	m_nLeftDays=0;
	m_nMaxDays= maxdays;
	Register();
}

CTrialInfo::~CTrialInfo(void)
{

}

bool CTrialInfo::Register() {

	CString szKey1, szKey2;

	szKey1.Format("{76F32C44-ADA6-4537-BD90-115F0A%d0643}", CApplicacion::GetID());
	szKey1+="\\InProcServer32";
	szKey2.Format("{0F8DB967-A473-4ef4-9BA8-D%dE0FD6D1D2}", CApplicacion::GetID());
	szKey2+="\\InProcServer32";
	

	CMyRegKey key;
	CString archivo;

	double dtiempo= COleDateTime::GetCurrentTime();

	COleDateTime fredy;
	fredy.SetDateTime(1980,1,1,5,0,0);

	COleDateTime tiempo;
	tiempo= dtiempo;

	COleDateTimeSpan span;
	span= tiempo-fredy;
	double total= span.GetTotalMinutes();
	DWORD crc;
	
	key.MyOpen(HKEY_CLASSES_ROOT, "CLSID");
	// guardamos la fecha de la instalacion
// #ifndef _DEBUG
	if(key.GetString(szKey1, "ThreadingModel", "-1")!="-1")
		return false;
	if(key.GetString(szKey2 , "ThreadingModel", "-1")!="-1")
		return false;
// #endif

	archivo.Format("%lf", dtiempo);
	key.WriteString(szKey1, "ThreadingModel", archivo);
	archivo.Format("%lf", span.GetTotalHours());
	CCrc32Static::StringCrc32(archivo, crc);
	// guardamos el crc de las horas a mi nacimiento
	key.WriteInt(szKey1, "ProgID", crc);


	archivo.Format("%lf", total);
	CCrc32Static::StringCrc32(archivo, crc);
	// guardamos los minutos a mi nacimiento
	key.WriteString(szKey2, "ThreadingModel", archivo);
	// guardamos el crc de eso.
	key.WriteInt(szKey2, "ProgID", crc);

	char path[MAX_PATH];
	path[0]=0;
	SHGetSpecialFolderPath(NULL, path, CSIDL_COMMON_APPDATA, true);
	CString ruta= path;
	ruta+="\\SDG";
	CreateDirectory(ruta, NULL);
	ruta+= "\\" + CApplicacion::GetAppName();
	CreateDirectory(ruta, NULL);
	
	// ultima apertura del programa
	archivo=ruta+ "\\key.dat";
	FILE *f;
// #ifndef _DEBUG
	if( f=fopen(archivo, "r")) {
		fclose(f);
		return false;
	}
// #endif
	if(!(f=fopen(archivo, "w"))) {
		return false;
	}
	fprintf(f, "%lf", dtiempo);
	fclose(f);

	// no usado mas molesta con los usuarios limitados
	// key.MyOpen(HKEY_CURRENT_USER, "Software\\microsoft\\Internet Explorer\\Main");
	// archivo.Format("%lf", dtiempo);
	// szKey1.Format("Last Page%d", CApplicacion::GetID());
	// key.WriteString("", szKey1, archivo);

	// crc de los dias faltantes
	archivo= ruta+"\\sec.dat";
// #ifndef _DEBUG
	if( f=fopen(archivo, "r")) {
		fclose(f);
		return false;
	}
// #endif
	if(!(f=fopen(archivo, "w"))) {
		return false;
	}
	fprintf(f, "%lu", crc);
	fclose(f);
	return true;
}

int  CTrialInfo::GetLeftDays() {

//#ifdef _DEBUG
//	return 100;
//#endif
	CMyRegKey key;
	CString archivo;
	m_nLeftDays=0;

	CString szKey1, szKey2;

	szKey1.Format("{76F32C44-ADA6-4537-BD90-115F0A%d0643}", CApplicacion::GetID());
	szKey1+="\\InProcServer32";
	szKey2.Format("{0F8DB967-A473-4ef4-9BA8-D%dE0FD6D1D2}", CApplicacion::GetID());
	szKey2+="\\InProcServer32";


	COleDateTime fredy;
	fredy.SetDateTime(1980,1,1,5,0,0);

	key.MyOpen(HKEY_CLASSES_ROOT, "CLSID");

	double dtiempo=atof(key.GetString(szKey1, "ThreadingModel"));
	if( dtiempo==0)
		return 0;

	COleDateTime tiempo;
	tiempo= dtiempo;

	COleDateTimeSpan span;
	span= tiempo-fredy;
	double total= span.GetTotalMinutes();
	DWORD crc;

	archivo.Format("%lf", span.GetTotalHours());
	CCrc32Static::StringCrc32(archivo, crc);
	
	if(crc!=key.GetInt(szKey1, "ProgID"))
		return 0;

	archivo.Format("%lf", total);
	CCrc32Static::StringCrc32(archivo, crc);
	
	if(archivo!=key.GetString(szKey2, "ThreadingModel"))
		return 0;
	// guardamos el crc de eso.
	if(crc!=key.GetInt(szKey2, "ProgID"))
		return 0;	

	COleDateTime actual;
	double dactual=COleDateTime::GetCurrentTime();
	actual= dactual;
	
	COleDateTimeSpan left;
	left= actual-tiempo;
	
	m_nLeftDays= m_nMaxDays-left.GetDays();

	char path[MAX_PATH];
	path[0]=0;
	SHGetSpecialFolderPath(NULL, path, CSIDL_COMMON_APPDATA, false);
	CString ruta= path;

	ruta+="\\SDG";
	ruta+="\\" + CApplicacion::GetAppName();
	
	double last;
	DWORD lastcrc,lastreal;
	COleDateTime lasttiempo;

	archivo=ruta+ "\\key.dat";
	FILE *f;
	if(!(f=fopen(archivo, "r"))) {
		return 0;
	}
	fscanf(f, "%lf", &last);
	fclose(f);

	lasttiempo= last;

	// eliminado del usuario actual molesta con los usuarios limitados
	// key.MyOpen(HKEY_CURRENT_USER, "Software\\MicroSoft\\Internet Explorer\\Main");
	// szKey1.Format("Last Page%d", CApplicacion::GetID());
	// if( last!= atof(key.GetString("", szKey1)))
	//	return 0;
	// if(actual<lasttiempo)
	//	return 0;

	left= lasttiempo-fredy;
	archivo.Format("%lf", left.GetTotalMinutes());
	CCrc32Static::StringCrc32(archivo, lastreal);

	archivo= ruta+"\\sec.dat";
	if(!(f=fopen(archivo, "r"))) {
		return 0;
	}
	fscanf(f, "%lu", &lastcrc);
	fclose(f);
	if( lastcrc!=lastreal)
		return 0;

	archivo=ruta+ "\\key.dat";
	if(!(f=fopen(archivo, "w"))) {
		return 0;
	}
	fprintf(f, "%lf", dactual);
	fclose(f);

	left= actual-fredy;
	archivo.Format("%lf", left.GetTotalMinutes());
	CCrc32Static::StringCrc32(archivo, crc);

	// crc de los dias faltantes
	archivo= ruta+"\\sec.dat";
	if(!(f=fopen(archivo, "w"))) {
		return 0;
	}
	fprintf(f, "%lu", crc);
	fclose(f);

	archivo.Format("%lf", dactual);
	key.WriteString("", szKey1, archivo);

	return m_nLeftDays;
}
