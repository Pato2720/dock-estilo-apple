#!/bin/sh
# Banco de pruebas de "Menús estilo Apple" en Wine: compila menu_muestra.exe
# (que incluye el mod tal cual), comprueba la lógica y dibuja menús de
# muestra en out/menu-*.bgra (a PNG con menus_png.py).
# Requiere: wine64 y pip install ziglang. Las muestras quedan en png/menus.png.
cd "$(dirname "$0")"
python3 -m ziglang c++ -target x86_64-windows-gnu -std=c++23 -O2 -municode \
  -DUNICODE -D_UNICODE -DWINVER=0x0A00 -U_WIN32_WINNT -D_WIN32_WINNT=0x0A00 \
  -Wall -Wextra -Wno-unused-parameter -Wno-missing-field-initializers -Wno-unused-function \
  -o menu_muestra.exe menu_muestra.cpp \
  -luxtheme -ldwmapi -lgdi32 -luser32 -ladvapi32 > build-menus.log 2>&1 || { echo "NO COMPILA"; grep error build-menus.log | head; exit 1; }
export WINEDEBUG=-all WINEPREFIX=${WINEPREFIX:-$HOME/.wine-dock}
mkdir -p out
timeout 120 wine menu_muestra.exe
