const int PIN_LATCH = 2;      
const int PIN_CLOCK = 3;       
const int PIN_DATA  = 4;      

const int LED_COUNT = 8;     
const int STEP_DELAY_MS = 150; 

class RunningLight {
  private:
    int latchPin;
    int clockPin;
    int dataPin;
    int currentLed;

    void sendByte(uint8_t data) {
      digitalWrite(latchPin, LOW); 

      for (int i = 7; i >= 0; i--) {
        digitalWrite(clockPin, LOW);

        if (bitRead(data, i) == 1) {
          digitalWrite(dataPin, HIGH);
        } else {
          digitalWrite(dataPin, LOW);
        }

        digitalWrite(clockPin, HIGH);
      }

      digitalWrite(latchPin, HIGH); 
    }

  public:
    RunningLight(int latch, int clock, int data) {
      latchPin = latch;
      clockPin = clock;
      dataPin = data;
      currentLed = 0;
    }

    void begin() {
      pinMode(latchPin, OUTPUT);
      pinMode(clockPin, OUTPUT);
      pinMode(dataPin, OUTPUT);
      clear();
    }

    void clear() {
      sendByte(0);
    }

    void step() {
      uint8_t pattern = (1 << currentLed);
      sendByte(pattern);

      currentLed++;
      if (currentLed >= LED_COUNT) {
        currentLed = 0;
      }
    }
};

RunningLight runner(PIN_LATCH, PIN_CLOCK, PIN_DATA);

void setup() {
  runner.begin();
}

void loop() {
  runner.step();
  delay(STEP_DELAY_MS);
}
