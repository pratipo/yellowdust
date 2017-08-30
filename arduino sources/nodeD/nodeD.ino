#include <SoftwareSerial.h>

#define SSerialRX        10  //Serial Receive pin
#define SSerialTX        11  //Serial Transmit pin

#define SSerialTxControl 3   //RS485 Direction control
#define RS485Transmit    HIGH
#define RS485Receive     LOW

SoftwareSerial RS485Serial(SSerialRX, SSerialTX); // RX, TX

unsigned int values[] = {0, 0, 0, 0}; // 4xint
byte sBuffer[] = {0, 0, 0, 0, 0, 0, 0, 0, 0}; // token byte + 4x2*byte
////

int p = 0;
int pins[] = {5, 6, 7, 8};

unsigned long duration;
unsigned long starttime;
unsigned long sampletime_ms = 2000;
unsigned long lowpulseoccupancy = 0;
float ratio = 0;
float concentration = 0;


void setup() {
  //declare pins
  pinMode(SSerialTxControl, OUTPUT);
  digitalWrite(SSerialTxControl, RS485Receive);

  // dust sensor pins
  for (int i = 0; i < 4; i++)
    pinMode(pins[i], INPUT);

  //init rs485serial
  RS485Serial.begin(4800);

  // debugging
  Serial.begin(9600);
  Serial.println("NodeSensors");

  starttime = millis();//get the current time;
}

void loop() {
  duration = pulseIn(pins[p], LOW);
  lowpulseoccupancy = lowpulseoccupancy + duration;

  if ((millis() - starttime) >= sampletime_ms) //if the sampel time = = 30s
  {
    ratio = lowpulseoccupancy / (sampletime_ms * 10.0); // Integer percentage 0=&gt;100
    lowpulseoccupancy = 0;
    values[p] = 1.1 * pow(ratio, 3) - 3.8 * pow(ratio, 2) + 520 * ratio + 0.62; // using spec sheet curve
    Serial.print("concentration on pin "); Serial.print(pins[p]);
    Serial.print(" : "); Serial.print(values[p]);
    Serial.println(" pcs/0.01cf\n");
    //convert to bytes
    sBuffer[2 * p + 1] = highByte(values[p]);
    sBuffer[2 * p + 2] = lowByte(values[p]);

    p++;

    if (p > 4) {
      p = 0; // reset counter

      digitalWrite(SSerialTxControl, RS485Transmit);
      RS485Serial.write(sBuffer, 9); //send values
      digitalWrite(SSerialTxControl, RS485Receive);  // Disable RS485 Transmit
      //reset values?
    }
    starttime = millis();
  }
}
