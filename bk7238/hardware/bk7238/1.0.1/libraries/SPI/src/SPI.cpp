#include "SPI.h"
#include <string.h>


void SPIClass::begin(uint8_t mode)
{
  if(mode == SPI_MASTER)
  {
      bk_printf("spi mster init\r\n");
      bk_spi_master_init(SPI_DEF_CLK_HZ, SPI_DEF_MODE);
  }
  else
  { 
      bk_printf("spi slave init\r\n");
      bk_spi_slave_init(SPI_DEF_CLK_HZ,SPI_DEF_MODE);
  }
}

void SPIClass::end()
{

}

void SPIClass::write(uint8_t data)
{
    struct spi_message msg;

    memset(&msg, 0, sizeof(struct spi_message));

    msg.send_buf = &data;
    msg.send_len = 1;
    msg.recv_buf = NULL;
    msg.recv_len = 0;

    bk_spi_master_xfer(&msg);
}

void SPIClass::write16(uint16_t data)
{
    uint8_t *send_data = (uint8_t*)&data;
    struct spi_message msg;

    memset(&msg, 0, sizeof(struct spi_message));

    msg.send_buf = send_data;
    msg.send_len = 2;
    msg.recv_buf = NULL;
    msg.recv_len = 0;
    
    bk_spi_master_xfer(&msg);
}

void SPIClass::write32(uint32_t data)
{
    uint8_t *send_data = (uint8_t*)&data;
    struct spi_message msg;

    memset(&msg, 0, sizeof(struct spi_message));

    msg.send_buf = send_data;
    msg.send_len = 4;
    msg.recv_buf = NULL;
    msg.recv_len = 0;
    
    bk_spi_master_xfer(&msg);
}

void SPIClass::writeBytes(const uint8_t * data, uint32_t size)
{
    struct spi_message msg;

    memset(&msg, 0, sizeof(struct spi_message));

    msg.send_buf = (uint8_t *)data;
    msg.send_len = size;
    msg.recv_buf = NULL;
    msg.recv_len = 0;
    
    bk_spi_master_xfer(&msg);
}

uint8_t SPIClass::transfer(uint8_t data)
{
    struct spi_message msg;
    uint8_t rcv_data;

    memset(&msg, 0, sizeof(struct spi_message));

    msg.send_buf = &data;
    msg.send_len = 1;
    msg.recv_buf = &rcv_data;
    msg.recv_len = 1;

    bk_printf("data:%x\r\n",data);

    bk_spi_master_xfer(&msg);
    return rcv_data;
}

uint16_t SPIClass::transfer16(uint16_t data)
{

    struct spi_message msg;
    uint16_t rcv_data;
    uint8_t *send_data = (uint8_t*)&data;

    memset(&msg, 0, sizeof(struct spi_message));

    msg.send_buf = send_data;
    msg.send_len = 2;
    msg.recv_buf = (uint8_t*)&rcv_data;
    msg.recv_len = 2;

    bk_spi_master_xfer(&msg);

    return rcv_data;
}

uint32_t SPIClass::transfer32(uint32_t data)
{
    struct spi_message msg;
    uint32_t rcv_data;
    uint8_t *send_data = (uint8_t*)&data;

    memset(&msg, 0, sizeof(struct spi_message));

    msg.send_buf = send_data;
    msg.send_len = 4;
    msg.recv_buf = (uint8_t*)&rcv_data;
    msg.recv_len = 4;

    bk_spi_master_xfer(&msg);
    return rcv_data;
}

void SPIClass::transferBytes(const uint8_t * data, uint8_t * out, uint32_t size)
{
    struct spi_message msg;

    memset(&msg, 0, sizeof(struct spi_message));

    msg.send_buf = (uint8_t * )data;
    msg.send_len = size;
    msg.recv_buf = out;
    msg.recv_len = size;

    bk_spi_master_xfer(&msg);
}

void SPIClass::transfer(void * data, uint32_t size) 
{ 
	  transferBytes((const uint8_t *)data, (uint8_t *)data, size); 
}

SPIClass SPI;
