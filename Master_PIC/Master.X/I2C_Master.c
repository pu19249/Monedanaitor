/*
 * File:   I2C_Master.c
 * Author: marco
 *
 * Created on September 1, 2021, 12:46 AM
 */

#include <xc.h>
#include "I2C_Master.h"

unsigned char RS, i2c_add, BackLight_State = LCD_BACKLIGHT;

//---------------[ I2C Routines ]-------------------
//--------------------------------------------------
void I2C_Master_Init()
{
  SSPCON  = 0x28;
  SSPCON2 = 0x00;
  SSPSTAT = 0x00;
  SSPADD = ((_XTAL_FREQ/4)/I2C_BaudRate) - 1;
  SCL_D = 1;
  SDA_D = 1; 
}

void I2C_Master_Wait()
{
    while ((SSPSTAT & 0x04) || (SSPCON2 & 0x1F));
}

void I2C_Master_Start()
{
    I2C_Master_Wait();
    SEN = 1;
}

void I2C_Master_RepeatedStart()
{
    I2C_Master_Wait();
    RSEN = 1;
}

void I2C_Master_Stop()
{
    I2C_Master_Wait();
    PEN = 1;
}

void I2C_ACK(void)
{
	ACKDT = 0;			// 0 -> ACK
	I2C_Master_Wait();
    ACKEN = 1;			// Send ACK
}

void I2C_NACK(void)
{
	ACKDT = 1;			// 1 -> NACK
    I2C_Master_Wait();
	ACKEN = 1;			// Send NACK
}

unsigned char I2C_Master_Write(unsigned char data)
{
    I2C_Master_Wait();
    SSPBUF = data;
    while(!SSPIF);  // Wait Until Completion
	SSPIF = 0;
    return ACKSTAT;
}

unsigned char I2C_Read_Byte(void)
{
    //---[ Receive & Return A Byte ]---
	I2C_Master_Wait();
    RCEN = 1;		  // Enable & Start Reception
	while(!SSPIF);	  // Wait Until Completion
	SSPIF = 0;		  // Clear The Interrupt Flag Bit
    I2C_Master_Wait();
    return SSPBUF;	  // Return The Received Byte
    
}

unsigned short I2C_Master_Read(unsigned short a)
{
    unsigned short temp;
    I2C_Master_Wait();      //espera que se cumplan las condiciones adecuadas
    SSPCON2bits.RCEN = 1;
    I2C_Master_Wait();      //espera que se cumplan las condiciones adecuadas
    temp = SSPBUF;
    I2C_Master_Wait();      //espera que se cumplan las condiciones adecuadas
    if(a == 1){
        SSPCON2bits.ACKDT = 0;
    }else{
        SSPCON2bits.ACKDT = 1;
    }
    SSPCON2bits.ACKEN = 1;          // Iniciar sequencia de Acknowledge
    return temp;                    // Regresar valor del dato leído
}

//*****************************************************************************
// Función para inicializar I2C Esclavo
//*****************************************************************************
void I2C_Slave_Init(unsigned char address)
{ 
    SSPADD = address;
    SSPCON = 0x36;      // 0b00110110
    SSPSTAT = 0x80;     // 0b10000000
    SSPCON2 = 0x01;     // 0b00000001
    TRISC3 = 1;
    TRISC4 = 1;
    GIE = 1;
    PEIE = 1;
    SSPIF = 0;
    SSPIE = 1;
}

//======================================================

//---------------[ LCD Routines ]----------------
//------------------------------------------------------

void LCD_Init(unsigned char I2C_Add) 
{
  i2c_add = I2C_Add;
  IO_Expander_Write(0x00);
  __delay_ms(30);
  LCD_CMD(0x03);
  __delay_ms(5);
  LCD_CMD(0x03);
  __delay_ms(5);
  LCD_CMD(0x03);
  __delay_ms(5);
  LCD_CMD(LCD_RETURN_HOME);
  __delay_ms(5);
  LCD_CMD(0x20 | (LCD_TYPE << 2));
  __delay_ms(50);
  LCD_CMD(LCD_TURN_ON);
  __delay_ms(50);
  LCD_CMD(LCD_CLEAR);
  __delay_ms(50);
  LCD_CMD(LCD_ENTRY_MODE_SET | LCD_RETURN_HOME);
  __delay_ms(50);
}

void IO_Expander_Write(unsigned char Data) 
{
  I2C_Master_Start();
  I2C_Master_Write(i2c_add);
  I2C_Master_Write(Data | BackLight_State);
  I2C_Master_Stop();
}

void LCD_Write_4Bit(unsigned char Nibble) 
{
  // Get The RS Value To LSB OF Data  
  Nibble |= RS;
  IO_Expander_Write(Nibble | 0x04);
  IO_Expander_Write(Nibble & 0xFB);
  __delay_us(50);
}

void LCD_CMD(unsigned char CMD) 
{
  RS = 0; // Command Register Select
  LCD_Write_4Bit(CMD & 0xF0);
  LCD_Write_4Bit((CMD << 4) & 0xF0);
}

void LCD_Write_Char(char Data)
{
  RS = 1;  // Data Register Select
  LCD_Write_4Bit(Data & 0xF0);
  LCD_Write_4Bit((Data << 4) & 0xF0);
}

void LCD_Write_String(char* Str)
{
    for(int i=0; Str[i]!='\0'; i++)
       LCD_Write_Char(Str[i]); 
}

void LCD_Set_Cursor(unsigned char ROW, unsigned char COL) 
{    
  switch(ROW) 
  {
    case 2:
      LCD_CMD(0xC0 + COL-1);
      break;
    case 3:
      LCD_CMD(0x94 + COL-1);
      break;
    case 4:
      LCD_CMD(0xD4 + COL-1);
      break;
    // Case 1  
    default:
      LCD_CMD(0x80 + COL-1);
  }
}

void Backlight() 
{
  BackLight_State = LCD_BACKLIGHT;
  IO_Expander_Write(0);
}

void noBacklight() 
{
  BackLight_State = LCD_NOBACKLIGHT;
  IO_Expander_Write(0);
}

void LCD_SL()
{
  LCD_CMD(0x18);
  __delay_us(40);
}

void LCD_SR()
{
  LCD_CMD(0x1C);
  __delay_us(40);
}

void LCD_Clear()
{
  LCD_CMD(0x01); 
  __delay_us(40);
}
