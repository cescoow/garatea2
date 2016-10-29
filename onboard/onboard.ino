#include "DHT.h"
#include "BUZZ.hpp"
#include "SERVO.h"
#include "GPS.h"
#include "UVX.h"
#include <avr/wdt.h> // Biblioteca utilizada pelo watchdog timer.

// Pin Define
#define DHTPIN 2
#define BUZZPIN 1 // Valor a ser corrigido
#define SERVOPIN 1 // Valor a ser corrigido
#define UVA_pin 1 // Valor a ser corrigido
#define UVB_pin 1 // Valor a ser corrigido
#define UVC_pin 1 // Valor a ser corrigido

#define DHTTYPE DHT22

// Create Objects
DHT dht(DHTPIN, DHTTYPE);
BUZZ buzzer(BUZZPIN);
SERVO myServo(SERVOPIN);
UVX uvx(UVA_pin, UVB_pin, UVC_pin);

float hic;
float h;
float t;

void setup(){

    Serial.begin(9600); 
    dht.begin();
    wdt_enable(WDTO_4S); // Computador ira reiniciar se o timer nao for resetado em 4s
}

void loop(){

    wdt_reset(); // Funcao que reseta o timer

    uvx.readUVX();

    delay(2000);
    h = dht.readHumidity();
    t = dht.readTemperature();
    hic = dht.computeHeatIndex(t, h, false);

    Serial.print("\nHumidity: ");
    Serial.print(h);
    Serial.print("\nTemperature: ");
    Serial.print(t);
    Serial.print(" *C ");
    Serial.print("\nHeat index: ");
    Serial.print(hic);
    Serial.print(" *C ");
}

