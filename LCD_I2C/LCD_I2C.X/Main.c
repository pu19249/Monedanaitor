/******************************************************************************
 * Proyecto 1 
 ******************************************************************************
 * File:   Main.c
 * Author: Marco
 * Clasificador de Monedas
 *
 */

//-----------------------------------------------------------------------------
//                            Librerias 
//-----------------------------------------------------------------------------

#define _XTAL_FREQ 8000000
#include <xc.h>
#include <stdint.h>
#include <stdio.h>  // Para usar printf
#include <string.h> // Concatenar
#include <stdlib.h>

// Librerias propias
#include "I2C_LCD.h"    // Libreria I2C_LCD


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

//-----------------------------------------------------------------------------
//                            Variables 
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//                            Prototipos 
//-----------------------------------------------------------------------------

void setup(void);

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
    
    I2C_Master_Init();  
    LCD_Init(0x4E);    // Initialize LCD module with I2C address = 0x4E
 
    LCD_Set_Cursor(1, 1);
    LCD_Write_String("  Monedanaitor");
    LCD_Set_Cursor(2, 1);  
    LCD_Write_String("   Digital 2");
    __delay_ms(2500);
    
    while(1)    // Equivale al loop
    {
    LCD_SR();
    __delay_ms(350);
    LCD_SR();
    __delay_ms(350);
    LCD_SL();
    __delay_ms(350);
    LCD_SL();
    __delay_ms(350);
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


