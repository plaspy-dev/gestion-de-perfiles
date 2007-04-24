#pragma once

class __JPRINT_EXPORT__ CNumberClass
{
protected:
	static CString convert_nnn(int val);
	static CString convert_nn(int val);
	static CString m_saCeroAVeinte[];
	static CString m_saCientos[];
	static CString m_saDeca[];
	static CString m_saMasDeMil[];
public:
	CNumberClass(void);
	~CNumberClass(void);
	// solo recibe numeros enteros o sea dobles redondeados desprecia los valores despues del punto.
	static CString ToLetras(double valor);
};
