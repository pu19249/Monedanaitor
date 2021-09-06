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

ADAFRUIT_IO_KEY = "aio_YYNg788WqlEZn1oVwDydDFWSdzSd"
ADAFRUIT_IO_USERNAME = "JonnyPu"
aio = Client(ADAFRUIT_IO_USERNAME, ADAFRUIT_IO_KEY)

#Feeds qe se van a utilizar
un_quet = aio.feeds('quetzal')
cincuenta_cen = aio.feeds('cincuenta')
veinte_cen = aio.feeds('veinte')
#PRUEBAS PARA MANDAR DATOS Y QUE SE RECIBAN EN ADAFRUIT
# var1 = 30
# aio.send_data(un_quet.key, var1)
# var2 = 25
# aio.send_data(cincuenta_cen.key, var2)
# var3 = 35
# aio.send_data(veinte_cen.key, var3)

#INICIALIZAR COMUNICACION SERIAL
ser = serial.Serial()
ser.baudrate = 9600
ser.timeout = 3
print("Abriendo puerto...")
ser.port = 'COM2'
print("Se ha abierto el puerto COM2")

ser.open()

#Esto para leer los contadores del pic




ser.write(b'b')
centenas = ser.readline(1).decode("ascii", "ignore")
print(type(centenas))
decenas = ser.readline(1).decode("ascii", "ignore")
print(type(decenas))
unidades = ser.readline(1).decode("ascii", "ignore")
print(type(unidades))
contador_botones = centenas+decenas+unidades #se concatenan
print(type(contador_botones))
cont = int(contador_botones)
aio.send_data(un_quet.key, cont)
# quetzales = ser.readline(1).decode("ascii", "ignore")
# cin_cen = ser.readline(1).decode("ascii", "ignore")
# vein_cen = ser.readline(1).decode("ascii", "ignore")
# time.sleep(0.01)
# aio.send_data(un_quet.key, quetzales)
# aio.send_data(cincuenta_cen.key, cin_cen)
# aio.send_data(veinte_cen.key, vein_cen)
# time.sleep(0.01)
    
    
#contador_botones = int(centenas+decenas+unidades) #se concatenan
# aio.send_data(contador.key, contador_botones)

# mandar_0 = aio.receive(mandar_value.key)
# mandar_1 = mandar_0.value

# centena1 = mandar_1[0:1].encode("ascii")
# decena1 = mandar_1[1:2].encode("ascii")
# unidad1 = mandar_1[2:3].encode("ascii")

# #Ahora ya solo quiero mandar los valores para que mi pic los reciba
# ser.write(centena1)
# time.sleep(0.01)
# ser.write(decena1)
# time.sleep(0.01)
# ser.write(unidad1)