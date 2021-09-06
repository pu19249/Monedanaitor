/*
 * File:   prueba_sensor.c
 * Author: duart
 *
 * Created on 25 de agosto de 2021, 11:47 PM
 */
#include <xc.h>
#include <stdint.h>
#include <stdio.h>//Libreria para usar la funcion SWAP

//------------------------------------------------------------------------------
//                         BITS DE CONFIGURACION
//------------------------------------------------------------------------------
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO 
                                   //oscillator: I/O function on RA6/OSC2/CLKOUT
                                   //pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR 
                                //pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code 
                                //protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code
                                //protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR enabled)
#pragma config IESO = OFF       // Internal External Switchover bit 
                                //(Internal/External Switchover mode is enabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit 
                                //(Fail-Safe Clock Monitor is enabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3/PGM 
                                //pin has PGM function, low voltage programming 
                                //enabled)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out 
                                //Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits 
                                //(Write protection off)

#define _XTAL_FREQ 4000000//Para usar la funcion de 'delay'


//------------------------------------------------------------------------------
//                                VARIABLES
//------------------------------------------------------------------------------
char M = 0;
//------------------------------------------------------------------------------
//                          PROTOTIPOS FUNCIONES 
//------------------------------------------------------------------------------
void setup(void); 

// Se establece el vector de interrupcion
void __interrupt() isr(void){
    
    if (PIR1bits.ADIF == 1)//Interrupcion del ADC 
    {
        if (ADCON0bits.CHS == 0)//si se esta en este canal que haga lo siguiente
        {
            M = ADRESH;
            if(M<=169)
            {
                PORTBbits.RB7 = 0; 
                PORTBbits.RB6 = 0;
                PORTBbits.RB5 = 1;  
            }
            if((M<=225)&&(M>=170))
            {
               PORTBbits.RB7 = 0;
               PORTBbits.RB6 = 1;
               PORTBbits.RB5 = 0;
            }
            if(M>=226)
            {
               PORTBbits.RB7 = 1;
               PORTBbits.RB6 = 0;
               PORTBbits.RB5 = 0;
            }
        }               
        __delay_us(50);//tiempo necesario para el cambio de canal 
        PIR1bits.ADIF = 0;//Se apaga el valor de la bandera de interrupcion ADC
    }
    
}
//------------------------------------------------------------------------------
//                             CICLO PRINCIPAL 
//------------------------------------------------------------------------------
void main(void){
    
    setup();// Se llama a la funcion setup para configuracion de I/O
    while (1) // Se implemta el loop
    {
        ADCON0bits.GO = 1;//para que se haga la conversion ADC cada vez
    }              
}
//------------------------------------------------------------------------------
//                             CONFIGURACIONES
//------------------------------------------------------------------------------
void setup(void){
    // configuracion de puertos 
    ANSEL = 0b00000001; //setea AN0
    ANSELH = 0X00;//se establecen los pines como entras y salidas digitales
    
    TRISA = 0xff; // Se pone todo el puerto A como entrada.
//    TRISD = 0X00;  
    TRISB = 0X00;
    
    PORTA = 0X00;
    PORTB = 0X00;
//    PORTD = 0X00;//Se limpian los puertos utilizados
    
    // configuracion del oscilador 
    OSCCONbits.IRCF2 = 0;
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF0 = 1; //Se configura el oscilador a una frecuencia de 500kHz
    OSCCONbits.SCS = 1;
    
    // configuracion del ADC
    ADCON0bits.CHS = 0; // CANAL AN0
    
    ADCON0bits.ADCS1 = 1;
    ADCON0bits.ADCS0 = 1; //Frc que trabaja con el oscilador interno
    
    ADCON0bits.ADON = 1; //Activa el modulo ADC
    
    ADCON1bits.ADFM = 0; // justificacion a la izquierda.
    ADCON1bits.VCFG0 = 0;
    ADCON1bits.VCFG1 = 0;  //Vss y Vcc
    
    // configuracion del timer 0 y pull-up internos
//    OPTION_REGbits.nRBPU = 0;
//    WPUB = 0b00000010;
//    IOCBbits.IOCB1 = 1;
    
    // configuracion de interrupciones 
    INTCONbits.GIE = 1;
    PIR1bits.ADIF = 0; // BANDERA de interrupcion del ADC
    PIE1bits.ADIE = 1; // Habilita la interrupcion del ADC
    INTCONbits.PEIE = 1; // Interrupcion de los perifericos
//    INTCONbits.RBIF = 0;
//    INTCONbits.RBIE = 1;
}
