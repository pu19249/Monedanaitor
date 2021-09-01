/*
 * File:   Master_Colector_Monedas.c
 * Author: YAMZARD
 *
 * Created on 8 de agosto de 2021, 01:18 AM
 */
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (RCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, RC on RA7/OSC1/CLKIN)
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

//*****************************************************************************
// Definición e importación de librerías
//*****************************************************************************
#include <xc.h>
#include <stdint.h>
#include <stdio.h>  // Para usar printf
#include <string.h> // Concatenar
#include <stdlib.h>

// Librerias propias
#include "I2C_LCD.h"    // Libreria I2C_LCD
//*****************************************************************************
// Definición de variables
//*****************************************************************************
#define _XTAL_FREQ 8000000


uint16_t temp;
uint8_t CONT;
uint8_t POT;
char unidades, decenas, centenas;

//*****************************************************************************
// Definición de funciones para que se puedan colocar después del main de lo 
// contrario hay que colocarlos todas las funciones antes del main
//*****************************************************************************
void setup(void);
//*****************************************************************************
// Main
//*****************************************************************************
void main(void) {
    setup();
    I2C_Master_Init();  
    LCD_Init(0x4E);    // Initialize LCD module with I2C address = 0x4E
    
    LCD_Set_Cursor(1, 1);
    LCD_Write_String("     Monedanaitor");
    LCD_Set_Cursor(2, 1);  
    LCD_Write_String("   Monedas = Q0.00");
    LCD_Set_Cursor(3, 1);  
    LCD_Write_String(" 1.00  0.5   0.25");
    while(1){
        
        I2C_Master_Start();
        I2C_Master_Write(0x51);
        POT = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(200);
        
        I2C_Master_Start();
        I2C_Master_Write(0x61);
        CONT = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(200);
        
        I2C_Master_Start();
        I2C_Master_Write(0x71);
        temp = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(200);
        
        PORTA = POT;
        PORTB = CONT;
        PORTD = temp;
        
    }
    return;
}
//*****************************************************************************
// Función de Inicialización
//*****************************************************************************
void setup(void){
    ANSEL = 0;
    ANSELH = 0;
    TRISB = 0;
    TRISA = 0;
    TRISD = 0;
    PORTA = 0;
    PORTB = 0;
    PORTD = 0;
    // configuracion del oscilador 
    OSCCONbits.IRCF2 = 1;
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF0 = 1; //Se configura el oscilador a una frecuencia de 8MHz
    OSCCONbits.SCS = 1;
    I2C_Master_Init(100000);        // Inicializar Comuncación I2C
}
