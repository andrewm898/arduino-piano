# arduino-piano
Code to make an arduino mechanical-button piano that can play more than 1 note at once!

*This would probably work with the capacitive touch library, I just did it mechanically bc I don't like the delay w capacitive touch*

Credit where credit is due- the basis of this code was taken from a response to my question on the Arduino forums:
https://forum.arduino.cc/index.php?topic=730600.0;topicseen
Thank you to @BlackFin for helping with this amazing code!

This uses this arduino Tone library, so you'll need to install that first. 
https://github.com/bhagman/Tone

I was using the ATMEGA 328 chip on my arduino and only had 2 speakers to work with, but you could definitely modify this to play more notes if you use a ATMEGA 1280
