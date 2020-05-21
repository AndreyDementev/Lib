// Запис, четене и изтриване на клетки в EEPROM 93C46, 128 x 8 (1K), x8 bit
// 26.06.2017, Arduino IDE v1.8.3, LZ2WSG
//---------------------------------------------------------------------------------------------------------
#include "at93c46.h"
at93c46 eeprom(10, 11, 12, 13);           // 93C46 pins: 1(CS), 3(DI), 4(DO), 2(SK)
//      _____
// CS -|  ^  |- VCC
// SK -|     |- NC
// DI -|     |- ORG to GND (x8 bit)
// DO -|_____|- GND

char ch;
//---------------------------------------------------------------------------------------------------------
void setup() {
  Serial.begin(57600);
}
//---------------------------------------------------------------------------------------------------------
void loop() {
  Serial.println("EEPROM 93C46:");
  Serial.println("r - Read");
  Serial.println("b - Blank (write 0xFF)");
  Serial.println("x - Write random data");
  Serial.println();

  while (1) {
    if (Serial.available() > 0) {
      ch = Serial.read();

      if (ch == 'r')                      // read eeprom
        eeprom_read();

      if (ch == 'b') {                    // blank eeprom
        eeprom.ERAL();
        eeprom_read();
      }

      if (ch == 'x') {                    // write random data
        eeprom_write_random_data();
        eeprom_read();
      }
    }
  }
}
//---------------------------------------------------------------------------------------------------------
void eeprom_read() {
  Serial.print("         ");
  for (uint8_t i = 0; i < 16; i++) {
    Serial.print('0');
    Serial.print(i, HEX);
    Serial.print(' ');
  }
  Serial.println();

  for (uint8_t j = 0; j < 8; j++) {
    Serial.print("000000");
    Serial.print(j, HEX);
    Serial.print("0 ");
    for (uint8_t k = 0; k < 16; k++) {
      uint8_t read_byte = eeprom.READ(j * 16 + k);
      if (read_byte < 16)
        Serial.print('0');
      Serial.print(read_byte, HEX);
      Serial.print(' ');
    }
    Serial.println();
  }
  Serial.println();
}
//---------------------------------------------------------------------------------------------------------
void eeprom_write_random_data() {
  for (uint8_t i = 0; i < 128; i++)
    // eeprom.WRITE(i, random(0, 256));
    eeprom.UPDATE(i, random(0, 256));
}
