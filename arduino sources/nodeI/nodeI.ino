int currentpin = A0;
int flowpin = A1;
int relaypin = A4;

unsigned long astart;
int asampletime = 100;


unsigned long sstart;
int ssampletime = 8000;

unsigned long current;
unsigned long flow;
unsigned long readings = 0;

void setup() {

  pinMode(relaypin, OUTPUT);
  digitalWrite(relaypin, HIGH);

  Serial.begin(9600);
  Serial.println("Node Interior");

  astart = millis();//get the current time;
  sstart = millis();//get the current time;
}

void loop() {

  if ((millis() - astart) >= asampletime)
  {
    current += analogRead(currentpin);
    flow += analogRead(flowpin);
    readings++;
    astart = millis();
  }

  if ((millis() - sstart) >= ssampletime)
  {
    int caverage = (int)(current/readings);
    int faverage = (int)(flow/readings);
    Serial.write((byte)0);
    Serial.write((byte)highByte(caverage));
    Serial.write((byte)lowByte(caverage));
    Serial.write((byte)highByte(faverage));
		Serial.write((byte)lowByte(faverage));
    sstart = millis();
  }

  if (Serial.available()){
    byte rstate = Serial.read();
    digitalWrite(relaypin, !(rstate));
  }

}
