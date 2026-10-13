#!/bin/sh
# Banco de pruebas de "Consulta rápida" en Wine: levanta un servidor falso
# compatible con OpenAI, compila prueba.cpp (que incluye el mod tal cual) y
# lo corre. Las imágenes quedan en out/*.bgra (a PNG con a_png.py).
# Requiere: wine64, Xvfb (DISPLAY=:99), python3 y pip install ziglang.
cd "$(dirname "$0")"
python3 -m ziglang c++ -target x86_64-windows-gnu -std=c++23 -O2 -municode \
  -DUNICODE -D_UNICODE -DWINVER=0x0A00 -U_WIN32_WINNT -D_WIN32_WINNT=0x0A00 \
  -Wall -Wextra -Wno-unused-parameter -Wno-missing-field-initializers -Wno-unused-function \
  -o prueba.exe prueba.cpp \
  -lwinhttp -lole32 -loleaut32 -luuid -ldwmapi -lgdi32 -luser32 -ladvapi32 > build.log 2>&1 || { echo "NO COMPILA"; grep error build.log | head; exit 1; }
python3 servidor_falso.py 8765 & SERVER=$!
sleep 1
export DISPLAY=${DISPLAY:-:99} WINEDEBUG=-all WINEPREFIX=${WINEPREFIX:-$HOME/.wine-dock}
mkdir -p out
timeout 300 wine prueba.exe
R=$?
kill $SERVER 2>/dev/null
rm -f ultimo.json
exit $R
