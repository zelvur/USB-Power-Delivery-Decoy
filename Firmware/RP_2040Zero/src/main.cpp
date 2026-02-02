#include <Arduino.h>

/* ========= PIN CONFIG ========= */
#define BTN_UP     2
#define BTN_DOWN   3

#define CFG1_PIN   13
#define CFG2_PIN   14
#define CFG3_PIN   15

#define TM_CLK     7
#define TM_DIO     8

/* ========= VOLTAGE + CFG TABLE ========= */
const uint8_t cfgTable[][3] = {
  {0,0,0}, // 5V
  {1,1,1}, // 9V
  {1,1,0}, // 12V
  {1,0,0}, // 15V
  {1,0,1}  // 20V
};

const uint8_t voltageTable[] = {5, 9, 12, 15, 20};
uint8_t level = 0;

/* ========= TM1637 DRIVER ========= */
const uint8_t segMap[] = {
  0x3f,0x06,0x5b,0x4f,0x66,
  0x6d,0x7d,0x07,0x7f,0x6f
};

void tmStart() {
  digitalWrite(TM_DIO, HIGH);
  digitalWrite(TM_CLK, HIGH);
  digitalWrite(TM_DIO, LOW);
}

void tmStop() {
  digitalWrite(TM_CLK, LOW);
  digitalWrite(TM_DIO, LOW);
  digitalWrite(TM_CLK, HIGH);
  digitalWrite(TM_DIO, HIGH);
}

void tmWrite(uint8_t data) {
  for (uint8_t i = 0; i < 8; i++) {
    digitalWrite(TM_CLK, LOW);
    digitalWrite(TM_DIO, data & 0x01);
    data >>= 1;
    digitalWrite(TM_CLK, HIGH);
  }
  digitalWrite(TM_CLK, LOW);
  pinMode(TM_DIO, INPUT);
  digitalWrite(TM_CLK, HIGH);
  pinMode(TM_DIO, OUTPUT);
}

void displayVoltage(uint8_t v) {
  uint16_t val = v * 10; // contoh: 5.0 -> 50
  uint8_t d[4] = {
    segMap[(val / 1000) % 10],
    segMap[(val / 100)  % 10],
    segMap[(val / 10)   % 10],
    segMap[val % 10]
  };

  tmStart(); tmWrite(0x40); tmStop();
  tmStart(); tmWrite(0xC0);
  for (uint8_t i = 0; i < 4; i++) tmWrite(d[i]);
  tmStop();
  tmStart(); tmWrite(0x88 | 0x07); tmStop(); // brightness max
}

/* ========= APPLY CFG ========= */
void applyCFG(uint8_t lvl) {
  digitalWrite(CFG1_PIN, cfgTable[lvl][0]);
  digitalWrite(CFG2_PIN, cfgTable[lvl][1]);
  digitalWrite(CFG3_PIN, cfgTable[lvl][2]);
  displayVoltage(voltageTable[lvl]);
}

void setup() {
  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);

  pinMode(CFG1_PIN, OUTPUT);
  pinMode(CFG2_PIN, OUTPUT);
  pinMode(CFG3_PIN, OUTPUT);

  pinMode(TM_CLK, OUTPUT);
  pinMode(TM_DIO, OUTPUT);

  applyCFG(level); // default 5V
}

void loop() {
  static uint32_t t = 0;
  if (millis() - t < 200) return;

  if (!digitalRead(BTN_UP) && level < 4) {
    level++;
    applyCFG(level);
    t = millis();
  }

  if (!digitalRead(BTN_DOWN) && level > 0) {
    level--;
    applyCFG(level);
    t = millis();
  }
}
