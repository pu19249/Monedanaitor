/*
 * File            : I2C.c
 * Author          : Ligo George
 * Company         : electroSome
 * Project         : I2C Library for MPLAB XC8
 * Microcontroller : PIC 16F877A
 * Created on April 15, 2017, 5:59 PM
 * Link: https://electrosome.com/i2c-pic-microcontroller-mplab-xc8/
 * Modificada por: Pablo Mazariegos con la ayuda del auxiliar Gustavo Ordo�ez 
 * Basado en Link: http://microcontroladores-mrelberni.com/i2c-pic-comunicacion-serial/
 */
#include "I2C.h"
//*****************************************************************************
// Funci�n para inicializar I2C Maestro
//*****************************************************************************
void I2C_Master_Init(const unsigned long c)
{
    SSPCON = 0b00101000;
    SSPCON2 = 0;
    SSPADD = (_XTAL_FREQ/(4*c))-1;
    SSPSTAT = 0;
    TRISCbits.TRISC3 = 1;
    TRISCbits.TRISC4 = 1;
}
//*****************************************************************************
// Funci�n de espera: mientras se est� iniciada una comunicaci�n,
// est� habilitado una recepci�n, est� habilitado una parada
// est� habilitado un reinicio de la comunicaci�n, est� iniciada
// una comunicaci�n o se este transmitiendo, el IC2 PIC se esperar�
// antes de realizar alg�n trabajo
//*****************************************************************************
void I2C_Master_Wait()
{
    while ((SSPSTAT & 0x04) || (SSPCON2 & 0x1F));
}
//*****************************************************************************
// Funci�n de inicio de la comunicaci�n I2C PIC
//*****************************************************************************
void I2C_Master_Start()
{
    I2C_Master_Wait();      //espera que se cumplan las condiciones adecuadas
    SSPCON2bits.SEN = 1;    //inicia la comunicaci�n i2c
}
//*****************************************************************************
// Funci�n de reinicio de la comunicaci�n I2C PIC
//*****************************************************************************
void I2C_Master_RepeatedStart()
{
    I2C_Master_Wait();      //espera que se cumplan las condiciones adecuadas
    SSPCON2bits.RSEN = 1;   //reinicia la comunicaci�n i2c
}
//*****************************************************************************
// Funci�n de parada de la comunicaci�n I2C PIC
//*****************************************************************************
void I2C_Master_Stop()
{
    I2C_Master_Wait();      //espera que se cumplan las condiciones adecuadas
    SSPCON2bits.PEN = 1;    //detener la comunicaci�n i2c
}
//*****************************************************************************
//Funci�n de transmisi�n de datos del maestro al esclavo
//esta funci�n devolver� un 0 si el esclavo a recibido
//el dato
//*****************************************************************************
void I2C_Master_Write(unsigned d)
{
    I2C_Master_Wait();      //espera que se cumplan las condiciones adecuadas
    SSPBUF = d;             
}
//*****************************************************************************
//Funci�n de recepci�n de datos enviados por el esclavo al maestro
//esta funci�n es para leer los datos que est�n en el esclavo
//*****************************************************************************
unsigned short I2C_Master_Read(unsigned short a)
{
    unsigned short temp;
    I2C_Master_Wait();      //espera que se cumplan las condiciones adecuadas
    SSPCON2bits.RCEN = 1;
    I2C_Master_Wait();      //espera que se cumplan las condiciones adecuadas
    temp = SSPBUF;
    I2C_Master_Wait();      //espera que se cumplan las condiciones adecuadas
    if(a == 1){
        SSPCON2bits.ACKDT = 0;
    }else{
        SSPCON2bits.ACKDT = 1;
    }
    SSPCON2bits.ACKEN = 1;          // Iniciar sequencia de Acknowledge
    return temp;                    // Regresar valor del dato le�do
}
//*****************************************************************************
// Funci�n para inicializar I2C Esclavo
//*****************************************************************************
void I2C_Slave_Init(uint8_t address)
{ 
    SSPADD = address;
    SSPCON = 0x36;      // 0b00110110
    SSPSTAT = 0x80;     // 0b10000000
    SSPCON2 = 0x01;     // 0b00000001
    TRISC3 = 1;
    TRISC4 = 1;
    GIE = 1;
    PEIE = 1;
    SSPIF = 0;
    SSPIE = 1;
}
//*****************************************************************************

