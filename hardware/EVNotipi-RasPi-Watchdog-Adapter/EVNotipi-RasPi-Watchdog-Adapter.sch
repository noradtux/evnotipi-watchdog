EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Connector:Conn_01x05_Male J1
U 1 1 5DD8D84A
P 750 950
F 0 "J1" H 858 1331 50  0000 C CNN
F 1 "Conn_Pi" H 858 1240 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x05_P2.54mm_Horizontal" H 750 950 50  0001 C CNN
F 3 "~" H 750 950 50  0001 C CNN
	1    750  950 
	1    0    0    -1  
$EndComp
Text GLabel 950  750  2    50   Input ~ 0
Pi_Running
Text GLabel 950  850  2    50   BiDi ~ 0
SDA
Text GLabel 950  950  2    50   BiDi ~ 0
SCL
Text GLabel 950  1050 2    50   Output ~ 0
VCC_Pi
$Comp
L power:GND #PWR0101
U 1 1 5DD8E6A5
P 950 1150
F 0 "#PWR0101" H 950 900 50  0001 C CNN
F 1 "GND" H 955 977 50  0000 C CNN
F 2 "" H 950 1150 50  0001 C CNN
F 3 "" H 950 1150 50  0001 C CNN
	1    950  1150
	1    0    0    -1  
$EndComp
Text GLabel 2750 1050 1    50   Output ~ 0
VCC_Pi
Text GLabel 3650 1850 2    50   BiDi ~ 0
SCL
Text GLabel 3650 1750 2    50   BiDi ~ 0
SDA
Text GLabel 2050 2950 0    50   Output ~ 0
Pi_Running
$Comp
L Connector:Raspberry_Pi_2_3 J2
U 1 1 5DD8ED00
P 2850 2350
F 0 "J2" H 2850 4150 50  0000 C CNN
F 1 "Raspberry_Pi_2_3" H 2850 4050 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_2x20_P2.54mm_Vertical" H 2850 2350 50  0001 C CNN
F 3 "https://www.raspberrypi.org/documentation/hardware/raspberrypi/schematics/rpi_SCH_3bplus_1p0_reduced.pdf" H 2850 2350 50  0001 C CNN
	1    2850 2350
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0102
U 1 1 5DDA0553
P 2450 3650
F 0 "#PWR0102" H 2450 3400 50  0001 C CNN
F 1 "GND" H 2455 3477 50  0000 C CNN
F 2 "" H 2450 3650 50  0001 C CNN
F 3 "" H 2450 3650 50  0001 C CNN
	1    2450 3650
	1    0    0    -1  
$EndComp
$EndSCHEMATC
