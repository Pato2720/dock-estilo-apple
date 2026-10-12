#!/bin/sh
# Banco de pruebas en Wine: compila test_host.exe (que incluye el mod tal
# cual) y recorre los escenarios con el mouse simulado.
# Requiere: wine64, Xvfb y pip install ziglang. Usa DISPLAY=:99:
#   Xvfb :99 -screen 0 1600x900x24 +extension Composite &
# El fondo de prueba (fondo.png) se copia a mano como
#   $WINEPREFIX/drive_c/users/$USER/AppData/Roaming/Microsoft/Windows/Themes/TranscodedWallpaper
cd "$(dirname "$0")"
python3 -m ziglang c++ -target x86_64-windows-gnu -std=c++23 -O2 -municode \
  -DUNICODE -D_UNICODE -DWINVER=0x0A00 -U_WIN32_WINNT -D_WIN32_WINNT=0x0A00 \
  -Wall -Wextra -Wno-unused-parameter -Wno-missing-field-initializers -Wno-unused-function \
  -o test_host.exe test_host.cpp \
  -lole32 -loleaut32 -lshell32 -lgdi32 -luser32 -ladvapi32 -ldwmapi -lgdiplus -lshcore -lversion -luuid > build.log 2>&1 || { echo "NO COMPILA"; grep error build.log | head; exit 1; }
export DISPLAY=${DISPLAY:-:99} WINEDEBUG=-all WINEPREFIX=${WINEPREFIX:-$HOME/.wine-dock}
wineserver -k 2>/dev/null; sleep 1; rm -rf out
timeout 600 wine test_host.exe > run.log 2>&1
grep -E "^\s+\[(OK  |FALLA)\]|^==|Resultado|fps" run.log
