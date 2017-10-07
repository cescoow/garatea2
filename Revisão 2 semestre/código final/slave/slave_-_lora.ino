#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <avr/wdt.h>
#include <LoRa.h>

#include <stdlib.h>
#include <string.h>
#include <util/crc16.h>
#define RADIOPIN 5
int counter = 0;
int i = 0;
String stringdata = "";
char _B[20];
File myFile;

void radioSetup(){
	pinMode(RADIOPIN, OUTPUT);
	pinMode(A1, OUTPUT);
	//digitalWrite(A1, HIGH); 
}

void setup(){  
 Serial.begin(115200);
 Wire.begin();
 LoRa.setPins(7, A0, 2);
 if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  
	
	while (!SD.begin(4)) {
    	Serial.println("Falha no SD.");
    	delay(1000);
  	}
  	Serial.println("SD inicializado.");

  	_B[0] = '\0';

  	radioSetup();
  	wdt_enable(WDTO_8S);
}


void save_data(){
	int time = millis();
	wdt_reset(); 
	Serial.println(F("Save."));
	myFile = SD.open("log.txt", FILE_WRITE);
	if(myFile){
		myFile.print(time);
		myFile.print(';');
		myFile.println(_B);
		myFile.close();
		Serial.println(F("Gravado."));
	}
}

void receive_data(){
	wdt_reset(); 
	Serial.println(F("Receive."));
	char c;
	_B[0] = '\0';
	int i = 0;

	Wire.requestFrom(94, 32);    // 6 bytes do endereço #8
 	while (Wire.available()) { // slave may send less than requested
    char c = Wire.read(); // receive a byte as character
    //Serial.print(c);         // print the character
    stringdata.concat(c);
    
  }
  Serial.println(stringdata);
  save_data();
}

void rtty_txbit (int bit) {
  if (bit) {
    digitalWrite(5, HIGH);
  }
  else {
    digitalWrite(5, LOW);
  }

  delayMicroseconds(10000);
  delayMicroseconds(10150); 
}

void rtty_txbyte (char c) {
	int i;
	rtty_txbit (0);
	for (i = 0; i < 7; i++) { // Change this here 7 or 8 for ASCII-7 / ASCII-8
		if (c & 1) rtty_txbit(1);
		else rtty_txbit(0);
		c = c >> 1;
	}
	rtty_txbit (1); // Stop bit
	rtty_txbit (1); // Stop bit
	wdt_reset();
}

void rtty_txstring (char * string) {
	char c;
	c = *string++;
	while ( c != '\0') {
		rtty_txbyte (c);
		c = *string++;
	}
}

uint16_t gps_CRC16_checksum (char *string) {
  size_t i;
  uint16_t crc;
  uint8_t c;
  crc = 0xFFFF;

  for (i = 2; i < strlen(string); i++) {
    c = string[i];
    crc = _crc_xmodem_update (crc, c);
  }
  return crc;
}

void loop(){
	receive_data();
	wdt_reset(); 
	//rtty_txstring("hi");
	wdt_reset();
	delay(1000);
  Serial.print("Sending packet: ");
  Serial.println(counter);

  // send packet
  LoRa.beginPacket();
  LoRa.print(stringdata);
  LoRa.print(counter);
  LoRa.endPacket();
  counter++;
  delay(1000);
  i++;
  if (i >=5 ){
    stringdata = "";
    i = 0;
  }
}
