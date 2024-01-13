#include "NTC.h"
//*******************************************************************************
//Calculate the 100K resistance to temperature value
//*******************************************************************************
const float m_fCoeffBBQNTCIndex[9]=
{
    6928.7832,   //-50 
    975.8038,    //-20 
    327.0195,    // 0 
    53.1635,     //40 
    12.3825,      //80 
    3.7268,      //120 
    1.0814,      //170 
    0.3907,      //220 
    0.1071       //300 
};

const float m_fCoeffBBQNTCTable[40]=
{
//------------------------- R to T  CalcPolynomial  (y = f(x))--------------------------------
//  ---- CalcPolynomial #0 ----
    4.402790000E-14,
    -8.604120000E-10,
    6.465850000E-06,
    -2.517128200E-02,
    -1.128084151E+00,
    
//  ---- CalcPolynomial #1 ----
    4.255400000E-11,
    -1.428870000E-07,
    1.940280000E-04,
    -1.453512980E-01,
    3.127397612E+01,
    
//  ---- CalcPolynomial #2 ----
    1.110760000E-08,
    -1.047260000E-05,
    3.828981000E-03,
    -7.318059290E-01,
    6.925166585E+01,
    
//  ---- CalcPolynomial #3 ----
    1.269090000E-05,
    -2.098652000E-03,
    1.374513020E-01,
    -4.828059606E+00,
    1.222519262E+02,

//  ---- CalcPolynomial #4 ----
    3.710729000E-03,
    -1.525174940E-01,
    2.517271766E+00,
    -2.261289479E+01,
    1.764205447E+02,

//  ---- CalcPolynomial #5 ----
    5.947347350E-01,
    -7.265514708E+00,
    3.553855264E+01,
    -9.407956788E+01,
    2.384492703E+02,

//  ---- CalcPolynomial #6 ----
    7.567591610E+01,
    -2.853741129E+02,
    4.352514542E+02,
    -3.624127837E+02,
    3.103588082E+02,

//  ---- CalcPolynomial #7 ----
    9.033174096E+03,
    -1.130075969E+04,
    5.620492975E+03,
    -1.488954864E+03,
    4.074747642E+02
};


float lib_CalcPolynomial(const float *fCoeff,uint8_t u8Degree,float fX)
{
	uint8_t i;
	float fTemp;	
	
	fTemp=*fCoeff;
	for(i=0;i<u8Degree;i++)
	{
		fTemp*=fX;
		fTemp+=*(fCoeff+i+1);
	}
	return fTemp;
}

//find index from high value to low value
uint8_t FindIndexH2L(const float *fArray, uint8_t u8ArrayLen, float fValue)
{
	uint8_t i;  
  	if(fValue >= *fArray) return 0;     
    for(i=1;i<u8ArrayLen;i++)
    {      
        if(fValue >= *(fArray+i)) return i-1;        
    }    	
	return (u8ArrayLen-2);
}

//find index from low value to high value
uint8_t FindIndexL2H(const float *fArray, uint8_t u8ArrayLen, float fValue)
{
	uint8_t i;  
  	if(fValue <= *fArray) return 0;   
    for(i=1;i<u8ArrayLen;i++)
    {
        if(fValue <= *(fArray+i)) return i-1;          
    }	  
	return (u8ArrayLen-2);
}


float lib_NTCBBQ_Res2Temp(float fRes)
{
    uint8_t u8Index;
    float fVal;
    u8Index = FindIndexH2L(m_fCoeffBBQNTCIndex,9,fRes);
    fVal = lib_CalcPolynomial(&m_fCoeffBBQNTCTable[u8Index*5],4,fRes);
    return fVal;
}