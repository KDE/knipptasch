!include "MUI.nsh"

!define MUI_ABORTWARNING
!define MUI_COMPONENTSPAGE_SMALLDESC

!define MUI_HEADERIMAGE
!define MUI_HEADERIMAGE_BITMAP "${NSISDIR}\Contrib\Graphics\Header\orange.bmp"

!define MUI_WELCOMEFINISHPAGE_BITMAP "${NSISDIR}\Contrib\Graphics\Wizard\orange.bmp"
!define MUI_UNWELCOMEFINISHPAGE_BITMAP "${NSISDIR}\Contrib\Graphics\Wizard\orange.bmp"

!define MUI_ICON "${NSISDIR}\Contrib\Graphics\Icons\orange-install.ico"
!define MUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\orange-uninstall.ico"

!define VERSION "@knipptasch_MAJOR_VERSION@.@knipptasch_MINOR_VERSION@.@knipptasch_PATCH_LEVEL@"
!define PRODUCT_VERSION "@knipptasch_MAJOR_VERSION@.@knipptasch_MINOR_VERSION@.@knipptasch_PATCH_LEVEL@.0"

; Where to find stuff on the system the package is being built? These
; variables DO NOT set any install destination paths.
!define KNIPPTASCH_SRC_PATH "..\" ; relative to the installer script
!define KNIPPTASCH_INSTALL_PATH "@CMAKE_INSTALL_PREFIX@"
!define QT_INSTALL_PATH "@QT_BINARY_DIR@\..\"

Name "Knipptasch ${VERSION}"
OutFile "knipptasch-${VERSION}-setup.exe"
InstallDir "$PROGRAMFILES\Knipptasch\"

BrandingText " "

SetCompressor /SOLID lzma

; Registry key to check for directory (so if you install again, it will
; overwrite the old one automatically)
InstallDirRegKey HKLM "SoftwareKnipptasch" "Install_Dir"

; ---------------------------
; Pages
; ---------------------------

!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE "${KNIPPTASCH_SRC_PATH}\COPYING"

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
        File "${KNIPPTASCH_INSTALL_PATH}\knipptasch.exe"
        File "${KNIPPTASCH_SRC_PATH}\AUTHORS"
	File "${KNIPPTASCH_SRC_PATH}\COPYING"

	; TRANSLATIONS
 	SetOutPath "$INSTDIR\translations"
        File "${KNIPPTASCH_INSTALL_PATH}\translations\*"

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
        SetOutPath "$INSTDIR\imageformats"
        File "${KNIPPTASCH_INSTALL_PATH}\imageformats\*.dll"

	SetOutPath "$INSTDIR"
        File "${KNIPPTASCH_INSTALL_PATH}\qt*.dll"
SectionEnd

Section $(startmenu) section_startmenu
	CreateDirectory "$SMPROGRAMS\Knipptasch"
	CreateShortCut "$SMPROGRAMS\Knipptasch\Knipptasch.lnk" "$INSTDIR\knipptasch.exe" "" "$INSTDIR\knipptasch.exe" 0
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

	Delete "$INSTDIR\*.exe"
        Delete "$INSTDIR\AUTHORS"
	Delete "$INSTDIR\COPYING"
	Delete "$INSTDIR\*.dll"
        Delete "$INSTDIR\*.url"

	; Remove directories used
	RMDir "$SMPROGRAMS\Knipptasch"

	RMDir /r "$INSTDIR\sqldrivers"
	RMDir /r "$INSTDIR\translations"
        RMDir /r "$INSTDIR\imageformats"

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
