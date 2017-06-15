# SpaceTheremin

SpaceTheremin is a virtual theremin controlled via the mouse or touchscreen. This app 
was originally released for Windows on http://zetacentauri.com and later for OSX via 
the Apple Store. Across all versions it has been downloaded more than 65,000 times.

An prebuilt installable version for Windows is available in the installer folder:
https://github.com/Xangis/SpaceTheremin/blob/master/installer/SpaceTheremin1.1Setup.exe

![SpaceTheremin Screenshot](https://github.com/Xangis/SpaceTheremin/blob/master/images/spacetheremin1.png)

This application has been tested on Windows, OSX and Linux, both 32-bit and 64-bit.
It is written in C++ and uses wxWidgets for the user interface and PortAudio for sound
generation (originally used with pa_stable_v19_20140130, but newer versions should also work).
It works with wxWidgets 3.0 and 3.1, but expects wxWidgets 3.0.

wxWidgets website:
https://www.wxwidgets.org/

PortAudio website:
http://www.portaudio.com/

A makefile is included for Linux. If your wx-config is in a different location than 
that in the Makfile, change the path at the top of the Makefile.

For the Visual Studio solution on Windows you'll have to change the paths for PortAudio
and wxWidgets to match where they are on your system.

On OSX, you'll need to run the OSXLibraryPaths.sh to set up the correct library paths
for the application.

Installers for Windows are under the "installer" folder. There is a project file for the 
Nullsoft Install System (NSIS) and one for InnoSetup.  The InnoSetup one is the one that 
is up to date, but chances are good that you'll have to fiddle with some paths (your VS
project will probably put files into different places, and some things have probably beeen moved).
That folder also includes prebuilt binaries, but those are likely out of date.

This software is released under the terms of the MIT License.

## Development Status

This application is no longer maintained, but I am willing to accept pull requests if you
have improvements. There is definitely room for improvement, since it makes some
assumptions about paths and development environment that are specific to my system.
