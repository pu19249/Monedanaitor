/* 
 * File:   i2c_Flex_LCD.h
 * Author: marco
 *
 * Created on August 25, 2021, 12:08 AM
 */

#ifndef __i2c_Flex_LCD_H
#define	__i2c_Flex_LCD_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <pic16f887.h>
#include <stdint.h>

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 8000000
#endif

void lcd_backlight_led(int bl);
void i2c_send_nibble(char data, int type);
void lcd_send_byte(int data, int type);
void lcd_clear();
void lcd_init(int ADDR, char col, int row);
void lcd_gotoxy(int x, int y);
void LCD_PUTC(const char in_data);

#endif	
