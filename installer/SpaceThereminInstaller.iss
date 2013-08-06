; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{DC6D93EB-D7E2-4309-B634-7F2A2E50D1D9}
AppName=SpaceTheremin
AppVerName=SpaceTheremin 1.01
AppPublisher=Zeta Centauri, Inc.
AppPublisherURL=http://zetacentauri.com
AppSupportURL=http://zetacentauri.com
AppUpdatesURL=http://zetacentauri.com
DefaultDirName={pf}\Zeta Centauri\SpaceTheremin
DefaultGroupName=Zeta Centauri\SpaceTheremin
LicenseFile=K:\src\_shelvedProjects\SpaceTheremin\Release\License.txt
OutputDir=K:\src\_shelvedProjects\SpaceTheremin\Release
OutputBaseFilename=SpaceTheremin1.01Setup
Compression=lzma
SolidCompression=yes

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked
Name: "quicklaunchicon"; Description: "{cm:CreateQuickLaunchIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "K:\src\_shelvedProjects\SpaceTheremin\Release\SpaceTheremin.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "K:\src\_shelvedProjects\SpaceTheremin\Release\alut.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "K:\src\_shelvedProjects\SpaceTheremin\Release\background.jpg"; DestDir: "{app}"; Flags: ignoreversion
Source: "K:\src\_shelvedProjects\SpaceTheremin\Release\info.xpm"; DestDir: "{app}"; Flags: ignoreversion
;Source: "F:\src\zc\SpaceTheremin\Release\OpenAL32.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "K:\src\_shelvedProjects\SpaceTheremin\Release\theremin.ico"; DestDir: "{app}"; Flags: ignoreversion
;Source: "F:\src\zc\SpaceTheremin\Release\wrap_oal.dll"; DestDir: "{app}"; Flags: ignoreversion
; NOTE: Don't use "Flags: ignoreversion" on any shared system files
Source: "K:\src\_shelvedProjects\SpaceTheremin\Release\vcredist_x86.exe"; DestDir: "{app}"; Flags: ignoreversion deleteafterinstall
Source: "K:\src\_shelvedProjects\SpaceTheremin\Release\oalinst.exe"; DestDir: "{app}"; Flags: ignoreversion deleteafterinstall

[Icons]
Name: "{group}\SpaceTheremin"; Filename: "{app}\SpaceTheremin.exe"; WorkingDir: "{app}"
Name: "{group}\{cm:ProgramOnTheWeb,SpaceTheremin}"; Filename: "http://zetacentauri.com/software_spacetheremin.htm"
Name: "{commondesktop}\SpaceTheremin"; Filename: "{app}\SpaceTheremin.exe"; Tasks: desktopicon; WorkingDir: "{app}"
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\SpaceTheremin"; Filename: "{app}\SpaceTheremin.exe"; Tasks: quicklaunchicon; WorkingDir: "{app}"

[Run]
Filename: "{app}\vcredist_x86.exe"; Parameters: "/q"; WorkingDir: "{app}";  StatusMsg: "Installing Visual C++ 2008 Redistributable..."; Flags: waituntilterminated
Filename: "{app}\oalinst.exe"; Parameters: "/s"; WorkingDir: "{app}";  StatusMsg: "Installing OpenAL..."; Flags: waituntilterminated
Filename: "{app}\SpaceTheremin.exe"; Description: "{cm:LaunchProgram,SpaceTheremin}"; Flags: nowait postinstall skipifsilent
