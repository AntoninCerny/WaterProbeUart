# WaterProbeUart
uart interface for water probe router
read uart on Raspberry Pi Zero 2W

turn off uart terminal and allow uart peripheral acording to 
https://www.raspberrypi.com/documentation/computers/configuration.html

sudo raspi-config
interface Options -> Serial Port -> disable serial console -> enable serial0 port 


modify boot/config.txt add: 

#enable uart
enable_uart=1

build with linked wiringPi library
gcc -o uart_main uart_main.c -l wiringPi

source:
https://www.ing.iac.es/~docs/external/serial/serial.pdf
