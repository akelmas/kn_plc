# KN PLC Programming Tool

This is a ladder logic editor developed using QT Widgets. User can create Ladder Logic networks and upload it to KN PLC.
KN PLC is a programmable logic controller with core functionality. It is built around Arduino Mega 2560.

It supports:
* 8 x Digital Inputs
* 8 x Digital Outputs
* 8 x AUX
* 2 x Counter
* 2 x Timer

`Object                 Arduino Mega 2560 Pin`
`Digital Input 0        22`
`Digital Input 1        24`
`Digital Input 2        26`
`Digital Input 3        28`
`Digital Input 4        30`
`Digital Input 5        32`
`Digital Input 6        34`
`Digital Input 7        36`
`Digital Output 0       4`
`Digital Output 1       5`
`Digital Output 2       6`
`Digital Output 3       7`
`Digital Output 4       8`
`Digital Output 5       9`
`Digital Output 6       10`
`Digital Output 7       11`

_Pin connections should be made using pull-down resistors to avoid floats_


## Dependencies/Pre-requisites
* platformio (CORE) >= 4.x _not required to build but in order to run_
  * To install platformio, see https://docs.platformio.org/en/latest/core/installation.html

* Qt >= 5.12.1
  * To install Qt, see https://doc.qt.io/qt-5/gettingstarted.html

## Build instructions
1. Clone this repo. 
2. Run the commandin top level directory: `qmake -makefile`
3. Build it: `mingw32-make`

## Usage
After succesfully compiled, execute the command:

`./release/kneditor.exe` 

![App screenshot](/data/ss.png)

NOTE: if _platformio_ is not in the PATH you cannot upload the program to Arduino.
