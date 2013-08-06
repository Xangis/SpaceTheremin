;Include Modern UI

  !include "MUI2.nsh"
  !include "FileAssociation.nsh"

Name "SpaceTheremin 1.01"
OutFile "SpaceTheremin1.01Setup.exe"
InstallDir "$PROGRAMFILES\Zeta Centauri\SpaceTheremin"
InstallDirRegKey HKLM "Software\SpaceTheremin" "Install_Dir"
RequestExecutionLevel admin
!define MUI_ICON "theremin.ico"
!define MUI_UNICON "theremin.ico"

;Version Information

  VIProductVersion "1.0.1.0"
  VIAddVersionKey /LANG=${LANG_ENGLISH} "ProductName" "SpaceTheremin"
;  VIAddVersionKey /LANG=${LANG_ENGLISH} "Comments" "A test comment"
  VIAddVersionKey /LANG=${LANG_ENGLISH} "CompanyName" "Zeta Centauri"
;  VIAddVersionKey /LANG=${LANG_ENGLISH} "LegalTrademarks" "Test Application is a trademark of Fake company"
  VIAddVersionKey /LANG=${LANG_ENGLISH} "LegalCopyright" "Copyright 2007-2012 Zeta Centauri"
  VIAddVersionKey /LANG=${LANG_ENGLISH} "FileDescription" "SpaceTheremin Installer"
  VIAddVersionKey /LANG=${LANG_ENGLISH} "FileVersion" "1.0.1.0"
  VIAddVersionKey /LANG=${LANG_ENGLISH} "ProductVersion" "1.0.1.0"

;Interface Settings

  !define MUI_ABORTWARNING

;Pages

  !insertmacro MUI_PAGE_LICENSE "License.txt"
;  !insertmacro MUI_PAGE_DIRECTORY
  !insertmacro MUI_PAGE_INSTFILES
      !define MUI_FINISHPAGE_NOAUTOCLOSE
      !define MUI_FINISHPAGE_RUN
      !define MUI_FINISHPAGE_RUN_CHECKED
      !define MUI_FINISHPAGE_RUN_TEXT "Launch SpaceTheremin"
      !define MUI_FINISHPAGE_RUN_FUNCTION "LaunchProgram"
      !define MUI_FINISHPAGE_SHOWREADME ""
      !define MUI_FINISHPAGE_SHOWREADME_NOTCHECKED
      !define MUI_FINISHPAGE_SHOWREADME_TEXT "Create Desktop Shortcut"
      !define MUI_FINISHPAGE_SHOWREADME_FUNCTION finishpageaction
  !insertmacro MUI_PAGE_FINISH
  
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES

;Languages
 
  !insertmacro MUI_LANGUAGE "English"

; The stuff to install
Section "SpaceTheremin"

  SectionIn RO
  
  ; Set output path to the installation directory.
  SetOutPath $INSTDIR
  
  ; Put file there
  File "SpaceTheremin.exe"
  File "License.txt"
  File "theremin.ico"
  File "background.jpg"
  File "info.xpm"
  File "alut.dll"
  File "vcredist_x86.exe"
  File "oalinst.exe"

  ; Write the installation path into the registry
  WriteRegStr HKLM SOFTWARE\SpaceTheremin "Install_Dir" "$INSTDIR"
  
  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\SpaceTheremin" "DisplayName" "SpaceTheremin"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\SpaceTheremin" "DisplayVersion" "1.01"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\SpaceTheremin" "Publisher" "Zeta Centauri"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\SpaceTheremin" "DisplayIcon" "$INSTDIR\theremin.ico"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\SpaceTheremin" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\SpaceTheremin" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\SpaceTheremin" "NoRepair" 1
  WriteUninstaller "uninstall.exe"

  ExecWait '"$INSTDIR\vcredist_x86.exe /q"' 
  ExecWait '"$INSTDIR\oalinst.exe"' 

SectionEnd

; Optional section (can be disabled by the user)
Section "Start Menu Shortcuts"

  CreateDirectory "$SMPROGRAMS\Zeta Centauri\SpaceTheremin"
  CreateShortCut "$SMPROGRAMS\Zeta Centauri\SpaceTheremin\SpaceTheremin.lnk" "$INSTDIR\SpaceTheremin.exe" "" "" 0
  ;CreateShortCut "$SMPROGRAMS\Zeta Centauri\SpaceTheremin\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
  WriteINIStr "$SMPROGRAMS\Zeta Centauri\SpaceTheremin\SpaceTheremin Website.url" "InternetShortcut" "URL" "http://zetacentauri.com/software_spacetheremin.htm"
  
SectionEnd

; Uninstaller

Section "Uninstall"
  
  ; Remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\SpaceTheremin"
  DeleteRegKey HKLM SOFTWARE\SpaceTheremin

  ; Remove files and uninstaller
  Delete $INSTDIR\SpaceTheremin.exe
  Delete $INSTDIR\License.txt
  Delete $INSTDIR\theremin.ico
  Delete $INSTDIR\background.jpg
  Delete $INSTDIR\info.xpm
  Delete $INSTDIR\alut.dll
  Delete $INSTDIR\uninstall.exe

  ; Remove shortcuts, if any
  Delete "$SMPROGRAMS\Zeta Centauri\SpaceTheremin\*.*"
  Delete "$DESKTOP\SpaceTheremin.lnk"
  Delete "$SMPROGRAMS\Zeta Centauri\SpaceTheremin\SpaceTheremin Website.url"
  ;DeleteINISec "$SMPROGRAMS\Zeta Centauri\SpaceTheremin\SpaceTheremin Website.url" "InternetShortcut"

  ; Remove directories used
  RMDir "$SMPROGRAMS\Zeta Centauri\SpaceTheremin"
  RMDir "$SMPROGRAMS\Zeta Centauri"
  RMDir "$INSTDIR"


SectionEnd

; This function is automatically executed when the installer finishes
Function .onGUIEnd
	; Un-register the offer manager plugin with the operating system
	UnRegDLL $PLUGINSDIR\BunndleOfferManager.reg.dll
        Delete $INSTDIR\vcredist_x86.exe
        Delete $INSTDIR\oalinst.exe
FunctionEnd

Function LaunchProgram
  ExecShell "" "$SMPROGRAMS\Zeta Centauri\SpaceTheremin\SpaceTheremin.lnk"
FunctionEnd

Function finishpageaction
  CreateShortcut "$DESKTOP\SpaceTheremin.lnk" "$INSTDIR\SpaceTheremin.exe"
FunctionEnd
