echo This program runs the desktop app packager command to create a Windows 10 store package from an existing installer.
pause
DesktopAppConverter.exe -Installer "C:\Users\Xangis\code\SpaceTheremin\installer\SpaceTheremin1.1Setup.exe" -PackageName "SpaceTheremin" -Version 1.1.0.0 -Makeappx -Destination "C:\Users\Xangis\code\SpaceTheremin\installer" -InstallerArguments "/VERYSILENT" -Publisher "CN=Jason Champion" -Sign
Certutil -addStore TrustedPeople SpaceTheremin\auto-generated.cer



