# MIDI2DMX

## Teensy driven DJ light and LED control via MIDI

### This project was developed to facilitate curriculum at Portland Community College for the Music and Sonic Arts department

MIDI2DMX provides a simple way to get total control over a wide variety of lights - using existing music software (Ableton Live, Logic, Bitwig, Garage Band, Pro Tools etc), as well as hardware midi sequencers like the Akai MPC series, the Elektron Octatrack, Alesis MMT-8, etc. It allows for easy interfacing with programming environments like Pure Data, MaxMSP, and Processing.

This makes it easy to sequence lights for live music performances, do algorithmic light programming, map lights using sound analysis, data from the internet, or sensor data etc.

It unlocks cheap DJ lights turning them into flexible and expressive tools/instruments.

I designed a PCB to make the circuit easy to build, and have assembled quite a number of them with students in my interactivity courses at Portland Community College. There is no wiring, it's a quick project and costs around $40 total.

Eagle files for the PCB are published here - but you can also order them (in multiples of 3) from OSHPark: https://oshpark.com/shared_projects/hyqqaC5r

This core of the design is a Teensy - with some solid and extremely fast DMX code from Paul Stoffregen. The result is much smoother and more reliable than any commercial solution I've used. Because of Paul's usb drivers - it can be programmed to be a USB midi device, so it will show up in any MIDI capable software on any OS. The code in this repo is setup to translate midi cc's into DMX channel values. (Mapping CC numbers directly to DMX channel numbers).

We've used the Teensy 3.2 for our builds - but an LC should also work https://www.pjrc.com/store/teensy32.html

The additional parts are available from mouser here: http://www.mouser.com/ProjectManager/ProjectDetail.aspx?AccessID=11855f0a7f

If you want a more detailed guide I've published this: http://www.instructables.com/id/MIDI2DMX/


###Other DMX solutions
It turns out that it's pretty hard to find a good, well supported, inexpensive DMX controller.

There are a lot of expensive DMX controllers with proprietary drivers. There are a lot of cheap DMX controllers with little or no documentation. (Although these days a lot of them are copied from the uDMX project). Some of them work intermittently, some work only on windows, and some work only with specific (unexciting) DMX control software.

The uDMX project http://www.anyma.ch/research/udmx/ is the only well documented open source option out there, but in my experience - it's not that good. It suffers from choppy framerates and frequent drop outs, and on top of that, all the units I tried became unresponsive fairly often in a variety of software applications. There is a fundamental design flaw: it relies on your computer to send DMX frames. If your computer is processing audio (from Pure Data or Max for instance) - you end up with pretty severe audio dropouts. Also your framerate suffers due to various computer and USB bus performance issues. By contrast - this code makes the Teensy handle sending all the DMX frames, it just keeps the current channel values in an array that gets updated as midi data gets sent. So only value changes get transmitted via USB and the Teensy does the work of sending continual, fast frames whether they get updated or not. There is no pause in the Arduino loop - it sends out frames as fast as it can resulting in super smooth curves and animations.


##PD:
In the PD folder you'll find a few example patches for Pure Data (http://puredata.info/). While you can control lights using this board from anything that can send midi - PD offers a lot more options and flexibility than most other midi software. I run patches like these on a Raspberry Pi for live use.

DMX_color_presets.pd shows how to translate RGB and HSV color values into messages you can send to your lights. Google has a nice color picker tool. https://www.google.com/webhp?sourceid=chrome-instant&ion=1&espv=2&ie=UTF-8#q=color%20picker

hsv2rbg.pd is an abstraction that the other patches rely on. All of these pd patches need to be put in the same folder to work correctly.

DMX_color_organ.pd has a simple midi mapping using Scriabin's pitch-color associations. https://en.wikipedia.org/wiki/Clavier_%C3%A0_lumi%C3%A8res

This makes any DMX light playable from a midi keyboard or midi sequencer like Ableton Live, Logic, Bitwig - as well as hardware sequencers like MPCs and Octatracks. This is my preferred method for programming custom light shows for live PA's.

Finally DMX_audio_analysis.pd demonstrates pitch and amplitude tracking and translation into light color and intensity using pd.

##Hardware Midi:
The hardware midi part of this board is untested - and the code doesn't support it yet. But it probably works, it just needs some arduino code to handle midi input on the Teensy's second Serial RX line.

##Future Dev:
Aside from implementing hardware midi - the plan is to setup some color and envelope control on the Teensy to replace some of work I'm currently doing with PD. An enclosure is also in progress.

##Schematic Errata:
Even though the schematic shows a PC110 optocoupler - it is actually wired and laid out for a Sharp PC900V optocoupler. The schmetic is correct - as long as you use a PC900V and not a PC110. It would need to be changed for a different optocoupler.
