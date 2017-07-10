# SpaceTheremin

SpaceTheremin is a virtual theremin controlled via the mouse or touchscreen. This app 
was originally released for Windows on http://zetacentauri.com in 2007 and later for 
OSX via the Apple Store as free but closed-source software. Across all versions it 
has been downloaded more than 65,000 times.

![SpaceTheremin Screenshot](https://github.com/Xangis/SpaceTheremin/blob/master/images/spacetheremin1.png)

An prebuilt installable version for Windows is available in the installer folder:

https://github.com/Xangis/SpaceTheremin/blob/master/installer/SpaceTheremin1.1Setup.exe

Unlike a standard analog theremin, which typically has a sinewave oscillator and some 
basic vibrato, SpaceTheremin lets you choose from six different oscillator types 
(sine, square, triangle, sinc, saw, and pulse). The same waveforms can also be used 
for vibrato.

It has an adjustable pitch range and checkboxes that let you select whether the pitch 
and volume ranges are logarithmic or linear.

Use of the vibrato can result in some incredibly freaky and bizarre sounds. Set the 
vibrato depth (in percent) and adjust the vibrato frequency via the slider to hear 
the effects. A vibrato depth of 50% and a triangle waveform gives an interesting 
sound, for example.

Although the vibrato frequency ranges from 0.1 to 12.0 Hz in 0.1 Hz increments, 
checking the "modulation" box changes the range maximum to 8000. Moving this slider 
into the audible frequency range (20Hz and up) allows you to get strange 
two-oscillator frequency modulation sounds.

Another neat trick is to use the tab and arrow keys to adjust parameters while playing 
tones with the mouse. For instance, if the waveform is selected, the arrow keys will 
change waveforms in mid-tone for an interesting effect. You can also do the same with 
the vibrato waveform. Adjusting the vibrato frequency via the keyboard can also be handy.

This program is more for generating strange sounds than for playing actual music. As 
such, you may get some really horrible noises out of it. It would be incredibly 
impressive if anyone managed to play a tune with it, even if it were something as simple 
as "twinkle twinkle little star".

This application has been tested on Windows, OSX and Linux, both 32-bit and 64-bit.

SpaceTheremin is released under the terms of the MIT License. See the LICENSE.TXT file
for details.

# Building

SpaceTheremin is written in C++ and uses wxWidgets for the user interface and 
PortAudio for sound generation (originally used with pa_stable_v19_20140130, 
but newer versions should also work). It works with wxWidgets 3.0 and 3.1, 
but expects wxWidgets 3.0.

wxWidgets website:
https://www.wxwidgets.org/

PortAudio website:
http://www.portaudio.com/

### Linux

A makefile is included for Linux. If your wx-config is in a different location than 
that in the Makefile, change the path at the top of the Makefile.

### Windows

For the Visual Studio solution on Windows you'll have to change the paths for PortAudio
and wxWidgets to match where they are on your system.

Installers for Windows are under the "installer" folder. There is a project file for the 
Nullsoft Install System (NSIS) and one for InnoSetup.  The InnoSetup one is the one that 
is up to date, but chances are good that you'll have to fiddle with some paths (your VS
project will probably put files into different places, and some things have probably beeen moved).
That folder also includes prebuilt binaries, but those are likely out of date.

### OSX

On OSX, you'll need to run the OSXLibraryPaths.sh to set up the correct library paths
for the application.

# Changelog

### Changes from Version 1.01 to 1.1:

- Switched sound generation from OpenAL to PortAudio because OpenAL is 
  abandonware at this point.
- The vibrato frequency slider will now grow larger when you enlarge the
  window to make it easier to scroll to exact values.

# Development Status

This application is no longer under active development, but I do still maintain it.

I am willing to accept pull requests if you have improvements. There is definitely 
room for improvement, since it makes some assumptions about paths and development 
environment that are specific to my system.
