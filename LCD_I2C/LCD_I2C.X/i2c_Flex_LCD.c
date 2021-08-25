//----------------------------------------------------------------------------- 
// Title:         i2c_Flex_LCD 
// Description:   Driver for common LCD with 1/2/3 or 4 rows by 1...20 columns 
//                using PCF8574T interface board with I2C protocol. 
// Date:          Nov-2013 
// Ver.Rev.:      1.1 
// Author:        Hugo Silva (sergio-hugo@bol.com.br) #Based on the routines of 
//                "20X4_LCD_I2C_DRIVER.h" from Pumrin S. and "lcd4_i2c.c" from XP8100 
//----------------------------------------------------------------------------- 
// 
// lcd_init() Must be called before any other function. 
// 
// lcd_putc(c) Will display c on the next position of the LCD. 
//  
//     \f Clear LCD display 
//     \n Set write position on next lcd line 
//     \b LCD backspace 
//     lcd_gotoxy(x,y) Set write position on LCD (upper left is 1,1) 
// 
// lcd_backlight_led(ON)/lcd_backlight_led(OFF) = Turn ON/OFF LCD Backlight LED 
// 
//----------------------------------------------------------------------------- 
// LCD pins D0-D3 are not used. 
//----------------------------------------------------------------------------- 
// 
// Comment   : Control of a compatible LCD (1...4 rows by 1...4 columns) from 
//              a bus I2C with an EXPANDER of I/O with connection I2C. 
//              The tests of these routines have been programmed using the IC 
//              Phillips PCF8574T. I've used 4 bits mode programming. 
//              The 8 bits mode programming is possible if you use 2 x PCF8574T. 
//              RW Pin is not being used. 
// 
// As defined in the following structure the pin connection is as follows: 
// 
//  PCF8574P     LCD 
//  ========     ====== 
//     P0        RS 
//     P1        RW    (Not used!) 
//     P2        Enable  
//     P3        Led Backlight 
//     P4        D4 
//     P5        D5 
//     P6        D6 
//     P7        D7 
// 
//  The SCL and SDA pins should be pull-up resistor as shown below: 
// 
//             +5v 
//               | 
//               < 
//               > 4.7K        
//               <          
//To PIC         |          To i2c slave 
//pin xx ------------------ SDA pin  
//(SDA)                      
//              +5v 
//               | 
//               < 
//               > 4.7K        
//               <          
//To PIC         |          To i2c slave 
//pin xx ------------------ SCL pin  
//(SCL) 
// 
//To PIC                    To i2c slave 
//Vss pin ----------------- Vss or ground pin  
//                | 
//              ----- 
//               ---  Ground 
//                -  
//  
// THIS DOCUMENT IS PROVIDED TO THE USER "AS IS" 
//----------------------------------------------------------------------------- 
#include "i2c_Flex_LCD.h"
#include "I2C.h"

char LCD_ADDR ;//             0x4E        //I2C slave address for LCD module 
char lcd_total_rows ;//       2           //Number of rows: 1,2,3 or 4 
char lcd_total_columns ;//    16          //Number of columns: 1...20   

#define RS                    0b00000001  //P0 - PCF8574T Pin connected to RS 
#define RW                    0b00000010  //P1 - PCF8574T Pin connected to RW 
#define ENABLE                0b00000100  //P2 - PCF8574T Pin connected to EN 
//#define LCD_BACKLIGHT         0b00001000  //P3 - PCF8574T Pin connected to BACKLIGHT LED 

#define addr_row_one          0x00        //LCD RAM address for row 1 
#define addr_row_two          0x40        //LCD RAM address for row 2 
#define addr_row_three        0x14        //LCD RAM address for row 3 
#define addr_row_four         0x54        //LCD RAM address for row 4 

#define ON                    1 
#define OFF                   0 
#define NOT                   ~ 
#define data_shifted          data<<4 
int new_row_request=1, BACKLIGHT_LED=LCD_BACKLIGHT; 

void lcd_backlight_led(int bl) 
{  
      if (bl)
      { 
          BACKLIGHT_LED=LCD_BACKLIGHT;
      } 
      else 
      {
          BACKLIGHT_LED=OFF; 
      }
} 

void i2c_send_nibble(char data, int type) 
{    
   switch (type) 
   {      
      case 0 :      
      I2C_Master_Write(data_shifted | BACKLIGHT_LED); 
      __delay_cycles(1); 
      I2C_Master_Write(data_shifted | ENABLE | BACKLIGHT_LED ); 
      __delay_us(2); 
      I2C_Master_Write(data_shifted & NOT ENABLE | BACKLIGHT_LED); 
      break; 
      
      case 1 : 
      I2C_Master_Write(data_shifted | RS | BACKLIGHT_LED); 
      __delay_cycles(1); 
      I2C_Master_Write(data_shifted | RS | ENABLE | BACKLIGHT_LED ); 
      __delay_us(2); 
      I2C_Master_Write(data_shifted | RS | BACKLIGHT_LED); 
      break; 
   } 
} 
    
void lcd_send_byte(int data, int type) 
   { 
        I2C_Master_Start(); 
        I2C_Master_Write(LCD_ADDR); 
        i2c_send_nibble(data >> 4 , type); 
        i2c_send_nibble(data & 0xf , type); 
        I2C_Master_Stop();        
   } 

void lcd_clear() 
{  
        lcd_send_byte(0x01,0); 
        __delay_ms(2); 
        new_row_request=1; 
} 

void lcd_init(int ADDR, char col, int row) 
{ 
   char i;
   char lcd_type=2;  // 0=5x7, 1=5x10, 2=2 lines 
   char LCD_INIT_STRING[4] = {0x20 | (lcd_type << 2), 0xc, 1, 6}; // These bytes need to be sent to the LCD to start it up.
   
   LCD_ADDR =ADDR;//             0x4E        //I2C slave address for LCD module 
   lcd_total_rows =row;//       2           //Number of rows: 1,2,3 or 4 
   lcd_total_columns = col ;
   INTCONbits.GIE = 0;     //desahabilita las interrupciones globales 
   __delay_ms(50); //LCD power up delay 
    
   I2C_Master_Start(); 
   I2C_Master_Write(LCD_ADDR); 
      i2c_send_nibble(0x00,0); 
      __delay_ms(15); 
    
   for (i=1;i<=3;++i)    
   { 
      i2c_send_nibble(0x03,0); 
      __delay_ms(5); 
   }    
      i2c_send_nibble(0x02,0); 
      __delay_ms(5); 
   I2C_Master_Stop(); 
    
   for (i=0;i<=3;++i) { 
   lcd_send_byte(LCD_INIT_STRING[i],0); 
   __delay_ms(5); 
   } 
   lcd_clear();  //Clear Display 
   INTCONbits.GIE = 1;     //habilita las interrupciones globales 
} 

void lcd_gotoxy(int x, int y) 
{ 
int row,column,row_addr,lcd_address; 
static char data; 

   if (y>lcd_total_rows){
       row = lcd_total_rows; 
   }
       else{
           row = y;
       }
  
   switch(row) 
   { 
      case 1:  row_addr=addr_row_one;     break; 
      case 2:  row_addr=addr_row_two;     break; 
      case 3:  row_addr=addr_row_three;   break; 
      case 4:  row_addr=addr_row_four;    break; 
      default: row_addr=addr_row_one;     break;  
   }  
    
   if (x>lcd_total_columns){
       column=lcd_total_columns;
   }
       else{
       column=x;
       }
   lcd_address=(row_addr+(column-1)); 
   lcd_send_byte(0x80|lcd_address,0); 
} 

//Display the character on LCD screen. 
void LCD_PUTC(const char in_data) 
{    
  switch(in_data) 
   {  
     case '\f': lcd_clear();    
     break;                
      
     case '\n': 
     new_row_request++; 
     if (new_row_request>lcd_total_rows){
         new_row_request=1; 
     }
     lcd_gotoxy(1, new_row_request); 
     break; 
                  
     case '\b': lcd_send_byte(0x10,0);
     break; 
        
     default: lcd_send_byte(in_data,1);
     break;      
      
   } 
} 
