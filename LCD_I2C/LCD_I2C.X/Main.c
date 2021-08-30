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
char counter;
float conv0 = 0;
char converted;
char converted02[10];
//-----------------------------------------------------------------------------
//                            Prototipos 
//-----------------------------------------------------------------------------

void setup(void);
void infrared(void);
void ADC_convert(char *data,float a, int place);

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
    LCD_Write_String("     Monedanaitor");
    LCD_Set_Cursor(2, 1);  
    LCD_Write_String("   Monedas = Q0.00");
    LCD_Set_Cursor(3, 1);  
    LCD_Write_String(" 1.00  0.5   0.25");
    LCD_Set_Cursor(4, 5); 
    LCD_Write_String("  Quetzales");
    __delay_ms(2500);
    
    while(1)    // Equivale al loop
    {
    LCD_Set_Cursor(4, 1);  
    LCD_Write_String(converted02);
    infrared();
    
    ADC_convert(converted02, counter, 2);
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
    TRISA0 = 1;
   
    // Puerto B
    TRISB0 = 1;
    TRISBbits.TRISB7 = 0;
    
    // Puerto C

    // Puerto D
    TRISD = 0;
        
    // Puerto E
  
    
    
    //Configurar reloj interno
    OSCCONbits.IRCF0 = 1;        //reloj interno de 8mhz
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF2 = 1;
    OSCCONbits.SCS = 1;  //internal oscillator is used for system clock
    
    //limpiar puertos
    PORTA = 0x00;
    PORTB = 0x00;
    PORTC = 0x00;
    PORTD = 0x00;
    PORTE = 0x00;
}

void infrared(void){
    if(RA0 == 1){
        RB7 = 1;
        counter = counter +1;
        __delay_ms(500);
        RB7 = 0;
    }
    else{
        RB7 = 0;
        
    }
}

/*******************************************************************************
 * Funciones para conversión del ADC 
 ******************************************************************************/
void ADC_convert(char *data,float a, int place) 
{
     int temp=a;
     float x=0.0;
     int digits=0;
     int i=0,mu=1;
     int j=0;
     if(a<0)
     {
            a=a*-1;
            data[i]='-';
            i++;
      }
     //exponent component
     while(temp!=0)
     {
         temp=temp/10;
         digits++;          
     }
     while(digits!=0)
     {
         if(digits==1)mu=1;
         else  for(j=2;j<=digits;j++)mu=mu*10;
         
         x=a/mu;
         a=a-((int)x*mu);
         data[i]=0x30+((int)x);
         i++;
         digits--;
         mu=1;
     }
     //mantissa component
     data[i]='.';
     i++;
     digits=0;
     for(j=1;j<=place;j++)mu=mu*10;
     x=(a-(int)a)*mu; //shift places
     a=x;
     temp=a;
     x=0.0;
     mu=1;
     digits=place;
     while(digits!=0)
     {
         if(digits==1)mu=1;
         else  for(j=2;j<=digits;j++)mu=mu*10;
         
         x=a/mu;
         a=a-((int)x*mu);
         data[i]=0x30+((int)x);
         i++;
         digits--;
         mu=1;
     }   
     
    data[i]='\n';
}