const int latchPin = 3;  // ST_CP (12 пін 74HC595)
const int clockPin = 4;  // SH_CP (11 пін 74HC595)
const int dataPin  = 2;  // DS (14 пін першого 74HC595)

// Ручна передача байта через зсувний регістр без використання shiftOut
void shiftOutManual(byte value) {
  for (int i = 7; i >= 0; --i) {
    digitalWrite(dataPin, (value >> i) & 1);
    digitalWrite(clockPin, HIGH);
    digitalWrite(clockPin, LOW);
  }
}

// Запис у каскадно з'єднані регістри: спочатку КОЛОНКИ, потім РЯДКИ
void writeColumnsAndRows(byte colsMask, byte rowsMask) {
  digitalWrite(latchPin, LOW);
  shiftOutManual(colsMask);  // перший 74HC595 (колонки)
  shiftOutManual(rowsMask);  // другий 74HC595 (рядки)
  digitalWrite(latchPin, HIGH);
}

void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin,  OUTPUT);
}

// Функція перевірки вершин (кутових точок 4 кутів матриці 8x8)
void testCorners(int durationMs) {
  const byte COLS_MASK = (1 << 7) | (1 << 0);
  unsigned long start = millis();
  while (millis() - start < durationMs) {
    // Верхній рядок (row 0): кути [0,0] та [0,7]
    writeColumnsAndRows(~COLS_MASK, 1 << 0);
    delayMicroseconds(1000);

    // Нижній рядок (row 7): кути [7,0] та [7,7]
    writeColumnsAndRows(~COLS_MASK, 1 << 7);
    delayMicroseconds(1000);
  }
}

// Анімація "хрест": біжуча точка по центральній вертикалі, потім по горизонталі
void loop() {
  const int centerCol = 3; // центральна вертикаль (колонка 3)
  const int centerRow = 3; // центральна горизонталь (рядок 3)
  const int stepDelay = 80; // затримка між кроками біжучого світлодіода

  // 1. Рух по центральній вертикалі зверху вниз (row: 0 -> 7, col: 3)
  for (int r = 0; r < 8; ++r) {
    writeColumnsAndRows(~(1 << centerCol), (1 << r));
    delay(stepDelay);
  }

  // 2. Рух по центральній горизонталі зліва направо (col: 0 -> 7, row: 3)
  for (int c = 0; c < 8; ++c) {
    writeColumnsAndRows(~(1 << c), (1 << centerRow));
    delay(stepDelay);
  }

  // Вимикаємо дисплей перед повтором циклу
  writeColumnsAndRows(0xFF, 0x00);
  delay(100);
}
