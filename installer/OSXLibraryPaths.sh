#!/bin/bash
install_name_tool -change /usr/local/lib/libportaudio.2.dylib @executable_path/../Frameworks/libportaudio.2.dylib SpaceTheremin.app/Contents/MacOS/SpaceTheremin
otool -L SpaceTheremin.app/Contents/MacOS/SpaceTheremin
