#!/bin/sh
# Compila el mod como lo hace Windhawk (clang + mingw-w64, x86-64, DLL) para
# revisar errores y advertencias fuera de Windhawk. Requiere: pip install ziglang
set -e
cd "$(dirname "$0")"
SRC=${SRC:-../../dock-estilo-apple.wh.cpp}
STD=${STD:-c++23}
python3 -m ziglang dlltool -d whapi.def -l libwhapi.a -m i386:x86-64
python3 -m ziglang c++ -target x86_64-windows-gnu -std=$STD -O2 -shared \
  -DUNICODE -D_UNICODE -DWINVER=0x0A00 -U_WIN32_WINNT -D_WIN32_WINNT=0x0A00 \
  -include windhawk_api.h -I. \
  -Wall -Wextra -Wno-unused-parameter -Wno-missing-field-initializers \
  -o dock.dll "$SRC" -Wl,--export-all-symbols \
  -L. -lwhapi -lole32 -loleaut32 -lshell32 -lgdi32 -luser32 -ladvapi32 -ldwmapi -lgdiplus -lshcore -lversion -luuid
echo "Listo: dock.dll"
