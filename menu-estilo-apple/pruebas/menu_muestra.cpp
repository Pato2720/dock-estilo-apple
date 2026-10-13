// Banco de pruebas de "Menús estilo Apple": carga el código del mod tal cual,
// comprueba la lógica (qué temas son de menú, el menú clásico del Explorador)
// y dibuja menús de muestra con los mismos ganchos que usa el mod, para
// revisar el aspecto sin Windows (en Wine los menús no usan uxtheme).
#define NOMINMAX
#include <windows.h>
#include <cstdarg>
#include <cstdio>
#include <map>
#include <string>

// ---- API de Windhawk simulada ----
static std::map<std::wstring, std::wstring> g_testSettings;
static std::map<std::wstring, int> g_testStore;

static void TestLog(const wchar_t* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    wchar_t buf[1024];
    _vsnwprintf(buf, 1023, fmt, ap);
    buf[1023] = 0;
    va_end(ap);
    fwprintf(stdout, L"[mod] %ls\n", buf);
}
#define Wh_Log(message, ...) TestLog(L"[%d:%hs]: " message, __LINE__, __FUNCTION__, ##__VA_ARGS__)

int Wh_GetIntSetting(PCWSTR name, ...) { return _wtoi(g_testSettings[name].c_str()); }
PCWSTR Wh_GetStringSetting(PCWSTR name, ...) { return _wcsdup(g_testSettings[name].c_str()); }
void Wh_FreeStringSetting(PCWSTR s) { free((void*)s); }
int Wh_GetIntValue(PCWSTR name, int def) { return g_testStore.count(name) ? g_testStore[name] : def; }
BOOL Wh_SetIntValue(PCWSTR name, int v) {
    g_testStore[name] = v;
    return TRUE;
}
// Sin enganchar de verdad: el "original" es la función misma.
BOOL Wh_SetFunctionHook(void* target, void* hook, void** original) {
    *original = target;
    return TRUE;
}

// ---- El mod, sin cambios ----
#include "../menu-estilo-apple.wh.cpp"

static int g_pass = 0, g_fail = 0;
static void Check(bool ok, const char* what) {
    printf("  [%s] %s\n", ok ? "OK  " : "FALLA", what);
    (ok ? g_pass : g_fail)++;
}

static bool KeyExists(const std::wstring& sub) {
    HKEY k = nullptr;
    if (RegOpenKeyExW(HKEY_CURRENT_USER, sub.c_str(), 0, KEY_READ, &k) != ERROR_SUCCESS) return false;
    RegCloseKey(k);
    return true;
}

// --- Menú de muestra: filas como las de un menú del escritorio ---
struct Row {
    const wchar_t* text;
    const wchar_t* shortcut;
    int state;     // 1 normal, 2 resaltada, 3 desactivada
    int check;     // 0 nada, 1 palomita, 3 punto
    bool submenu;
    bool sep;
};

static const Row kRows[] = {
    {L"Ver", nullptr, 1, 0, true, false},
    {L"Ordenar por", nullptr, 1, 0, true, false},
    {L"Actualizar", nullptr, 1, 0, false, false},
    {nullptr, nullptr, 0, 0, false, true},
    {L"Deshacer Eliminar", L"Ctrl+Z", 3, 0, false, false},
    {L"Pegar", L"Ctrl+V", 2, 0, false, false},
    {L"Pegar acceso directo", nullptr, 3, 0, false, false},
    {nullptr, nullptr, 0, 0, false, true},
    {L"Mostrar iconos del escritorio", nullptr, 1, 1, false, false},
    {L"Iconos medianos", nullptr, 1, 3, false, false},
    {L"Nuevo", nullptr, 1, 0, true, false},
    {nullptr, nullptr, 0, 0, false, true},
    {L"Personalizar", nullptr, 1, 0, false, false},
};

// Dibuja el menú en un DIB con los ganchos del mod y lo guarda (BGRA).
static void DrawSample(const char* path, HTHEME theme, bool dark) {
    const int W = 280, rowH = 26, sepH = 9, pad = 4, gutter = 30;
    int H = 2 * pad;
    for (auto& r : kRows) H += r.sep ? sepH : rowH;
    BITMAPINFO bi{};
    bi.bmiHeader.biSize = sizeof(bi.bmiHeader);
    bi.bmiHeader.biWidth = W;
    bi.bmiHeader.biHeight = -H;
    bi.bmiHeader.biPlanes = 1;
    bi.bmiHeader.biBitCount = 32;
    bi.bmiHeader.biCompression = BI_RGB;
    void* bits = nullptr;
    HDC dc = CreateCompatibleDC(nullptr);
    HBITMAP dib = CreateDIBSection(dc, &bi, DIB_RGB_COLORS, &bits, nullptr, 0);
    HGDIOBJ old = SelectObject(dc, dib);
    HFONT font = CreateFontW(-15, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, 0, 0, ANTIALIASED_QUALITY, 0,
                             L"Tahoma");
    HGDIOBJ oldFont = SelectObject(dc, font);
    SetBkMode(dc, TRANSPARENT);

    RECT all{0, 0, W, H};
    DrawThemeBackground_Hook(theme, dc, kPopupBackground, 0, &all, nullptr);
    RECT gutterRc{pad, pad, pad + gutter - 4, H - pad};
    DrawThemeBackground_Hook(theme, dc, kPopupGutter, 0, &gutterRc, nullptr);
    int y = pad;
    for (auto& r : kRows) {
        if (r.sep) {
            RECT rc{pad + gutter, y, W - pad, y + sepH};
            DrawThemeBackground_Hook(theme, dc, kPopupSeparator, 0, &rc, nullptr);
            y += sepH;
            continue;
        }
        RECT rc{pad, y, W - pad, y + rowH};
        DrawThemeBackground_Hook(theme, dc, kPopupItem, r.state, &rc, nullptr);
        if (r.check) {
            RECT cr{pad + 4, y + 3, pad + 4 + 20, y + rowH - 3};
            DrawThemeBackground_Hook(theme, dc, kPopupCheckBackground, 1, &cr, nullptr);
            DrawThemeBackground_Hook(theme, dc, kPopupCheck, r.check, &cr, nullptr);
        }
        // El texto lo pinta Windows con el color que le pasa el mod.
        SetTextColor(dc, ItemTextColor(r.state, dark, false));
        RECT tr{pad + gutter, y, W - pad - 28, y + rowH};
        DrawTextW(dc, r.text, -1, &tr, DT_SINGLELINE | DT_VCENTER | DT_LEFT);
        if (r.shortcut) {
            // El atajo va aparte, alineado a la derecha (como lo hace Windows).
            SetTextColor(dc, ItemTextColor(r.state, dark, true));
            DrawTextW(dc, r.shortcut, -1, &tr, DT_SINGLELINE | DT_VCENTER | DT_RIGHT);
        }
        if (r.submenu) {
            RECT sr{W - pad - 24, y, W - pad - 4, y + rowH};
            DrawThemeBackground_Hook(theme, dc, kPopupSubmenu, 1, &sr, nullptr);
        }
        y += rowH;
    }
    GdiFlush();
    FILE* f = fopen(path, "wb");
    if (f) {
        int hdr[2] = {W, H};
        fwrite(hdr, sizeof(hdr), 1, f);
        fwrite(bits, 4, (size_t)W * H, f);
        fclose(f);
    }
    SelectObject(dc, oldFont);
    DeleteObject(font);
    SelectObject(dc, old);
    DeleteObject(dib);
    DeleteDC(dc);
}

int wmain() {
    setvbuf(stdout, nullptr, _IONBF, 0);
    auto defaults = [] {
        g_testSettings.clear();
        g_testSettings[L"theme"] = L"auto";
        g_testSettings[L"palette"] = L"graphite";
        g_testSettings[L"highlight"] = L"gray";
        g_testSettings[L"highlightColor"] = L"#0A84FF";
        g_testSettings[L"highlightIntensity"] = L"100";
        g_testSettings[L"highlightShape"] = L"pill";
        g_testSettings[L"highlightRadius"] = L"6";
        g_testSettings[L"highlightMargin"] = L"5";
        g_testSettings[L"separator"] = L"line";
        g_testSettings[L"checkStyle"] = L"check";
        g_testSettings[L"arrowStyle"] = L"chevron";
        g_testSettings[L"dimShortcuts"] = L"1";
        g_testSettings[L"showGutter"] = L"0";
        g_testSettings[L"corners"] = L"round";
        g_testSettings[L"border"] = L"1";
        g_testSettings[L"menuBar"] = L"1";
        g_testSettings[L"animation"] = L"system";
        g_testSettings[L"submenuDelay"] = L"-1";
        g_testSettings[L"accessKeys"] = L"0";
        g_testSettings[L"classicExplorer"] = L"0";
        LoadSettings();
    };
    defaults();
    Check(Wh_ModInit(), "Wh_ModInit engancha uxtheme");

    printf("== Clases de tema ==\n");
    Check(IsMenuClassList(L"Menu"), "\"Menu\" es de menú");
    Check(IsMenuClassList(L"ImmersiveStart::Menu"), "\"ImmersiveStart::Menu\" es de menú");
    Check(IsMenuClassList(L"Toolbar;DarkMode::menu"), "Una lista con \"DarkMode::menu\" es de menú");
    Check(!IsMenuClassList(L"MenuBand") && !IsMenuClassList(L"Button") && !IsMenuClassList(nullptr),
          "\"MenuBand\", \"Button\" y nada no lo son");

    printf("== Temas abiertos ==\n");
    HTHEME light = (HTHEME)(ULONG_PTR)0x1001, dark = (HTHEME)(ULONG_PTR)0x1002, other = (HTHEME)(ULONG_PTR)0x1003;
    g_themes[light] = ThemeInfo{true, false};
    g_themes[dark] = ThemeInfo{true, true};
    g_themes[other] = ThemeInfo{false, false};
    bool d = false;
    HDC mem = CreateCompatibleDC(nullptr);
    Check(MenuTheme(light, mem, d) && !d, "Un tema de menú claro se reconoce");
    Check(MenuTheme(dark, mem, d) && d, "Y uno oscuro");
    Check(!MenuTheme(other, mem, d), "Un tema de otra clase no se toca");
    Check(!MenuTheme((HTHEME)(ULONG_PTR)0x2000, mem, d) && !g_themes.count((HTHEME)(ULONG_PTR)0x2000),
          "Uno desconocido fuera de un menú no se toca ni se anota");
    CloseThemeData_Hook(other);
    Check(!g_themes.count(other), "Al cerrarlo se olvida");
    {
        // Un tema ya abierto antes del mod (como los del Explorador): se
        // reconoce por su clase, aunque no se vea en una ventana de menú.
        GetThemeClass_t saved = g_getThemeClass;
        g_getThemeClass = [](HTHEME t, LPWSTR out, int n) -> HRESULT {
            wcsncpy(out, t == (HTHEME)(ULONG_PTR)0x3001 ? L"ImmersiveStart::Menu" : L"Button", n);
            return S_OK;
        };
        HTHEME old = (HTHEME)(ULONG_PTR)0x3001, btn = (HTHEME)(ULONG_PTR)0x3002;
        Check(ThemeClassIsMenu(old) == 1, "Un tema de menú abierto antes se reconoce por su clase");
        Check(!MenuTheme(btn, mem, d) && g_themes.count(btn) && !g_themes[btn].menu,
              "Y uno de botón queda anotado como que no es de menú");
        g_getThemeClass = saved;
    }
    Check(!IsStyledPart(1) && !IsStyledPart(7) && IsStyledPart(kPopupItem) && IsStyledPart(kPopupItemFocusable),
          "Solo las partes de los menús");
    Check(ItemTextColor(2, false, false) == RGB(28, 28, 30) && ItemTextColor(3, true, false) == RGB(140, 140, 146),
          "Colores del texto como el menú del Dock");
    Check(ItemTextColor(1, false, true) == RGB(150, 150, 156), "Los atajos de teclado van en gris");
    {
        Palette lp = MakePalette(CurrentStyle(), false);
        Check(lp.hilite == RGB(0, 0, 0) && lp.hiliteA < 0.12f && Luma(Mix(lp.bg, lp.hilite, lp.hiliteA)) > 210,
              "El resaltado de fábrica es gris claro");
    }

    printf("== Configuración ==\n");
    Check(ParseColor(L"#0A84FF") == RGB(10, 132, 255) && ParseColor(L"fff") == RGB(255, 255, 255) &&
              ParseColor(L"") == kNoColor && ParseColor(L"#12345G") == kNoColor,
          "Colores en #RRGGBB, RRGGBB o #RGB");
    Check(IsExcluded(L"Chrome.exe; code", L"code.exe") && IsExcluded(L"Chrome.exe; code", L"chrome.exe") &&
              !IsExcluded(L"Chrome.exe; code", L"notepad.exe") && !IsExcluded(L"", L"notepad.exe"),
          "Apps excluidas, con o sin .exe y sin importar mayúsculas");
    g_testSettings[L"dimShortcuts"] = L"0";
    LoadSettings();
    Check(ItemTextColor(1, false, true) == RGB(28, 28, 30), "Sin la opción, el atajo va como el nombre");
    g_testSettings[L"darkBackground"] = L"#FAFAFA";
    g_testSettings[L"highlight"] = L"custom";
    g_testSettings[L"highlightColor"] = L"#FFD60A";
    LoadSettings();
    {
        Palette p = MakePalette(CurrentStyle(), true);
        Check(!p.dark && p.bg == RGB(250, 250, 250) && Luma(p.text) < 100,
              "Un fondo propio claro en el tema oscuro lleva texto oscuro");
        Check(Luma(p.hotText) < 100, "Sobre un resaltado propio claro, el texto va oscuro");
    }
    g_testSettings[L"theme"] = L"dark";
    LoadSettings();
    Check(MenuTheme(light, mem, d) && d, "Con el tema forzado a oscuro, un menú claro se pinta oscuro");
    defaults();
    {
        // El marco se pinta sin tapar las opciones de adentro.
        BITMAPINFO bi{};
        bi.bmiHeader = {sizeof(bi.bmiHeader), 40, -40, 1, 32, BI_RGB};
        void* bits = nullptr;
        HBITMAP dib = CreateDIBSection(mem, &bi, DIB_RGB_COLORS, &bits, nullptr, 0);
        HGDIOBJ o = SelectObject(mem, dib);
        RECT all{0, 0, 40, 40};
        FillSolid(mem, all, RGB(255, 0, 0));
        DrawThemeBackground_Hook(light, mem, kPopupBorders, 0, &all, nullptr);
        GdiFlush();
        auto px = [&](int x, int y) { return ((uint32_t*)bits)[y * 40 + x] & 0xFFFFFF; };
        Check(px(20, 20) == 0xFF0000 && px(0, 0) == 0xF6F6F8, "El borde pinta el marco y deja el centro");
        // La opción resaltada y la palomita en su mismo renglón.
        RECT item{0, 10, 40, 30}, check{2, 12, 18, 28}, other{2, 32, 18, 38};
        DrawThemeBackground_Hook(light, mem, kPopupItem, 2, &item, nullptr);
        Check(OnHotItem(mem, check) && !OnHotItem(mem, other), "Se recuerda el renglón resaltado");
        DrawThemeBackground_Hook(light, mem, kPopupItem, 1, &item, nullptr);
        Check(!OnHotItem(mem, check), "Y se olvida al dejar de estarlo");
        SelectObject(mem, o);
        DeleteObject(dib);
    }
    DeleteDC(mem);

    printf("== Menú clásico del Explorador ==\n");
    std::wstring sub = std::wstring(kClassicMenuKey) + L"\\InprocServer32";
    RegDeleteTreeW(HKEY_CURRENT_USER, kClassicMenuKey);
    g_isExplorer = false;
    ApplyClassicMenu(true);
    Check(!KeyExists(sub), "Fuera del Explorador no toca el registro");
    g_isExplorer = true;
    ApplyClassicMenu(true);
    Check(KeyExists(sub) && Wh_GetIntValue(L"classicKeyCreated", 0) == 1, "Activado: registra el menú clásico");
    ApplyClassicMenu(false);
    Check(!KeyExists(sub) && Wh_GetIntValue(L"classicKeyCreated", 0) == 0, "Desactivado: lo quita");
    HKEY k = nullptr;
    RegCreateKeyExW(HKEY_CURRENT_USER, sub.c_str(), 0, nullptr, 0, KEY_SET_VALUE, nullptr, &k, nullptr);
    if (k) RegCloseKey(k);
    ApplyClassicMenu(true);
    ApplyClassicMenu(false);
    Check(KeyExists(sub), "Si ya estaba puesto a mano, no lo borra");
    RegDeleteTreeW(HKEY_CURRENT_USER, kClassicMenuKey);
    g_isExplorer = false;

    printf("== Ajustes de Windows ==\n");
    {
        DWORD delay0 = 0;
        SystemParametersInfoW(SPI_GETMENUSHOWDELAY, 0, &delay0, 0);
        bool anim0 = GetSpiBool(SPI_GETMENUANIMATION);
        g_isExplorer = true;
        g_testSettings[L"submenuDelay"] = L"120";
        g_testSettings[L"animation"] = L"none";
        LoadSettings();
        ApplySystemMenuSettings(CurrentStyle());
        DWORD delay1 = 0;
        SystemParametersInfoW(SPI_GETMENUSHOWDELAY, 0, &delay1, 0);
        Check(delay1 == 120 && !GetSpiBool(SPI_GETMENUANIMATION), "Aplica el retraso de los submenús y la animación");
        RestoreSystemMenuSettings();
        DWORD delay2 = 0;
        SystemParametersInfoW(SPI_GETMENUSHOWDELAY, 0, &delay2, 0);
        Check(delay2 == delay0 && GetSpiBool(SPI_GETMENUANIMATION) == anim0, "Y al desactivarlo vuelven como estaban");
        g_isExplorer = false;
        defaults();
    }

    printf("== Muestras ==\n");
    CreateDirectoryW(L"out", nullptr);
    struct Variant {
        const char* file;
        bool dark;
        std::map<std::wstring, std::wstring> set;
    };
    const Variant variants[] = {
        {"out\\1-grafito-claro.bgra", false, {}},
        {"out\\2-grafito-oscuro.bgra", true, {}},
        {"out\\3-arena-contorno.bgra", false,
         {{L"palette", L"sand"}, {L"highlightShape", L"outline"}, {L"checkStyle", L"box"}, {L"separator", L"dots"}}},
        {"out\\4-hielo-barra.bgra", true,
         {{L"palette", L"ice"}, {L"highlightShape", L"bar"}, {L"arrowStyle", L"triangle"}, {L"showGutter", L"1"}}},
        {"out\\5-acento-renglon.bgra", false,
         {{L"highlight", L"accent"}, {L"highlightShape", L"full"}, {L"separator", L"thick"}}},
        {"out\\6-contraste-propio.bgra", true,
         {{L"palette", L"contrast"}, {L"highlight", L"custom"}, {L"highlightColor", L"#FF9F0A"},
          {L"highlightRadius", L"12"}, {L"checkStyle", L"dot"}}},
    };
    for (auto& v : variants) {
        defaults();
        for (auto& kv : v.set) g_testSettings[kv.first] = kv.second;
        LoadSettings();
        DrawSample(v.file, v.dark ? dark : light, v.dark);
    }
    defaults();
    Check(true, "Muestras en out\\*.bgra");

    Wh_ModUninit();
    printf("Resultado: %d bien, %d fallas\n", g_pass, g_fail);
    return g_fail ? 1 : 0;
}
