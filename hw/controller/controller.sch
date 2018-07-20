EESchema Schematic File Version 4
LIBS:controller-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "Ostur Controller"
Date "2018-07-19"
Rev "2.1"
Comp "alvarop.com"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text Label 3600 4950 1    60   ~ 0
nRST
$Comp
L power:GND #PWR01
U 1 1 58292F8E
P 7750 1500
F 0 "#PWR01" H 7750 1250 50  0001 C CNN
F 1 "GND" H 7750 1350 50  0000 C CNN
F 2 "" H 7750 1500 50  0000 C CNN
F 3 "" H 7750 1500 50  0000 C CNN
	1    7750 1500
	1    0    0    -1  
$EndComp
$Comp
L power:VDD #PWR02
U 1 1 58292FB1
P 7750 1100
F 0 "#PWR02" H 7750 950 50  0001 C CNN
F 1 "VDD" H 7750 1250 50  0000 C CNN
F 2 "" H 7750 1100 50  0000 C CNN
F 3 "" H 7750 1100 50  0000 C CNN
	1    7750 1100
	1    0    0    -1  
$EndComp
Text Label 1750 3700 0    60   ~ 0
SDA
Text Label 1750 3800 0    60   ~ 0
SCL
Text Label 1750 3500 0    60   ~ 0
UART1_RX
Text Label 1750 3400 0    60   ~ 0
UART1_TX
Text Label 5350 1650 0    60   ~ 0
SDA
Text Label 5350 1550 0    60   ~ 0
SCL
Text Label 1750 3900 0    60   ~ 0
TCA_nRST
$Comp
L power:VDD #PWR03
U 1 1 5829665A
P 6250 750
F 0 "#PWR03" H 6250 600 50  0001 C CNN
F 1 "VDD" H 6250 900 50  0000 C CNN
F 2 "" H 6250 750 50  0000 C CNN
F 3 "" H 6250 750 50  0000 C CNN
	1    6250 750 
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR04
U 1 1 5829669D
P 6250 2050
F 0 "#PWR04" H 6250 1800 50  0001 C CNN
F 1 "GND" H 6250 1900 50  0000 C CNN
F 2 "" H 6250 2050 50  0000 C CNN
F 3 "" H 6250 2050 50  0000 C CNN
	1    6250 2050
	1    0    0    -1  
$EndComp
Text Label 5050 1350 2    60   ~ 0
TCA_nRST
Text Label 5350 1150 0    60   ~ 0
TCA_A0
Text Label 5350 1250 0    60   ~ 0
TCA_A1
Text Label 5350 1450 0    60   ~ 0
TCA_A2
$Comp
L power:GND #PWR05
U 1 1 582968CB
P 5100 1150
F 0 "#PWR05" H 5100 900 50  0001 C CNN
F 1 "GND" H 5100 1000 50  0000 C CNN
F 2 "" H 5100 1150 50  0000 C CNN
F 3 "" H 5100 1150 50  0000 C CNN
	1    5100 1150
	0    1    1    0   
$EndComp
Text Label 7300 1350 2    60   ~ 0
SDA2
Text Label 7300 1250 2    60   ~ 0
SCL2
Text Label 7300 1150 2    60   ~ 0
SDA3
Text Label 7300 1050 2    60   ~ 0
SCL3
Text Label 1750 3600 0    60   ~ 0
FRIDGE
Text Label 3500 4950 1    60   ~ 0
LED0
Text Label 3400 4950 1    60   ~ 0
LED1
Text Label 3300 4950 1    60   ~ 0
LED2
$Comp
L controller-rescue:CONN_01X04 P14
U 1 1 58299458
P 6550 5750
F 0 "P14" H 6550 6000 50  0000 C CNN
F 1 "I2C0" V 6650 5750 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x04_Pitch2.54mm" H 6550 5750 50  0001 C CNN
F 3 "" H 6550 5750 50  0000 C CNN
	1    6550 5750
	1    0    0    -1  
$EndComp
Text Label 6050 5600 0    60   ~ 0
VDD
Text Label 6050 5700 0    60   ~ 0
SCL0
Text Label 6050 5800 0    60   ~ 0
SDA0
Text Label 6050 5900 0    60   ~ 0
GND
$Comp
L controller-rescue:CONN_01X04 P15
U 1 1 5829A537
P 6550 6300
F 0 "P15" H 6550 6550 50  0000 C CNN
F 1 "I2C1" V 6650 6300 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x04_Pitch2.54mm" H 6550 6300 50  0001 C CNN
F 3 "" H 6550 6300 50  0000 C CNN
	1    6550 6300
	1    0    0    -1  
$EndComp
Text Label 6050 6150 0    60   ~ 0
VDD
Text Label 6050 6250 0    60   ~ 0
SCL1
Text Label 6050 6350 0    60   ~ 0
SDA1
Text Label 6050 6450 0    60   ~ 0
GND
$Comp
L controller-rescue:CONN_01X04 P16
U 1 1 5829A599
P 6550 6850
F 0 "P16" H 6550 7100 50  0000 C CNN
F 1 "I2C2" V 6650 6850 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x04_Pitch2.54mm" H 6550 6850 50  0001 C CNN
F 3 "" H 6550 6850 50  0000 C CNN
	1    6550 6850
	1    0    0    -1  
$EndComp
Text Label 6050 6700 0    60   ~ 0
VDD
Text Label 6050 6800 0    60   ~ 0
SCL2
Text Label 6050 6900 0    60   ~ 0
SDA2
Text Label 6050 7000 0    60   ~ 0
GND
$Comp
L controller-rescue:CONN_01X04 P17
U 1 1 5829A601
P 6550 7400
F 0 "P17" H 6550 7650 50  0000 C CNN
F 1 "I2C3" V 6650 7400 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x04_Pitch2.54mm" H 6550 7400 50  0001 C CNN
F 3 "" H 6550 7400 50  0000 C CNN
	1    6550 7400
	1    0    0    -1  
$EndComp
Text Label 6050 7250 0    60   ~ 0
VDD
Text Label 6050 7350 0    60   ~ 0
SCL3
Text Label 6050 7450 0    60   ~ 0
SDA3
Text Label 6050 7550 0    60   ~ 0
GND
$Comp
L controller-rescue:LED-RESCUE-controller-RESCUE-controller D3
U 1 1 5829AECD
P 1650 7500
F 0 "D3" H 1650 7600 50  0000 C CNN
F 1 "LED2" H 1650 7400 50  0000 C CNN
F 2 "LEDs:LED_0603" H 1650 7500 50  0001 C CNN
F 3 "" H 1650 7500 50  0000 C CNN
F 4 "LG R971-KN-1" H 1650 7500 60  0001 C CNN "MPN"
	1    1650 7500
	-1   0    0    1   
$EndComp
$Comp
L controller-rescue:LED-RESCUE-controller-RESCUE-controller D2
U 1 1 5829AF4F
P 1650 7200
F 0 "D2" H 1650 7300 50  0000 C CNN
F 1 "LED1" H 1650 7100 50  0000 C CNN
F 2 "LEDs:LED_0603" H 1650 7200 50  0001 C CNN
F 3 "" H 1650 7200 50  0000 C CNN
F 4 "LG R971-KN-1" H 1650 7200 60  0001 C CNN "MPN"
	1    1650 7200
	-1   0    0    1   
$EndComp
$Comp
L controller-rescue:LED-RESCUE-controller-RESCUE-controller D1
U 1 1 5829AFC5
P 1650 6900
F 0 "D1" H 1650 7000 50  0000 C CNN
F 1 "LED0" H 1650 6800 50  0000 C CNN
F 2 "LEDs:LED_0603" H 1650 6900 50  0001 C CNN
F 3 "" H 1650 6900 50  0000 C CNN
F 4 "LG R971-KN-1" H 1650 6900 60  0001 C CNN "MPN"
	1    1650 6900
	-1   0    0    1   
$EndComp
$Comp
L controller-rescue:R R5
U 1 1 5829B032
P 1200 7500
F 0 "R5" V 1280 7500 50  0000 C CNN
F 1 "680" V 1200 7500 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 1130 7500 50  0001 C CNN
F 3 "" H 1200 7500 50  0000 C CNN
F 4 "RC0805JR-07680RL" V 1200 7500 60  0001 C CNN "MPN"
	1    1200 7500
	0    1    1    0   
$EndComp
$Comp
L controller-rescue:R R4
U 1 1 5829B12A
P 1200 7200
F 0 "R4" V 1280 7200 50  0000 C CNN
F 1 "680" V 1200 7200 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 1130 7200 50  0001 C CNN
F 3 "" H 1200 7200 50  0000 C CNN
F 4 "RC0805JR-07680RL" V 1200 7200 60  0001 C CNN "MPN"
	1    1200 7200
	0    1    1    0   
$EndComp
$Comp
L controller-rescue:R R3
U 1 1 5829B199
P 1200 6900
F 0 "R3" V 1280 6900 50  0000 C CNN
F 1 "680" V 1200 6900 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 1130 6900 50  0001 C CNN
F 3 "" H 1200 6900 50  0000 C CNN
F 4 "RC0805JR-07680RL" V 1200 6900 60  0001 C CNN "MPN"
	1    1200 6900
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR06
U 1 1 5829BC1C
P 2300 7250
F 0 "#PWR06" H 2300 7000 50  0001 C CNN
F 1 "GND" H 2300 7100 50  0000 C CNN
F 2 "" H 2300 7250 50  0000 C CNN
F 3 "" H 2300 7250 50  0000 C CNN
	1    2300 7250
	1    0    0    -1  
$EndComp
Text Label 800  6900 0    60   ~ 0
LED0
Text Label 800  7200 0    60   ~ 0
LED1
Text Label 800  7500 0    60   ~ 0
LED2
$Comp
L controller-rescue:CONN_01X02 P2
U 1 1 582A726E
P 3200 6450
F 0 "P2" H 3200 6600 50  0000 C CNN
F 1 "FRIDGE" V 3300 6450 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x02_Pitch2.54mm" H 3200 6450 50  0001 C CNN
F 3 "" H 3200 6450 50  0000 C CNN
	1    3200 6450
	1    0    0    -1  
$EndComp
Text Label 2650 6500 0    60   ~ 0
GND
Text Label 2650 6400 0    60   ~ 0
FRIDGE
$Comp
L power:GND #PWR07
U 1 1 582A906E
P 5450 6700
F 0 "#PWR07" H 5450 6450 50  0001 C CNN
F 1 "GND" H 5450 6550 50  0000 C CNN
F 2 "" H 5450 6700 50  0000 C CNN
F 3 "" H 5450 6700 50  0000 C CNN
	1    5450 6700
	0    -1   -1   0   
$EndComp
$Comp
L power:+5V #PWR08
U 1 1 582A97A5
P 3950 7450
F 0 "#PWR08" H 3950 7300 50  0001 C CNN
F 1 "+5V" H 3950 7590 50  0000 C CNN
F 2 "" H 3950 7450 50  0000 C CNN
F 3 "" H 3950 7450 50  0000 C CNN
	1    3950 7450
	-1   0    0    1   
$EndComp
Text Label 4450 7350 0    60   ~ 0
VDD_5
$Comp
L controller-rescue:C_Small C10
U 1 1 582AD781
P 3050 1050
F 0 "C10" H 3060 1120 50  0000 L CNN
F 1 "1uF" H 3060 970 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 3050 1050 50  0001 C CNN
F 3 "" H 3050 1050 50  0000 C CNN
F 4 "CL21F105ZAFNNNG" H 3050 1050 60  0001 C CNN "MPN"
	1    3050 1050
	1    0    0    -1  
$EndComp
$Comp
L controller-rescue:C_Small C11
U 1 1 582AD978
P 4350 1050
F 0 "C11" H 4360 1120 50  0000 L CNN
F 1 "1uF" H 4360 970 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 4350 1050 50  0001 C CNN
F 3 "" H 4350 1050 50  0000 C CNN
F 4 "CL21F105ZAFNNNG" H 4350 1050 60  0001 C CNN "MPN"
	1    4350 1050
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR09
U 1 1 582AE5E7
P 3700 1250
F 0 "#PWR09" H 3700 1000 50  0001 C CNN
F 1 "GND" H 3700 1100 50  0000 C CNN
F 2 "" H 3700 1250 50  0000 C CNN
F 3 "" H 3700 1250 50  0000 C CNN
	1    3700 1250
	1    0    0    -1  
$EndComp
$Comp
L power:VDD #PWR010
U 1 1 582AE68B
P 4350 700
F 0 "#PWR010" H 4350 550 50  0001 C CNN
F 1 "VDD" H 4350 850 50  0000 C CNN
F 2 "" H 4350 700 50  0000 C CNN
F 3 "" H 4350 700 50  0000 C CNN
	1    4350 700 
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR011
U 1 1 582AEA2C
P 3050 700
F 0 "#PWR011" H 3050 550 50  0001 C CNN
F 1 "+5V" H 3050 840 50  0000 C CNN
F 2 "" H 3050 700 50  0000 C CNN
F 3 "" H 3050 700 50  0000 C CNN
	1    3050 700 
	1    0    0    -1  
$EndComp
NoConn ~ 4950 6950
$Comp
L controller-rescue:R R11
U 1 1 582C3EF6
P 7100 3700
F 0 "R11" V 7180 3700 50  0000 C CNN
F 1 "10k" V 7100 3700 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 7030 3700 50  0001 C CNN
F 3 "" H 7100 3700 50  0000 C CNN
F 4 "311-10KARCT-ND" V 7100 3700 60  0001 C CNN "MPN"
	1    7100 3700
	0    -1   -1   0   
$EndComp
$Comp
L controller-rescue:R R12
U 1 1 582C458A
P 7100 3850
F 0 "R12" V 7180 3850 50  0000 C CNN
F 1 "10k" V 7100 3850 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 7030 3850 50  0001 C CNN
F 3 "" H 7100 3850 50  0000 C CNN
F 4 "311-10KARCT-ND" V 7100 3850 60  0001 C CNN "MPN"
	1    7100 3850
	0    -1   -1   0   
$EndComp
Text Label 7500 3700 2    60   ~ 0
SDA0
Text Label 7500 3850 2    60   ~ 0
SCL0
$Comp
L controller-rescue:R R9
U 1 1 582C5371
P 7100 3350
F 0 "R9" V 7180 3350 50  0000 C CNN
F 1 "10k" V 7100 3350 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 7030 3350 50  0001 C CNN
F 3 "" H 7100 3350 50  0000 C CNN
F 4 "311-10KARCT-ND" V 7100 3350 60  0001 C CNN "MPN"
	1    7100 3350
	0    -1   -1   0   
$EndComp
$Comp
L controller-rescue:R R10
U 1 1 582C5377
P 7100 3500
F 0 "R10" V 7180 3500 50  0000 C CNN
F 1 "10k" V 7100 3500 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 7030 3500 50  0001 C CNN
F 3 "" H 7100 3500 50  0000 C CNN
F 4 "311-10KARCT-ND" V 7100 3500 60  0001 C CNN "MPN"
	1    7100 3500
	0    -1   -1   0   
$EndComp
Text Label 7500 3350 2    60   ~ 0
SDA
Text Label 7500 3500 2    60   ~ 0
SCL
$Comp
L controller-rescue:R R13
U 1 1 582C53DB
P 7100 4050
F 0 "R13" V 7180 4050 50  0000 C CNN
F 1 "10k" V 7100 4050 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 7030 4050 50  0001 C CNN
F 3 "" H 7100 4050 50  0000 C CNN
F 4 "311-10KARCT-ND" V 7100 4050 60  0001 C CNN "MPN"
	1    7100 4050
	0    -1   -1   0   
$EndComp
$Comp
L controller-rescue:R R14
U 1 1 582C53E1
P 7100 4200
F 0 "R14" V 7180 4200 50  0000 C CNN
F 1 "10k" V 7100 4200 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 7030 4200 50  0001 C CNN
F 3 "" H 7100 4200 50  0000 C CNN
F 4 "311-10KARCT-ND" V 7100 4200 60  0001 C CNN "MPN"
	1    7100 4200
	0    -1   -1   0   
$EndComp
Text Label 7500 4050 2    60   ~ 0
SDA1
Text Label 7500 4200 2    60   ~ 0
SCL1
$Comp
L controller-rescue:R R21
U 1 1 582C628A
P 7100 4500
F 0 "R21" V 7180 4500 50  0000 C CNN
F 1 "10k" V 7100 4500 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 7030 4500 50  0001 C CNN
F 3 "" H 7100 4500 50  0000 C CNN
F 4 "311-10KARCT-ND" V 7100 4500 60  0001 C CNN "MPN"
	1    7100 4500
	0    -1   -1   0   
$EndComp
$Comp
L controller-rescue:R R22
U 1 1 582C6290
P 7100 4650
F 0 "R22" V 7180 4650 50  0000 C CNN
F 1 "10k" V 7100 4650 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 7030 4650 50  0001 C CNN
F 3 "" H 7100 4650 50  0000 C CNN
F 4 "311-10KARCT-ND" V 7100 4650 60  0001 C CNN "MPN"
	1    7100 4650
	0    -1   -1   0   
$EndComp
Text Label 7500 4500 2    60   ~ 0
SDA2
Text Label 7500 4650 2    60   ~ 0
SCL2
$Comp
L controller-rescue:R R23
U 1 1 582C629B
P 7100 4850
F 0 "R23" V 7180 4850 50  0000 C CNN
F 1 "10k" V 7100 4850 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 7030 4850 50  0001 C CNN
F 3 "" H 7100 4850 50  0000 C CNN
F 4 "311-10KARCT-ND" V 7100 4850 60  0001 C CNN "MPN"
	1    7100 4850
	0    -1   -1   0   
$EndComp
$Comp
L controller-rescue:R R24
U 1 1 582C62A1
P 7100 5000
F 0 "R24" V 7180 5000 50  0000 C CNN
F 1 "10k" V 7100 5000 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 7030 5000 50  0001 C CNN
F 3 "" H 7100 5000 50  0000 C CNN
F 4 "311-10KARCT-ND" V 7100 5000 60  0001 C CNN "MPN"
	1    7100 5000
	0    -1   -1   0   
$EndComp
Text Label 7500 4850 2    60   ~ 0
SDA3
Text Label 7500 5000 2    60   ~ 0
SCL3
$Comp
L power:VDD #PWR012
U 1 1 582C95F9
P 6750 3950
F 0 "#PWR012" H 6750 3800 50  0001 C CNN
F 1 "VDD" H 6750 4100 50  0000 C CNN
F 2 "" H 6750 3950 50  0000 C CNN
F 3 "" H 6750 3950 50  0000 C CNN
	1    6750 3950
	1    0    0    -1  
$EndComp
$Comp
L controller-rescue:C_Small C2
U 1 1 58765643
P 7900 1300
F 0 "C2" H 7910 1370 50  0000 L CNN
F 1 "10nF" H 7910 1220 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 7900 1300 50  0001 C CNN
F 3 "" H 7900 1300 50  0000 C CNN
F 4 "CL21B103KBANNNC" H 7900 1300 60  0001 C CNN "MPN"
	1    7900 1300
	1    0    0    -1  
$EndComp
$Comp
L ostur:tca9546a U3
U 1 1 5876DD07
P 5650 1000
F 0 "U3" H 5800 1250 60  0000 R CNN
F 1 "tca9546a" H 5800 1150 60  0000 R CNN
F 2 "Housings_SSOP:TSSOP-16_4.4x5mm_Pitch0.65mm" H 5650 1000 60  0001 C CNN
F 3 "" H 5650 1000 60  0000 C CNN
F 4 "TCA9546APWR" H 5650 1000 60  0001 C CNN "MPN"
	1    5650 1000
	1    0    0    -1  
$EndComp
Text Label 7300 1450 2    60   ~ 0
SCL1
Text Label 7300 1550 2    60   ~ 0
SDA1
Text Label 7300 1650 2    60   ~ 0
SCL0
Text Label 7300 1750 2    60   ~ 0
SDA0
NoConn ~ 2250 3300
NoConn ~ 3000 4450
$Comp
L power:PWR_FLAG #FLG013
U 1 1 58C44F8F
P 4850 7450
F 0 "#FLG013" H 4850 7525 50  0001 C CNN
F 1 "PWR_FLAG" H 4850 7600 50  0000 C CNN
F 2 "" H 4850 7450 50  0001 C CNN
F 3 "" H 4850 7450 50  0001 C CNN
	1    4850 7450
	-1   0    0    1   
$EndComp
$Comp
L controller-rescue:USB_OTG J2
U 1 1 58DB0B16
P 5250 7150
F 0 "J2" H 5050 7600 50  0000 L CNN
F 1 "USB_OTG" H 5050 7500 50  0000 L CNN
F 2 "footprints:USB_MICRO_B" H 5400 7100 50  0001 C CNN
F 3 "" H 5400 7100 50  0001 C CNN
F 4 "10118192-0001LF" H 5250 7150 60  0001 C CNN "MPN"
	1    5250 7150
	-1   0    0    1   
$EndComp
$Comp
L ostur:MIC550X U2
U 1 1 597CB58E
P 3700 1050
F 0 "U2" H 3800 1000 60  0000 C CNN
F 1 "MIC550X" H 3700 1450 60  0000 C CNN
F 2 "footprints:SOT-23(M5)" H 3700 1050 60  0001 C CNN
F 3 "" H 3700 1050 60  0000 C CNN
	1    3700 1050
	1    0    0    -1  
$EndComp
NoConn ~ 4100 900 
$Comp
L ostur:SHT3x-DIS U8
U 1 1 598E61C8
P 10450 1200
F 0 "U8" H 10450 700 60  0000 C CNN
F 1 "SHT3x-DIS" H 10450 1700 60  0000 C CNN
F 2 "footprints:SHT3x" H 10200 1200 60  0001 C CNN
F 3 "" H 10200 1200 60  0000 C CNN
	1    10450 1200
	1    0    0    -1  
$EndComp
$Comp
L controller-rescue:C_Small C17
U 1 1 598E61CF
P 8550 1150
F 0 "C17" H 8560 1220 50  0000 L CNN
F 1 "100nF" H 8550 1050 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 8550 1150 50  0001 C CNN
F 3 "" H 8550 1150 50  0000 C CNN
F 4 "CL21B104MBCNNNC" H 8550 1150 60  0001 C CNN "MPN"
	1    8550 1150
	1    0    0    -1  
$EndComp
$Comp
L controller-rescue:R R26
U 1 1 598E61D6
P 8800 1000
F 0 "R26" V 8880 1000 50  0000 C CNN
F 1 "10k" V 8800 1000 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 8730 1000 50  0001 C CNN
F 3 "" H 8800 1000 50  0000 C CNN
F 4 "311-10KARCT-ND" V 8800 1000 60  0001 C CNN "MPN"
	1    8800 1000
	-1   0    0    1   
$EndComp
Text Label 8900 1200 0    60   ~ 0
ADDR_LOCAL
Text Label 9850 1250 2    60   ~ 0
VDD
Text Label 9850 1150 2    60   ~ 0
SCL3
Text Label 9850 850  2    60   ~ 0
SDA3
Text Label 9850 1550 2    60   ~ 0
GND
Text Label 9850 950  2    60   ~ 0
ADDR_LOCAL
NoConn ~ 10000 1050
$Comp
L controller-rescue:C_Small C9
U 1 1 582977AF
P 7600 1300
F 0 "C9" H 7610 1370 50  0000 L CNN
F 1 "4.7uF" H 7610 1220 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 7600 1300 50  0001 C CNN
F 3 "" H 7600 1300 50  0000 C CNN
F 4 "CL21A475KQFNNNG" H 7600 1300 60  0001 C CNN "MPN"
	1    7600 1300
	1    0    0    -1  
$EndComp
$Comp
L alvarop:NRF52810-QCAA U1
U 1 1 5A650D0A
P 3250 3550
F 0 "U1" H 2500 4500 60  0000 R CNN
F 1 "NRF52810-QCAA" H 2500 4400 60  0000 R CNN
F 2 "alvarop:NRF52810_QCAA_QFN32" H 2250 3900 60  0001 C CNN
F 3 "" H 2250 3900 60  0001 C CNN
	1    3250 3550
	1    0    0    -1  
$EndComp
$Comp
L controller-rescue:C_Small C3
U 1 1 5A652654
P 2650 2350
F 0 "C3" H 2660 2420 50  0000 L CNN
F 1 "4.7uF" H 2660 2270 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 2650 2350 50  0001 C CNN
F 3 "" H 2650 2350 50  0000 C CNN
F 4 "CL21A475KQFNNNG" H 2650 2350 60  0001 C CNN "MPN"
	1    2650 2350
	1    0    0    -1  
$EndComp
$Comp
L controller-rescue:C_Small C5
U 1 1 5A65278B
P 3150 2000
F 0 "C5" H 3160 2070 50  0000 L CNN
F 1 "1uF" H 3160 1920 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 3150 2000 50  0001 C CNN
F 3 "" H 3150 2000 50  0000 C CNN
F 4 "CL21F105ZAFNNNG" H 3150 2000 60  0001 C CNN "MPN"
	1    3150 2000
	1    0    0    -1  
$EndComp
$Comp
L controller-rescue:C_Small C4
U 1 1 5A6528F7
P 2650 4850
F 0 "C4" H 2660 4920 50  0000 L CNN
F 1 "100nF" H 2650 4750 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 2650 4850 50  0001 C CNN
F 3 "" H 2650 4850 50  0000 C CNN
F 4 "CL21B104MBCNNNC" H 2650 4850 60  0001 C CNN "MPN"
	1    2650 4850
	1    0    0    -1  
$EndComp
$Comp
L controller-rescue:C_Small C6
U 1 1 5A652B4E
P 3900 2450
F 0 "C6" H 3910 2520 50  0000 L CNN
F 1 "100nF" H 3900 2350 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 3900 2450 50  0001 C CNN
F 3 "" H 3900 2450 50  0000 C CNN
F 4 "CL21B104MBCNNNC" H 3900 2450 60  0001 C CNN "MPN"
	1    3900 2450
	1    0    0    -1  
$EndComp
$Comp
L controller-rescue:C_Small C1
U 1 1 5A652D89
P 1800 3000
F 0 "C1" H 1810 3070 50  0000 L CNN
F 1 "100nF" H 1800 2900 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 1800 3000 50  0001 C CNN
F 3 "" H 1800 3000 50  0000 C CNN
F 4 "CL21B104MBCNNNC" H 1800 3000 60  0001 C CNN "MPN"
	1    1800 3000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR014
U 1 1 5A653232
P 3150 2250
F 0 "#PWR014" H 3150 2000 50  0001 C CNN
F 1 "GND" H 3150 2100 50  0000 C CNN
F 2 "" H 3150 2250 50  0000 C CNN
F 3 "" H 3150 2250 50  0000 C CNN
	1    3150 2250
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR015
U 1 1 5A6536F1
P 2650 2500
F 0 "#PWR015" H 2650 2250 50  0001 C CNN
F 1 "GND" H 2650 2350 50  0000 C CNN
F 2 "" H 2650 2500 50  0000 C CNN
F 3 "" H 2650 2500 50  0000 C CNN
	1    2650 2500
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR016
U 1 1 5A65377A
P 1800 3150
F 0 "#PWR016" H 1800 2900 50  0001 C CNN
F 1 "GND" H 1800 3000 50  0000 C CNN
F 2 "" H 1800 3150 50  0000 C CNN
F 3 "" H 1800 3150 50  0000 C CNN
	1    1800 3150
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR017
U 1 1 5A653803
P 3900 2600
F 0 "#PWR017" H 3900 2350 50  0001 C CNN
F 1 "GND" H 3900 2450 50  0000 C CNN
F 2 "" H 3900 2600 50  0000 C CNN
F 3 "" H 3900 2600 50  0000 C CNN
	1    3900 2600
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR018
U 1 1 5A6538E0
P 5150 3400
F 0 "#PWR018" H 5150 3150 50  0001 C CNN
F 1 "GND" H 5150 3250 50  0000 C CNN
F 2 "" H 5150 3400 50  0000 C CNN
F 3 "" H 5150 3400 50  0000 C CNN
	1    5150 3400
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR019
U 1 1 5A653969
P 2650 5000
F 0 "#PWR019" H 2650 4750 50  0001 C CNN
F 1 "GND" H 2650 4850 50  0000 C CNN
F 2 "" H 2650 5000 50  0000 C CNN
F 3 "" H 2650 5000 50  0000 C CNN
	1    2650 5000
	1    0    0    -1  
$EndComp
Text Label 4750 3900 2    60   ~ 0
SWDCLK
Text Label 4750 3800 2    60   ~ 0
SWDIO
$Comp
L controller-rescue:C_Small C12
U 1 1 5A654DC2
P 4950 2650
F 0 "C12" H 4960 2720 50  0000 L CNN
F 1 "12pF" V 4800 2550 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 4950 2650 50  0001 C CNN
F 3 "" H 4950 2650 50  0000 C CNN
	1    4950 2650
	0    1    1    0   
$EndComp
$Comp
L controller-rescue:C_Small C7
U 1 1 5A654F7A
P 4350 3100
F 0 "C7" H 4360 3170 50  0000 L CNN
F 1 "12pF" V 4400 2850 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 4350 3100 50  0001 C CNN
F 3 "" H 4350 3100 50  0000 C CNN
	1    4350 3100
	-1   0    0    1   
$EndComp
$Comp
L controller-rescue:C_Small C8
U 1 1 5A655040
P 4900 3400
F 0 "C8" H 4910 3470 50  0000 L CNN
F 1 "100pF" V 4950 3100 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 4900 3400 50  0001 C CNN
F 3 "" H 4900 3400 50  0000 C CNN
	1    4900 3400
	0    1    1    0   
$EndComp
$Comp
L controller-rescue:C_Small C13
U 1 1 5A6552D4
P 5050 3850
F 0 "C13" H 5060 3920 50  0000 L CNN
F 1 "0.8pF" V 4900 3800 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 5050 3850 50  0001 C CNN
F 3 "" H 5050 3850 50  0000 C CNN
	1    5050 3850
	-1   0    0    1   
$EndComp
$Comp
L controller-rescue:L_Small L1
U 1 1 5A655A30
P 5250 3700
F 0 "L1" V 5350 3700 50  0000 L CNN
F 1 "3.9nH" V 5200 3550 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 5250 3700 50  0001 C CNN
F 3 "" H 5250 3700 50  0001 C CNN
	1    5250 3700
	0    -1   -1   0   
$EndComp
$Comp
L controller-rescue:C_Small C14
U 1 1 5A6562C2
P 5600 3900
F 0 "C14" H 5610 3970 50  0000 L CNN
F 1 "DNP" V 5450 3850 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 5600 3900 50  0001 C CNN
F 3 "" H 5600 3900 50  0000 C CNN
	1    5600 3900
	-1   0    0    1   
$EndComp
$Comp
L controller-rescue:Antenna AE1
U 1 1 5A656652
P 6100 3300
F 0 "AE1" H 6025 3375 50  0000 R CNN
F 1 "Antenna" H 6025 3300 50  0000 R CNN
F 2 "alvarop:AMCA31-2R450G-S1F-T3" H 6100 3300 50  0001 C CNN
F 3 "" H 6100 3300 50  0001 C CNN
	1    6100 3300
	1    0    0    -1  
$EndComp
$Comp
L controller-rescue:Crystal_GND24 Y1
U 1 1 5A657BE1
P 4750 2900
F 0 "Y1" V 4900 3050 50  0000 L CNN
F 1 "32MHz" V 4550 2600 50  0000 L CNN
F 2 "Crystals:Crystal_SMD_2016-4pin_2.0x1.6mm" H 4750 2900 50  0001 C CNN
F 3 "" H 4750 2900 50  0001 C CNN
F 4 "CX2016DB32000D0FLJCC" V 4750 2900 60  0001 C CNN "MPN"
	1    4750 2900
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR020
U 1 1 5A65816A
P 4500 3000
F 0 "#PWR020" H 4500 2750 50  0001 C CNN
F 1 "GND" H 4500 2850 50  0000 C CNN
F 2 "" H 4500 3000 50  0000 C CNN
F 3 "" H 4500 3000 50  0000 C CNN
	1    4500 3000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR021
U 1 1 5A658208
P 5050 3000
F 0 "#PWR021" H 5050 2750 50  0001 C CNN
F 1 "GND" H 5050 2850 50  0000 C CNN
F 2 "" H 5050 3000 50  0000 C CNN
F 3 "" H 5050 3000 50  0000 C CNN
	1    5050 3000
	1    0    0    -1  
$EndComp
$Comp
L controller-rescue:C_Small C15
U 1 1 5A65903C
P 6050 3900
F 0 "C15" H 6060 3970 50  0000 L CNN
F 1 "DNP" V 5900 3850 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 6050 3900 50  0001 C CNN
F 3 "" H 6050 3900 50  0000 C CNN
	1    6050 3900
	-1   0    0    1   
$EndComp
$Comp
L controller-rescue:R R1
U 1 1 5A6595B5
P 5800 3700
F 0 "R1" V 5880 3700 50  0000 C CNN
F 1 "0" V 5800 3700 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 5730 3700 50  0001 C CNN
F 3 "" H 5800 3700 50  0000 C CNN
	1    5800 3700
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR022
U 1 1 5A659FBB
P 5600 4050
F 0 "#PWR022" H 5600 3800 50  0001 C CNN
F 1 "GND" H 5600 3900 50  0000 C CNN
F 2 "" H 5600 4050 50  0000 C CNN
F 3 "" H 5600 4050 50  0000 C CNN
	1    5600 4050
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR023
U 1 1 5A65A05F
P 6050 4050
F 0 "#PWR023" H 6050 3800 50  0001 C CNN
F 1 "GND" H 6050 3900 50  0000 C CNN
F 2 "" H 6050 4050 50  0000 C CNN
F 3 "" H 6050 4050 50  0000 C CNN
	1    6050 4050
	1    0    0    -1  
$EndComp
$Comp
L power:VDD #PWR024
U 1 1 5A65ABD7
P 2650 2200
F 0 "#PWR024" H 2650 2050 50  0001 C CNN
F 1 "VDD" H 2650 2350 50  0000 C CNN
F 2 "" H 2650 2200 50  0000 C CNN
F 3 "" H 2650 2200 50  0000 C CNN
	1    2650 2200
	1    0    0    -1  
$EndComp
$Comp
L power:VDD #PWR025
U 1 1 5A65AC7B
P 3800 2250
F 0 "#PWR025" H 3800 2100 50  0001 C CNN
F 1 "VDD" H 3800 2400 50  0000 C CNN
F 2 "" H 3800 2250 50  0000 C CNN
F 3 "" H 3800 2250 50  0000 C CNN
	1    3800 2250
	1    0    0    -1  
$EndComp
$Comp
L power:VDD #PWR026
U 1 1 5A65AD1F
P 2650 4500
F 0 "#PWR026" H 2650 4350 50  0001 C CNN
F 1 "VDD" H 2650 4650 50  0000 C CNN
F 2 "" H 2650 4500 50  0000 C CNN
F 3 "" H 2650 4500 50  0000 C CNN
	1    2650 4500
	1    0    0    -1  
$EndComp
NoConn ~ 3500 2650
NoConn ~ 3000 2650
NoConn ~ 4250 3500
$Comp
L power:GND #PWR027
U 1 1 5A65C086
P 4950 4000
F 0 "#PWR027" H 4950 3750 50  0001 C CNN
F 1 "GND" H 4950 3850 50  0000 C CNN
F 2 "" H 4950 4000 50  0000 C CNN
F 3 "" H 4950 4000 50  0000 C CNN
	1    4950 4000
	1    0    0    -1  
$EndComp
$Comp
L power:PWR_FLAG #FLG028
U 1 1 5A65D2D1
P 4050 1300
F 0 "#FLG028" H 4050 1375 50  0001 C CNN
F 1 "PWR_FLAG" H 4050 1450 50  0000 C CNN
F 2 "" H 4050 1300 50  0001 C CNN
F 3 "" H 4050 1300 50  0001 C CNN
	1    4050 1300
	-1   0    0    1   
$EndComp
Wire Wire Line
	2250 3800 1750 3800
Wire Wire Line
	2250 3900 1750 3900
Wire Wire Line
	1750 3500 2250 3500
Wire Wire Line
	5650 1650 5350 1650
Wire Wire Line
	5650 1550 5350 1550
Wire Wire Line
	5100 1150 5250 1150
Wire Wire Line
	5250 1250 5650 1250
Wire Wire Line
	2250 3700 1750 3700
Wire Wire Line
	5050 1350 5650 1350
Wire Wire Line
	5250 1450 5650 1450
Wire Wire Line
	5250 1150 5250 1250
Connection ~ 5250 1150
Wire Wire Line
	6850 1350 7300 1350
Wire Wire Line
	6850 1250 7300 1250
Wire Wire Line
	6850 1150 7300 1150
Wire Wire Line
	6850 1050 7300 1050
Wire Wire Line
	6350 5600 6050 5600
Wire Wire Line
	6350 5900 6050 5900
Wire Wire Line
	6350 5700 6050 5700
Wire Wire Line
	6350 5800 6050 5800
Wire Wire Line
	6350 6150 6050 6150
Wire Wire Line
	6350 6450 6050 6450
Wire Wire Line
	6350 6250 6050 6250
Wire Wire Line
	6350 6350 6050 6350
Wire Wire Line
	6350 6700 6050 6700
Wire Wire Line
	6350 7000 6050 7000
Wire Wire Line
	6350 6800 6050 6800
Wire Wire Line
	6350 6900 6050 6900
Wire Wire Line
	6350 7250 6050 7250
Wire Wire Line
	6350 7550 6050 7550
Wire Wire Line
	6350 7350 6050 7350
Wire Wire Line
	6350 7450 6050 7450
Wire Wire Line
	1350 7500 1500 7500
Wire Wire Line
	1350 7200 1500 7200
Wire Wire Line
	1350 6900 1500 6900
Wire Wire Line
	1050 6900 800  6900
Wire Wire Line
	1050 7200 800  7200
Wire Wire Line
	1050 7500 800  7500
Wire Wire Line
	1800 6900 2100 6900
Wire Wire Line
	1800 7200 2100 7200
Wire Wire Line
	2100 7500 1800 7500
Wire Wire Line
	2100 6900 2100 7200
Connection ~ 2100 7200
Wire Wire Line
	2300 7200 2300 7250
Wire Wire Line
	3000 6400 2650 6400
Wire Wire Line
	3000 6500 2650 6500
Wire Wire Line
	3050 700  3050 800 
Wire Wire Line
	4350 700  4350 800 
Wire Wire Line
	3050 1250 3700 1250
Wire Wire Line
	3050 1150 3050 1250
Wire Wire Line
	4350 1250 4350 1150
Connection ~ 3700 1250
Connection ~ 4350 800 
Connection ~ 3050 800 
Wire Wire Line
	7250 3700 7500 3700
Wire Wire Line
	7250 3850 7500 3850
Wire Wire Line
	6950 3350 6950 3500
Wire Wire Line
	7250 3350 7500 3350
Wire Wire Line
	7250 3500 7500 3500
Wire Wire Line
	7250 4050 7500 4050
Wire Wire Line
	7250 4200 7500 4200
Wire Wire Line
	7250 4500 7500 4500
Wire Wire Line
	7250 4650 7500 4650
Wire Wire Line
	7250 4850 7500 4850
Wire Wire Line
	7250 5000 7500 5000
Connection ~ 6950 3700
Connection ~ 6950 3500
Connection ~ 6950 4050
Connection ~ 6950 3850
Connection ~ 6950 4200
Connection ~ 6950 4850
Connection ~ 6950 4650
Wire Wire Line
	6950 4050 6750 4050
Wire Wire Line
	6750 4050 6750 3950
Wire Wire Line
	6850 1450 7300 1450
Wire Wire Line
	7300 1550 6850 1550
Wire Wire Line
	6850 1650 7300 1650
Wire Wire Line
	6850 1750 7300 1750
Connection ~ 6950 4500
Connection ~ 5250 1250
Wire Wire Line
	2250 3600 1750 3600
Wire Wire Line
	5350 6750 5350 6700
Wire Wire Line
	5250 6700 5350 6700
Wire Wire Line
	5250 6750 5250 6700
Connection ~ 5350 6700
Wire Wire Line
	4100 800  4350 800 
Wire Wire Line
	3050 800  3150 800 
Wire Wire Line
	3300 900  3150 900 
Wire Wire Line
	3150 900  3150 800 
Connection ~ 3150 800 
Wire Wire Line
	8800 1150 8800 1200
Wire Wire Line
	8800 800  8800 850 
Wire Wire Line
	8550 1050 8550 800 
Wire Wire Line
	8550 1600 8550 1250
Wire Wire Line
	8800 1200 8900 1200
Wire Wire Line
	10000 850  9850 850 
Wire Wire Line
	10000 950  9850 950 
Wire Wire Line
	10000 1150 9850 1150
Wire Wire Line
	9850 1250 9900 1250
Wire Wire Line
	10000 1350 9900 1350
Wire Wire Line
	9900 1350 9900 1250
Connection ~ 9900 1250
Wire Wire Line
	9850 1550 9900 1550
Wire Wire Line
	10000 1450 9900 1450
Wire Wire Line
	9900 1450 9900 1550
Connection ~ 9900 1550
Wire Wire Line
	7600 1450 7600 1400
Wire Wire Line
	7600 1450 7750 1450
Wire Wire Line
	7900 1450 7900 1400
Wire Wire Line
	7900 1150 7900 1200
Wire Wire Line
	7600 1150 7750 1150
Wire Wire Line
	7600 1150 7600 1200
Wire Wire Line
	7750 1150 7750 1100
Connection ~ 7750 1150
Wire Wire Line
	7750 1500 7750 1450
Connection ~ 7750 1450
Wire Wire Line
	1750 3400 2250 3400
Wire Wire Line
	1800 2900 1800 2850
Wire Wire Line
	1800 2850 2150 2850
Wire Wire Line
	2150 2850 2150 3200
Wire Wire Line
	2150 3200 2250 3200
Wire Wire Line
	2900 2200 2650 2200
Wire Wire Line
	2650 2200 2650 2250
Wire Wire Line
	3050 1850 3050 2500
Wire Wire Line
	3050 1850 3150 1850
Wire Wire Line
	3150 1850 3150 1900
Wire Wire Line
	3150 2100 3150 2200
Wire Wire Line
	3050 2500 3100 2500
Wire Wire Line
	3150 2200 3250 2200
Wire Wire Line
	3250 2200 3250 2500
Wire Wire Line
	3250 2500 3200 2500
Connection ~ 3150 2200
Wire Wire Line
	3600 2250 3800 2250
Wire Wire Line
	3900 2250 3900 2350
Wire Wire Line
	3900 2550 3900 2600
Wire Wire Line
	2650 4950 2650 5000
Wire Wire Line
	2650 4750 2650 4500
Wire Wire Line
	2650 4500 2900 4500
Wire Wire Line
	2900 4500 2900 4450
Wire Wire Line
	2650 2450 2650 2500
Wire Wire Line
	4250 3400 4800 3400
Wire Wire Line
	5150 3400 5000 3400
Wire Wire Line
	4250 3800 4750 3800
Wire Wire Line
	4250 3900 4750 3900
Wire Wire Line
	4250 3600 4900 3600
Wire Wire Line
	4900 3600 4900 4000
Wire Wire Line
	4900 4000 4950 4000
Wire Wire Line
	5050 4000 5050 3950
Wire Wire Line
	4250 3700 5050 3700
Wire Wire Line
	5050 3700 5050 3750
Connection ~ 5050 3700
Wire Wire Line
	5350 3700 5600 3700
Wire Wire Line
	4250 3300 4350 3300
Wire Wire Line
	4250 2650 4250 3200
Wire Wire Line
	4750 3300 4750 3050
Wire Wire Line
	4750 2750 4750 2650
Wire Wire Line
	4250 2650 4750 2650
Wire Wire Line
	5050 2650 5050 2900
Wire Wire Line
	5050 2900 4950 2900
Wire Wire Line
	4500 3000 4500 2900
Wire Wire Line
	4350 2900 4500 2900
Connection ~ 4750 2650
Connection ~ 5050 2900
Wire Wire Line
	4350 3200 4350 3300
Connection ~ 4350 3300
Wire Wire Line
	4350 2900 4350 3000
Connection ~ 4500 2900
Wire Wire Line
	5600 3800 5600 3700
Connection ~ 5600 3700
Wire Wire Line
	5950 3700 6050 3700
Wire Wire Line
	6050 3700 6050 3800
Wire Wire Line
	6100 3700 6100 3500
Connection ~ 6050 3700
Wire Wire Line
	5600 4000 5600 4050
Wire Wire Line
	6050 4000 6050 4050
Connection ~ 2650 4500
Connection ~ 2650 2200
Connection ~ 3800 2250
Connection ~ 4950 4000
Wire Wire Line
	1800 3100 1800 3150
Wire Wire Line
	4050 1300 4050 1250
Connection ~ 4050 1250
$Comp
L ostur:ARM-SWD J1
U 1 1 5A65E8A9
P 9150 3300
F 0 "J1" H 9150 3000 60  0000 C CNN
F 1 "ARM-SWD" H 9150 3600 60  0000 C CNN
F 2 "alvarop:ARM_SWD_SMD" H 8850 3150 60  0001 C CNN
F 3 "" H 8850 3150 60  0000 C CNN
	1    9150 3300
	1    0    0    -1  
$EndComp
Wire Wire Line
	8550 3100 8450 3100
Wire Wire Line
	8450 3100 8450 3000
Wire Wire Line
	8550 3300 8450 3300
Wire Wire Line
	8450 3300 8450 3200
Wire Wire Line
	8200 3200 8450 3200
Wire Wire Line
	8200 3200 8200 3300
Connection ~ 8450 3200
Wire Wire Line
	9750 3500 10050 3500
Wire Wire Line
	9750 3200 10050 3200
Wire Wire Line
	9750 3100 10050 3100
$Comp
L power:GND #PWR029
U 1 1 5A65F426
P 8550 1600
F 0 "#PWR029" H 8550 1350 50  0001 C CNN
F 1 "GND" H 8550 1450 50  0000 C CNN
F 2 "" H 8550 1600 50  0000 C CNN
F 3 "" H 8550 1600 50  0000 C CNN
	1    8550 1600
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR030
U 1 1 5A65F557
P 8200 3300
F 0 "#PWR030" H 8200 3050 50  0001 C CNN
F 1 "GND" H 8200 3150 50  0000 C CNN
F 2 "" H 8200 3300 50  0000 C CNN
F 3 "" H 8200 3300 50  0000 C CNN
	1    8200 3300
	1    0    0    -1  
$EndComp
$Comp
L power:VDD #PWR031
U 1 1 5A65F5F5
P 8550 800
F 0 "#PWR031" H 8550 650 50  0001 C CNN
F 1 "VDD" H 8550 950 50  0000 C CNN
F 2 "" H 8550 800 50  0000 C CNN
F 3 "" H 8550 800 50  0000 C CNN
	1    8550 800 
	1    0    0    -1  
$EndComp
$Comp
L power:VDD #PWR032
U 1 1 5A65F693
P 8450 3000
F 0 "#PWR032" H 8450 2850 50  0001 C CNN
F 1 "VDD" H 8450 3150 50  0000 C CNN
F 2 "" H 8450 3000 50  0000 C CNN
F 3 "" H 8450 3000 50  0000 C CNN
	1    8450 3000
	1    0    0    -1  
$EndComp
Wire Wire Line
	8550 800  8800 800 
Connection ~ 8550 800 
NoConn ~ 9750 3300
NoConn ~ 9750 3400
NoConn ~ 8550 3400
NoConn ~ 8550 3500
Text Label 10050 3100 2    60   ~ 0
SWDIO
Text Label 10050 3200 2    60   ~ 0
SWDCLK
Text Label 10050 3500 2    60   ~ 0
nRST
Wire Wire Line
	5250 1150 5650 1150
Wire Wire Line
	2100 7200 2300 7200
Wire Wire Line
	2100 7200 2100 7500
Wire Wire Line
	3700 1250 4050 1250
Wire Wire Line
	4350 800  4350 950 
Wire Wire Line
	3050 800  3050 950 
Wire Wire Line
	6950 3700 6950 3850
Wire Wire Line
	6950 3500 6950 3700
Wire Wire Line
	6950 4050 6950 4200
Wire Wire Line
	6950 3850 6950 4050
Wire Wire Line
	6950 4200 6950 4500
Wire Wire Line
	6950 4850 6950 5000
Wire Wire Line
	6950 4650 6950 4850
Wire Wire Line
	6950 4500 6950 4650
Wire Wire Line
	5250 1250 5250 1450
Wire Wire Line
	5350 6700 5450 6700
Wire Wire Line
	3150 800  3300 800 
Wire Wire Line
	9900 1250 10000 1250
Wire Wire Line
	9900 1550 10000 1550
Wire Wire Line
	7750 1150 7900 1150
Wire Wire Line
	7750 1450 7900 1450
Wire Wire Line
	3150 2200 3150 2250
Wire Wire Line
	5050 3700 5150 3700
Wire Wire Line
	4750 2650 4850 2650
Wire Wire Line
	5050 2900 5050 3000
Wire Wire Line
	4350 3300 4750 3300
Wire Wire Line
	4500 2900 4550 2900
Wire Wire Line
	5600 3700 5650 3700
Wire Wire Line
	6050 3700 6100 3700
Wire Wire Line
	3800 2250 3900 2250
Wire Wire Line
	4950 4000 5050 4000
Wire Wire Line
	4050 1250 4350 1250
Wire Wire Line
	8450 3200 8550 3200
$Comp
L alvarop:FT234XD U4
U 1 1 5B4C93FD
P 4600 5800
F 0 "U4" H 4600 6147 60  0000 C CNN
F 1 "FT234XD" H 4600 6041 60  0000 C CNN
F 2 "alvarop:FTDI_DFN12" H 4600 5800 60  0001 C CNN
F 3 "" H 4600 5800 60  0001 C CNN
F 4 "FT234XD-R" H 4600 5800 50  0001 C CNN "MPN"
F 5 "768-1178-1-ND" H 4600 5800 50  0001 C CNN "DKPN"
	1    4600 5800
	1    0    0    -1  
$EndComp
Wire Wire Line
	5450 5700 5800 5700
Wire Wire Line
	5150 5900 5550 5900
Wire Wire Line
	5150 6000 5500 6000
Wire Wire Line
	5150 6200 5550 6200
Wire Wire Line
	4050 6000 3700 6000
Wire Wire Line
	4050 5800 3700 5800
Wire Wire Line
	3750 5700 3400 5700
Text Label 5550 6200 2    60   ~ 0
UART1_RX
Text Label 5550 5900 2    60   ~ 0
UART1_TX
$Comp
L power:GND #PWR0101
U 1 1 5B579D02
P 3500 6150
F 0 "#PWR0101" H 3500 5900 50  0001 C CNN
F 1 "GND" H 3500 6000 50  0000 C CNN
F 2 "" H 3500 6150 50  0000 C CNN
F 3 "" H 3500 6150 50  0000 C CNN
	1    3500 6150
	1    0    0    -1  
$EndComp
Wire Wire Line
	3500 6150 3500 6100
Wire Wire Line
	3500 6100 4050 6100
Text Label 3400 5700 0    50   ~ 0
USB_DM
Text Label 5800 5700 2    50   ~ 0
USB_DP
Wire Wire Line
	4950 7050 4550 7050
Wire Wire Line
	4950 7150 4550 7150
Text Label 4550 7150 0    50   ~ 0
USB_DP
Text Label 4550 7050 0    50   ~ 0
USB_DM
$Comp
L controller-rescue:R R6
U 1 1 5B5CA8F4
P 5300 5700
F 0 "R6" V 5380 5700 50  0000 C CNN
F 1 "27" V 5300 5700 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 5230 5700 50  0001 C CNN
F 3 "" H 5300 5700 50  0000 C CNN
F 4 "RC0603FR-0727RL" V 5300 5700 50  0001 C CNN "MPN"
F 5 "311-27.0HRCT-ND" V 5300 5700 50  0001 C CNN "DKPN"
	1    5300 5700
	0    -1   -1   0   
$EndComp
NoConn ~ 4050 5900
$Comp
L power:VDD #PWR0102
U 1 1 5B600393
P 3650 5900
F 0 "#PWR0102" H 3650 5750 50  0001 C CNN
F 1 "VDD" H 3650 6050 50  0000 C CNN
F 2 "" H 3650 5900 50  0000 C CNN
F 3 "" H 3650 5900 50  0000 C CNN
	1    3650 5900
	0    -1   -1   0   
$EndComp
$Comp
L power:VDD #PWR0103
U 1 1 5B600602
P 5500 6000
F 0 "#PWR0103" H 5500 5850 50  0001 C CNN
F 1 "VDD" H 5500 6150 50  0000 C CNN
F 2 "" H 5500 6000 50  0000 C CNN
F 3 "" H 5500 6000 50  0000 C CNN
	1    5500 6000
	0    1    1    0   
$EndComp
Wire Wire Line
	3700 5900 3650 5900
Wire Wire Line
	3700 5800 3700 5900
Wire Wire Line
	3700 5900 3700 6000
Connection ~ 3700 5900
NoConn ~ 5150 5800
NoConn ~ 5150 6100
NoConn ~ 4050 6200
$Comp
L controller-rescue:R R2
U 1 1 5B64F91A
P 3900 5700
F 0 "R2" V 3980 5700 50  0000 C CNN
F 1 "27" V 3900 5700 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 3830 5700 50  0001 C CNN
F 3 "" H 3900 5700 50  0000 C CNN
F 4 "RC0603FR-0727RL" V 3900 5700 50  0001 C CNN "MPN"
F 5 "311-27.0HRCT-ND" V 3900 5700 50  0001 C CNN "DKPN"
	1    3900 5700
	0    -1   -1   0   
$EndComp
$Comp
L Device:Ferrite_Bead_Small L2
U 1 1 5B64FBF6
P 4250 7350
F 0 "L2" V 4013 7350 50  0000 C CNN
F 1 "FB" V 4104 7350 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 4180 7350 50  0001 C CNN
F 3 "~" H 4250 7350 50  0001 C CNN
F 4 "587-1926-1-ND" V 4250 7350 50  0001 C CNN "MPN"
F 5 "BKP1608HS330-T" V 4250 7350 50  0001 C CNN "DKPN"
	1    4250 7350
	0    1    1    0   
$EndComp
Connection ~ 4850 7350
Wire Wire Line
	4850 7350 4950 7350
Wire Wire Line
	4150 7350 3950 7350
Wire Wire Line
	4850 7450 4850 7350
$Comp
L controller-rescue:C_Small C18
U 1 1 5B6A04EF
P 4450 7500
F 0 "C18" H 4460 7570 50  0000 L CNN
F 1 "10nF" H 4460 7420 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 4450 7500 50  0001 C CNN
F 3 "" H 4450 7500 50  0000 C CNN
F 4 "CL21B103KBANNNC" H 4450 7500 60  0001 C CNN "MPN"
	1    4450 7500
	1    0    0    -1  
$EndComp
Wire Wire Line
	4450 7400 4450 7350
$Comp
L power:GND #PWR0104
U 1 1 5B6AE015
P 4450 7650
F 0 "#PWR0104" H 4450 7400 50  0001 C CNN
F 1 "GND" H 4450 7500 50  0000 C CNN
F 2 "" H 4450 7650 50  0000 C CNN
F 3 "" H 4450 7650 50  0000 C CNN
	1    4450 7650
	1    0    0    -1  
$EndComp
Wire Wire Line
	4450 7650 4450 7600
$Comp
L controller-rescue:C_Small C16
U 1 1 5B6BC061
P 2750 1050
F 0 "C16" H 2760 1120 50  0000 L CNN
F 1 "100nF" H 2750 950 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 2750 1050 50  0001 C CNN
F 3 "" H 2750 1050 50  0000 C CNN
F 4 "CL21B104MBCNNNC" H 2750 1050 60  0001 C CNN "MPN"
	1    2750 1050
	1    0    0    -1  
$EndComp
Wire Wire Line
	2750 950  2750 800 
Wire Wire Line
	2750 800  3050 800 
Wire Wire Line
	2750 1150 2750 1250
Wire Wire Line
	2750 1250 3050 1250
Connection ~ 3050 1250
Wire Wire Line
	4450 7350 4850 7350
Wire Wire Line
	4450 7350 4350 7350
Connection ~ 4450 7350
Wire Wire Line
	3950 7450 3950 7350
Wire Wire Line
	3300 4450 3300 4950
Wire Wire Line
	3400 4450 3400 4950
Wire Wire Line
	3500 4450 3500 4950
Wire Wire Line
	3600 4450 3600 4950
Wire Wire Line
	2900 2200 2900 2650
Wire Wire Line
	3100 2500 3100 2650
Wire Wire Line
	3200 2500 3200 2650
Wire Wire Line
	3600 2250 3600 2650
NoConn ~ 3100 4450
NoConn ~ 3200 4450
NoConn ~ 3400 2650
NoConn ~ 3300 2650
$EndSCHEMATC
