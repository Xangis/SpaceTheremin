#!/bin/bash
codesign -f --deep -s "3rd Party Mac Developer Application: Jason Champion (G6PFF85X2R)" SpaceTheremin.app --entitlements SpaceTheremin.app/Contents/Resources/Entitlements.plist
productbuild --sign "3rd Party Mac Developer Installer: Jason Champion (G6PFF85X2R)" --component SpaceTheremin.app /Applications SpaceTheremin.pkg
