# KN Tool

This is a ladder logic editor developed using Qt Widgets. User can create Ladder Logic networks and upload it to KN PLC.
KN PLC is a programmable logic controller with core functionality. It is built around Arduino Mega 2560.

It supports:
* 8 x Digital Inputs
* 8 x Digital Outputs
* 8 x AUX
* 2 x Counter
* 1 x Timer


## Dependencies/Pre-requisites
* Qt >= 5.12.1
  * To install Qt, see https://doc.qt.io/qt-5/gettingstarted.html

## Build instructions
1. Clone this repo. 
2. Run the commandin top level directory: 

```
cmake -B build -DQT_DIR=<path/to/qtX.cmake>
```

> `QT_DIR` is required to find Qt packages.

3. Build it: 

```
cmake --build build
```

## Usage
After succesfully compiled, find the executable in `build` directory and run:

```
./build/kn
```

