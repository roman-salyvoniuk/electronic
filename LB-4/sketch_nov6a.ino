class RunningLight {
  private:
    static const int DEFAULT_NUM_BITS = 8; 
    static const int FIRST_STEP       = 0; 
    static const int STEP_INCREMENT   = 1; 
    static const byte ACTIVE_LED_BIT  = 1; 
    static const byte LSB_MASK        = 1; 

    int dataPin;
    int clockPin;
    int latchPin;
    unsigned int speedMs;
    int numBits;
    int currentStep;


    bool isBitSet(byte pattern, int bitPosition) {
      return ((pattern >> bitPosition) & LSB_MASK) == ACTIVE_LED_BIT;
    }

    // Формує один тактовий імпульс на піні CLOCK_PIN (SRCLK)
    void pulseClock() {
      digitalWrite(clockPin, HIGH);
      digitalWrite(clockPin, LOW);
    }

    // Власна побітова передача байта замість вбудованого shiftOut (режим MSBFIRST)
    void sendByteManual(byte pattern) {
      int highestBitIndex = numBits - STEP_INCREMENT;
      int lowestBitIndex  = FIRST_STEP;

      digitalWrite(clockPin, LOW);

      for (int bitIndex = highestBitIndex; bitIndex >= lowestBitIndex; bitIndex -= STEP_INCREMENT) {
        digitalWrite(dataPin, isBitSet(pattern, bitIndex) ? HIGH : LOW);
        pulseClock();
      }
    }

    // Відправляє байт у регістр 74HC595 і фіксує його защібкою LATCH_PIN (RCLK)
    void writeToRegister(byte pattern) {
      digitalWrite(latchPin, LOW);
      sendByteManual(pattern);
      digitalWrite(latchPin, HIGH);
    }

    // Переходить до наступного світлодіода (якщо дійшли до кінця — починаємо спочатку)
    void moveToNextLed() {
      currentStep += STEP_INCREMENT;
      if (currentStep >= numBits) {
        currentStep = FIRST_STEP;
      }
    }

  public:
    // Конструктор класу
    RunningLight(int dPin, int cPin, int lPin, unsigned int delayTimeMs, int bitsCount = DEFAULT_NUM_BITS) {
      dataPin     = dPin;
      clockPin    = cPin;
      latchPin    = lPin;
      speedMs     = delayTimeMs;
      numBits     = bitsCount;
      currentStep = FIRST_STEP;
    }

    // Налаштування пінів (викликається в setup)
    void begin() {
      pinMode(dataPin, OUTPUT);
      pinMode(clockPin, OUTPUT);
      pinMode(latchPin, OUTPUT);

      digitalWrite(dataPin, LOW);
      digitalWrite(clockPin, LOW);
      digitalWrite(latchPin, LOW);
    }

    // Один крок ефекту "бігучого світла" (викликається в loop)
    void update() {
      byte pattern = (ACTIVE_LED_BIT << currentStep);

      writeToRegister(pattern);
      delay(speedMs);
      moveToNextLed();
    }
};

// Конфігурація пінів і параметрів (відповідно до твого початкового коду)
const int DATA_PIN           = 3;   // Пін 14 мікросхеми 74HC595 (SER / Дані)
const int CLOCK_PIN          = 4;   // Пін 11 мікросхеми 74HC595 (SRCLK / Такт)
const int LATCH_PIN          = 5;   // Пін 12 мікросхеми 74HC595 (RCLK / Защібка)
const unsigned int SPEED_MS  = 150; // Затримка перемикання світлодіодів (мс)
const int NUM_BITS           = 8;   // Кількість світлодіодів

// Створення об'єкта класу RunningLight
RunningLight runningLight(DATA_PIN, CLOCK_PIN, LATCH_PIN, SPEED_MS, NUM_BITS);

void setup() {
  runningLight.begin();
}

void loop() {
  runningLight.update();
}