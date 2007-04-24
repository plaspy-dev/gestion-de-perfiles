#if !defined(AFX_IMAGEPROCESSORS_H__1FDE0A4E_8AB4_11D6_95AD_EFA89432A428__INCLUDED_)
#define AFX_IMAGEPROCESSORS_H__1FDE0A4E_8AB4_11D6_95AD_EFA89432A428__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "enbitmap.h"

//////////////////////////////////////////////////////////////////////////////////////
// derived image processors

class __COMPONENTES_EXPORT__ CImageRotator : public C32BitImageProcessor
{
public:
	CImageRotator(int nDegrees, BOOL bEnableWeighting = TRUE);
	CImageRotator(double dRadians);
	virtual ~CImageRotator();

	virtual CSize CalcDestSize(CSize sizeSrc);
	virtual BOOL ProcessPixels(RGBX* pSrcPixels, CSize sizeSrc, RGBX* pDestPixels, CSize &sizeDest);

protected:
	double m_dRadians;
};

class __COMPONENTES_EXPORT__ CImageShearer : public C32BitImageProcessor
{
public:
	CImageShearer(int nHorz, int nVert, BOOL bEnableWeighting = TRUE);
	virtual ~CImageShearer();

	virtual CSize CalcDestSize(CSize sizeSrc);
	virtual BOOL ProcessPixels(RGBX* pSrcPixels, CSize sizeSrc, RGBX* pDestPixels, CSize &sizeDest);

protected:
	int m_nHorz, m_nVert;
};

class __COMPONENTES_EXPORT__ CImageGrayer : public C32BitImageProcessor
{
protected:
	int  m_nRed;
	int  m_nGreen;
	int  m_nBlue;
public:
	CImageGrayer(DWORD bkColor=-1);
	virtual ~CImageGrayer();
	bool	IsBkColor(RGBX *p);

	virtual BOOL ProcessPixels(RGBX* pSrcPixels, CSize sizeSrc, RGBX* pDestPixels, CSize &sizeDest);
};

class __COMPONENTES_EXPORT__  CImageBlurrer : public C32BitImageProcessor
{
public:
	CImageBlurrer(int nAmount = 5); // 1 - 10 (10 is very blurred)
	virtual ~CImageBlurrer();

	virtual BOOL ProcessPixels(RGBX* pSrcPixels, CSize sizeSrc, RGBX* pDestPixels, CSize &sizeDest);

protected:
	int m_nAmount;
};

class __COMPONENTES_EXPORT__  CImageSharpener : public C32BitImageProcessor
{
public:
	CImageSharpener(int nAmount = 5); // 1 - 10 (10 is very sharp)
	virtual ~CImageSharpener();

	virtual BOOL ProcessPixels(RGBX* pSrcPixels, CSize sizeSrc, RGBX* pDestPixels, CSize &sizeDest);

protected:
	int m_nAmount;
};

class __COMPONENTES_EXPORT__  CImageResizer : public C32BitImageProcessor
{
public:
	CImageResizer(double dFactor);
	CImageResizer(double dFactorx,double dFactory);
	CImageResizer(int tamanox,int tamanoy);
	virtual ~CImageResizer();

	virtual CSize CalcDestSize(CSize sizeSrc);
	virtual BOOL ProcessPixels(RGBX* pSrcPixels, CSize sizeSrc, RGBX* pDestPixels, CSize &sizeDest);

protected:
	double m_dFactorY;
	double m_dFactorX;
	CSize m_sizeTamano;

protected:
	BOOL Enlarge(RGBX* pSrcPixels, CSize sizeSrc, RGBX* pDestPixels, CSize &sizeDest);
	BOOL Shrink(RGBX* pSrcPixels, CSize sizeSrc, RGBX* pDestPixels, CSize &sizeDest);
};

class __COMPONENTES_EXPORT__  CImageNegator : public C32BitImageProcessor
{
public:
	CImageNegator();
	virtual ~CImageNegator();

	virtual BOOL ProcessPixels(RGBX* pSrcPixels, CSize sizeSrc, RGBX* pDestPixels, CSize &sizeDest);
};

class __COMPONENTES_EXPORT__  CImageFlipper : public C32BitImageProcessor
{
public:
	CImageFlipper(BOOL bHorz, BOOL bVert = 0); 
	virtual ~CImageFlipper();

	virtual BOOL ProcessPixels(RGBX* pSrcPixels, CSize sizeSrc, RGBX* pDestPixels, CSize &sizeDest);

protected:
	BOOL m_bHorz, m_bVert;
};

class __COMPONENTES_EXPORT__  CColorReplacer : public C32BitImageProcessor
{
public:
	CColorReplacer(COLORREF crFrom, COLORREF crTo);
	virtual ~CColorReplacer();

	virtual BOOL ProcessPixels(RGBX* pSrcPixels, CSize sizeSrc, RGBX* pDestPixels, CSize &sizeDest);

protected:
	COLORREF m_crFrom, m_crTo;
};

class  __COMPONENTES_EXPORT__  CImageHigh : public C32BitImageProcessor
{
protected:
	int  m_nRed;
	int  m_nGreen;
	int  m_nBlue;
	bool	IsBkColor(RGBX *p);	
public:
	CImageHigh(float fL=0.1,DWORD bkColor=-1);
	virtual BOOL ProcessPixels(RGBX* pSrcPixels, CSize sizeSrc, RGBX* pDestPixels, CSize &sizeDest);
protected:
	float m_fLumDecr;
};



#endif