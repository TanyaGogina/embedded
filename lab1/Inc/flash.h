#ifndef FLASH_H
#define FLASH_H

#include "stm32f4xx_hal.h"
	
void SendByte(uint8_t);
uint8_t ReceiveByte(void);

void ReadDataArrayFromAddress(uint32_t,uint8_t*,uint16_t);
uint8_t ReadStatusRegister(void);
void WriteDataArray(uint32_t,uint8_t*,uint8_t);
void WriteDataArrayWithAAI(uint32_t,uint8_t*,uint8_t);
void WaitUntilBusy(void);
void WriteToStatusRegister(uint8_t);
void DisableWrite(void);
void EnableWrite(void);
void WriteDataToAddress(uint32_t,uint8_t);

void ClearSector(uint32_t);
void FullClear(void);
void setAddress(uint32_t);


#endif
