# YoutubeVideoPlayer


## Table of contents
- [Table of contents](#table-of-contents)
- [About this project](#about-this-project)
- [Project Structure](#project-structure)
- [Requirements](#requirements)
    * [Hardware](#hardware)
    * [Software](#software)
    * [External libraries](#external-libraries)
- [Project setup](#project-setup)
    * [Hardware](#hardware-1)
    * [Software](#software-1)
    * [External libraries](#external-libraries1)
        - [MSP432](#msp432)
        - [ESP32](#esp32)
- [Features](#features)
- [Contributors](#contributors)
- [Contacts](#contacts)

## About this project
This project is a Youtube remote controller, allowing you to pause, play, mute, unmute, go forwards or backwards with the video and so on. The user will first need to access a web page, on which he can play the desired youtube video, then the instructions received by the MSP432P401R by the BOOSTERPACK analog joystick and buttons are first processed to notify the LCD screen of the changes in the video flow, then sent via WiFi to the host playing the video.

## Project structure

```

├───MSP432
│   ├───include
│   ├───src
├───ESP32
│   ├───ESP32.ino
│   ├───index.h


```

## Requirements

### Hardware
* MSP432P401R LaunchPad
* MSP432P401R BoosterPack BOOSTXL-EDUMKII
* ESP32 (ESP32-WROOM-32)

### Software
* Code Composer Studio
* Arduino IDE
* Visual Studio Code

### External libraries
* Driverlib
* Grlib
* WiFi
* HTTPClient

## Project setup

### Hardware

 * Connect the MSP432 to the BoosterPack, being careful to align the pins correctly
 * Connect the MSP432 to the computer using a USB cable
 * Connect the MSP432 to the ESP32 as shown below.

### ESP32

 * Connect pin 3.3 of the MSP432 to pin 16 of the ESP32
 * Connect pin 3.2 of the MSP432 to pin 17 of the ESP32

<table align="center">
        <tr>
            <th>MSP432 pin</th>
            <th>ESP32 pin</th>
        </tr>
        <tr>
        	<td>5V</td>
        	<td>5V</td>
        </tr>
        <tr>
        	<td>GND</td>
        	<td>GND</td>
        </tr>
        <tr>
        	<td>P3.3</td>
        	<td>16</td>
        </tr>
        <tr>
        	<td>P3.2</td>
        	<td>17</td>
        </tr>
    </table>

### Software

1. Clone the GitHub repository on your PC
2. Open Code Composer Studio ([download](http://www.ti.com/tool/CCSTUDIO) it first in case you don't have it) and create a new CCS project
3. Chose from target MSP432 Family the board MSP432P401R as shown below:

     <img align="center" src="https://github.com/RobertoSavi/YoutubeVideoPlayer/blob/main/photos/Create%20CCS%20project%201.png">

4. Copy the files in src, paste them in your new project and add external libraries
5. Build the project and flash it in the MSP432
6. Open Arduino IDE ([download](https://www.arduino.cc/en/software) it first in case you don't have it), add the libraries and open the file 'ESP32.ino'
7. Flash the code in the ESP32

### External libraries

#### MSP432

1. Download the [driverlib](https://www.ti.com/tool/MSPDRIVERLIB) and [grlib](https://www.ti.com/tool/MSP-GRLIB) libraries
2. Add the libraries to the linker and loader in the project properties as shown below:
3. Open CCS, right click on your project then click on 'Properties'
4. Go to 'Build' > 'Arm Compiler' > 'Include Options' as shown below:

     <img align="center" src="https://github.com/RobertoSavi/YoutubeVideoPlayer/blob/main/photos/Create%20CCS%20project%202.png">

     5. Click 'Add...' and copy the path to your driverlib directory
        
6. Go to 'Build' > 'Arm Linker' > 'File Search Path' as shown below:

     <img align="center" src="https://github.com/RobertoSavi/YoutubeVideoPlayer/blob/main/photos/Create%20CCS%20project%203.png">

     7. Click 'Add...' and copy the path to your 'msp432xx_driverlib.lib' and 'grlib.a'     


#### ESP32

1. Install the 'esp32 by Espressif Systems' board  from the Arduino IDE boards manager as shown below: 

    <img align="center" src="https://github.com/RobertoSavi/YoutubeVideoPlayer/blob/main/photos/Install%20Arduino%20board.png">

2. Install the following libraries from the Arduino IDE library manager as shown below: 
    * WiFi
    * HTTPClient

    <img align="center" src="https://github.com/RobertoSavi/YoutubeVideoPlayer/blob/main/photos/Install%20Arduino%20lib.png">

3. Replace the ssid and password in the `ESP32.ino` file with your own, changing WiFi mode if necessary


## Features
* Play
* Pause
* Skip forward
* Skip previous
* Volume Up
* Volume Down
* Reduce playback speed
* Increase playback speed
* Go to the next video
* Go to the previous video


## Contributors
### Roberto Savi
* UART Communication
* Clock and timers synchronization
* WiFi Communication
### Merak Winston Hall
* Websocket connection
* WiFi communication
* Web server part
### Diamand Muka
* Web server part
* WiFi communication
### Lorenzo Midiri




## Contacts
* Roberto Savi [roberto.savi@studenti.unitn.it]
* Merak Winston Hall [merakwinston.hall@studenti.unitn.it]
* Diamand Muka [diamand.muka@studenti.unitn.it]
* Lorenzo Midiri [lorenzo.midiri@studenti.unitn.it]
