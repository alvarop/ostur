EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:ostur
LIBS:sht31_breakout-cache
EELAYER 25 0
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
L R R2
U 1 1 582130E8
P 1500 1750
F 0 "R2" V 1580 1750 50  0000 C CNN
F 1 "DNP" V 1500 1750 50  0000 C CNN
F 2 "Capacitors_SMD:C_0603" V 1430 1750 50  0001 C CNN
F 3 "" H 1500 1750 50  0000 C CNN
	1    1500 1750
	1    0    0    -1  
$EndComp
$Comp
L R R3
U 1 1 5821318D
P 1500 2250
F 0 "R3" V 1580 2250 50  0000 C CNN
F 1 "10k" V 1500 2250 50  0000 C CNN
F 2 "Capacitors_SMD:C_0603" V 1430 2250 50  0001 C CNN
F 3 "" H 1500 2250 50  0000 C CNN
	1    1500 2250
	1    0    0    -1  
$EndComp
$Comp
L SHT3x-DIS U1
U 1 1 58213308
P 3800 2050
F 0 "U1" H 3800 1550 60  0000 C CNN
F 1 "SHT3x-DIS" H 3800 2550 60  0000 C CNN
F 2 "footprints:SHT3x" H 3550 2050 60  0001 C CNN
F 3 "" H 3550 2050 60  0000 C CNN
	1    3800 2050
	1    0    0    -1  
$EndComp
Wire Wire Line
	3350 1700 2950 1700
Wire Wire Line
	3350 2000 2950 2000
Wire Wire Line
	3350 2100 2950 2100
Wire Wire Line
	2950 2400 3350 2400
Wire Wire Line
	3350 2300 3250 2300
Wire Wire Line
	3250 2300 3250 2400
Connection ~ 3250 2400
Text Label 2950 1700 0    60   ~ 0
SDA
Text Label 2950 2000 0    60   ~ 0
SCL
Text Label 2950 2100 0    60   ~ 0
VDD
Text Label 2950 2400 0    60   ~ 0
VSS
Wire Wire Line
	3350 1800 2950 1800
Text Label 2950 1800 0    60   ~ 0
ADDR
$Comp
L GND #PWR01
U 1 1 58213437
P 1500 2500
F 0 "#PWR01" H 1500 2250 50  0001 C CNN
F 1 "GND" H 1500 2350 50  0000 C CNN
F 2 "" H 1500 2500 50  0000 C CNN
F 3 "" H 1500 2500 50  0000 C CNN
	1    1500 2500
	1    0    0    -1  
$EndComp
Wire Wire Line
	1500 1900 1500 2100
Wire Wire Line
	1500 2400 1500 2500
Wire Wire Line
	1500 1500 1500 1600
$Comp
L VDD #PWR02
U 1 1 58213A04
P 1500 1500
F 0 "#PWR02" H 1500 1350 50  0001 C CNN
F 1 "VDD" H 1500 1650 50  0000 C CNN
F 2 "" H 1500 1500 50  0000 C CNN
F 3 "" H 1500 1500 50  0000 C CNN
	1    1500 1500
	1    0    0    -1  
$EndComp
Wire Wire Line
	1100 1550 1700 1550
Connection ~ 1500 1550
Wire Wire Line
	1100 2450 1700 2450
Connection ~ 1500 2450
Wire Wire Line
	1500 2000 1700 2000
Connection ~ 1500 2000
Text Label 1700 1550 0    60   ~ 0
VDD
Text Label 1700 2450 0    60   ~ 0
VSS
Text Label 1700 2000 0    60   ~ 0
ADDR
Wire Wire Line
	4200 2950 3800 2950
Wire Wire Line
	4200 3150 3800 3150
Wire Wire Line
	4200 3050 3800 3050
Wire Wire Line
	4200 2850 3800 2850
Text Label 3800 2950 0    60   ~ 0
VDD
Text Label 3800 3150 0    60   ~ 0
SCL
Text Label 3800 3050 0    60   ~ 0
SDA
Text Label 3800 2850 0    60   ~ 0
VSS
$Comp
L C C1
U 1 1 582143D4
P 1100 2000
F 0 "C1" H 1125 2100 50  0000 L CNN
F 1 "0.1u" H 1125 1900 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 1138 1850 50  0001 C CNN
F 3 "" H 1100 2000 50  0000 C CNN
	1    1100 2000
	1    0    0    -1  
$EndComp
Wire Wire Line
	1100 1850 1100 1550
Wire Wire Line
	1100 2150 1100 2450
$Comp
L CONN_01X04 J1
U 1 1 596A2ADA
P 4400 3000
F 0 "J1" H 4400 3250 50  0000 C CNN
F 1 "CONN_01X04" V 4500 3000 50  0000 C CNN
F 2 "footprints:smd_flex_4" H 4400 3000 50  0001 C CNN
F 3 "" H 4400 3000 50  0001 C CNN
	1    4400 3000
	1    0    0    -1  
$EndComp
NoConn ~ 3350 1900
Wire Wire Line
	3350 2200 3250 2200
Wire Wire Line
	3250 2200 3250 2100
Connection ~ 3250 2100
$EndSCHEMATC
