!include "MUI.nsh"

!define MUI_ABORTWARNING
!define MUI_COMPONENTSPAGE_SMALLDESC

!define MUI_HEADERIMAGE
!define MUI_HEADERIMAGE_BITMAP "${NSISDIR}\Contrib\Graphics\Header\orange.bmp"

!define MUI_WELCOMEFINISHPAGE_BITMAP "${NSISDIR}\Contrib\Graphics\Wizard\orange.bmp"
!define MUI_UNWELCOMEFINISHPAGE_BITMAP "${NSISDIR}\Contrib\Graphics\Wizard\orange.bmp"

!define MUI_ICON "${NSISDIR}\Contrib\Graphics\Icons\orange-install.ico"
!define MUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\orange-uninstall.ico"

!define VERSION "0.9.50"
!define PRODUCT_VERSION "0.9.50.0"

; Where to find stuff on the system the package is being built? These
; variables DO NOT set any install destination paths.
!define KNIPPTASCH_SRC_PATH "..\" ; relative to the installer script
!define KNIPPTASCH_INSTALL_PATH "${KNIPPTASCH_SRC_PATH}\install\" ; == CMAKE_INSTALL_PREFIX
!define QT_INSTALL_PATH "\qt\4.7.0\"

Name "Knipptasch ${VERSION}"
OutFile "knipptasch-${VERSION}-setup.exe"
InstallDir "$PROGRAMFILES\Knipptasch\"

BrandingText " "

SetCompressor /FINAL bzip2

; Registry key to check for directory (so if you install again, it will 
; overwrite the old one automatically)
InstallDirRegKey HKLM "SoftwareKnipptasch" "Install_Dir"

; ---------------------------
; Pages
; ---------------------------

!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE "..\COPYING"

!insertmacro MUI_PAGE_COMPONENTS
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH
  
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_UNPAGE_FINISH

; ---------------------------
; Languages
; ---------------------------
 
!insertmacro MUI_LANGUAGE "English"
 
LangString knipptasch ${LANG_ENGLISH} "Knipptasch program files"
LangString DESC_knipptasch ${LANG_ENGLISH} "The program files required for Knipptasch."

LangString qt_dlls ${LANG_ENGLISH} "Qt libraries Knipptasch needs to run"
LangString DESC_qt_dlls ${LANG_ENGLISH} "If you have Qt 4.7 or newer installed, you don't need to install this."

LangString startmenu ${LANG_ENGLISH} "Start menu entry"
LangString DESC_startmenu ${LANG_ENGLISH} "Create a start menu entry for Knipptasch."

LangString desktopicon ${LANG_ENGLISH} "Desktop icon"
LangString DESC_desktopicon ${LANG_ENGLISH} "Create an icon on the desktop for Knipptasch."

; ---------------------------
; Version stuff
; ---------------------------

VIProductVersion "${PRODUCT_VERSION}"

VIAddVersionKey /LANG=${LANG_ENGLISH} "ProductName" "Knipptasch ${VERSION} Setup"
VIAddVersionKey /LANG=${LANG_ENGLISH} "Comments" "Installs Knipptasch ${VERSION} on your computer."
VIAddVersionKey /LANG=${LANG_ENGLISH} "CompanyName" "Stefan Böhmann"
VIAddVersionKey /LANG=${LANG_ENGLISH} "LegalTrademarks" ""
VIAddVersionKey /LANG=${LANG_ENGLISH} "LegalCopyright" "2010 Stefan Böhmann"
VIAddVersionKey /LANG=${LANG_ENGLISH} "FileDescription" "Knipptasch ${VERSION} Setup"
VIAddVersionKey /LANG=${LANG_ENGLISH} "FileVersion" "${VERSION}"


; ---------------------------
; Callbacks
; ---------------------------

Function .onInit
	!insertmacro MUI_LANGDLL_DISPLAY
FunctionEnd

Function un.onInit
	!insertmacro MUI_LANGDLL_DISPLAY
FunctionEnd


; ---------------------------
; The stuff to install
; ---------------------------

Section $(knipptasch) section_knipptasch
	SectionIn RO
  
	SetOutPath "$INSTDIR"
	File "${KNIPPTASCH_INSTALL_PATH}\bin\*.exe"
	File "${KNIPPTASCH_INSTALL_PATH}\bin\*.dll"
	File "${KNIPPTASCH_SRC_PATH}\README"
	File "${KNIPPTASCH_SRC_PATH}\COPYING"

	File "${KNIPPTASCH_INSTALL_PATH}\share\apps\knipptasch\init.sql"

	; TRANSLATIONS
 	SetOutPath "$INSTDIR\translations"
	File "${KNIPPTASCH_INSTALL_PATH}\share\apps\knipptasch\translations\*"

	; MARBLE
 	SetOutPath "$INSTDIR\marble\data\maps\earth\openstreetmap"
	File /r "${MARBLE_INSTALL_PATH}\data\maps\earth\openstreetmap\*"

 	SetOutPath "$INSTDIR\marble\plugins"
	File "${MARBLE_INSTALL_PATH}\plugins\QNamNetworkPlugin.dll"
	
	; Write the installation path into the registry
	WriteRegStr HKLM "Software\Knipptasch" "Install_Dir" "$INSTDIR"
  
	; Write the uninstall keys for Windows
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Knipptasch" "DisplayName" "Knipptasch ${VERSION}"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Knipptasch" "UninstallString" '"$INSTDIR\uninstall.exe"'
	WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Knipptasch" "NoModify" 1
	WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Knipptasch" "NoRepair" 1
	WriteUninstaller "uninstall.exe"
SectionEnd

Section $(qt_dlls) section_qt_dlls
	SetOutPath "$INSTDIR"
	File "${QT_INSTALL_PATH}\bin\qtcore4.dll"
	File "${QT_INSTALL_PATH}\bin\qtgui4.dll"
	File "${QT_INSTALL_PATH}\bin\qtxml4.dll"
	File "${QT_INSTALL_PATH}\bin\qtsql4.dll"
	File "${QT_INSTALL_PATH}\bin\qtsvg4.dll"
	File "${QT_INSTALL_PATH}\bin\qtnetwork4.dll"
	File "${QT_INSTALL_PATH}\bin\qtwebkit4.dll"
	File "${QT_INSTALL_PATH}\bin\phonon4.dll"
	
	SetOutPath "$INSTDIR\sqldrivers"
	File "${QT_INSTALL_PATH}\plugins\sqldrivers\qsqlite4.dll"
SectionEnd

Section $(startmenu) section_startmenu
	CreateDirectory "$SMPROGRAMS\Knipptasch"
	CreateShortCut "$SMPROGRAMS\Knipptasch\Knipptasch.lnk" "$INSTDIR\knipptasch.exe" "" "$INSTDIR\knipptasch.exe" 0
	CreateShortCut "$SMPROGRAMS\Knipptasch\README.lnk" "$INSTDIR\README" "" "$INSTDIR\README" 0
	CreateShortCut "$SMPROGRAMS\Knipptasch\COPYING.lnk" "$INSTDIR\COPYING" "" "$INSTDIR\COPYING" 0
	CreateShortCut "$SMPROGRAMS\Knipptasch\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
SectionEnd

Section $(desktopicon) section_desktopicon
	CreateShortCut "$DESKTOP\Knipptasch.lnk" "$INSTDIR\knipptasch.exe" "" "$INSTDIR\knipptasch.exe" 0
SectionEnd

; ---------------------------
; Uninstaller
; ---------------------------

Section "Uninstall"
	; Remove registry keys
	DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Knipptasch"
	DeleteRegKey HKLM SOFTWARE\Knipptasch

	; startmenu and desktop icon
	Delete "$SMPROGRAMS\Knipptasch\*.*"
	Delete "$DESKTOP\Knipptasch.lnk"
	
	; Remove files and uninstaller
	Delete "$INSTDIR\uninstall.exe"
  
	Delete "$INSTDIR\*.exe"
	Delete "$INSTDIR\README"
	Delete "$INSTDIR\COPYING"
	Delete "$INSTDIR\*.dll"
	Delete "$INSTDIR\init.sql"

	; Remove directories used
	RMDir "$SMPROGRAMS\Knipptasch"

	RMDir /r "$INSTDIR\sqldrivers"
	RMDir /r "$INSTDIR\translations"
	RMDir /r "$INSTDIR\marble"

	RMDir "$INSTDIR"

SectionEnd


; ---------------------------
; Section descriptions
; ---------------------------

!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
	!insertmacro MUI_DESCRIPTION_TEXT ${section_knipptasch} $(DESC_knipptasch)
	!insertmacro MUI_DESCRIPTION_TEXT ${section_qt_dlls} $(DESC_qt_dlls)
	!insertmacro MUI_DESCRIPTION_TEXT ${section_startmenu} $(DESC_startmenu)
	!insertmacro MUI_DESCRIPTION_TEXT ${section_desktopicon} $(DESC_desktopicon)
!insertmacro MUI_FUNCTION_DESCRIPTION_END

