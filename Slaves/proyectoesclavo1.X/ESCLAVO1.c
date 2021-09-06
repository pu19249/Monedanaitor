/*
 * File:   ESCLAVO2.c
 * Author: duart
 *
 * Created on 24 de agosto de 2021, 12:00 AM
 */
/*
 * File: ESCLAVO1.c
 * Author: Monedanaitor
 * Hardware: 
 * Funcionamiento: 
 * Created on August 24, 2021
 */
/*=============================================================================
                        BITS DE CONFIGURACION
 =============================================================================*/


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

//***************************
//                            LIBRERIAS
//***************************
#include <stdint.h>
#include <pic16f887.h>
#include "I2C.h"
#include <xc.h>
//***************************
//                              VARIABLES
//***************************
#define _XTAL_FREQ 8000000
uint8_t C, infra, quetzal;
uint8_t CONT;//variable para guardar el valor de adresh
char M = 0;
//***************************
// Definición de funciones para que se puedan colocar después del main de lo 
// contrario hay que colocarlos todas las funciones antes del main
//***************************
void setup(void);
void infrared(void);
void servo_1_1(void);
void servo_1_2(void);
void servo_1_3(void);
void servo_1_4(void);
void servo_1_5(void);

//***************************
//                      CODIGO DE INTERRUPCION
//***************************
void __interrupt() isr(void){
    
    if(RBIF == 1){
        if(RB1 == 0){
            servo_1_5();
            servo_1_3();
            servo_1_2();
            __delay_ms(2000);
            servo_1_1();
            quetzal++;
        }
       
        RBIF = 0;
    }
    
//   if(PIR1bits.SSPIF == 1){ 
//
//        SSPCONbits.CKP = 0;
//       
//        if ((SSPCONbits.SSPOV) || (SSPCONbits.WCOL)){
//            C = SSPBUF;                 // Read the previous value to clear the buffer
//            SSPCONbits.SSPOV = 0;       // Clear the overflow flag
//            SSPCONbits.WCOL = 0;        // Clear the collision bit
//            SSPCONbits.CKP = 1;         // Enables SCL (Clock)
//        }
//
//        if(!SSPSTATbits.D_nA && !SSPSTATbits.R_nW) {
//            __delay_us(7);
//            C = SSPBUF;                 // Lectura del SSBUF para limpiar el buffer y la bandera BF
//            __delay_us(2);
//            PIR1bits.SSPIF = 0;         // Limpia bandera de interrupción recepción/transmisión SSP
//            SSPCONbits.CKP = 1;         // Habilita entrada de pulsos de reloj SCL
//            while(!SSPSTATbits.BF);     // Esperar a que la recepción se complete
//            CONT = SSPBUF;             // Guardar en el PORTD el valor del buffer de recepción
//            __delay_us(250);
//            
//        }else if(!SSPSTATbits.D_nA && SSPSTATbits.R_nW){
//            C = SSPBUF;
//            BF = 0;
//            SSPBUF = CONT;
//            __delay_ms(250);
//            SSPBUF = quetzal;
//            SSPCONbits.CKP = 1;
//            __delay_us(250);
//            while(SSPSTATbits.BF);
//        }
//       
//        PIR1bits.SSPIF = 0;    
//    }
//   
//   
//   if (PIR1bits.ADIF == 1)//Interrupcion del ADC 
//    {
//        if (ADCON0bits.CHS == 0)//si se esta en este canal que haga lo siguiente
//        {
//            M = ADRESH;
//            if(M<=169)
//            {
//                RB6 = 0; 
//                RB5 = 0;
//                RB4 = 1;  
//            }
//            if((M<=225)&&(M>=170))
//            {
//               RB6 = 0;
//               RB5 = 1;
//               RB4 = 0;
//            }
//            if(M>=226)
//            {
//               RB6 = 1;
//               RB5 = 0;
//               RB4 = 0;
//            }
//        }               
//        __delay_us(50);//tiempo necesario para el cambio de canal 
//        PIR1bits.ADIF = 0;//Se apaga el valor de la bandera de interrupcion ADC
//    }
  
 }

//***************************
//                                 MAIN
//***************************
void main(void) {
    setup();
    //*************************
    //                          LOOP INFINITO
    //*************************
    while(1){
        
//        infrared();
         __delay_us(100);
        ADCON0bits.GO = 1; //inicia la conversion otra vez
    }
    return;
}
//***************************
//                          FUNCIONES
//***************************
void infrared(void){
    if(RA1 == 0){
        RB7 = 1;
        __delay_ms(500);
        RB7 = 0;
        CONT++;
        }
    
    else if (RA1 == 0){
        RB7 = 0;
    }
}

void servo_1_1(void){           //rango de posicion 1 para el servo1
    RD0 = 1;
    __delay_ms(0.7);            //siempre suman 20ms el periodo del servo
    RD0 = 0;
    __delay_ms(19.3);
}

void servo_1_2(void){           //rango de posicion 2 para el servo1
    RD0 = 1;
    __delay_ms(1.25);
    RD0 = 0;
    __delay_ms(18.75);
}

void servo_1_3(void){           //rango de posicion 3 para el servo1
    RD0 = 1;
    __delay_ms(1.5);
    RD0 = 0;
    __delay_ms(18.5);
}

void servo_1_4(void){           //rango de posicion 4 para el servo1
    RD0 = 1;
    __delay_ms(1.75);
    RD0 = 0;
    __delay_ms(18.25);
}

void servo_1_5(void){           //rango de posicion 5 para el servo1
    RD0 = 1;
    __delay_ms(2);
    RD0 = 0;
    __delay_ms(18);
}

void setup(void){
    //ENTRADAS GENERALES PARA EL SLAVE
    TRISB1 = 1; //Boton para paso de servomotor
    TRISA1 = 1; //Entrada digital para el sensor infrarrojo
    TRISA0 = 1;
    ANSEL = 0b00000001; //RA0 tiene la entrada analogica del FRS
    ANSELH = 0;
    
    //SALIDAS GENERALES PARA EL SLAVE
    TRISBbits.TRISB7 = 0; //LED indicador que paso algo por el infrarrojo
    TRISBbits.TRISB6 = 0; //LED rojo para FSR
    TRISBbits.TRISB5 = 0; //LED amarillo para FSR
    TRISBbits.TRISB4 = 0; //LED verde para FSR
    TRISD0 = 0; //pin de salida del servomotor
    
    // Configuacion de las interrupciones
    INTCONbits.GIE = 1;
    INTCONbits.RBIF = 1;    // Para que el boton funcione a la primera
    INTCONbits.RBIE = 1;    // Interrupcion Puerto B
    INTCONbits.PEIE = 1;    // Periferical interrupt
    INTCONbits.T0IF = 0; 
    PIE1bits.ADIE = 1;      // Activar la interrupcion del ADC
    PIR1bits.ADIF = 0;      // Bandera del ADC
    
    //CONFIGURACION PARA SERVOMOTOR
    
    OPTION_REGbits.nRBPU = 0; //internal pull-ups are enabled
    WPUB = 0b00000011;
    IOCBbits.IOCB1 = 1;     //Boton de inc
    IOCBbits.IOCB0 = 1;
    
    //CONFIGURACION DEL OSCILADOR
    OSCCONbits.IRCF2 = 1;
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF0 = 1; //Se configura el oscilador a una frecuencia de 8MHz
    OSCCONbits.SCS = 1;
//    I2C_Slave_Init(0x60);  
    
    //CONFIGURACION DEL ADC
    ADCON0bits.CHS = 0; // CANAL AN0
    ADCON0bits.ADCS1 = 1;
    ADCON0bits.ADCS0 = 1; //Frc que trabaja con el oscilador interno
    ADCON0bits.ADON = 1; //Activa el modulo ADC
    ADCON1bits.ADFM = 0; // justificacion a la izquierda.
    ADCON1bits.VCFG0 = 0;
    ADCON1bits.VCFG1 = 0;  //Vss y Vcc
   
    PORTA = 0x00;
    PORTC = 0x00;
    PORTB = 0X00;
    PORTD = 0x00;
    PORTE = 0x00;
}
