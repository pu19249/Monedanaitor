 /*
 * File            : I2C.h
 * Author          : Ligo George
 * Company         : electroSome
 * Project         : I2C Library for MPLAB XC8
 * Microcontroller : PIC 16F877A
 * Created on April 15, 2017, 5:59 PM
 * Link: https://electrosome.com/i2c-pic-microcontroller-mplab-xc8/
 * Modificada por: Pablo Mazariegos con la ayuda del auxiliar Gustavo Ordo�ez 
 * Basado en Link: http://microcontroladores-mrelberni.com/i2c-pic-comunicacion-serial/
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef __I2C_H
#define	__I2C_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <pic16f887.h>
#include <stdint.h>

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 8000000
#endif

//*****************************************************************************
// Funci�n para inicializar I2C Maestro
//*****************************************************************************
void I2C_Master_Init(const unsigned long c);
//*****************************************************************************
// Funci�n de espera: mientras se est� iniciada una comunicaci�n,
// est� habilitado una recepci�n, est� habilitado una parada
// est� habilitado un reinicio de la comunicaci�n, est� iniciada
// una comunicaci�n o se este transmitiendo, el IC2 PIC se esperar�
// antes de realizar alg�n trabajo
//*****************************************************************************
void I2C_Master_Wait(void);
//*****************************************************************************
// Funci�n de inicio de la comunicaci�n I2C PIC
//*****************************************************************************
void I2C_Master_Start(void);
//*****************************************************************************
// Funci�n de reinicio de la comunicaci�n I2C PIC
//*****************************************************************************
void I2C_Master_RepeatedStart(void);
//*****************************************************************************
// Funci�n de parada de la comunicaci�n I2C PIC
//*****************************************************************************
void I2C_Master_Stop(void);
//*****************************************************************************
//Funci�n de transmisi�n de datos del maestro al esclavo
//esta funci�n devolver� un 0 si el esclavo a recibido
//el dato
//*****************************************************************************
void I2C_Master_Write(unsigned d);
//*****************************************************************************
//Funci�n de recepci�n de datos enviados por el esclavo al maestro
//esta funci�n es para leer los datos que est�n en el esclavo
//*****************************************************************************
unsigned short I2C_Master_Read(unsigned short a);
//*****************************************************************************
// Funci�n para inicializar I2C Esclavo
//*****************************************************************************
void I2C_Slave_Init(uint8_t address);
//*****************************************************************************
#endif	/* __I2C_H */


