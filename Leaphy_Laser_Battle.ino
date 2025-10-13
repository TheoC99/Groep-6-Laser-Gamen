/* 
  Leaphy Laser Battle – Startversie
  ---------------------------------
  Basis logica voor autonome IR-gevechtsrobot
  - Willekeurige beweging
  - Randsensor check
  - IR-hit detectie
  - Schiet-cooldown
*/

#define LEFT_FWD   4
#define LEFT_BWD   5
#define RIGHT_FWD  6
#define RIGHT_BWD  7

#define TRIG_PIN   8
#define ECHO_PIN   9
#define RGB_R      10
#define RGB_G      11
#define RGB_B      12
#define BUZZER     3

#define IR_TX      2     // PWM-uitgang voor IR-LED (38 kHz)
#define IR_RX      A3    // Ingang voor IR-ontvanger
#define EDGE_LEFT  A0
#define EDGE_MID   A1
#define EDGE_RIGHT A2

int lives = 3;
bool stunned = false;
unsigned long lastFire = 0;
const unsigned long FIRE_COOLDOWN = 2500; // ms
unsigned long stunEnd = 0;

void setup() {
  pinMode(LEFT_FWD, OUTPUT);
  pinMode(LEFT_BWD, OUTPUT);
  pinMode(RIGHT_FWD, OUTPUT);
  pinMode(RIGHT_BWD, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(IR_TX, OUTPUT);
  pinMode(IR_RX, INPUT);
  pinMode(RGB_R, OUTPUT);
  pinMode(RGB_G, OUTPUT);
  pinMode(RGB_B, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  Serial.begin(9600);
  randomSeed(analogRead(0));

  // 38 kHz PWM voor IR-laser
  startIRPWM();
}

void loop() {
  if (lives <= 0) {
    stopMotors();
    digitalWrite(BUZZER, HIGH);
    rgbColor(255, 0, 0);
    delay(2000);
    digitalWrite(BUZZER, LOW);
    while (true); // game over
  }

  if (stunned && millis() < stunEnd) {
    stopMotors();
    return;
  } else if (stunned) {
    stunned = false;
  }

  // Edge detectie
  if (detectEdge()) {
    avoidEdge();
  } else {
    // Willekeurige beweging
    moveForward();
    delay(random(200, 500));
  }

  // Willekeurig schieten
  if (millis() - lastFire > FIRE_COOLDOWN) {
    shootLaser();
    lastFire = millis();
  }

  // Check voor hit
  if (digitalRead(IR_RX) == LOW) { // TSOP geeft LOW bij detectie
    registerHit();
  }
}

// ===== Functies =====

void startIRPWM() {
  // 38kHz PWM op pin 2 (Timer2)
  // Arduino Nano: pin 3 = OC2B, pin 11 = OC2A → hier aangepast voor pin 2 indien beschikbaar
  // Simpele software-pulse als alternatief:
  tone(IR_TX, 38000); // snel werkende noodoptie
}

void shootLaser() {
  rgbColor(0, 0, 255);
  tone(BUZZER, 1500, 100);
  digitalWrite(IR_TX, HIGH);
  delay(200);
  digitalWrite(IR_TX, LOW);
  rgbColor(0, 255, 0);
}

void registerHit() {
  lives--;
  stunned = true;
  stunEnd = millis() + 1500;
  rgbColor(255, 0, 0);
  tone(BUZZER, 800, 200);
  Serial.print("Hit! Remaining lives: ");
  Serial.println(lives);
}

bool detectEdge() {
  int l = analogRead(EDGE_LEFT);
  int m = analogRead(EDGE_MID);
  int r = analogRead(EDGE_RIGHT);
  // Kalibratie: zwart = laag, wit = hoog
  return (l > 800 || m > 800 || r > 800);
}

void avoidEdge() {
  stopMotors();
  moveBackward();
  delay(300);
  turnRight();
  delay(400);
}

void moveForward() {
  digitalWrite(LEFT_FWD, HIGH);
  digitalWrite(LEFT_BWD, LOW);
  digitalWrite(RIGHT_FWD, HIGH);
  digitalWrite(RIGHT_BWD, LOW);
}

void moveBackward() {
  digitalWrite(LEFT_FWD, LOW);
  digitalWrite(LEFT_BWD, HIGH);
  digitalWrite(RIGHT_FWD, LOW);
  digitalWrite(RIGHT_BWD, HIGH);
}

void turnRight() {
  digitalWrite(LEFT_FWD, HIGH);
  digitalWrite(LEFT_BWD, LOW);
  digitalWrite(RIGHT_FWD, LOW);
  digitalWrite(RIGHT_BWD, HIGH);
}

void stopMotors() {
  digitalWrite(LEFT_FWD, LOW);
  digitalWrite(LEFT_BWD, LOW);
  digitalWrite(RIGHT_FWD, LOW);
  digitalWrite(RIGHT_BWD, LOW);
}

void rgbColor(int r, int g, int b) {
  analogWrite(RGB_R, r);
  analogWrite(RGB_G, g);
  analogWrite(RGB_B, b);
}
