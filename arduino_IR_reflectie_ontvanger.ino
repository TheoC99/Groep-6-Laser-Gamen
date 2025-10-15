/*
  TSOP58338 uitlezen met Arduino Nano
  Alleen detectie van IR-signaal (38 kHz)
*/

const int irPin = 2;   // TSOP OUT op digitale pin 2
const int ledPin = 13; // Onboard LED van de Nano

void setup() {
  pinMode(irPin, INPUT);
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  Serial.println("TSOP58338 uitlezing gestart (Arduino Nano)...");
}

void loop() {
  int status = digitalRead(irPin);  // Lees de uitgang van de TSOP

  if (status == LOW) {
    // LOW = IR 38 kHz gedetecteerd
    digitalWrite(ledPin, HIGH);
    Serial.println("IR-signaal gedetecteerd");
  } else {
    digitalWrite(ledPin, LOW);
  }

  delay(50);  // kleine vertraging om seriële output leesbaar te houden
}
