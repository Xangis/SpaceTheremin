# SpaceTheremin

A virtual theremin controlled via the mouse or touchscreen. This app was originally
released for Windows on http://zetacentauri.com and later for OSX via the Apple Store.
Across all versions it has been downloaded more than 65,000 times.

An prebuilt installable version for Windows is available in the installer folder.

![SpaceTheremin Screenshot](https://github.com/Xangis/SpaceTheremin/blob/master/images/spacetheremin1.png)

This application has been tested on Windows, OSX and Linux, both 32-bit and 64-bit.
It is written in C++ and uses wxWidgets for the user interface. On Linux it also
requires JACK. It works with wxWidgets 3.0 and 3.1.

A makefile is included for Linux. If your wx-config is in a different location
than that in the Makfile, change the path at the top of the Makefile.

On Windows, you'll have to create your own Visual Studio project that uses all of
the .h and .cpp files. That should be easy.  Setting paths and libraries in Visual
Studio is the most annoying part of creating a project, and wxWidgets is a particular
nuisance in that respect.

On OSX, you'll need to run the OSXLibraryPaths.sh to set up the correct library paths
for the application.

Installers for Windows are under the "installer" folder.  There is one for the Nullsoft
Install System (NSIS) and one for InnoSetup.  The InnoSetup one is the one that is up to
date, but chances are good that you'll have to fiddle with some paths (your VS project will 
probably put files into different places, and some things have probably beeen moved).
That folder also includes prebuilt binaries, but those are likely out of date.

This software is released under the terms of the MIT License.

## Development Status

This application is no longer maintained, but I am willing to accept pull requests if you
have improvements. There is definitely room for improvement, since it makes some
assumptions about paths and development environment that is specific to me.
