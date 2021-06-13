# Design of Human Temperature Measurement and Storage Module
The system uses STM32 series single-chip microcomputer as the core control module. The OLED display module for displays the temperature data detected by the temperature sensor and the current time and date. The data storage module comes to the FM24C256 chip. The body temperature data is collected by the DS18B20 temperature sensor. The sensor performs as for measurement, and the AMS1117 chip is used for voltage conversion, designed for power supply for the whole system. The program download port adopts SWD mode. 
The system can set current time and date by pressing the button, and the measured temperature data can also be recorded by pressing designed button, and historical temperature data can be reviewed in history searching mode. The design also compares the selection of the microcontroller, several ways to display and the final selection of proper components, which met the requirement of functions. After test it proves that the system is stable and reliable, and shows a strong advantage compared to traditional thermometer.