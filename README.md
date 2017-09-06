# Albate-Synthesizer
The project that I developed for the final year project/dissertation of my undergraduate studies. It uses the Arduino programming language.

## How it works
This is a synthesizer that works by producing 8 different audio signals. Every two signals are then multiplied with one another, in order to produce a new composite signal. As a result, the device
output 4 individual composite signals, for the user to hear. Additional features are also available, such as audio effects (delay and distortion), audio filters, and an 8-step sequencer.

## Hardware
The device was developed with a [Teensy 3.2 development board](https://www.pjrc.com/store/teensy32.html), with the addition of the [audio adapter](https://www.pjrc.com/store/teensy3_audio.html).

## Software
The project uses the Audio library that is provided with the [Teensy loader](https://www.pjrc.com/teensy/tutorial.html), whilst being programmed within the Arduino language.

## Installation

In order to install the software, you will first need to go through the tutorial for installing and using Teensy with Arduino. Afterwards, download the project from this repository, and 
unzip the file. When the folder is extracted, be sure to rename the main folder to "Teensy_Test", so that the program can be run.

Finally, plug the board into the computer, open the code through the Arduino IDE, and execute it when the Teensy has been detected by the computer. The program should then compile and install
onto the device.
