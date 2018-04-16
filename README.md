# Cs207 Alarm Clock
An Arduino supported clock with the added feature of an alarm.

This repository will be going through on everything you need to replicate the alarm clock build that I have added on from the original creator of just a binary clock Lagsilva (1). Alongside the alarm is not just a normal buzzer, it plays the tetris theme at an increased tempo which orginialy was aquired from lukecyca on Github (2).

This clock makes use of a Tiny I2C RTC Module and a Maxim Max 7219 Led Matrix. It was made with the idea of wanting a catchier alarm and to avoid using ones phone as it could then persuade someone to play on there phone over sleeping. The rest of this will have build materials the schematics of the build and the use of the Alarm clock.

![alt text][pic1]

[pic1]: https://github.com/Ubertracker/Cs207_Alarm_Clock/blob/master/img/ClockFront.jpg "Clock Front View"

Repository Contents
============

* **/src**- The Software files for the Clock are here
* **/libraries**- Hds The libraries needed for the code to run
* **/img**- Where all images for the project reside
* **/LICENSE**- The License File
* **/README.md** - The file that contains an overview of the project and what you are reading

Requirements and Materials
============

The libraries required for this project are as followed:

Libraries:
* DS1307RTC- https://github.com/PaulStoffregen/DS1307RTC
* LedControl- https://github.com/wayoda/LedControl
* Time- https://github.com/PaulStoffregen/Time

The Materials needed for this project are as followed:

Materials:
* 2 x Push Buttons
* 2 x 10K ohm Resistor
* 1 x Piezo Speaker
* 1 X Tiny I2C DS1307RTC
* 1 x MAX7219 8X8 LED Dot Matrix Display Module (you can implement this build to run two LED matrixes as you can see in Lagsilva's     orginal code that he uses two Matrixes )
* 1 x breadboard
* 1 x Arduino Uno
* Wires

Build Instructions
==================

As seen in the picture below there is a lot of wiring that needs to be done for each separte component needed for this build. Although it may look like a lot the build itself is not that complicated.

1. First connect a Piezo speaker to both the ground and then to Pin 9.
2. Next create two pull down buttons by connecting a resistor and 5v power supply on one side and then connecting to pin 12 and 13 for each button respectively.
3. With the DS1307RTC module connect it to the 5v power supply and ground and connect SDA to analog pin A4 and SLC to analog pin A5.
4. With the Led Dot Matrix Display connect the 5v power supply and gound from either the top or bottom. Connect DIN to Digital pin 2, Connect CS to Digital pin 3, Connect CLK to Digital pin 4.

![alt text][pic2]

[pic2]: https://github.com/Ubertracker/Cs207_Alarm_Clock/blob/master/img/TopClockWiring.jpg "Top View of Clock"

![alt text][pic3]

[pic3]: https://github.com/Ubertracker/Cs207_Alarm_Clock/blob/master/img/ClockSchematicSketch.jpg "Schematic view of Clock"

![alt_text][pic4]
  
[pic4]: https://github.com/Ubertracker/Cs207_Alarm_Clock/blob/master/img/ClockBreadboardBuild.jpg "Breadboard view of Clock"

Note: some of these parts may require soldering to hook up header pins so prepare to solder. 

Firmware Installation
=====================

To get access to the libraries you must download the 3 libraries in the libraries folder and then make sure to include the libraries in the arduino program to get them to work. 

Usage
=====

To use this device is to set the time that you would like it to show and then upload the code onto the arduino and the clock will do the rest from there. When the Alarm will go off you can press one of the two buttons and whichever one is connected to pin 12 will stop the alarm and will sound off when it reaches that time again. The button connected to pin 13 will snooze the alarm for 5 minutes and go off again and when you press the button to stop the alarm it will then go back to the original alarm set time. 

Team
=====

Nicholas Chodak-This was done by myself from the coding to the building and the Repository.

Credits
=======

(1) Lagsilva - Where the original idea and code for the clock was implemented from 
https://www.hackster.io/lagsilva/digital-and-binary-clock-with-two-led-matrix-and-rtc-0def6a

(2) lukecyca - Created the tetris theme used during when the alarm is on
https://github.com/lukecyca/TetrisThemeArduino

PaulStoffregen - Created the DS1307 and the Time library for use in this code.
https://github.com/PaulStoffregen/DS1307RTC & https://github.com/PaulStoffregen/Time

wayoda - Created the LEDControl Library used in this code.
https://github.com/wayoda/LedControl
