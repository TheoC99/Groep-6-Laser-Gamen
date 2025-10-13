/* Actuators Test
   Doel: controleren van motoren, RGB-led, buzzer en IR-zender.
   - Motoren rijden vooruit/achteruit en sturen links/rechts.
   - RGB-led cyclust door kleuren om kanalen te checken.
   - Buzzer geeft korte piepjes.
   - IR TX stuurt 38 kHz met tone() (snelle check, geen gecodeerd protocol).
*/

#define LEFT_FWD   4
#define LEFT_BWD   5
#define RIGHT_FWD  6
#define RIGHT_BWD  7
#define RGB_R      10
#define RGB_G      11
#define RGB_B      12
#define BUZZER     3
#define IR_TX      2

void motorsStop() {
  digitalWrite(LEFT_FWD, LOW);  digitalWrite(LEFT_BWD, LOW);
  digitalWrite(RIGHT_FWD, LOW); digitalWrite(RIGHT_BWD, LOW);
}

void motorsForward() {
  digitalWrite(LEFT_FWD, HIGH);  digitalWrite(LEFT_BWD, LOW);
  digitalWrite(RIGHT_FWD, HIGH); digitalWrite(RIGHT_BWD, LOW);
}

void motorsBackward() {
  digitalWrite(LEFT_FWD, LOW);  digitalWrite(LEFT_BWD, HIGH);
  digitalWrite(RIGHT_FWD, LOW); digitalWrite(RIGHT_BWD, HIGH);
}

void motorsLeft() {
  digitalWrite(LEFT_FWD, LOW);  digitalWrite(LEFT_BWD, HIGH);
  digitalWrite(RIGHT_FWD, HIGH); digitalWrite(RIGHT_BWD, LOW);
}

void motorsRight() {
  digitalWrite(LEFT_FWD, HIGH);  digitalWrite(LEFT_BWD, LOW);
  digitalWrite(RIGHT_FWD, LOW);  digitalWrite(RIGHT_BWD, HIGH);
}

void rgb(int r, int g, int b) {
  analogWrite(RGB_R, r); // pas aan naar HIGH/LOW als geen PWM-led
  analogWrite(RGB_G, g);
  analogWrite(RGB_B, b);
}

void setup() {
  pinMode(LEFT_FWD, OUTPUT);  pinMode(LEFT_BWD, OUTPUT);
  pinMode(RIGHT_FWD, OUTPUT); pinMode(RIGHT_BWD, OUTPUT);
  pinMode(RGB_R, OUTPUT); pinMode(RGB_G, OUTPUT); pinMode(RGB_B, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(IR_TX, OUTPUT);
}

void loop() {
  // RGB test
  rgb(255,0,0); delay(400);
  rgb(0,255,0); delay(400);
  rgb(0,0,255); delay(400);
  rgb(0,0,0);

  // Buzzer test
  tone(BUZZER, 1200, 150); delay(250);
  tone(BUZZER, 800, 150); delay(250);

  // Motor test
  motorsForward(); delay(600);
  motorsStop();    delay(300);
  motorsBackward();delay(600);
  motorsStop();    delay(300);
  motorsLeft();    delay(500);
  motorsRight();   delay(500);
  motorsStop();    delay(400);

  // IR-zender 38 kHz korte burst (controle met camera/TSOP)
  tone(IR_TX, 38000);  // carrier aan
  delay(200);
  noTone(IR_TX);       // carrier uit
  delay(600);
}
