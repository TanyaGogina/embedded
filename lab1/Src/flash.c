#include "flash.h"

void disableFlash(void)
{
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_SET);
}

void enableFlash(void)
{
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_RESET);
}


void WriteDataToAddress(uint32_t address,uint8_t data){	
	EnableWrite();
  __nop();
  enableFlash();
  SendByte(0x02);
  setAddress(address);
  SendByte(data);
  disableFlash();
  WaitUntilBusy();
}

void setAddress(uint32_t addr){
	SendByte(addr >> 16);
  SendByte(addr >> 8);  
  SendByte(addr);
}


uint8_t ReadStatusRegister(void){
	uint8_t data = 0;
	enableFlash();
  SendByte(0x05);
  data = ReceiveByte();
  disableFlash();
	return data;
}

void WaitUntilBusy(void){
	
  while (1)
  {
    if ((ReadStatusRegister()&0x01)!=0x01) break;
    __nop();
  }
}

void WriteToStatusRegister(uint8_t statusData){
	enableFlash();
	SendByte(0x50);	
	disableFlash();
	enableFlash();
	SendByte(0x01);
	SendByte(statusData);
	disableFlash();
}

void DisableWrite(void){
	enableFlash();
  SendByte(0x04);
  disableFlash();
}

void EnableWrite(void){
	enableFlash();
  SendByte(0x06);
  disableFlash();
}

void WriteDataArrayWithAAI(uint32_t address,uint8_t* data,uint8_t len){
	EnableWrite();
  __nop();
	enableFlash();
	SendByte(0xAD);
	setAddress(address);
	SendByte(data[0]);
	SendByte(data[1]);
	disableFlash();
	WaitUntilBusy();
	for(uint16_t i=1;i<len/2;i++){
		enableFlash();
		SendByte(0xAD);
		SendByte(data[i*2]);
		SendByte(data[i*2+1]);
		disableFlash();
		WaitUntilBusy();
	}
	if(len%2==1){
		enableFlash();
		SendByte(0xAD);
		SendByte(data[len-2]);
		SendByte(0x00);
		disableFlash();
		WaitUntilBusy();
	}
	DisableWrite();
	WaitUntilBusy();
}

void WriteDataArray(uint32_t address,uint8_t* data,uint8_t len){
	for(uint16_t i=0;i<len;i++){
		WriteDataToAddress(address+i,data[i]);
	}
}

void ReadDataArrayFromAddress(uint32_t address,uint8_t* dest,uint16_t count){
	enableFlash();
	SendByte(0x03);
	setAddress(address);
	for(uint16_t i=0;i<count;i++){ 
		dest[i]=ReceiveByte();
	}
	disableFlash();
}

void ClearSector(uint32_t address){
	EnableWrite();
	__nop();
	enableFlash();
	SendByte(0x20);
	setAddress(address);
	disableFlash();
	HAL_Delay(20);
}

void FullClear(void){
	EnableWrite();
  __nop();
  enableFlash();
  SendByte(0x60);
  disableFlash();
  WaitUntilBusy();
  HAL_Delay(40);
}
