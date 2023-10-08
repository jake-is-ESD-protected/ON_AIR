# ON AIR
## **Overkill "do not disturb" - sign with ESP32, LED array, webserver, bell and status bar!**

### **Check it out on [YouTube](https://www.youtube.com/watch?v=-_JxScByp8Y&list=PL6OebxzKPNYUDOob-UIvixP95LtMlOk7M&pp=gAQBiAQB)!**

### |**[Overview](#overview)**|**[How it works](#how-it-works)**|**[Demo](#demo)**|**[Disclaimer](#please-be-aware)**|

***

![main](Images/doc/IMG_5282.jpg)

## Overview
This project utilizes an **ESP32** from AZ-Delivery to create an **old school** "On Air"-sign with a modern twist. This sign can be wirelessly controlled within a LAN via an async **webserver** hosted directly on the ESP32 itself. The webserver then displays a page where the user can select **the signs status** with additional info on the screen. It even features a **bell button** which sends a message to the user inside the room, letting them know via the website that someone would like to enter.

## How it works

### The sign
This sturdy 4mm thick sign made from white PLA was designed in FreeCAD and printed with a Prusa Mini 3D-printer. It utilizes a stamp font to avoid floating letter parts and consists of 3 parts in total: Main case with font and cutouts, back panel with slide holes for easy screw mounting and a transparent flat piece behind the letters for light diffusion (also 3D-printed). Check the [CAD folder](CAD/) for source files. All pieces are combined with various M3 screws.

### The PCB
The DipTrace made PCB provides more stability and a connection point for the ESP32. Other than that it includes a red LED array of 6 pieces, a button with a builtin LED and a standard 16x2 I²C LCD for additional messages. See the [DipTrace folder](DipTrace/) but please also read the [disclaimer](#please-be-aware).

### The program
The program consist of a **core** which recieves commands from the peripherals. It uses **interrupts** and **FreeRTOS** to perform multiple jobs at the same time. Additionally it hosts the ajax-driven webserver which is hosted on an IP that the program prints during boot. The lights only glow when the selected state actually matches an "on air" situation like recording, working or attending a meeting. See [`states.h`](Code/lib/MAIN/states.h) for a list of all states and [`lcd.h`](Code/lib/UI/lcd.h) for their associated messages. The embedded driving code for the webserver can be found in [`webserver`](Code/lib/webserver/).

### The website
The website is fairly simple and includes a small amount of styling and java script for event driven updates. A screenshot of the webserver can be found in the [demo](#demo). The code itself can be found in [`website`](Code/website/).

## Demo

![](Images/doc/IMG_5279.jpg)

|||
|-|-|
|![](Images/doc/IMG_5282.jpg)|![](Images/doc/IMG_5284.jpg)|
|![](Images/doc/IMG_5294.jpg)|![](Images/doc/IMG_5295.jpg)|
|![](Images/doc/IMG_5293.jpg)|![](Images/doc/IMG_5296.jpg)|

![](Images/doc/IMG_5281.jpg)

![](Images/doc/MVI_5292.gif)

![](Images/doc/Screenshot%20from%202023-06-25%2019-31-45.png)

## Please be aware
This was an on-off project for me within the last 2 years without any guidelines or specific goal. The source files in this repo are therefore of very fluctuating quality and were created chronologically far apart from one another. I do not know much about web-dev or CAD and even my embedded systems and PCB-design skills were not that advanced when i started this project. Check the git history to see the files' last update dates. 