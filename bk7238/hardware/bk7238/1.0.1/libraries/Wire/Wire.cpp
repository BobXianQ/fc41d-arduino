extern "C" {
  #include <stdlib.h>
  #include <string.h>
}

#include "ql_hal.h"
#include "Wire.h"

void TwoWire::begin(void)
{
    rxBufferIndex = 0;
    rxBufferLength = 0;
    error = 0;
    transmitting = false;
    
    i2c_init();
}

void TwoWire::begin(uint8_t address)
{
    begin();
    i2c_set_slave_id(address);
}

void TwoWire::begin(int address)
{
    begin((uint8_t)address);
}

void TwoWire::beginTransmission(uint8_t address)
{
    i2c_set_slave_id(address);
}

uint8_t TwoWire::endTransmission(uint8_t sendStop)
{
    return 0;
}

size_t TwoWire::write(uint8_t data) 
{
    if (i2c_write(data)) 
    {
        return 1;
    } 
    else 
    {
        if (error == 0) 
            error = 3;
        return 0;
    }
}

size_t TwoWire::write(const uint8_t *data, size_t quantity) 
{
    size_t trans = 0;
    for(size_t i = 0; i < quantity; ++i)
    {
      trans += write(data[i]);
    }
    return trans;
}

uint8_t TwoWire::requestFrom(uint8_t address, uint8_t quantity,uint32_t iaddress, uint8_t isize, uint8_t sendStop) 
{
    // clamp to buffer length
    if(quantity > BUFFER_LENGTH)
        quantity = BUFFER_LENGTH;

    if (error == 0) 
        error = 2;

    i2c_read(rxBuffer,quantity);

    // set rx buffer iterator vars
    rxBufferIndex = 0;
    rxBufferLength = quantity;

    return quantity;
}

int TwoWire::read(void) 
{
    int value = -1;
    if(rxBufferIndex < rxBufferLength)
    {
      value = rxBuffer[rxBufferIndex];
      ++rxBufferIndex;
    }
    return value;
}

int TwoWire::available(void) 
{
    return rxBufferLength - rxBufferIndex;
}

int TwoWire::peek(void) 
{
    int value = -1;
    
    if(rxBufferIndex < rxBufferLength)
    {
        value = rxBuffer[rxBufferIndex];
    }

    return value;
}

TwoWire Wire;
