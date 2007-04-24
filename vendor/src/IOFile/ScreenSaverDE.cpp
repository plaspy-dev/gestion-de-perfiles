#include "StdAfx.h"
#include "ScreenSaverDE.h"

CScreenSaverDE::CScreenSaverDE(void)
{
}

CScreenSaverDE::~CScreenSaverDE(void)
{
}

bool CScreenSaverDE::EnableScreenSaver(bool Enable/*=true*/) {
	
	BOOL pvParam;
	if(SystemParametersInfo(SPI_GETSCREENSAVEACTIVE, 0, &pvParam, 0))
	{
		if (pvParam != Enable) {
			SystemParametersInfo(SPI_SETSCREENSAVEACTIVE, Enable, 0, 0); 
			return true;
		}
	}
	return false;
}