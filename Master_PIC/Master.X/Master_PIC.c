/*
 * File:   Master_PIC.c
 * Author: Yamzard
 * Proyecto 1 - Digital 2
 *  Clasificador de Monedas
 *      Configuracion del PIC Maestro
 * Created on September 1, 2021, 12:20 AM
 */
#define _XTAL_FREQ 8000000
#include <xc.h>
#include <stdint.h>
#include <stdio.h>  // Para usar printf
#include <string.h> // Concatenar
#include <stdlib.h>

// Librerias propias
#include "I2C_Master.h"    // Libreria I2C_LCD


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
float conv1 = 0;
float conv2 = 0;
char converted01[10];
char converted02[10];
char converted03[10];
float converted04[10];
uint16_t temp;
uint8_t CONT;
uint8_t POT;
char valor, hundreds, residuo, tens, units;
char slave01, slave02, slave03, sum, dec1, dec2, dec3;

//-----------------------------------------------------------------------------
//                            Prototipos 
//-----------------------------------------------------------------------------

void setup(void);
void infrared(void);
void ADC_convert(char *data,float a, int place);
void I2C_Comunication(void);
void Text(void);
char division (char valor);
void LCD_Start(void);
void LCD_Send(void);

//-----------------------------------------------------------------------------
//                            Interrupciones
//-----------------------------------------------------------------------------
void __interrupt() isr(void)
{
    // Interrupcion ADC
    if (ADIF == 1){
        if (ADCON0bits.CHS == 0){
            CCPR1L = (ADRESH>>1)+124;  //para que tome el rango desde el centro
            CCP1CONbits.DC1B1 = ADRESH & 0b01; //toma uno de los b que falta
            CCP1CONbits.DC1B0 = ADRESL>>7; //para el otro bit

            }

        
            ADIF = 0;           //apaga la bandera
    }
}

//-----------------------------------------------------------------------------
//                            Main
//-----------------------------------------------------------------------------
void main(void) {
    
    setup();    // Llamo a mi configuracion
    LCD_Start();    // Se inicializa la LCD
    
    while(1)    // Equivale al loop
    {

        // Comunicacion con la LCD
        LCD_Send();
        
        // Comunicacion con los esclavos
        I2C_Comunication();
        
        // Reinicio de lector ADC
         __delay_us(100);
        ADCON0bits.GO = 1; //inicia la conversion otra vez

    }
    return;
}

//-----------------------------------------------------------------------------
//                            Funciones
//-----------------------------------------------------------------------------

// Donde se realizan todas las configuraciones de nuestro PIC
void setup(void){
    
    ANSEL = 0b00000001; //RA0 tiene la entrada analogica 
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
    
    //configurar el modulo ADC
    ADCON0bits.CHS = 0;     //canal 0
    __delay_us(100);
    
    ADCON0bits.ADCS0 = 0;   //para que el clock select sea FOSC/32
    ADCON0bits.ADCS1 = 1;   //que tiene el osc int hasta 500kHz maximo
    ADCON0bits.ADON = 1;    //ADC enable bit
    ADCON1bits.ADFM = 0;    //left justified
    ADCON1bits.VCFG1 = 0;   //5 voltios
    ADCON1bits.VCFG0 = 0;   //tierra
    
    //configuracion del PWM junto con el TMR2
    TRISCbits.TRISC2 = 1;   //habilitar momentaneamente el pin de salida
    TRISCbits.TRISC1 = 1;
    PR2 = 250;               //queremos que sea de 20ms por el servo
    CCP1CONbits.P1M = 0;    //modo PWM single output     
    CCP2CONbits.CCP2M = 0b1111; //para que sea PWM
    CCP1CONbits.CCP1M = 0b00001100; //PWM mode, P1A, P1C active-high
    
    
    CCPR1L = 0x0F;          //ciclo de trabajo
    CCP1CONbits.DC1B = 0;   //los bits menos significativos
    CCPR2L = 0x0F;
    CCP2CONbits.DC2B0 = 0; 
    CCP2CONbits.DC2B1 = 0;
    
    
    PIR1bits.TMR2IF = 0;    //limpiar la interrupcion del timer2
    T2CONbits.T2CKPS0 = 0;   //configurar el prescaler a 16
    T2CONbits.T2CKPS1 = 1;        
    T2CONbits.TMR2ON = 1;   //habilitar el tmr2on
    while (PIR1bits.TMR2IF == 0);
    PIR1bits.TMR2IF = 0;    //limpiar nuevamente
    TRISCbits.TRISC2 = 0;   //regresar el pin a salida
    TRISCbits.TRISC1 = 0;
    PIE1bits.ADIE = 1;      //enable de la int del ADC
    PIR1bits.ADIF = 0;      //limpiar la interrupcion del ADC
    
    //limpiar puertos
    PORTA = 0x00;
    PORTB = 0x00;
    PORTC = 0x00;
    PORTD = 0x00;
    PORTE = 0x00;
    
    // I2C configuracion Maestro
    I2C_Master_Init();        // Inicializar Comuncaci�n I2C
}

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

// Funcion para enviar por USART
void Text(void){
     __delay_ms(250); //Tiempos para el despliegue de los caracteres
     division(counter);
        
     if (RCREG == 'a'){              
     __delay_ms(50);
    if(TXIF == 1){
        TXREG = hundreds; 
    }
    __delay_ms(50);
    if(TXIF == 1){
        TXREG = tens; 
       }
    __delay_ms(50);
    if(TXIF == 1){
        TXREG = units; 
       }
    __delay_ms(50);
        
     }
}

// Se configura la comunicacion I2C
void I2C_Comunication(void){
    
    I2C_Master_Start();
    I2C_Master_Write(0x61); // Direccion del SLAVE 1
    slave01 = I2C_Read_Byte();
    I2C_Master_Stop();
    __delay_ms(200);
        
    I2C_Master_Start();
    I2C_Master_Write(0x71); // Direccion del SLAVE 2
    slave02 = I2C_Read_Byte();
    I2C_Master_Stop();
    __delay_ms(200);
       
    I2C_Master_Start();
    I2C_Master_Write(0x81); // Direccion del SLAVE 3
    slave03 = I2C_Read_Byte();
    I2C_Master_Stop();
    __delay_ms(200);
}

// Funcion para inicializar la LCD
void LCD_Start(void){
    I2C_Master_Init();  
    LCD_Init(0x4E);    // Initialize LCD module with I2C address = 0x4E
 
    LCD_Set_Cursor(1, 1);
    LCD_Write_String(" Q1.00:");
    LCD_Set_Cursor(2, 1);  
    LCD_Write_String(" Q0.50:");
    LCD_Set_Cursor(3, 1);  
    LCD_Write_String(" Q0.25:");
    LCD_Set_Cursor(4, 1);  
    LCD_Write_String(" Total:Q");
    __delay_ms(2500);
}

// Funcion para mandar sensores en tiempo real a la LCD
void LCD_Send(void){
    __delay_ms(2500);
    // Se despliega el sensor de las monedas de 1 quetzal
    LCD_Set_Cursor(1, 9);  
    LCD_Write_String(converted01);
    
    // Se despliega el sensor de las monedas de 0.5 quetzales
    LCD_Set_Cursor(2, 9);  
    LCD_Write_String(converted02);
    
    // Se despliega el sensor de las monedas de 0.25 quetzales
    LCD_Set_Cursor(3, 9);  
    LCD_Write_String(converted03);
    
    // Se despliega el total del dinero que se tiene
//    LCD_Set_Cursor(4, 9);  
//    LCD_Write_String(converted04);
    
    ADC_convert(converted01, slave01, 2);
    ADC_convert(converted02, slave02, 2);
    ADC_convert(converted03, slave03, 2);
    
    dec1 = slave02 * 50;
    dec2 = slave03 * 25;
    dec3 = slave01 * 100;
    sum = dec3 + dec1 + dec2;
    
    division(sum);
    LCD_Set_Cursor(4, 9);
    LCD_Write_Char(hundreds);
    LCD_Set_Cursor(4, 10);
    LCD_Write_Char(46);
    LCD_Set_Cursor(4, 11);
    LCD_Write_Char(tens);
    LCD_Set_Cursor(4, 12);
    LCD_Write_Char(units);
   
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

// Funcion para poder dividir el contador
char division (char valor){
    hundreds = valor/100;//esto me divide entre 100 y se queda con el entero
    residuo = valor%100; //el residuo de lo que estoy operando
    tens = residuo/10; 
    units = residuo%10; //se queda con las units de las tens
    // Se les suma 47 para que me den al valor requerido
    hundreds = hundreds + 48;
    tens = tens + 48;
    units = units + 48;
} 

// Funcion para poder concatenar
int concat(int a, int b)
{
 
    char s1[20];
    char s2[20];

 
    // Convert both the integers to string
    sprintf(s1, "%d", a);
    sprintf(s2, "%d", b);

 
    // Concatenate both strings
    strcat(s1, s2);
 
    // Convert the concatenated string
    // to integer
    int c = atoi(s1);
 
    // return the formed integer
    return c;
}

// Funcion para que funcione el USART
void putch(char data){      // Funcion especifica de libreria stdio.h
    while(TXIF == 0);
    TXREG = data; // lo que se muestra de la cadena
    return;
}   