/* Points Indication Test
   Doel: punten/levens testen met duidelijke feedback.
   - Gebruik IR_RX als "hit" trigger (LOW = geraakt) of overbrug A3 kort naar GND.
   - RGB-led toont status (groen = OK, rood = geraakt), buzzer piept.
   - Seriële monitor toont actuele score/levens.
*/

#define IR_RX   A3
#define RGB_R   10
#define RGB_G   11
#define RGB_B   12
#define BUZZER  3

volatile int lives = 3;
unsigned long lastHitMs = 0;
const unsigned long HIT_DEBOUNCE_MS = 400; // voorkom dubbel tellen

void rgb(int r,int g,int b){
  analogWrite(RGB_R,r); analogWrite(RGB_G,g); analogWrite(RGB_B,b);
}

void setup() {
  Serial.begin(115200);
  pinMode(IR_RX, INPUT_PULLUP); // actief LOW
  pinMode(RGB_R, OUTPUT); pinMode(RGB_G, OUTPUT); pinMode(RGB_B, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  rgb(0,255,0);
  Serial.println("Points test: bring IR_RX LOW to simulate a hit");
}

void loop() {
  // "hit" detecteren
  if (digitalRead(IR_RX) == LOW) {
    unsigned long now = millis();
    if (now - lastHitMs > HIT_DEBOUNCE_MS && lives > 0) {
      lastHitMs = now;
      lives--;
      // NL: feedback
      rgb(255,0,0);
      tone(BUZZER, 800, 120);
      Serial.print("Hit registered. Lives: ");
      Serial.println(lives);
      delay(200);
      rgb(0,255,0);
    }
  }

  // game-over indicatie
  if (lives <= 0) {
    rgb(255,0,0);
    tone(BUZZER, 400, 500);
    Serial.println("Game Over");
    while(true) { delay(1000); } // stoppen
  }

  // periodieke status
  static unsigned long t0 = 0;
  if (millis() - t0 > 1000) {
    t0 = millis();
    Serial.print("Status -> Lives: "); Serial.println(lives);
  }
}
