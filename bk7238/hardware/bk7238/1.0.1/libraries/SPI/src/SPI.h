#ifndef _SPI_ARDUINO_H_
#define _SPI_ARDUINO_H_

#include <stdlib.h>
#include "ql_hal.h"

class SPIClass
{
private:
    uint32_t _clock;
    uint8_t  _bitOrder;
    uint8_t  _dataMode;
    void writePattern_(const uint8_t * data, uint8_t size, uint8_t repeat);

public:
    SPIClass() :_clock(1000000), _bitOrder(SPI_MSB), _dataMode(SPI_MODE_0) {};
    ~SPIClass(){};
    void begin(uint8_t mode = SPI_MASTER);
    void end();

    void setBitOrder(uint8_t bitOrder)  {_bitOrder = bitOrder;}
    void setDataMode(uint8_t dataMode)  {_dataMode = dataMode;}
    void setFrequency(uint32_t freq)    {_clock = freq;}

    void transfer(void * data, uint32_t size);
    uint8_t transfer(uint8_t data);
    uint16_t transfer16(uint16_t data);
    uint32_t transfer32(uint32_t data);
  
    void transferBytes(const uint8_t * data, uint8_t * out, uint32_t size);
    void transferBits(uint32_t data, uint32_t * out, uint8_t bits);

    void write(uint8_t data);
    void write16(uint16_t data);
    void write32(uint32_t data);
    void writeBytes(const uint8_t * data, uint32_t size);
    void writePixels(const void * data, uint32_t size);
    void writePattern(const uint8_t * data, uint8_t size, uint32_t repeat);

};

extern SPIClass SPI;

#endif
