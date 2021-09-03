/*
 * File:   ESCLAVO1.c
 * Author: duart
 *
 * Created on 24 de agosto de 2021, 12:30 AM
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

//#pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

//*****************************************************************************
// Definición e importación de librerías
//*****************************************************************************
#include <stdint.h>
#include <pic16f887.h>
#include "I2C.h"
#include <xc.h>
//*****************************************************************************
// Definición de variables
//*****************************************************************************
#define _XTAL_FREQ 8000000
uint8_t C, infra, quetzal;
uint8_t CONT;//variable para guardar el valor de adresh
char counter;
//*****************************************************************************
// Definición de funciones para que se puedan colocar después del main de lo 
// contrario hay que colocarlos todas las funciones antes del main
//*****************************************************************************
void setup(void);
void infrared(void);
void servo_1_1(void);
void servo_1_2(void);
void servo_1_3(void);
void servo_1_4(void);
void servo_1_5(void);

//*****************************************************************************
// Código de Interrupción 
//*****************************************************************************
void __interrupt() isr(void){
   if(PIR1bits.SSPIF == 1){ 

        SSPCONbits.CKP = 0;
       
        if ((SSPCONbits.SSPOV) || (SSPCONbits.WCOL)){
            C = SSPBUF;                 // Read the previous value to clear the buffer
            SSPCONbits.SSPOV = 0;       // Clear the overflow flag
            SSPCONbits.WCOL = 0;        // Clear the collision bit
            SSPCONbits.CKP = 1;         // Enables SCL (Clock)
        }

        if(!SSPSTATbits.D_nA && !SSPSTATbits.R_nW) {
            __delay_us(7);
            C = SSPBUF;                 // Lectura del SSBUF para limpiar el buffer y la bandera BF
            __delay_us(2);
            PIR1bits.SSPIF = 0;         // Limpia bandera de interrupción recepción/transmisión SSP
            SSPCONbits.CKP = 1;         // Habilita entrada de pulsos de reloj SCL
            while(!SSPSTATbits.BF);     // Esperar a que la recepción se complete
            CONT = SSPBUF;             // Guardar en el PORTD el valor del buffer de recepción
            __delay_us(250);
            
        }else if(!SSPSTATbits.D_nA && SSPSTATbits.R_nW){
            C = SSPBUF;
            BF = 0;
            SSPBUF = CONT;
            __delay_ms(250);
            SSPBUF = counter;
            SSPCONbits.CKP = 1;
            __delay_us(250);
            while(SSPSTATbits.BF);
        }
       
        PIR1bits.SSPIF = 0;    
    }
   if(RBIF){
        if(RB1 == 0){
            servo_1_5();
            __delay_ms(1000);
            servo_1_1();
            quetzal++;
        }
       
        RBIF = 0;
    }
    
   }

//*****************************************************************************
// Main
//*****************************************************************************
void main(void) {
    setup();
    //*************************************************************************
    // Loop infinito
    //*************************************************************************
    while(1){
//         PORTD = CONT;
//        __delay_ms(750);
               
        infrared();
        
    }
    return;
}
//*****************************************************************************
// Funciones
//*****************************************************************************
// Funcion para el contador del Infrarojo
void infrared(void){
    if(RA0 == 0){
        RB7 = 1;
        counter++;
        __delay_ms(500);
        RB7 = 0;
    }
    else{
        RB7 = 0;        
    }
}

void servo_1_1(void){           //rango de posicion 1 para el servo1
    RE0 = 1;
    __delay_ms(0.7);            //siempre suman 20ms el periodo del servo
    RE0 = 0;
    __delay_ms(19.3);
}

void servo_1_2(void){           //rango de posicion 2 para el servo1
    RE0 = 1;
    __delay_ms(1.25);
    RE0 = 0;
    __delay_ms(18.75);
}

void servo_1_3(void){           //rango de posicion 3 para el servo1
    RE0 = 1;
    __delay_ms(1.5);
    RE0 = 0;
    __delay_ms(18.5);
}

void servo_1_4(void){           //rango de posicion 4 para el servo1
    RE0 = 1;
    __delay_ms(1.75);
    RE0 = 0;
    __delay_ms(18.25);
}

void servo_1_5(void){           //rango de posicion 5 para el servo1
    RE0 = 1;
    __delay_ms(2);
    RE0 = 0;
    __delay_ms(18);
}

void setup(void){
    ANSEL = 0;
    ANSELH = 0;
    
    // Puerto A
    TRISA0 = 1;
   
    // Puerto B
    TRISB0 = 1;
    TRISBbits.TRISB7 = 0;
    
    TRISD = 0X00;
    
//    configurar interrupciones
    INTCONbits.GIE = 1;     //habilita las interrupciones globales
    INTCONbits.PEIE = 1;    //periferical interrupts
    
    //CONFIGURACION PARA SERVOMOTOR
    TRISB1 = 1; //Boton para paso de servomotor
    TRISE0 = 0; //pin del servomotor
    OPTION_REGbits.nRBPU = 0; //internal pull-ups are enabled
    WPUB = 0b00000010;
    IOCBbits.IOCB1 = 1;     //Boton de inc
    

    
    // configuracion del oscilador 
    OSCCONbits.IRCF2 = 1;
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF0 = 1; //Se configura el oscilador a una frecuencia de 8MHz
    OSCCONbits.SCS = 1; 
    
    // configuracion de interrupciones
    INTCONbits.GIE = 1;
    
    INTCONbits.RBIE = 1;
    INTCONbits.RBIF = 0;    //limpiar bandera de interrupcion
    INTCONbits.PEIE = 1;    //periferical interrupts
    
    PORTA = 0x00;
    PORTC = 0x00;
    PORTB = 0X00;
    PORTD = 0x00;
    PORTE = 0x00;
    
    // I2C configuracion esclavo
    I2C_Slave_Init(0x50);   
}