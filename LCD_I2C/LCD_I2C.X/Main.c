/******************************************************************************
 * Laboratorio 5
 ******************************************************************************
 * File:   Lab01.c
 * Author: Marco
 * 
 *
 */

//-----------------------------------------------------------------------------
//                            Librerias 
//-----------------------------------------------------------------------------

#define _XTAL_FREQ 4000000
#include <xc.h>
#include <stdint.h>
#include <stdio.h>  // Para usar printf
#include <string.h> // Concatenar
#include <stdlib.h>

// Librerias propias
#include "I2C.h"
  
#include "i2c_Flex_LCD.c"  


//-----------------------------------------------------------------------------
//                            Bits de configuracion 
//-----------------------------------------------------------------------------
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

//#use i2c(Master,Fast=100000, sda=PIN_B0, scl=PIN_B1,force_sw)
//#DEVICE ADC=10
//#USE DELAY(clock=20000000,crystal)
//#FUSES HS,NOPROTECT,NOWDT,NOBROWNOUT,PUT,NOLVP
//-----------------------------------------------------------------------------
//                            Variables 
//-----------------------------------------------------------------------------
int i=0; 

//-----------------------------------------------------------------------------
//                            Prototipos 
//-----------------------------------------------------------------------------

void setup(void);
void LCD(void);

//-----------------------------------------------------------------------------
//                            Interrupciones
//-----------------------------------------------------------------------------
void __interrupt() isr(void)
{
    
}

//-----------------------------------------------------------------------------
//                            Main
//-----------------------------------------------------------------------------
void main(void) {
    
    setup();    // Llamo a mi configuracion
    
    lcd_init(0x4E,16,2);
    
    while(1)    // Equivale al loop
    {
        LCD();
    }
    return;
}

//-----------------------------------------------------------------------------
//                            Funciones
//-----------------------------------------------------------------------------

void setup(void){
    
    ANSEL = 0;
    ANSELH = 0;
    
    // Puerto A
   
    // Puerto B
    
    // Puerto C

    // Puerto D
        
    // Puerto E
  
    //limpiar puertos
    PORTA = 0x00;
    PORTB = 0x00;
    PORTC = 0x00;
    PORTD = 0x00;
    PORTE = 0x00;
    
    //Configurar reloj interno
    OSCCONbits.IRCF0 = 1;        //reloj interno de 8mhz
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF2 = 1;
    OSCCONbits.SCS = 1;  //internal oscillator is used for system clock
}

void LCD(void){
   lcd_clear();  //Limpia el LCD

   // Envio de Strings al LCD usando la funci�n printf
   printf(LCD_PUTC, "\fSuscribete a"); 
   __delay_ms(1000); 
   printf(LCD_PUTC, "\nControl      "); 
   __delay_ms(1000); 
   printf(LCD_PUTC, "\nAutomatico    "); 
   __delay_ms(1000); 
   printf(LCD_PUTC, "\nEducacion     "); 
   __delay_ms(1000); 

   printf(LCD_PUTC, "\fAprende:"); 
   __delay_ms(1000); 
   printf(LCD_PUTC, "\n- PIC"); 
   __delay_ms(1000); 
   printf(LCD_PUTC, "\n- Arduino"); 
   __delay_ms(1000); 
   printf(LCD_PUTC, "\n- Control"); 
   __delay_ms(1000); 

   // Limpia el LCD 
   printf(LCD_PUTC, "\f"); 
   __delay_ms(500); 

   //Funci�n: lcd_gotoxy()
   //Si colocan un gotoxy mayor al del LCD usado, la propia funci�n
   //internamente coloca los l�mites correctos. A modo de ejemplo
   //coloquemos las esquinas correspondientes a un LCD 20x4 y la funci�n
   //Loa adaptar� si se usa un LCD 16x2

   lcd_gotoxy(3, 1);        
   printf(LCD_PUTC, "Numeros en");    
   lcd_gotoxy(4, 2);        
   printf(LCD_PUTC, "Esquinas"); 
   __delay_ms(500); 
   lcd_gotoxy(1, 1);        
   printf(LCD_PUTC, "1"); 
   __delay_ms(500); 
   lcd_gotoxy(20, 1);        
   printf(LCD_PUTC, "2"); 
   __delay_ms(500); 
   lcd_gotoxy(20, 4);        
   printf(LCD_PUTC, "3"); 
   __delay_ms(500); 
   lcd_gotoxy(1, 4);        
   printf(LCD_PUTC, "4");    
   __delay_ms(2000); 
   

   // Prueba de la funci�n de borrado Backspace
   printf(LCD_PUTC, "\f �Suscribete!\n"); 
   printf(LCD_PUTC,   "Activa: CAMPANA"); 
   __delay_ms(2000); 

   // Ultima columna y fila  2
   //Borro la fila 2 con back space
   lcd_gotoxy(20, 2);        

   // Backspace over 2nd line. 
//   for(i = 0; i < lcd_total_columns; i++) 
//      { 
//       printf(LCD_PUTC," \b\b"); 
//       __delay_ms(100); 
//      } 
//
//   printf(LCD_PUTC,   " es GRATIS!!!!!!");  
//   __delay_ms(3000); 
//
////Apaga Luz de Fondo 
//         lcd_backlight_led(OFF); 
//         printf(LCD_PUTC,"\fLCD BackLight\n     OFF      "); 
//         __delay_ms(3000); 
//
////Enciende Luz de Fondo
//         lcd_backlight_led(ON); 
//         printf(LCD_PUTC,"\fLCD BackLight\n     ON      ");  
//         __delay_ms(3000); 
}

