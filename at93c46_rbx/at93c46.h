#include "Arduino.h"

#ifndef at93c46_h
#define at93c46_h
//---------------------------------------------------------------------------------------------------------
class at93c46 {
  public:
    at93c46(uint8_t, uint8_t, uint8_t, uint8_t);    // Arduino pins to CS, DO, DI, SK
    uint8_t READ(uint8_t m_addr);                   // чете съдържанието на клетка с адрес m_addr
    void ERASE(uint8_t m_addr);                     // изтрива съдържанието на клетка с адрес m_addr
    void WRITE(uint8_t m_addr, uint8_t m_data);     // записва клетка на адрес m_addr с данни m_data
    void UPDATE(uint8_t m_addr, uint8_t m_data);    // чете клетка и записва върху нея данни само при разлика
    void WRAL(uint8_t m_data);                      // записва всички клетки от паметта с данни m_data
    void ERAL();                                    // изтрива съдържанието на всички клетки от паметта
    
  private:
    uint8_t CS;                                     // Chip Select
    uint8_t DO;                                     // Serial Data Output
    uint8_t DI;                                     // Serial Data Input
    uint8_t SK;                                     // Serial Data Clock
    
    void EWEN();                                    // разрешение за запис
    void EWDS();                                    // забрана за запис
};
//---------------------------------------------------------------------------------------------------------
#endif
