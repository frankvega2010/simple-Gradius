:: Copiador de Assets para el .exe // Franco Vega

:: %1 %2

@echo off
echo Copiador de Assets para el .exe by Franco Vega
echo Copiando Archivos de Assets....

set targetdir=%1
set solutiondir=%2

MKDIR "%targetdir%res/assets"
XCOPY "%solutiondir%res/assets" "%targetdir%res/assets" /E /Y /S

echo Copia de Assets terminada!