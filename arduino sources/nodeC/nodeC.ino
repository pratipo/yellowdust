#include <SoftwareSerial.h>

#define SSerialRX        10  //Serial Receive pin
#define SSerialTX        11  //Serial Transmit pin

#define SSerialTxControl 12  //RS485 Direction control
#define RS485Transmit    HIGH
#define RS485Receive     LOW

SoftwareSerial RS485Serial(SSerialRX, SSerialTX); // RX, TX
byte byteReceived = 0;
int protocol = 0;
int p = 0;
byte sBuffer[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

int rpins[] = {6, 7, 8, 9};

void setup() {

  //declare pins
  pinMode(SSerialTxControl, OUTPUT);
  digitalWrite(SSerialTxControl, RS485Receive);

  for (int i = 0; i < 4; i++) {
    pinMode(rpins[i], OUTPUT);
    digitalWrite(rpins[i], HIGH);
  }

  //init serial // arduino-rpi
  Serial.begin(9600);
  //Serial.println("Master");

  //init rs485serial
  RS485Serial.begin(4800);

}

void relay(byte state) {
  if (state==0){
    for (int i = 0; i < 4; i++)
      digitalWrite(rpins[i], HIGH); // OFF
  }
  else if (state==1){
      digitalWrite(rpins[0], LOW); // ARCO 1
      digitalWrite(rpins[1], LOW); // ARCO 2
      digitalWrite(rpins[2], HIGH); // ARCO 2
      digitalWrite(rpins[3], HIGH);  // ARCO 1
  }
  else if (state==2){
    for (int i = 0; i < 4; i++)
      digitalWrite(rpins[i], LOW); // ON
  }
}

void loop() {

  if (Serial.available()) // control leds (osc->Rpi -> Arduino)
  {
    byteReceived = Serial.read();
    relay(byteReceived); // turn leds ON
  }

  if (RS485Serial.available())  //Look for data from dust sensors
  {
    byteReceived = RS485Serial.read();    // Read received byte
    Serial.write(byteReceived);
    //Serial.print("sensor data: ");
    //Serial.println(byteReceived);

    /*if (byteReceived == 255) {
      protocol++;
      if (protocol == 2){
        protocol = 0;
        p = 2;
      }
    }
    else{
      protocol = 0;
    }
    
    if (p >= 2){
      sBuffer[p++] = byteReceived;

      if (p > 9) {
        Serial.write(sBuffer, 10);
        p=0;
        /// DEBUG
//        for (int i =0; i<9; i++) {
//          Serial.println(sBuffer[i]);
//        }
      }
    }*/
  }
}

/*
// read current
//if (millis - t > 500)
//  t = millis();
//c = analogRead(currentPin);
// send current data ?


if 255
  p 0->1



*/
