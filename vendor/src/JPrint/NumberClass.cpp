# include "stdafx.h"
#include "NumberClass.h"
# include "JPrint.h"
# include <math.h>

CString CNumberClass::m_saCeroAVeinte[] = {"cero", "un", "dos", "tres", "cuatro", "cinco", "seis", "siete", "ocho", "nueve", "diez",
"once", "doce", "trece", "catorce", "quince", "dieciseis", "diecisiete", "dieciocho", "diecinueve"};
CString CNumberClass::m_saCientos[]= {"", "ciento", "docientos","trecientos","cuatrocientos","quinientos","seiscientos","setecientos","ochocientos","novecientos"};
CString CNumberClass::m_saDeca[] = {"veinte", "treinta", "cuarenta", "cincuenta", "sesenta", "setenta", "ochenta", "noventa"};
CString CNumberClass::m_saMasDeMil[]= {"", "mil", "millon", "mil millones", "billon"};

CNumberClass::CNumberClass(void)
{
}

CNumberClass::~CNumberClass(void)
{
}

CString CNumberClass::convert_nn(int val) {

	if (val < 20)
		return m_saCeroAVeinte[val];
	for (int v = 0; v < 10; v++) {
		CString dcap = m_saDeca[v];
		int dval = 20 + 10 * v;
		if (dval + 10 > val) {
			if ((val % 10) != 0)
				return dcap + " y " + m_saCeroAVeinte[val % 10];
			return dcap;
		}        
	}
	return "";
}

CString CNumberClass::convert_nnn(int val) {

	CString word = "";
	int rem = val / 100;
	int mod = val % 100;
	if (rem > 0) {
		word = m_saCientos[rem];
		if (mod > 0) {
			word = word + " ";
		}
	}
	if (mod > 0) {
		word = word + convert_nn(mod);
	} else {
		word.Delete(word.GetLength()-1);
		word.Delete(word.GetLength()-1);
	}
	return word;
}

CString CNumberClass::ToLetras(double valor) {

	// redondeamos sin cifras significativas
	ASSERT(valor>=0);
	valor= floor((double)valor);
	if(valor<100) 
		return convert_nn(valor);
	if (valor < 1000) 
		return convert_nnn(valor);

	for (int v = 1; v < 5; v++) {

		double dval = pow((double)1000, (double)v);
		if (dval > valor) {
			int didx= v-1;
			double mod = pow((double)1000, (double)didx);
			double l = floor(valor/mod);
			double r = valor - (l * mod);
			CString ret;
			if(didx>1 && l!=1) {
				ret = convert_nnn(l) + " " + m_saMasDeMil[didx] + "es";
			} else {
				if(l==1) {
					if(didx==1 || didx==3)
						ret = m_saMasDeMil[didx];
					else ret= convert_nnn(l) + " " + m_saMasDeMil[didx];
				} else ret = convert_nnn(l) + " " + m_saMasDeMil[didx];
			}
			if (r > 0) {
				ret = ret + ", " + ToLetras(r);
			}
			return ret;
		}
	}
	return "";
}

