/* Sensors Test
   Doel: uitlezen van lijnsensoren (A0,A1,A2), ultrasoon (D8,D9) en IR-ontvanger (A3)
   Seriële output toont waarden; gebruik dit om drempels te kalibreren.
   - Lijnsensoren geven hogere waarde op witte tape (afhankelijk van oppervlak).
   - Ultrasoon geeft afstand in cm; 0 of 999 betekent out-of-range/time-out.
   - IR-ontvanger (TSOP) trekt pin LOW bij detectie van gemoduleerd IR.
*/

#define EDGE_LEFT  A0
#define EDGE_MID   A1
#define EDGE_RIGHT A2
#define IR_RX      A3
#define TRIG_PIN   8
#define ECHO_PIN   9

long readUltrasonicCm() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long dur = pulseIn(ECHO_PIN, HIGH, 20000UL); // timeout 20 ms
  if (dur == 0) return 999;                    // geen echo ontvangen
  return dur / 58;                             // conversie naar cm
}

void setup() {
  Serial.begin(115200);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(IR_RX, INPUT_PULLUP); // TSOP actief LOW
}

void loop() {
  int L = analogRead(EDGE_LEFT);
  int M = analogRead(EDGE_MID);
  int R = analogRead(EDGE_RIGHT);
  long cm = readUltrasonicCm();
  int ir = digitalRead(IR_RX); // 0 = IR gedetecteerd

  // compacte seriële regel voor logging en drempelbepaling
  Serial.print("Line L/M/R: ");
  Serial.print(L); Serial.print(" / ");
  Serial.print(M); Serial.print(" / ");
  Serial.print(R);
  Serial.print(" | US: ");
  Serial.print(cm); Serial.print(" cm");
  Serial.print(" | IR_RX: ");
  Serial.println(ir == LOW ? "HIT(LOW)" : "IDLE(HIGH)");

  delay(150);
}
