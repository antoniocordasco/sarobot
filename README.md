# Sarobot

 #### The purpose of this project is to create a device that can press an external phisical button, after a specific amount of time.
 The components used are:
  - an Arduino nano
  - a servo motor, to press external the button
  - an OLED display, to show the amount of time remaining
  - a button, to switch between seconds and minutes and reset the timer
  - a potentiometer, to set the number of seconds or minutes

## Installation:
 - Connect the servo motor to the 3-pin connector, with the brown wire closer to the power connector. 
   Please check the **[device.png](device.png)** image of the assembled device, in this repository.
 - Connect the power supply unit to your mains socket
 - Connect the power supply unit barrel jack to the power connector of the Sarobot device
 -  aaaand you're done!

## Usage:
 Simply press the only button on the device to switch between seconds and minutes and also start the timer.
 Use the potentiometer to set the amount of seconds or minutes. 
 The timer can be set from a minimum of 10 seconds to a maximum of 600 minutes (10 hours).
 After the specified amount of time as passed, the servo motor will move the arm, and hopefully press the intended button.

