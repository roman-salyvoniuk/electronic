
const int DATA_PIN  = 3; 
const int CLOCK_PIN = 4; 
const int LATCH_PIN = 5; 

const int SPEED_MS  = 150; 
const int NUM_BITS  = 8;   

void PinInits() {
  pinMode(DATA_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);
  

  digitalWrite(DATA_PIN, LOW);
  digitalWrite(CLOCK_PIN, LOW);
  digitalWrite(LATCH_PIN, LOW);
}

void Blinking() {
  for (int i = 0; i < NUM_BITS; i++) {
    byte pattern = (1 << i); 
    digitalWrite(LATCH_PIN, LOW);
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, pattern);
    digitalWrite(LATCH_PIN, HIGH);
    delay(SPEED_MS);
  }
}

void setup() {
  PinInits();
}

void loop() {
  Blinking();
}