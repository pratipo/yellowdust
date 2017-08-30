#include <SoftwareSerial.h>

#define SSerialRX        10  //Serial Receive pin
#define SSerialTX        11  //Serial Transmit pin

#define SSerialTxControl 3   //RS485 Direction control
#define RS485Transmit    HIGH
#define RS485Receive     LOW

SoftwareSerial RS485Serial(SSerialRX, SSerialTX); // RX, TX
byte sBuffer[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
int p = 1;
byte byteReceived = 0;

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

void relay(int state) {
  for (int i = 0; i < 4; i++)
    digitalWrite(rpins[i], !state);
}

void loop() {

  if (Serial.available()) // control leds (osc->Rpi -> Arduino)
  {
    byteReceived = Serial.read();
    if (byteReceived == 97) {
      relay(0); // turn leds OFF
    }
    else if (byteReceived == 98) {
      relay(1); // turn leds ON
    }
  }

  if (RS485Serial.available())  //Look for data from dust sensors
  {
    byteReceived = RS485Serial.read();    // Read received byte
    //Serial.print("sensor data: ");
    //Serial.println(byteReceived);

    if (byteReceived == 0) {
      p = 1;
    }
    else {
      sBuffer[p++] = byteReceived;

      if (p == 9) {
        Serial.write(sBuffer, 9);
      }
    }
  }
}

// read current
//if (millis - t > 500)
//  t = millis();
//c = analogRead(currentPin);
// send current data ?


////////////////////////////////////////////////////////



