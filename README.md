# MIDI2DMX

##Control DJ lights and LEDs from any software or hardware that can transmit MIDI

##This project came out of research associated with my work at Portland Community College as an Audio Programming Instructor in the Music and Sonic Arts department.

This is an easy way to get total control over a wide variety of lights - using existing music software (ableton live, logic, bitwig, garage band, pro tools etc). It also allows for easy interfacing with programming environments like Pure Data, MaxMSP, Processing etc).

This makes it easy to sequencing lights for live music performances, do algorithmic light programming, map lights using sound analysis, data from the internet, or sensor data etc.

This unlocks cheap DJ lights, turning them into flexible and expressive tools/instruments.

It turns out that it's pretty hard to find a good, well supported, inexpensive DMX controller.

There are a lot of expensive DMX controllers with proprietary drivers. There are a lot of cheap DMX controllers with little or no documentation. (Although these days a lot of them are copied from the uDMX project). Some of them work intermittently, some work only on windows, and some work only with specific (unexciting) DMX control software.

The uDMX project http://www.anyma.ch/research/udmx/ is the only well documented open source option out there, but in my experience - it's not that good. It suffers from choppy framerates and frequent drop outs, and on top of that, all the units I tried became unresponsive fairly often in a variety of software applications. There is a fundamental design flaw: it relies on your computer to send DMX frames. If your computer is processing audio (from Pure Data or Max for instance) - you end up with pretty severe audio dropouts. Also your framerate suffers due to varies computer and USB bus performance issues.

This project uses a Teensy - with some rock solid and extremely fast code from Paul Stoffregen.

The Teensy is programmed to be a USB midi device, and is setup to translate midi cc's into DMX channel values. (Mapping CC numbers directly to DMX channel numbers).

I designed this PCB to make the circuit easy to build, and have assembled quite a number of them with some of students in my interactivity courses at Portland Community College.
