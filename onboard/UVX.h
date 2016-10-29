#ifndef UVX_h
#define UVX_h

#include <Arduino.h>

class UVX{
private:
	float UVA, UVB, UVC, UVA_in, UVB_in, UVC_in;
	int UVA_pin, UVB_pin, UVC_pin;
	const float ReA = 0.06, ReB = 0.12, ReC = 0.18; // W/m2
	const float Aa = 0.055*0.000001, Ab = 0.1*0.000001, Ac = 0.1*0.000001; // m2
	const float eA = 0.49363, eB = 0.710568, eC = 0.698965;
	const float Ra = 15.15*1000000, Rb = 6.61*1000000, Rc = 8.04*1000000; // Ohms
	const float GainA = ReA * Aa * eA * Ra;
	const float GainB = ReB * Ab * eB * Rb; 
	const float GainC = ReC * Ac * eC * Rc;
public:
	UVX(int uva, int uvb, int uvc);
	//~UVX(void);
	void readUVX(void);
	float get_UVA();
	float get_UVB();
	float get_UVC();
};


#endif