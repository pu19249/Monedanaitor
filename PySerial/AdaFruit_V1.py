# AdafruitIO
# Universidad del Valle de Guatemala
# Digital 2
# PROYECTO 1 PIC16F887 MONEDANAITOR
# Jonathan Pu - Marco Duarte - Alejandro Duarte
# Adafruit IO
# https://io.adafruit.com/

# if Module not Found. Open Terminal/CMD and execute:
# pip3 install Adafruit_IO

from Adafruit_IO import Client, RequestError, Feed
import serial
import time
import sys

centenas = 0
decenas = 0
unidades = 0
enviar = 0
centena1 = 0
decena1 = 0
unidad1 = 0

ADAFRUIT_IO_KEY = "aio_pzao58yfl14l3Ke8pmeZuwrJnlWy"
ADAFRUIT_IO_USERNAME = "JonnyPu"
aio = Client(ADAFRUIT_IO_USERNAME, ADAFRUIT_IO_KEY)



#PRUEBAS PARA MANDAR DATOS Y QUE SE RECIBAN EN ADAFRUIT
# var1 = 30
# aio.send_data(un_quet.key, var1)
# var2 = 25
# aio.send_data(cincuenta_cen.key, var2)
# var3 = 35
# aio.send_data(veinte_cen.key, var3)

#INICIALIZAR COMUNICACION SERIAL
#ser = serial.Serial()
#ser.baudrate = 9600
#ser.timeout = 3
ser = serial.Serial('COM2', baudrate = 9600)
print("Abriendo puerto...")
#ser.port = 'COM2'
print("Se ha abierto el puerto COM2")



#Esto para leer los contadores del pic

while 1:
    ser.close()
    ser.open()

#ser.write(b'b')
    total = ser.readline(3).decode("ascii", "ignore")
    slave01 = ser.readline(3).decode("ascii", "ignore")
#unidades = ser.readline(1).decode("ascii", "ignore")
#unidades = str(unidades)
#decenas = str(decenas)
#centenas = str(centenas)
#contador_botones = int(centenas+decenas+unidades) #se concatenan
    print(total)
    print(slave01)
    print('Sending...')

#Feeds qe se van a utilizar
    un_quet = aio.feeds('quetzal')
    cincuenta_cen = aio.feeds('cincuenta')
    veinte_cen = aio.feeds('veinte')
#cont = int(contador_botones, base=10)
    aio.send_data(un_quet.key, total)
    aio.send_data(cincuenta_cen.key, slave01)
#aio.send_data(veinte_cen.key, int(unidades))