#pragma once

#include <afxtempl.h>

#pragma pack(push)
#pragma pack(1)

struct RGBX
{
public:
	RGBX() { btRed = btBlue = btGreen = btUnused = 0; }
	RGBX(BYTE red, BYTE green, BYTE blue) { btRed = red; btBlue = blue; btGreen = green; btUnused = 0; }
	RGBX(COLORREF color) { btRed = GetRValue(color); btBlue = GetBValue(color); btGreen = GetGValue(color); btUnused = 0; }

	BYTE btBlue;
	BYTE btGreen;
	BYTE btRed;

protected:
	BYTE btUnused;

public:
	inline BOOL Equals(const RGBX& rgb) { return (btRed == rgb.btRed && btGreen == rgb.btGreen && btBlue == rgb.btBlue); }
	inline BOOL IsGray() const { return (btRed == btGreen && btGreen == btBlue); }

	RGBX Gray() 
	{ 
		BYTE btGray = ((int)btBlue + (int)btGreen * 6 + (int)btRed * 3) / 10;
		return RGBX(btGray, btGray, btGray);
	}

};

#pragma pack(pop)

//***************************************************************************
//
// AUTHOR:  JUHU
//
// DESCRIPTION: Una base class for image processing
//
// ORIGINAL: 
//
//***************************************************************************

class __COMPONENTES_EXPORT__ C32BitImageProcessor
{
public:
	C32BitImageProcessor(BOOL bEnableWeighting = FALSE);
	virtual ~C32BitImageProcessor();

	virtual CSize CalcDestSize(CSize sizeSrc);
	virtual BOOL ProcessPixels(RGBX* pSrcPixels, CSize sizeSrc, RGBX* pDestPixels, CSize &sizeDest);

	inline RGBX CalcWeightedColor(RGBX* pPixels, CSize size, double dX, double dY);

protected:
	BOOL m_bWeightingEnabled;
};

typedef CArray<C32BitImageProcessor*, C32BitImageProcessor*> C32BIPArray;

//***************************************************************************
//
// AUTHOR:  JUHU
//
// DESCRIPTION: Procesamiento de imagenes
//
// ORIGINAL: 
//
//***************************************************************************

class __COMPONENTES_EXPORT__ CEnBitmap : public CBitmap  
{
// Construction
public:
	//***********************************************************************
	// Name:		CEnBitmap
	// Description:	Default constructor.
	// Parameters:	COLORREF crBkgnd = RGB(255, 255, 255).
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	CEnBitmap(COLORREF crBkgnd = RGB(255, 255, 255));
	//***********************************************************************
	// Name:		CEnBitmap
	// Description:	Destructor.
	// Parameters:	None
	// Return:		None.	
	// Notes:		None.
	//***********************************************************************
	virtual ~CEnBitmap();

	//***********************************************************************
	// Name:		LoadImage
	// Description:	Cargar una imagen para edicion.
	// Parameters:	LPCTSTR szImagePath (Paht), COLORREF crBack = -1(color de Fondo)
	// Return:		BOOL.	
	// Notes:		Especifique el color de fondo.
	//***********************************************************************
	BOOL LoadImage(LPCTSTR szImagePath, COLORREF crBack = -1);
	//***********************************************************************
	// Name:		LoadImage
	// Description:	Cargar recursos etc.
	// Parameters:	UINT uIDRes, LPCTSTR szResourceType(Icon,etc), HMODULE hInst = NULL, COLORREF crBack = -1(color de Fondo)
	// Return:		BOOL.	
	// Notes:		Especifique el color de fondo.
	//***********************************************************************
	BOOL LoadImage(UINT uIDRes, LPCTSTR szResourceType, HMODULE hInst = NULL, COLORREF crBack = -1); 
	//***********************************************************************
	// Name:		CopyImage
	// Description:	Crear una nueva instancia (otra pocicion de memoria)
	// Parameters:	HBITMAP hBitmap
	// Return:		BOOL.	
	// Notes:		
	//***********************************************************************
	BOOL CopyImage(HBITMAP hBitmap);
	//***********************************************************************
	// Name:		CopyImage
	// Description:	Crear una nueva instancia (otra pocicion de memoria)
	// Parameters:	CBitmap* pBitmap
	// Return:		BOOL.	
	// Notes:		None.
	//***********************************************************************
	BOOL CopyImage(CBitmap* pBitmap);
	//***********************************************************************
	// Name:		RotateImage
	// Description:	Rotar la imagen
	// Parameters:	int nDegrees, BOOL bEnableWeighting = FALSE (ajustar imagen)
	// Return:		BOOL.	
	// Notes:		rotates about centrepoint, +ve (clockwise) or -ve 
	//				(anticlockwise) from 12 o'clock
	//***********************************************************************
	BOOL RotateImage(int nDegrees, BOOL bEnableWeighting = FALSE); 
	//***********************************************************************
	// Name:		ShearImage
	// Description:	Shear la imagen
	// Parameters:	int nHorz, int nVert, BOOL bEnableWeighting = FALSE
	// Return:		BOOL.	
	// Notes:		shears +ve (right, down) or -ve (left, up)
	//***********************************************************************
	BOOL ShearImage(int nHorz, int nVert, BOOL bEnableWeighting = FALSE); 
	//***********************************************************************
	// Name:		GrayImage
	// Description:	Escala de grises
	// Parameters:	None
	// Return:		BOOL.	
	// Notes:		None
	//***********************************************************************
	BOOL GrayImage();
	//***********************************************************************
	// Name:		BlurImage
	// Description:	Distorciona la imagen.
	// Parameters:	None
	// Return:		BOOL.	
	// Notes:		None
	//***********************************************************************
	BOOL BlurImage(int nAmount = 5);
	BOOL SharpenImage(int nAmount = 5);
	BOOL ResizeImage(double dFactor);
	BOOL ResizeImage(double dFactorX, double dFactorY);
	BOOL ResizeImage(int cx, int cy);
	BOOL FlipImage(BOOL bHorz, BOOL bVert = 0);
	BOOL NegateImage();
	BOOL ReplaceColor(COLORREF crFrom, COLORREF crTo);
	BOOL MakeDisabled(COLORREF bk);
	BOOL MakeNotActive(COLORREF bk);

	BOOL ProcessImage(C32BitImageProcessor* pProcessor);
	BOOL ProcessImage(C32BIPArray& aProcessors); // ordered list of processors
	CSize GetTamano();

	// helpers
	static HBITMAP LoadImageFile(LPCTSTR szImagePath, COLORREF crBack = -1);
	static HBITMAP LoadImageResource(UINT uIDRes, LPCTSTR szResourceType, HMODULE hInst = NULL, COLORREF crBack = 0); 
	static BOOL GetResource(LPCTSTR lpName, LPCTSTR lpType, HMODULE hInst, void* pResource, int& nBufSize);
	static IPicture* LoadFromBuffer(BYTE* pBuff, int nSize);
	static HBITMAP LoadImageBuffer(CByteArray &byte, COLORREF crBack = -1);

protected:
	COLORREF m_crBkgnd;

protected:
	RGBX* GetDIBits32();
	BOOL PrepareBitmapInfo32(BITMAPINFO& bi, HBITMAP hBitmap = NULL);

	static HBITMAP ExtractBitmap(IPicture* pPicture, COLORREF crBack);
	static int GetFileType(LPCTSTR szImagePath);
	static BOOL Fill(RGBX* pPixels, CSize size, COLORREF color);
};

// inline weighting function
inline RGBX C32BitImageProcessor::CalcWeightedColor(RGBX* pPixels, CSize size, double dX, double dY)
{
	// ASSERT (m_bWeightingEnabled);

	// interpolate between the current pixel and its pixel to the right and down
	int nX = (int)dX;
	int nY = (int)dY;

	if (dX < 0 || dY < 0)
		return pPixels[max(0, nY) * size.cx + max(0, nX)]; // closest

	RGBX* pRGB = &pPixels[nY * size.cx + nX]; // current

	double dXFraction = dX - nX;
	double dX1MinusFraction = 1 - dXFraction;
	
	double dYFraction = dY - nY;
	double dY1MinusFraction = 1 - dYFraction;
	
	RGBX* pRGBXP = &pPixels[nY * size.cx + min(nX + 1, size.cx - 1)]; // x + 1
	RGBX* pRGBYP = &pPixels[min(nY + 1, size.cy - 1) * size.cx + nX]; // y + 1
	
	int nRed = (int)((dX1MinusFraction * pRGB->btRed +
						dXFraction * pRGBXP->btRed +
						dY1MinusFraction * pRGB->btRed +
						dYFraction * pRGBYP->btRed) / 2);
	
	int nGreen = (int)((dX1MinusFraction * pRGB->btGreen +
						dXFraction * pRGBXP->btGreen +
						dY1MinusFraction * pRGB->btGreen +
						dYFraction * pRGBYP->btGreen) / 2);
	
	int nBlue = (int)((dX1MinusFraction * pRGB->btBlue +
						dXFraction * pRGBXP->btBlue +
						dY1MinusFraction * pRGB->btBlue +
						dYFraction * pRGBYP->btBlue) / 2);

	return RGBX(nRed, nGreen, nBlue);
}

