# serial-remote-control-sample
Simple example code for using the Teensy serial port via USB for remote control

If you have your Teensy and h-bridge all wired up, you can use this code to remotely control your bot via Serial.

You may have to reverse directions of wheels depending upon how you have your bot wired up compared to mine.  I've done a little bit of that, so sometimes the directions mentioned in the code aren't consistent with what the bot does.

If you set up your Pi on Wifi, you can SSH to your Pi and then connect to the Arduino via serial.  Once connected, you can use the following keyboard keys to execute controls:

s: Stop
w: both wheels forward
a: rotate left
d: rotate right
x: both wheels back
u: left wheel speed ++
i: right wheel speed ++
j: left wheel speed --
k: right wheel speed --
