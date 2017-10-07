#include "hBMP.h"
#include "hDHT.h"
#include <Wire.h>
#include <string.h>
#include <SoftwareSerial.h>
#include <avr/wdt.h>
#include "TinyGPS++.h"
int flag = 0;

String stringdata1 = "";
String stringdata2 = "";
String stringdata3 = "";
String stringdata4 = "";
String stringdata5 = "";

static const uint32_t GPSBaud = 9600;

TinyGPSPlus gps;

SoftwareSerial ss(3 , 4);
SoftwareSerial ubk(10 , 11);

hDHT _dht(2, DHT22);
hBMP bmp;
bool _f1, _f2, _f3, _f4;
String _B1, _B2;

void requestEvent() {
  //Serial.print(stringdata1);
  // Serial.print(stringdata2);
	
	//Serial.println(F("Requisicao recebida."));
  
	if(_f1){
   Serial.print(stringdata1);
	 Serial.println(F("B1 requisitada."));
   char chardata1[32];
   stringdata1.toCharArray(chardata1, 32);
	 Wire.write(chardata1);
   //Wire.endTransmission();
	}
  wdt_reset();
	if(_f2){
    Serial.print(stringdata2);
		Serial.println(F("B2 requisitada."));
    char chardata2[32];
    stringdata2.toCharArray(chardata2, 32);
		Wire.write(chardata2);
    //Wire.endTransmission();
	}
  if(_f3){
    Serial.print(stringdata3);
    Serial.println(F("B3 requisitada."));
    char chardata3[32];
    stringdata3.toCharArray(chardata3, 32);
    Wire.write(chardata3);
    //Wire.endTransmission();
  }
   if(_f4){
    Serial.print(stringdata4);
    Serial.println(F("B4 requisitada."));
    char chardata4[32];
    stringdata4.toCharArray(chardata4, 32);
    Wire.write(chardata4);
    //Wire.endTransmission();
  }
	
	if(_f1){
		_f1 = false;
    _f2 = true;
    _f3 = false;
    _f4 = false;
	} else if(_f2){
    _f1 = false;
    _f2 = false;
    _f3 = true;
    _f4 = false;
	} else if(_f3){
    _f1 = false;
    _f2 = false;
    _f3 = false;
    _f4 = true;
	} else if(_f4){
    _f1 = true;
    _f2 = false;
    _f3 = false;
    _f4 = false;
 }

	Serial.println(F("Enviado"));
  digitalWrite(7, HIGH);  
  delay(200);              
  digitalWrite(7, LOW);       
  stringdata1 = "";
  stringdata2 = "";
  stringdata3 = "";
  stringdata4 = "";
}



void setup(){
  pinMode(7, OUTPUT);
  Serial.begin(115200);
  ss.begin(GPSBaud);
  ubk.begin(GPSBaud);
  ubk.listen();
  Serial.print(F("Testing TinyGPS++ library v. ")); Serial.println(TinyGPSPlus::libraryVersion());
  wdt_enable(WDTO_8S);
	Serial.begin(115200);
	Serial.println("Setup incializado.");

	_dht.begin();
    Serial.println(F("DHT incializado."));

	Wire.begin(94);  //endereço i2c             
	Wire.onRequest(requestEvent);

	_f1 = true;
	Serial.println("Setup finalizado.");

	wdt_reset();
}


void loop(){

  static const double Home_LAT = -22.000818, Home_LON = -47.901968;

  unsigned long distanceKmToHome =
    (unsigned long)TinyGPSPlus::distanceBetween(
      gps.location.lat(),
      gps.location.lng(),
      Home_LAT, 
      Home_LON) / 1000;

  double courseToLondon =
    TinyGPSPlus::courseTo(
      gps.location.lat(),
      gps.location.lng(),
      Home_LAT, 
      Home_LON);


  const char *cardinalToLondon = TinyGPSPlus::cardinal(courseToLondon);


  //Serial.println("string:  ");
  //stringdata1.concat(gps.time);stringdata1.concat(";"); // arrumar 
  if (flag == 0){
    if (gps.altitude.isUpdated()||gps.hdop.isUpdated() || gps.location.isUpdated() || gps.satellites.isUpdated() || gps.course.isUpdated() || gps.speed.isUpdated()){
      stringdata1 = "";
      stringdata2 = "";
      stringdata4 = "";
      stringdata5 = "";
      stringdata1.concat("u");stringdata1.concat(";");    
      stringdata1.concat(gps.hdop.value());stringdata1.concat(";"); //saude do fix
      stringdata1.concat((gps.location.lat()*10000));stringdata1.concat(";");
      stringdata1.concat(gps.location.lng()*10000);stringdata1.concat(";");
      stringdata1.concat(gps.satellites.value());stringdata1.concat(";");
      stringdata2.concat(gps.altitude.meters());stringdata2.concat(";");
      stringdata2.concat(gps.course.deg());stringdata2.concat(";");
      stringdata2.concat(gps.speed.kmph());stringdata2.concat(";");
      stringdata2.concat(distanceKmToHome);stringdata2.concat(";");
      stringdata2.concat(courseToLondon);stringdata2.concat(";");
      //stringdata2.concat(cardinalToLondon);stringdata2.concat(";");
      //stringdata2.concat(TinyGPSPlus::cardinal(gps.course.value()));stringdata2.concat("\n");

      stringdata5.concat(_dht.getTemp());stringdata5.concat(";");
      stringdata5.concat(_dht.getHumd());stringdata5.concat(";");
      

  
      flag = 1;
   } 
  }

   if (flag == 1){ 
    if (gps.altitude.isUpdated()||gps.hdop.isUpdated() || gps.location.isUpdated() || gps.satellites.isUpdated() || gps.course.isUpdated() || gps.speed.isUpdated()){
      stringdata3 = "";
      stringdata4 = "";
      stringdata5 = "";
      stringdata3.concat("a");stringdata3.concat(";");    
      stringdata3.concat(gps.hdop.value());stringdata3.concat(";"); //saude do fix
      stringdata3.concat((gps.location.lat()*10000));stringdata3.concat(";");
      stringdata3.concat(gps.location.lng()*10000);stringdata3.concat(";");
      stringdata3.concat(gps.satellites.value());stringdata3.concat(";");
      stringdata4.concat(gps.altitude.meters());stringdata4.concat(";");
      stringdata4.concat(gps.course.deg());stringdata4.concat(";");
      stringdata4.concat(gps.speed.kmph());stringdata4.concat(";");
      stringdata4.concat(distanceKmToHome);stringdata4.concat(";");
      stringdata4.concat(courseToLondon);stringdata4.concat(";");
      //stringdata4.concat(cardinalToLondon);stringdata4.concat(";");
      //stringdata4.concat(TinyGPSPlus::cardinal(gps.course.value()));stringdata4.concat("\n");

      stringdata5.concat(_dht.getTemp());stringdata5.concat(";");
      stringdata5.concat(_dht.getHumd());stringdata5.concat(";");
      

      
      flag = 0;
   } 
  }
  
  

  // gps.location.isValid() retorna true se tem fix - Seria legal implementar
  smartDelay(1000); // Mantem a comunicação ativa até que não tenha mais pacotes. Não alterar

  if (millis() > 5000 && gps.charsProcessed() < 10)
    Serial.println(F("No GPS data received: check wiring")); // colocar alarme sonoro
  
	//Serial.println(F("Esperando requisicao."));
	wdt_reset();
}



// As funções para o Ublox

static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    if (flag == 0){
    ubk.listen();
    while (ubk.available() > 0){
      gps.encode(ubk.read());
    } 
  }else{
    ss.listen();
    while (ss.available() > 0){
      gps.encode(ss.read());
    }
  }
      
  } while (millis() - start < ms);
}



static void printDateTime(TinyGPSDate &d, TinyGPSTime &t) 
{
  if (!d.isValid())
  {
    Serial.print(F("********** "));
  }
  else
  {
    char sz[32];
    sprintf(sz, "%02d/%02d/%02d ", d.month(), d.day(), d.year());
    Serial.print(sz);
  }
  
  if (!t.isValid())
  {
    Serial.print(F("******** "));
  }
  else
  {
    char sz[32];
    sprintf(sz, "%02d:%02d:%02d ", t.hour(), t.minute(), t.second());
    Serial.print(sz);
  }

  smartDelay(0);
}



