EESchema Schematic File Version 4
LIBS:EVNotipi-Watchdog_Trinket_M0-cache
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
L Connector:Conn_01x05_Female J1
U 1 1 5D62720F
P 1500 1000
F 0 "J1" H 1528 1026 50  0000 L CNN
F 1 "Conn_Left" H 1528 935 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x05_P2.54mm_Vertical" H 1500 1000 50  0001 C CNN
F 3 "~" H 1500 1000 50  0001 C CNN
	1    1500 1000
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x05_Female J2
U 1 1 5D6276A2
P 1500 1550
F 0 "J2" H 1528 1576 50  0000 L CNN
F 1 "Conn_Right" H 1528 1485 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x05_P2.54mm_Vertical" H 1500 1550 50  0001 C CNN
F 3 "~" H 1500 1550 50  0001 C CNN
	1    1500 1550
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x05_Male J3
U 1 1 5D628156
P 3000 1000
F 0 "J3" H 3108 1381 50  0000 C CNN
F 1 "Conn_Pi" H 3108 1290 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x05_P2.54mm_Vertical" H 3000 1000 50  0001 C CNN
F 3 "~" H 3000 1000 50  0001 C CNN
	1    3000 1000
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x02_Male J4
U 1 1 5D6292E1
P 3000 1500
F 0 "J4" H 3108 1681 50  0000 C CNN
F 1 "Conn_Pwr" H 3108 1590 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 3000 1500 50  0001 C CNN
F 3 "~" H 3000 1500 50  0001 C CNN
	1    3000 1500
	1    0    0    -1  
$EndComp
Text GLabel 1300 1100 0    50   Input ~ 0
Pi_Running
Text GLabel 1300 1350 0    50   Input ~ 0
VCC5V
Text GLabel 1300 1450 0    50   BiDi ~ 0
SDA
Text GLabel 1300 1650 0    50   BiDi ~ 0
SCL
Text GLabel 1300 1550 0    50   Input ~ 0
ADC
Text Label 1500 1450 0    50   ~ 0
PA02_AIN[0]
Text GLabel 3200 1500 2    50   UnSpc ~ 0
GND
Text GLabel 3200 1100 2    50   Input ~ 0
VCC_Pi
Text GLabel 3200 900  2    50   BiDi ~ 0
SDA
Text GLabel 3200 1000 2    50   BiDi ~ 0
SCL
Text GLabel 3200 800  2    50   Output ~ 0
Pi_Running
$Comp
L Regulator_Switching:TSR_1-2450 U1
U 1 1 5D62B8BF
P 1400 2600
F 0 "U1" H 1400 2967 50  0000 C CNN
F 1 "TSR_2-2450" H 1400 2876 50  0000 C CNN
F 2 "Converter_DCDC:Converter_DCDC_TRACO_TSR-1_THT" H 1400 2450 50  0001 L CIN
F 3 "http://www.tracopower.com/products/tsr1.pdf" H 1400 2600 50  0001 C CNN
	1    1400 2600
	1    0    0    -1  
$EndComp
Text GLabel 1800 2500 2    50   Output ~ 0
VCC5V
$Comp
L Transistor_FET:BS170 Q1
U 1 1 5D62DCCF
P 2800 2500
F 0 "Q1" H 3006 2546 50  0000 L CNN
F 1 "BS170" H 3006 2455 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-92_Inline" H 3000 2425 50  0001 L CIN
F 3 "http://www.fairchildsemi.com/ds/BS/BS170.pdf" H 2800 2500 50  0001 L CNN
	1    2800 2500
	1    0    0    -1  
$EndComp
$Comp
L Transistor_FET:IRF4905 Q2
U 1 1 5D63050F
P 3750 2500
F 0 "Q2" H 3956 2546 50  0000 L CNN
F 1 "IRF5305" H 3956 2455 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-220-3_Horizontal_TabDown" H 3950 2425 50  0001 L CIN
F 3 "http://www.infineon.com/dgdl/irf4905.pdf?fileId=5546d462533600a4015355e32165197c" H 3750 2500 50  0001 L CNN
	1    3750 2500
	1    0    0    -1  
$EndComp
$Comp
L Device:R R3
U 1 1 5D632A96
P 3500 2800
F 0 "R3" V 3707 2800 50  0000 C CNN
F 1 "R27K" V 3616 2800 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 3430 2800 50  0001 C CNN
F 3 "~" H 3500 2800 50  0001 C CNN
	1    3500 2800
	0    -1   -1   0   
$EndComp
Wire Wire Line
	3550 2500 3350 2500
Wire Wire Line
	3350 2800 3350 2500
Wire Wire Line
	3650 2800 3850 2800
Wire Wire Line
	3850 2800 3850 2700
Wire Wire Line
	3850 3000 3850 2800
Connection ~ 3850 2800
Wire Wire Line
	3850 2300 3850 2150
Text GLabel 3850 3000 0    50   Input ~ 0
VCC5V
Text GLabel 3850 2150 2    50   Output ~ 0
VCC_Pi
Text GLabel 1300 1000 0    50   Output ~ 0
5V_ENA
Text GLabel 2600 2500 0    50   Input ~ 0
5V_ENA
$Comp
L power:GND #PWR0101
U 1 1 5D63D76F
P 2900 2700
F 0 "#PWR0101" H 2900 2450 50  0001 C CNN
F 1 "GND" H 2905 2527 50  0000 C CNN
F 2 "" H 2900 2700 50  0001 C CNN
F 3 "" H 2900 2700 50  0001 C CNN
	1    2900 2700
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0102
U 1 1 5D63F97F
P 1400 2800
F 0 "#PWR0102" H 1400 2550 50  0001 C CNN
F 1 "GND" H 1405 2627 50  0000 C CNN
F 2 "" H 1400 2800 50  0001 C CNN
F 3 "" H 1400 2800 50  0001 C CNN
	1    1400 2800
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0103
U 1 1 5D640129
P 1300 900
F 0 "#PWR0103" H 1300 650 50  0001 C CNN
F 1 "GND" V 1305 772 50  0000 R CNN
F 2 "" H 1300 900 50  0001 C CNN
F 3 "" H 1300 900 50  0001 C CNN
	1    1300 900 
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR0104
U 1 1 5D6407C6
P 3200 1200
F 0 "#PWR0104" H 3200 950 50  0001 C CNN
F 1 "GND" V 3205 1072 50  0000 R CNN
F 2 "" H 3200 1200 50  0001 C CNN
F 3 "" H 3200 1200 50  0001 C CNN
	1    3200 1200
	0    -1   -1   0   
$EndComp
Wire Wire Line
	2900 2300 3350 2300
Wire Wire Line
	3350 2300 3350 2500
Connection ~ 3350 2500
$Comp
L power:+12V #PWR0105
U 1 1 5D654254
P 4300 800
F 0 "#PWR0105" H 4300 650 50  0001 C CNN
F 1 "+12V" V 4315 928 50  0000 L CNN
F 2 "" H 4300 800 50  0001 C CNN
F 3 "" H 4300 800 50  0001 C CNN
	1    4300 800 
	0    1    1    0   
$EndComp
Text GLabel 4300 800  0    50   UnSpc ~ 0
VCC12V
Text GLabel 3200 1600 2    50   Output ~ 0
VCC12V
Text GLabel 1000 2500 0    50   Input ~ 0
VCC12V
Text GLabel 1000 3500 0    50   Input ~ 0
VCC12V
$Comp
L power:GND #PWR0106
U 1 1 5D6649C0
P 1000 3800
F 0 "#PWR0106" H 1000 3550 50  0001 C CNN
F 1 "GND" V 1005 3672 50  0000 R CNN
F 2 "" H 1000 3800 50  0001 C CNN
F 3 "" H 1000 3800 50  0001 C CNN
	1    1000 3800
	0    1    1    0   
$EndComp
Text GLabel 1450 3650 2    50   Output ~ 0
ADC
$Comp
L Device:R R1
U 1 1 5D665205
P 1150 3500
F 0 "R1" V 1357 3500 50  0000 C CNN
F 1 "R100K" V 1266 3500 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 1080 3500 50  0001 C CNN
F 3 "~" H 1150 3500 50  0001 C CNN
	1    1150 3500
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R2
U 1 1 5D66562B
P 1150 3800
F 0 "R2" V 1357 3800 50  0000 C CNN
F 1 "R27K" V 1266 3800 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 1080 3800 50  0001 C CNN
F 3 "~" H 1150 3800 50  0001 C CNN
	1    1150 3800
	0    -1   -1   0   
$EndComp
Wire Wire Line
	1450 3650 1400 3650
Wire Wire Line
	1400 3650 1400 3500
Wire Wire Line
	1400 3500 1300 3500
Wire Wire Line
	1300 3800 1400 3800
Wire Wire Line
	1400 3800 1400 3650
Connection ~ 1400 3650
$Comp
L Connector:Conn_01x04_Male J5
U 1 1 5D6BB1FE
P 3950 1250
F 0 "J5" H 4058 1531 50  0000 C CNN
F 1 "Conn_01x04_Male" H 4058 1440 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical" H 3950 1250 50  0001 C CNN
F 3 "~" H 3950 1250 50  0001 C CNN
	1    3950 1250
	1    0    0    -1  
$EndComp
Text GLabel 4150 1150 2    50   BiDi ~ 0
SDA
Text GLabel 4150 1250 2    50   BiDi ~ 0
SCL
Text GLabel 4150 1350 2    50   Input ~ 0
VCC_Pi
$Comp
L power:GND #PWR0107
U 1 1 5D6BC7C0
P 4150 1450
F 0 "#PWR0107" H 4150 1200 50  0001 C CNN
F 1 "GND" V 4155 1322 50  0000 R CNN
F 2 "" H 4150 1450 50  0001 C CNN
F 3 "" H 4150 1450 50  0001 C CNN
	1    4150 1450
	0    -1   -1   0   
$EndComp
$EndSCHEMATC
