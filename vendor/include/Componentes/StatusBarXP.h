#pragma once
//***************************************************************************
//
// AUTHOR:  JUHU
//
// DESCRIPTION: CStatusBarXP barra de estado
//
// ORIGINAL: SplashScreenEx.cpp : header file by John O'Byrne 01/10/2002
//
//***************************************************************************
class  __COMPONENTES_EXPORT__ CStatusBarXP : public CStatusBar
{
DECLARE_DYNCREATE(CStatusBarXP)

// Attributes

// Operations
public:
    BOOL SetIndicators (const UINT* lpIDArray, int nIDCount);
protected:
    afx_msg void OnPaint();

DECLARE_MESSAGE_MAP()
};
