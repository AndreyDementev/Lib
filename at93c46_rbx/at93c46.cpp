// EEPROM 93C46 library for Arduino, 128 x 8 (1K), x8 bit only, v0.5
// 26.06.2017, Arduino IDE v1.8.3, LZ2WSG
//---------------------------------------------------------------------------------------------------------
#include "at93c46.h"
#include "Arduino.h"
//      _____
// CS -|  ^  |- VCC
// SK -|     |- NC
// DI -|     |- ORG to GND (x8 bit)
// DO -|_____|- GND
//---------------------------------------------------------------------------------------------------------
at93c46::at93c46(uint8_t _CS, uint8_t _DO, uint8_t _DI, uint8_t _SK) {
  CS = _CS;
  DO = _DO;
  DI = _DI;
  SK = _SK;

  pinMode(CS, OUTPUT);
  pinMode(DO, OUTPUT);
  pinMode(DI, INPUT);
  pinMode(SK, OUTPUT);

  digitalWrite(CS, LOW);
}
//---------------------------------------------------------------------------------------------------------
uint8_t at93c46::READ(uint8_t m_addr) {                // reads data stored in memory, at specified address
  uint8_t m_data;                                      // чете съдържанието на клетка с адрес m_addr

  if (m_addr < 128) {
    digitalWrite(CS, HIGH);
    shiftOut(DO, SK, MSBFIRST, 0b00000011);            // READ: 1, 1 и 0, А6, А5, А4, А3, А2, А1, А0
    shiftOut(DO, SK, MSBFIRST, m_addr);
    m_data = shiftIn(DI, SK, MSBFIRST);                // еднократно при 8 битов достъп
    digitalWrite(CS, LOW);
  }
  return m_data;
}
//---------------------------------------------------------------------------------------------------------
void at93c46::EWEN() {                                 // write enable must precede all programming modes
  digitalWrite(CS, HIGH);                              // разрешение за запис
  shiftOut(DO, SK, MSBFIRST, 0b00000010);              // EWEN: 1, 0 и 011, X4, X3, X2, X1, X0
  shiftOut(DO, SK, MSBFIRST, 0b01100000);
  digitalWrite(CS, LOW);
  delay(2);
}
//---------------------------------------------------------------------------------------------------------
void at93c46::ERASE(uint8_t m_addr) {                  // erase memory location
  if (m_addr < 128) {                                  // изчиства съдържанието на клетка с адрес m_addr
    EWEN();
    digitalWrite(CS, HIGH);
    shiftOut(DO, SK, MSBFIRST, 0b00000011);            // ERASE: 1, 1 и 1, A6, A5, A4, A3, A2, A1, A0
    bitSet(m_addr, 7);
    shiftOut(DO, SK, MSBFIRST, m_addr);
    digitalWrite(CS, LOW);
    delay(5);
    EWDS();
  }
}
//---------------------------------------------------------------------------------------------------------
void at93c46::WRITE(uint8_t m_addr, uint8_t m_data) {  // writes memory location
  if (m_addr < 128) {                                  // записва клетка на адрес m_addr с данни m_data
    EWEN();
    digitalWrite(CS, HIGH);
    shiftOut(DO, SK, MSBFIRST, 0b00000010);            // WRITE: 1, 0 и 1, А6, А5, А4, А3, А2, А1, А0
    bitSet(m_addr, 7);
    shiftOut(DO, SK, MSBFIRST, m_addr);
    shiftOut(DO, SK, MSBFIRST, m_data);
    digitalWrite(CS, LOW);
    delay(5);
    EWDS();
  }
}
//---------------------------------------------------------------------------------------------------------
void at93c46::UPDATE(uint8_t m_addr, uint8_t m_data) { // eeprom endurance
  uint8_t m_exist_data;                                // чете клетка и записва върху нея данни само при разлика

  if (m_addr < 128) {
    m_exist_data = READ(m_addr);
    if (m_data != m_exist_data) {                      // записвай ако данните за запис са различни от записаните
      EWEN();
      digitalWrite(CS, HIGH);
      shiftOut(DO, SK, MSBFIRST, 0b00000010);          // WRITE: 1, 0 и 1, А6, А5, А4, А3, А2, А1, А0
      bitSet(m_addr, 7);
      shiftOut(DO, SK, MSBFIRST, m_addr);
      shiftOut(DO, SK, MSBFIRST, m_data);
      digitalWrite(CS, LOW);
      delay(5);
      EWDS();
    }
  }
}
//---------------------------------------------------------------------------------------------------------
void at93c46::ERAL() {                                 // erases all memory locations
  EWEN();                                              // изчиства съдържанието на всички клетки от паметта
  digitalWrite(CS, HIGH);
  shiftOut(DO, SK, MSBFIRST, 0b00000010);              // ERAL: 1, 0 и 010, X4, X3, X2, X1, X0
  shiftOut(DO, SK, MSBFIRST, 0b01000000);
  digitalWrite(CS, LOW);
  delay(5);
  EWDS();
}
//---------------------------------------------------------------------------------------------------------
void at93c46::WRAL(uint8_t m_data) {                   // writes all memory locations
  EWEN();                                              // записва всички клетки от паметта с данни m_data
  digitalWrite(CS, HIGH);
  shiftOut(DO, SK, MSBFIRST, 0b00000010);              // WRAL: 1, 0 и 001, X4, X3, X2, X1, X0
  shiftOut(DO, SK, MSBFIRST, 0b00100000);
  shiftOut(DO, SK, MSBFIRST, m_data);
  digitalWrite(CS, LOW);
  delay(5);
  EWDS();
}
//---------------------------------------------------------------------------------------------------------
void at93c46::EWDS() {                                 // disables all programming instructions
  digitalWrite(CS, HIGH);                              // забрана за запис
  shiftOut(DO, SK, MSBFIRST, 0b00000010);              // EWDS: 1, 0 и 000, X4, X3, X2, X1, X0
  shiftOut(DO, SK, MSBFIRST, 0b00000000);
  digitalWrite(CS, LOW);
  delay(2);
}
