# AirChrony-S3
DIY Chronograph revisited

This project is still a work in progress.
It's been about 10 years since my first blog entry on my DIY chronograph using an Ardunino Nano and a Bluetooth transmitter.  This connected to an app on my phone and transmitted the time between trigger 1 and 2, which the app then used, along with the pellet weight, to calculate the speed and power of the rifle/pistol.

I have a few projects under my belt since then and although I enjoy designing the hardware I'm not so good at writing the software needed to operate it.

As many others have done I've started using AI, specifically Google Gemini, to help writing code for, not just the hardware, but also the user interface on the phone/PC.

I realised that I could revisit the DIY Chronograph using an EPS32, my favorite being the Xiao ESP32 - S3 from Seeedstudio.  This runs at 240MHz which is a little faster than the older Arduino Nano and with it also having it's own webserver capability I could include the user interface on the Chronograph itself and just visit the webpage to interact with the device (this can be done on your phone, tablet or laptop).

Hardware
The basics are much the same as the original design with a couple of IR emitters and detectors spaced 50mm apart creating two beams that when broken start and end a timer that measures how long it takes for the pellet to travel 50mm and from that the speed of the pellet can be determined.

Chronograph - Top Down Section View
**<img width="1349" height="752" alt="General layout" src="https://github.com/user-attachments/assets/dc5491a5-53dd-4032-8448-067cf01a7798" />
**
Chronograph - Side Section View
<img width="2439" height="1622" alt="General layout  Side Sectionpng" src="https://github.com/user-attachments/assets/4c963da6-ec7f-4361-875d-3e72ce9063a8" />
 
Chronograph - X-RAY View
<img width="2372" height="1519" alt="X-Ray View" src="https://github.com/user-attachments/assets/f2c40c09-9fc2-434d-831f-c3266fca7d7d" />

The outer casing is made in two parts that slide over the electronics and tube, they interface using two 2mm dowels and an M3 screw to lock them together.
<img width="2750" height="1280" alt="20260831_165100" src="https://github.com/user-attachments/assets/377cd6ae-cbc7-4b12-bf7a-db51822e38c6" />

Currently power is supplied using a USB C lead from a 5V power source but I may add an internal battery later.  Please note: the USB power lead head will need to be narrow enough to fit in the recess which is 11.5mm x 6.5mm.



With the original design this "time" value was transmitted to the phone where the app would calculate the speed/power etc. with the ESP32 solution all the calculations are done on the device and placed on the webpage on the ESP32 server.  To view the User Interface you just connect you device to the WiFi "AirChrony3" then visit "192.168.4.1" with your browser.
User interface
The user interface opens to show the current shot data on the "Live Chrono" tab, there are other tabs including "Pellet profiles" here you can add details of your pellets like brand and size and weight, you can pick from these profiles to calculate the power of the airgun.  There is also a "settings" tab where you can change things like voice announcements, distance between the sensors and other things.

This interface is constantly under revision as I think of new things and you are welcome to use AI to create your own specific user interface.

## Assembled parts
<img width="2000" height="924" alt="20260908_101220" src="https://github.com/user-attachments/assets/eefc79a5-c6a8-480e-87d4-6afd441d461f" />


<img width="2000" height="924" alt="20260908_101303" src="https://github.com/user-attachments/assets/8d5c9b83-0081-4f0e-a9df-7cb649c86318" />

<img width="2000" height="924" alt="20260907_190150" src="https://github.com/user-attachments/assets/ff7f0fff-ddf6-477d-a45a-ba57b9e5554f" />



