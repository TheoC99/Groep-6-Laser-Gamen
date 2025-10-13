/* Control Test (State Machine)
   Doel: simpele besturing via Bluetooth met ESP32
   - verbinding maken met telefoon / controller
   - Toon via Serial monitor de geregistreerde bewegingen
*/

#define LEFT_FWD   4
#define LEFT_BWD   5
#define RIGHT_FWD  6
#define RIGHT_BWD  7

#define EDGE_LEFT  A0
#define EDGE_MID   A1
#define EDGE_RIGHT A2

int edgeThr = 700; // kalibreren op locatie

enum State { PATROL, AVOID };
State state = PATROL;

void motorsStop(){ 
   digitalWrite(LEFT_FWD,LOW); 
   digitalWrite(LEFT_BWD,LOW); 
   digitalWrite(RIGHT_FWD,LOW); 
   digitalWrite(RIGHT_BWD,LOW); }
void motorsForward(){ 
   digitalWrite(LEFT_FWD,HIGH); 
   digitalWrite(LEFT_BWD,LOW); 
   digitalWrite(RIGHT_FWD,HIGH);
   digitalWrite(RIGHT_BWD,LOW); }
void motorsBackward(){ 
   digitalWrite(LEFT_FWD,LOW); 
   digitalWrite(LEFT_BWD,HIGH); 
   digitalWrite(RIGHT_FWD,LOW); 
   digitalWrite(RIGHT_BWD,HIGH); }
void motorsRight(){ 
   digitalWrite(LEFT_FWD,HIGH); 
   digitalWrite(LEFT_BWD,LOW); 
   digitalWrite(RIGHT_FWD,LOW); 
   digitalWrite(RIGHT_BWD,HIGH); }

bool edgeHit() {
  // indien wit tape hoge waarde geeft: gebruik '>' vergelijking
  int L = analogRead(EDGE_LEFT);
  int M = analogRead(EDGE_MID);
  int R = analogRead(EDGE_RIGHT);
  return (L > edgeThr) || (M > edgeThr) || (R > edgeThr);
}

void setup() {
  pinMode(LEFT_FWD,OUTPUT); pinMode(LEFT_BWD,OUTPUT);
  pinMode(RIGHT_FWD,OUTPUT); pinMode(RIGHT_BWD,OUTPUT);
}

void loop() {
  switch(state) {
    case PATROL:
      motorsForward();
      if (edgeHit()) {
        motorsStop();
        state = AVOID;
      }
      delay(10); // korte wachttijd, CPU ontlasten
      break;

    case AVOID:
      // simpele ontsnapping: beetje achteruit, dan rechtsaf
      motorsBackward(); delay(250);
      motorsRight();    delay(300);
      motorsStop();     delay(50);
      state = PATROL;
      break;
  }
}
