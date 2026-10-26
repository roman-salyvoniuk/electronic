const int PullReset = 2;         
const int TriggerResetButton = 3;     

const int TriggerPinQ = 5;       
const int TriggerPinNotQ = 4;     

const int ForbiddenInputPin = 6;  
const int LedForbidden = 13;      


const unsigned long BAUD_RATE = 9600;
const unsigned long LOOP_DELAY_MS = 25;


const int STATE_NORMAL = 0;
const int STATE_FORBIDDEN = 1;


void PinInits() {
  pinMode(PullReset, INPUT);      
  pinMode(TriggerResetButton, OUTPUT);
  pinMode(TriggerPinQ, INPUT);
  pinMode(TriggerPinNotQ, INPUT);
  pinMode(ForbiddenInputPin, INPUT);
  pinMode(LedForbidden, OUTPUT);

  digitalWrite(TriggerResetButton, LOW);
  digitalWrite(LedForbidden, LOW);
}

void HandleResetButton() {
  int resetBtn = digitalRead(PullReset);

  if (resetBtn == HIGH) {
    digitalWrite(TriggerResetButton, HIGH);
  } else {
    digitalWrite(TriggerResetButton, LOW);
  }
}

int ReadForbiddenState() {
  int sensorState = digitalRead(ForbiddenInputPin);
  int forbidden = STATE_NORMAL;

  if (sensorState == HIGH) {
    forbidden = STATE_FORBIDDEN;
    digitalWrite(LedForbidden, HIGH);
  } else {
    forbidden = STATE_NORMAL;
    digitalWrite(LedForbidden, LOW);
  }

  return forbidden;
}

void SerialPrint(int qState, int notQState, int forbidden) {
  Serial.print("Q:");
  Serial.print(qState);
  Serial.print(" notQ:");
  Serial.print(notQState);
  Serial.print(" Forbidden:");
  Serial.println(forbidden);
}

void setup() {
  PinInits();
  Serial.begin(BAUD_RATE);
}

void loop() {
  HandleResetButton();

  int qState = digitalRead(TriggerPinQ);
  int notQState = digitalRead(TriggerPinNotQ);

  int forbidden = ReadForbiddenState();

  SerialPrint(qState, notQState, forbidden);

  delay(LOOP_DELAY_MS);
}
