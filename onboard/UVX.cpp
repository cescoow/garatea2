#include "UVX.h"

UVX::UVX(int uva, int uvb, int uvc){
	this->UVA_pin = uva;
	this->UVB_pin = uvb;
	this->UVC_pin = uvc;
	this->UVA = 0;
	this->UVB = 0;
	this->UVC = 0;
}

float UVX::get_UVA(){
	return this->UVA;
}
float UVX::get_UVB(){
	return this->UVB;
}
float UVX::get_UVC(){
	return this->UVC;
}

void UVX::readUVX(void){
	float UVAtemp = 0;
    float UVBtemp = 0;
    float UVCtemp = 0;

    this->UVA_in = analogRead(UVA_pin);
    this->UVB_in = analogRead(UVB_pin);
    this->UVC_in = analogRead(UVC_pin);

    // Fazer um loop com algumas medidas e fazer uma media
    for (int a = 0; a < 10; a++) {
        UVAtemp += 3.3*UVA_in/(0.06*this->GainA); // Leitura em W
        UVBtemp += 3.3*UVB_in/(0.12*this->GainB);
        UVCtemp += 3.3*UVC_in/(0.18*this->GainC);
    }
    UVA = UVAtemp/10;
    UVB = UVBtemp/10;
    UVC = UVCtemp/10;
}