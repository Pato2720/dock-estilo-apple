// ==WindhawkMod==
// @id              menu-estilo-apple
// @name            Menús estilo Apple
// @description     Menús del clic derecho y desplegables con el estilo de macOS: esquinas redondeadas, resaltado en pastilla, marcas vectoriales y los colores del Dock
// @version         1.1
// @author          local
// @include         *
// @compilerOptions -luxtheme -ldwmapi -lgdi32 -luser32 -ladvapi32
// ==/WindhawkMod==

// ==WindhawkModReadme==
/*
# Menús estilo Apple

Cambia el aspecto de los menús del clic derecho y de los menús desplegables
(Archivo, Edición…) en el escritorio, el Explorador y las aplicaciones, para
que se vean como los de macOS. Es un mod independiente: no necesita ningún
otro. Usa los mismos colores que el menú de **Dock estilo Apple**, así que
hacen juego si se usan juntos.

## Qué hace

* **Fondo liso** gris claro u oscuro según el tema de cada menú (claro u
  oscuro), sin la franja de los iconos a la izquierda.
* **Resaltado en pastilla**: la opción bajo el mouse se marca con un
  rectángulo redondeado, un poco separado de los bordes. Gris neutro (como
  el menú del Dock) o con el color de acento de Windows y el texto en blanco.
* **Marcas vectoriales** con antialias: palomita, punto de opción y flecha
  de submenú, dibujadas al tamaño exacto en cualquier escala.
* **Separadores** finos que no tocan los bordes.
* **Esquinas** más redondeadas y un **borde** fino, como los de macOS.
* Las opciones desactivadas se ven tenues y no se resaltan.

## El menú del escritorio y del Explorador

El menú nuevo de Windows 11 (el de iconos arriba) no es un menú clásico y no
se puede restilar. Con **Menú clásico en el escritorio y el Explorador**
activado, el mod hace que el Explorador use el menú completo de siempre (el
de "Mostrar más opciones"), que sí toma este estilo.

**Después de activarlo, reinicia el Explorador una vez**: Administrador de
tareas → Procesos → Explorador de Windows → Reiniciar (o cierra sesión y
vuelve a entrar). Windows lee esa opción al arrancar el Explorador.

Para volver al menú de Windows 11, apaga la opción (y reinicia el
Explorador) antes de quitar el mod.

## Notas

* Funciona con los menús que dibuja Windows (casi todas las apps de
  escritorio). Las apps que dibujan sus propios menús (Chrome, Firefox,
  Office, apps de la Tienda) no cambian.
* En el registro de Windhawk (Registros del mod) aparece "Primer menú con
  estilo" en cada proceso donde ya se aplicó.
*/
// ==/WindhawkModReadme==

// ==WindhawkModSettings==
/*
- highlight: neutral
  $name: Resaltado de la opción bajo el mouse
  $options:
  - neutral: Gris neutro (como el menú del Dock)
  - accent: Color de acento de Windows, con el texto en blanco
- corners: round
  $name: Esquinas del menú
  $options:
  - round: Redondeadas
  - small: Poco redondeadas
  - square: Rectas
- border: true
  $name: Borde fino alrededor del menú
- menuBar: true
  $name: Resaltar también la barra de menús de las apps
  $description: Archivo, Edición… en las apps clásicas
- classicExplorer: true
  $name: Menú clásico en el escritorio y el Explorador
  $description: El menú nuevo de Windows 11 no se puede restilar; con esto se usa el completo de siempre. Reinicia el Explorador después de cambiarla
*/
// ==/WindhawkModSettings==

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <dwmapi.h>
#include <uxtheme.h>
#include <vssym32.h>

#include <algorithm>
#include <atomic>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <string>
#include <unordered_map>

// ---------------------------------------------------------------------------
// Constantes
// ---------------------------------------------------------------------------

// Partes de la clase "Menu" del tema (vssym32.h), con nombre propio: no todas
// las versiones del SDK traen las más nuevas.
enum : int {
    kBarItem = 8,
    kPopupBackground = 9,
    kPopupBorders = 10,
    kPopupCheck = 11,
    kPopupCheckBackground = 12,
    kPopupGutter = 13,
    kPopupItem = 14,
    kPopupSeparator = 15,
    kPopupSubmenu = 16,
    kPopupItemFocusable = 27,  // Windows 11: opción con foco de teclado
};

// Atributos de DWM de Windows 11 (no están en todos los encabezados).
constexpr DWORD kDwmCornerPreference = 33;
constexpr DWORD kDwmBorderColor = 34;
constexpr DWORD kCornerSquare = 1, kCornerRound = 2, kCornerSmall = 3;
constexpr COLORREF kDwmColorNone = 0xFFFFFFFE;

constexpr ATOM kMenuClassAtom = 0x8000;  // #32768, la clase de los menús

static const wchar_t kClassicMenuKey[] = L"Software\\Classes\\CLSID\\{86ca1aa0-34aa-4e8b-a509-50c905bae2a2}";

// ---------------------------------------------------------------------------
// Configuración (se lee en un hilo y se usa al dibujar en otros: atómicos)
// ---------------------------------------------------------------------------

static std::atomic<bool> g_accent{false};
static std::atomic<DWORD> g_corners{kCornerRound};
static std::atomic<bool> g_border{true};
static std::atomic<bool> g_menuBar{true};
static std::atomic<bool> g_classicExplorer{true};
static std::atomic<UINT> g_settingsGen{1};  // cambia con cada configuración nueva

static std::wstring StringSetting(PCWSTR name) {
    PCWSTR s = Wh_GetStringSetting(name);
    std::wstring r = s ? s : L"";
    if (s) Wh_FreeStringSetting(s);
    return r;
}

static void LoadSettings() {
    g_accent = StringSetting(L"highlight") == L"accent";
    std::wstring c = StringSetting(L"corners");
    g_corners = c == L"square" ? kCornerSquare : c == L"small" ? kCornerSmall : kCornerRound;
    g_border = Wh_GetIntSetting(L"border") != 0;
    g_menuBar = Wh_GetIntSetting(L"menuBar") != 0;
    g_classicExplorer = Wh_GetIntSetting(L"classicExplorer") != 0;
    g_settingsGen++;
}

// ---------------------------------------------------------------------------
// Colores (los mismos del menú del Dock)
// ---------------------------------------------------------------------------

struct Palette {
    COLORREF bg, text, dim, hotText;
    COLORREF hilite;
    float hiliteA;
    COLORREF sep;     // línea del separador, ya mezclada con el fondo
    COLORREF border;  // borde de la ventana (DWM)
};

static int Luma(COLORREF c) { return (GetRValue(c) * 299 + GetGValue(c) * 587 + GetBValue(c) * 114) / 1000; }

static COLORREF Mix(COLORREF a, COLORREF b, float t) {
    auto ch = [&](int x, int y) { return (BYTE)std::lround(x + (y - x) * t); };
    return RGB(ch(GetRValue(a), GetRValue(b)), ch(GetGValue(a), GetGValue(b)), ch(GetBValue(a), GetBValue(b)));
}

static std::atomic<COLORREF> g_accentColor{0};
static std::atomic<ULONGLONG> g_accentTick{0};

// Color de acento de Windows (se vuelve a leer cada dos segundos, no en
// cada opción que se pinta).
static COLORREF AccentColor() {
    ULONGLONG now = GetTickCount64();
    if (g_accentTick && now - g_accentTick < 2000) return g_accentColor;
    COLORREF result = RGB(10, 132, 255);
    DWORD c = 0;
    BOOL opaque = FALSE;
    if (SUCCEEDED(DwmGetColorizationColor(&c, &opaque))) {
        COLORREF rgb = RGB((c >> 16) & 0xFF, (c >> 8) & 0xFF, c & 0xFF);
        // Muy oscuro o muy claro no sirve para resaltar: azul de sistema.
        int l = Luma(rgb);
        if (l > 40 && l < 215) result = rgb;
    }
    g_accentColor = result;
    g_accentTick = now;
    return result;
}

static Palette MakePalette(bool dark) {
    Palette p;
    p.bg = dark ? RGB(36, 36, 40) : RGB(246, 246, 248);
    p.text = dark ? RGB(242, 242, 245) : RGB(28, 28, 30);
    p.dim = dark ? RGB(140, 140, 146) : RGB(150, 150, 156);
    if (g_accent) {
        p.hilite = AccentColor();
        p.hiliteA = 1.0f;
        p.hotText = RGB(255, 255, 255);
    } else {
        // Resaltado gris neutro, como el menú del Dock.
        p.hilite = dark ? RGB(255, 255, 255) : RGB(0, 0, 0);
        p.hiliteA = dark ? 40 / 255.0f : 24 / 255.0f;
        p.hotText = p.text;
    }
    p.sep = Mix(p.bg, dark ? RGB(255, 255, 255) : RGB(0, 0, 0), dark ? 40 / 255.0f : 30 / 255.0f);
    p.border = dark ? RGB(72, 72, 78) : RGB(200, 200, 206);
    return p;
}

static bool AppsUseLightTheme() {
    DWORD v = 1, size = sizeof(v);
    RegGetValueW(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize",
                 L"AppsUseLightTheme", RRF_RT_REG_DWORD, nullptr, &v, &size);
    return v != 0;
}

// ---------------------------------------------------------------------------
// Qué temas son de menú
// ---------------------------------------------------------------------------
// Ante todo se le pregunta a uxtheme la clase del tema (GetThemeClass, que
// solo se exporta por número). Así se reconocen también los temas que ya
// estaban abiertos cuando llegó el mod, como los del Explorador. Si no se
// puede, se anotan al abrirse (OpenThemeData y compañía) o al dibujar en
// una ventana de menú.

struct ThemeInfo {
    bool menu = false;
    bool dark = false;
};

static SRWLOCK g_themeLock = SRWLOCK_INIT;
static std::unordered_map<HTHEME, ThemeInfo> g_themes;

using GetThemeClass_t = HRESULT(WINAPI*)(HTHEME, LPWSTR, int);
static GetThemeClass_t g_getThemeClass = nullptr;  // uxtheme, ordinal 74
static std::atomic<bool> g_loggedFirst{false};

// "Menu", "ImmersiveStart::Menu", "DarkMode::Menu" o varias separadas por ';'.
static bool IsMenuClassList(LPCWSTR list) {
    if (!list) return false;
    const wchar_t* p = list;
    while (*p) {
        const wchar_t* end = wcschr(p, L';');
        size_t n = end ? (size_t)(end - p) : wcslen(p);
        const wchar_t* name = p;
        size_t len = n;
        for (size_t i = 0; i + 1 < n; i++) {
            if (p[i] == L':' && p[i + 1] == L':') {
                name = p + i + 2;
                len = n - i - 2;
            }
        }
        if (len == 4 && _wcsnicmp(name, L"Menu", 4) == 0) return true;
        if (!end) break;
        p = end + 1;
    }
    return false;
}

// ¿Tema oscuro? Por el color de texto que el propio tema da a sus opciones.
static bool ThemeIsDark(HTHEME t) {
    COLORREF c = 0;
    if (SUCCEEDED(GetThemeColor(t, kPopupItem, 1, TMT_TEXTCOLOR, &c))) return Luma(c) > 128;
    return !AppsUseLightTheme();
}

static void RememberTheme(HTHEME t, bool menu) {
    if (!t) return;
    ThemeInfo info;
    info.menu = menu;
    if (menu) info.dark = ThemeIsDark(t);
    AcquireSRWLockExclusive(&g_themeLock);
    g_themes[t] = info;
    ReleaseSRWLockExclusive(&g_themeLock);
}

static bool IsMenuWindow(HWND h) { return h && (ATOM)GetClassLongPtrW(h, GCW_ATOM) == kMenuClassAtom; }

static bool InMenuMode() {
    GUITHREADINFO gi{sizeof(gi)};
    return GetGUIThreadInfo(GetCurrentThreadId(), &gi) && (gi.flags & (GUI_INMENUMODE | GUI_POPUPMENUMODE));
}

// Clase del tema según uxtheme: 1 menú, 0 otra, -1 no se sabe.
static int ThemeClassIsMenu(HTHEME t) {
    if (!g_getThemeClass) return -1;
    wchar_t cls[256] = {};
    if (FAILED(g_getThemeClass(t, cls, ARRAYSIZE(cls))) || !cls[0]) return -1;
    return IsMenuClassList(cls) ? 1 : 0;
}

static bool LooksLikeMenuTheme(HTHEME t) {
    // Partes que solo un menú tiene juntas (la clase "Window" tiene muchas
    // más, de ahí la última).
    return IsThemePartDefined(t, kPopupItem, 0) && IsThemePartDefined(t, kPopupSeparator, 0) &&
           IsThemePartDefined(t, kPopupSubmenu, 0) && !IsThemePartDefined(t, 30, 0);
}

static bool MenuTheme(HTHEME t, HDC hdc, bool& dark) {
    if (!t) return false;
    AcquireSRWLockShared(&g_themeLock);
    auto it = g_themes.find(t);
    bool known = it != g_themes.end();
    ThemeInfo info = known ? it->second : ThemeInfo{};
    ReleaseSRWLockShared(&g_themeLock);
    if (!known) {
        int byClass = ThemeClassIsMenu(t);
        if (byClass >= 0) {
            RememberTheme(t, byClass == 1);
        } else if (IsMenuWindow(WindowFromDC(hdc)) || (InMenuMode() && LooksLikeMenuTheme(t))) {
            RememberTheme(t, true);
        } else {
            return false;  // sin decidir: no se anota, se vuelve a mirar la próxima vez
        }
        return MenuTheme(t, hdc, dark);
    }
    if (info.menu && !g_loggedFirst.exchange(true)) {
        Wh_Log(L"Primer menú con estilo (tema %s)", info.dark ? L"oscuro" : L"claro");
    }
    dark = info.dark;
    return info.menu;
}

// ---------------------------------------------------------------------------
// Ventanas de menú: esquinas y borde (DWM)
// ---------------------------------------------------------------------------

static SRWLOCK g_wndLock = SRWLOCK_INIT;
static std::unordered_map<HWND, UINT> g_styled;  // ventana -> configuración con la que se arregló

static void StyleMenuWindow(HWND h, bool dark) {
    UINT want = g_settingsGen * 2 + (dark ? 1 : 0);
    AcquireSRWLockExclusive(&g_wndLock);
    auto it = g_styled.find(h);
    bool done = it != g_styled.end() && it->second == want;
    if (!done) {
        if (g_styled.size() > 64) {
            for (auto i = g_styled.begin(); i != g_styled.end();) i = IsWindow(i->first) ? std::next(i) : g_styled.erase(i);
        }
        g_styled[h] = want;
    }
    ReleaseSRWLockExclusive(&g_wndLock);
    if (done) return;
    DWORD corner = g_corners;
    DwmSetWindowAttribute(h, kDwmCornerPreference, &corner, sizeof(corner));
    COLORREF border = g_border ? MakePalette(dark).border : kDwmColorNone;
    DwmSetWindowAttribute(h, kDwmBorderColor, &border, sizeof(border));
}

struct ThreadMenusCtx {
    bool dark;
};

static BOOL CALLBACK StyleThreadMenuProc(HWND h, LPARAM lParam) {
    if (IsMenuWindow(h) && IsWindowVisible(h)) StyleMenuWindow(h, ((ThreadMenusCtx*)lParam)->dark);
    return TRUE;
}

// Al pintar el fondo de un menú: arreglar su ventana. Si se pinta en un DC
// de memoria (sin ventana), las ventanas de menú del hilo.
static void StyleMenuWindowsFor(HDC hdc, bool dark) {
    HWND w = WindowFromDC(hdc);
    if (IsMenuWindow(w)) {
        StyleMenuWindow(w, dark);
        return;
    }
    ThreadMenusCtx ctx{dark};
    EnumThreadWindows(GetCurrentThreadId(), StyleThreadMenuProc, (LPARAM)&ctx);
}

// ---------------------------------------------------------------------------
// Dibujo con antialias en un lienzo propio (ARGB premultiplicado) que luego
// se mezcla sobre el DC del menú.
// ---------------------------------------------------------------------------

struct Canvas {
    HDC dc = nullptr;
    HBITMAP dib = nullptr;
    HGDIOBJ old = nullptr;
    uint32_t* bits = nullptr;
    int w = 0, h = 0;

    Canvas(int width, int height) : w(width), h(height) {
        BITMAPINFO bi{};
        bi.bmiHeader.biSize = sizeof(bi.bmiHeader);
        bi.bmiHeader.biWidth = w;
        bi.bmiHeader.biHeight = -h;
        bi.bmiHeader.biPlanes = 1;
        bi.bmiHeader.biBitCount = 32;
        bi.bmiHeader.biCompression = BI_RGB;
        void* p = nullptr;
        dc = CreateCompatibleDC(nullptr);
        dib = dc ? CreateDIBSection(dc, &bi, DIB_RGB_COLORS, &p, nullptr, 0) : nullptr;
        if (dib && p) {
            old = SelectObject(dc, dib);
            bits = (uint32_t*)p;
            memset(bits, 0, (size_t)w * h * 4);
        }
    }
    ~Canvas() {
        if (old) SelectObject(dc, old);
        if (dib) DeleteObject(dib);
        if (dc) DeleteDC(dc);
    }
    Canvas(const Canvas&) = delete;
    Canvas& operator=(const Canvas&) = delete;

    // Mezcla el lienzo sobre el DC en (x, y).
    void Present(HDC dst, int x, int y) const {
        if (!bits) return;
        GdiFlush();
        BLENDFUNCTION bf{AC_SRC_OVER, 0, 255, AC_SRC_ALPHA};
        GdiAlphaBlend(dst, x, y, w, h, dc, 0, 0, w, h, bf);
    }
};

// Mezcla premultiplicada: (a, r, g, b) ya premultiplicados (0..255) encima.
static inline void BlendOver(uint32_t* dst, float a, float r, float g, float b) {
    if (a <= 0.0f) return;
    uint32_t d = *dst;
    float k = 1.0f - a;
    int oa = (int)(a * 255.0f + (float)(d >> 24) * k + 0.5f);
    int orr = (int)(r + (float)((d >> 16) & 0xFF) * k + 0.5f);
    int og = (int)(g + (float)((d >> 8) & 0xFF) * k + 0.5f);
    int ob = (int)(b + (float)(d & 0xFF) * k + 0.5f);
    oa = std::min(oa, 255);
    *dst = ((uint32_t)oa << 24) | ((uint32_t)std::min(orr, oa) << 16) | ((uint32_t)std::min(og, oa) << 8) |
           (uint32_t)std::min(ob, oa);
}

static inline void Plot(Canvas& cv, int x, int y, float cov, COLORREF c) {
    if (cov <= 0.002f) return;
    BlendOver(cv.bits + (size_t)y * cv.w + x, cov, GetRValue(c) * cov, GetGValue(c) * cov, GetBValue(c) * cov);
}

// Distancia con signo a un rectángulo redondeado (negativa adentro).
static inline float RoundRectSdf(float px, float py, float cx, float cy, float hw, float hh, float r) {
    float qx = std::fabs(px - cx) - (hw - r);
    float qy = std::fabs(py - cy) - (hh - r);
    float ox = std::max(qx, 0.0f), oy = std::max(qy, 0.0f);
    return std::sqrt(ox * ox + oy * oy) + std::min(std::max(qx, qy), 0.0f) - r;
}

static void FillRoundRect(Canvas& cv, float x0, float y0, float w, float h, float r, COLORREF c, float alpha) {
    if (!cv.bits || w <= 0 || h <= 0) return;
    float cx = x0 + w / 2, cy = y0 + h / 2, hw = w / 2, hh = h / 2;
    r = std::min(r, std::min(hw, hh));
    int ix0 = std::max(0, (int)std::floor(x0)), ix1 = std::min(cv.w, (int)std::ceil(x0 + w));
    int iy0 = std::max(0, (int)std::floor(y0)), iy1 = std::min(cv.h, (int)std::ceil(y0 + h));
    for (int y = iy0; y < iy1; y++) {
        for (int x = ix0; x < ix1; x++) {
            float d = RoundRectSdf(x + 0.5f, y + 0.5f, cx, cy, hw, hh, r);
            Plot(cv, x, y, std::clamp(0.5f - d, 0.0f, 1.0f) * alpha, c);
        }
    }
}

static void FillDot(Canvas& cv, float cx, float cy, float r, COLORREF c) {
    FillRoundRect(cv, cx - r, cy - r, 2 * r, 2 * r, r, c, 1.0f);
}

// Trazo de una línea quebrada con puntas y uniones redondas.
static void StrokePolyline(Canvas& cv, const float* pts, int n, float width, COLORREF c) {
    if (!cv.bits || n < 2) return;
    float half = width / 2;
    for (int y = 0; y < cv.h; y++) {
        for (int x = 0; x < cv.w; x++) {
            float px = x + 0.5f, py = y + 0.5f, best = 1e9f;
            for (int i = 0; i + 1 < n; i++) {
                float ax = pts[i * 2], ay = pts[i * 2 + 1], vx = pts[i * 2 + 2] - ax, vy = pts[i * 2 + 3] - ay;
                float wx = px - ax, wy = py - ay;
                float len2 = vx * vx + vy * vy;
                float t = len2 > 0 ? std::clamp((wx * vx + wy * vy) / len2, 0.0f, 1.0f) : 0.0f;
                float dx = wx - vx * t, dy = wy - vy * t;
                best = std::min(best, dx * dx + dy * dy);
            }
            Plot(cv, x, y, std::clamp(half + 0.5f - std::sqrt(best), 0.0f, 1.0f), c);
        }
    }
}

// Rellena un rectángulo con un color liso (sin crear pinceles).
static void FillSolid(HDC hdc, const RECT& rc, COLORREF c) {
    COLORREF old = SetBkColor(hdc, c);
    ExtTextOutW(hdc, 0, 0, ETO_OPAQUE, &rc, nullptr, 0, nullptr);
    SetBkColor(hdc, old);
}

// Escala de la pantalla del DC (1 = 100 %).
static float ScaleOf(HDC hdc) {
    HWND w = WindowFromDC(hdc);
    UINT dpi = w ? GetDpiForWindow(w) : 0;
    if (!dpi) dpi = (UINT)GetDeviceCaps(hdc, LOGPIXELSY);
    return std::max(1.0f, dpi / 96.0f);
}

// Limita el dibujo al recorte que pide el tema, mientras dura.
struct ClipScope {
    HDC dc;
    int saved = 0;
    ClipScope(HDC d, const RECT* clip) : dc(d) {
        if (clip) {
            saved = SaveDC(d);
            IntersectClipRect(d, clip->left, clip->top, clip->right, clip->bottom);
        }
    }
    ~ClipScope() {
        if (saved) RestoreDC(dc, saved);
    }
};

// La opción resaltada que se acaba de pintar (por hilo y DC): la palomita y
// la flecha de esa opción van en el color del texto resaltado.
struct HotItem {
    DWORD tid = 0;
    HDC dc = nullptr;
    RECT rc{};
};
static SRWLOCK g_hotLock = SRWLOCK_INIT;
static HotItem g_hot;

static void SetHotItem(HDC hdc, const RECT* rc) {
    AcquireSRWLockExclusive(&g_hotLock);
    if (rc) {
        g_hot = HotItem{GetCurrentThreadId(), hdc, *rc};
    } else if (g_hot.tid == GetCurrentThreadId() && g_hot.dc == hdc) {
        g_hot = HotItem{};
    }
    ReleaseSRWLockExclusive(&g_hotLock);
}

static bool OnHotItem(HDC hdc, const RECT& rc) {
    AcquireSRWLockShared(&g_hotLock);
    bool on = g_hot.tid == GetCurrentThreadId() && g_hot.dc == hdc && rc.top >= g_hot.rc.top &&
              rc.bottom <= g_hot.rc.bottom;
    ReleaseSRWLockShared(&g_hotLock);
    return on;
}

// Grosor del marco de una ventana de menú (lo que rodea al área de las
// opciones).
static int FrameThickness(HDC hdc, float s) {
    HWND w = WindowFromDC(hdc);
    if (IsMenuWindow(w)) {
        RECT wr{};
        POINT o{0, 0};
        if (GetWindowRect(w, &wr) && ClientToScreen(w, &o)) return std::clamp((int)(o.x - wr.left), 1, 12);
    }
    return std::max(1, (int)std::lround(3 * s));
}

static void FillFrame(HDC hdc, const RECT& rc, int t, COLORREF c) {
    FillSolid(hdc, RECT{rc.left, rc.top, rc.right, rc.top + t}, c);
    FillSolid(hdc, RECT{rc.left, rc.bottom - t, rc.right, rc.bottom}, c);
    FillSolid(hdc, RECT{rc.left, rc.top + t, rc.left + t, rc.bottom - t}, c);
    FillSolid(hdc, RECT{rc.right - t, rc.top + t, rc.right, rc.bottom - t}, c);
}

// Resaltado en pastilla dentro del cuadro de la opción.
static void DrawHilite(HDC hdc, const RECT& rc, float s, const Palette& p, float insetX, float insetY) {
    int w = rc.right - rc.left, h = rc.bottom - rc.top;
    if (w <= 0 || h <= 0) return;
    Canvas cv(w, h);
    float ix = std::min(insetX * s, w / 4.0f), iy = std::min(insetY * s, h / 4.0f);
    FillRoundRect(cv, ix, iy, w - 2 * ix, h - 2 * iy, 5.0f * s, p.hilite, p.hiliteA);
    cv.Present(hdc, rc.left, rc.top);
}

// Palomita o punto de opción, centrados en su cuadro.
static void DrawCheckMark(HDC hdc, const RECT& rc, float s, bool bullet, COLORREF c) {
    int w = rc.right - rc.left, h = rc.bottom - rc.top;
    if (w <= 0 || h <= 0) return;
    Canvas cv(w, h);
    float cx = w / 2.0f, cy = h / 2.0f, g = std::min(10.0f * s, std::min(w, h) * 0.62f);
    if (bullet) {
        FillDot(cv, cx, cy, g * 0.26f, c);
    } else {
        float pts[] = {cx - g * 0.42f, cy + g * 0.02f, cx - g * 0.12f, cy + g * 0.32f, cx + g * 0.44f, cy - g * 0.34f};
        StrokePolyline(cv, pts, 3, std::max(1.5f * s, g * 0.16f), c);
    }
    cv.Present(hdc, rc.left, rc.top);
}

// Flecha de submenú (un ángulo fino, como en macOS).
static void DrawChevron(HDC hdc, const RECT& rc, float s, COLORREF c) {
    int w = rc.right - rc.left, h = rc.bottom - rc.top;
    if (w <= 0 || h <= 0) return;
    Canvas cv(w, h);
    float cx = w / 2.0f, cy = h / 2.0f, g = std::min(9.0f * s, std::min(w, h) * 0.6f);
    float pts[] = {cx - g * 0.18f, cy - g * 0.40f, cx + g * 0.22f, cy, cx - g * 0.18f, cy + g * 0.40f};
    StrokePolyline(cv, pts, 3, std::max(1.4f * s, g * 0.15f), c);
    cv.Present(hdc, rc.left, rc.top);
}

// Pinta una parte del menú con el estilo propio. false = que la dibuje el tema.
static bool PaintMenuPart(HDC hdc, int part, int state, const RECT& rc, const RECT* clip, bool dark) {
    if (rc.right <= rc.left || rc.bottom <= rc.top) return false;
    Palette p = MakePalette(dark);
    float s = ScaleOf(hdc);
    ClipScope scope(hdc, clip);
    switch (part) {
        case kPopupBackground:
            StyleMenuWindowsFor(hdc, dark);
            FillSolid(hdc, rc, p.bg);
            return true;
        case kPopupBorders:
            // Solo el marco: el centro ya tiene las opciones.
            FillFrame(hdc, rc, FrameThickness(hdc, s), p.bg);
            return true;
        case kPopupGutter:  // sin franja de iconos a la izquierda
            FillSolid(hdc, rc, p.bg);
            return true;
        case kPopupCheckBackground:  // sin recuadro detrás de la palomita
            return true;
        case kPopupSeparator: {
            FillSolid(hdc, rc, p.bg);
            int inset = (int)std::lround(10 * s), th = std::max(1, (int)std::lround(s));
            int y = (rc.top + rc.bottom - th) / 2;
            RECT line{rc.left + inset, y, rc.right - inset, y + th};
            if (line.right > line.left) FillSolid(hdc, line, p.sep);
            return true;
        }
        case kPopupItem:
        case kPopupItemFocusable: {
            FillSolid(hdc, rc, p.bg);
            if (state == 2) {  // las desactivadas no se resaltan (como en macOS)
                DrawHilite(hdc, rc, s, p, 5.0f, 1.0f);
                SetHotItem(hdc, &rc);
            } else if (OnHotItem(hdc, rc)) {
                SetHotItem(hdc, nullptr);
            }
            return true;
        }
        case kPopupCheck: {
            bool disabled = state == 2 || state == 4;
            COLORREF c = disabled ? p.dim : OnHotItem(hdc, rc) ? p.hotText : p.text;
            DrawCheckMark(hdc, rc, s, state >= 3, c);
            return true;
        }
        case kPopupSubmenu: {
            COLORREF c = state == 2 ? p.dim : OnHotItem(hdc, rc) ? p.hotText : p.text;
            DrawChevron(hdc, rc, s, c);
            return true;
        }
        case kBarItem:
            // Barra de menús: solo el resaltado, sobre el fondo que ya tenga.
            if (!g_menuBar || (state != 2 && state != 3)) return false;
            p.hilite = dark ? RGB(255, 255, 255) : RGB(0, 0, 0);
            p.hiliteA = dark ? 40 / 255.0f : 24 / 255.0f;
            DrawHilite(hdc, rc, s, p, 2.0f, 2.0f);
            return true;
    }
    return false;
}

static bool IsStyledPart(int part) {
    return (part >= kBarItem && part <= kPopupSubmenu) || part == kPopupItemFocusable;
}

// Color del texto de una opción según su estado.
static COLORREF ItemTextColor(int state, bool dark) {
    Palette p = MakePalette(dark);
    switch (state) {
        case 2:
            return p.hotText;
        case 3:
        case 4:
            return p.dim;
        default:
            return p.text;
    }
}

// ---------------------------------------------------------------------------
// Ganchos de uxtheme
// ---------------------------------------------------------------------------

using OpenThemeData_t = HTHEME(WINAPI*)(HWND, LPCWSTR);
using OpenThemeDataEx_t = HTHEME(WINAPI*)(HWND, LPCWSTR, DWORD);
using OpenThemeDataForDpi_t = HTHEME(WINAPI*)(HWND, LPCWSTR, UINT);
using CloseThemeData_t = HRESULT(WINAPI*)(HTHEME);
using DrawThemeBackground_t = HRESULT(WINAPI*)(HTHEME, HDC, int, int, LPCRECT, LPCRECT);
using DrawThemeBackgroundEx_t = HRESULT(WINAPI*)(HTHEME, HDC, int, int, LPCRECT, const DTBGOPTS*);
using DrawThemeText_t = HRESULT(WINAPI*)(HTHEME, HDC, int, int, LPCWSTR, int, DWORD, DWORD, LPCRECT);
using DrawThemeTextEx_t = HRESULT(WINAPI*)(HTHEME, HDC, int, int, LPCWSTR, int, DWORD, LPRECT, const DTTOPTS*);

static OpenThemeData_t g_origOpenThemeData = nullptr;
static OpenThemeDataEx_t g_origOpenThemeDataEx = nullptr;
static OpenThemeDataForDpi_t g_origOpenThemeDataForDpi = nullptr;
static CloseThemeData_t g_origCloseThemeData = nullptr;
static DrawThemeBackground_t g_origDrawThemeBackground = nullptr;
static DrawThemeBackgroundEx_t g_origDrawThemeBackgroundEx = nullptr;
static DrawThemeText_t g_origDrawThemeText = nullptr;
static DrawThemeTextEx_t g_origDrawThemeTextEx = nullptr;

static HTHEME WINAPI OpenThemeData_Hook(HWND h, LPCWSTR classes) {
    HTHEME t = g_origOpenThemeData(h, classes);
    RememberTheme(t, IsMenuClassList(classes));
    return t;
}

static HTHEME WINAPI OpenThemeDataEx_Hook(HWND h, LPCWSTR classes, DWORD flags) {
    HTHEME t = g_origOpenThemeDataEx(h, classes, flags);
    RememberTheme(t, IsMenuClassList(classes));
    return t;
}

static HTHEME WINAPI OpenThemeDataForDpi_Hook(HWND h, LPCWSTR classes, UINT dpi) {
    HTHEME t = g_origOpenThemeDataForDpi(h, classes, dpi);
    RememberTheme(t, IsMenuClassList(classes));
    return t;
}

static HRESULT WINAPI CloseThemeData_Hook(HTHEME t) {
    AcquireSRWLockExclusive(&g_themeLock);
    g_themes.erase(t);
    ReleaseSRWLockExclusive(&g_themeLock);
    return g_origCloseThemeData(t);
}

static HRESULT WINAPI DrawThemeBackground_Hook(HTHEME t, HDC hdc, int part, int state, LPCRECT rc, LPCRECT clip) {
    bool dark = false;
    if (rc && IsStyledPart(part) && MenuTheme(t, hdc, dark) && PaintMenuPart(hdc, part, state, *rc, clip, dark)) {
        return S_OK;
    }
    return g_origDrawThemeBackground(t, hdc, part, state, rc, clip);
}

static HRESULT WINAPI DrawThemeBackgroundEx_Hook(HTHEME t, HDC hdc, int part, int state, LPCRECT rc,
                                                 const DTBGOPTS* opts) {
    bool dark = false;
    if (rc && IsStyledPart(part) && MenuTheme(t, hdc, dark)) {
        const RECT* clip = opts && (opts->dwFlags & DTBG_CLIPRECT) ? &opts->rcClip : nullptr;
        if (PaintMenuPart(hdc, part, state, *rc, clip, dark)) return S_OK;
    }
    return g_origDrawThemeBackgroundEx(t, hdc, part, state, rc, opts);
}

static bool IsItemText(int part) { return part == kPopupItem || part == kPopupItemFocusable; }

static HRESULT WINAPI DrawThemeTextEx_Hook(HTHEME t, HDC hdc, int part, int state, LPCWSTR text, int len, DWORD flags,
                                           LPRECT rc, const DTTOPTS* opts) {
    bool dark = false;
    if (IsItemText(part) && MenuTheme(t, hdc, dark)) {
        DTTOPTS o{};
        if (opts) memcpy(&o, opts, std::min<size_t>(opts->dwSize, sizeof(o)));
        o.dwSize = sizeof(o);
        o.dwFlags |= DTT_TEXTCOLOR;
        o.crText = ItemTextColor(state, dark);
        return g_origDrawThemeTextEx(t, hdc, part, state, text, len, flags, rc, &o);
    }
    return g_origDrawThemeTextEx(t, hdc, part, state, text, len, flags, rc, opts);
}

static HRESULT WINAPI DrawThemeText_Hook(HTHEME t, HDC hdc, int part, int state, LPCWSTR text, int len, DWORD flags,
                                         DWORD flags2, LPCRECT rc) {
    bool dark = false;
    if (rc && g_origDrawThemeTextEx && IsItemText(part) && MenuTheme(t, hdc, dark)) {
        DTTOPTS o{};
        o.dwSize = sizeof(o);
        o.dwFlags = DTT_TEXTCOLOR;
        o.crText = ItemTextColor(state, dark);
        RECT r = *rc;
        return g_origDrawThemeTextEx(t, hdc, part, state, text, len, flags, &r, &o);
    }
    return g_origDrawThemeText(t, hdc, part, state, text, len, flags, flags2, rc);
}

// ---------------------------------------------------------------------------
// Menú clásico en el Explorador
// ---------------------------------------------------------------------------
// Registrar vacío el componente del menú nuevo hace que el Explorador use el
// menú completo de siempre. Solo lo hace el Explorador, y solo borra la
// clave si la creó el mod.

static bool g_isExplorer = false;

static void ApplyClassicMenu(bool on) {
    if (!g_isExplorer) return;
    bool created = Wh_GetIntValue(L"classicKeyCreated", 0) != 0;
    std::wstring sub = std::wstring(kClassicMenuKey) + L"\\InprocServer32";
    if (on) {
        HKEY k = nullptr;
        if (RegOpenKeyExW(HKEY_CURRENT_USER, sub.c_str(), 0, KEY_READ, &k) == ERROR_SUCCESS) {
            RegCloseKey(k);  // ya está (del mod o puesta a mano)
            return;
        }
        if (RegCreateKeyExW(HKEY_CURRENT_USER, sub.c_str(), 0, nullptr, 0, KEY_SET_VALUE, nullptr, &k, nullptr) ==
            ERROR_SUCCESS) {
            RegSetValueExW(k, nullptr, 0, REG_SZ, (const BYTE*)L"", sizeof(wchar_t));
            RegCloseKey(k);
            Wh_SetIntValue(L"classicKeyCreated", 1);
            Wh_Log(L"Menú clásico del Explorador activado");
        }
    } else if (created) {
        RegDeleteTreeW(HKEY_CURRENT_USER, kClassicMenuKey);
        Wh_SetIntValue(L"classicKeyCreated", 0);
        Wh_Log(L"Menú clásico del Explorador desactivado");
    }
}

static bool IsExplorerProcess() {
    wchar_t path[MAX_PATH] = {};
    DWORD n = GetModuleFileNameW(nullptr, path, ARRAYSIZE(path));
    const wchar_t* tail = L"\\explorer.exe";
    size_t t = wcslen(tail);
    return n >= t && _wcsicmp(path + n - t, tail) == 0;
}

// ---------------------------------------------------------------------------
// Windhawk
// ---------------------------------------------------------------------------

template <typename T>
static bool Hook(HMODULE mod, const char* name, T hook, T* original) {
    void* target = (void*)GetProcAddress(mod, name);
    if (!target) return false;
    return Wh_SetFunctionHook(target, (void*)hook, (void**)original) != FALSE;
}

BOOL Wh_ModInit() {
    LoadSettings();
    HMODULE ux = LoadLibraryW(L"uxtheme.dll");
    if (!ux) return FALSE;
    bool ok = Hook(ux, "DrawThemeBackground", DrawThemeBackground_Hook, &g_origDrawThemeBackground) &&
              Hook(ux, "DrawThemeTextEx", DrawThemeTextEx_Hook, &g_origDrawThemeTextEx);
    if (!ok) {
        Wh_Log(L"No se pudieron enganchar las funciones de uxtheme");
        return FALSE;
    }
    Hook(ux, "DrawThemeBackgroundEx", DrawThemeBackgroundEx_Hook, &g_origDrawThemeBackgroundEx);
    Hook(ux, "DrawThemeText", DrawThemeText_Hook, &g_origDrawThemeText);
    Hook(ux, "OpenThemeData", OpenThemeData_Hook, &g_origOpenThemeData);
    Hook(ux, "OpenThemeDataEx", OpenThemeDataEx_Hook, &g_origOpenThemeDataEx);
    Hook(ux, "OpenThemeDataForDpi", OpenThemeDataForDpi_Hook, &g_origOpenThemeDataForDpi);
    Hook(ux, "CloseThemeData", CloseThemeData_Hook, &g_origCloseThemeData);
    // GetThemeClass no tiene nombre exportado; antes de usarlo se comprueba
    // con un tema conocido que de verdad devuelve la clase.
    g_getThemeClass = (GetThemeClass_t)(void*)GetProcAddress(ux, MAKEINTRESOURCEA(74));
    if (g_getThemeClass) {
        HTHEME probe = OpenThemeData(nullptr, L"Menu");
        wchar_t cls[64] = {};
        bool works = probe && SUCCEEDED(g_getThemeClass(probe, cls, ARRAYSIZE(cls))) && IsMenuClassList(cls);
        if (probe) CloseThemeData(probe);
        if (!works) g_getThemeClass = nullptr;
    }
    Wh_Log(L"Menús estilo Apple listo (clase de tema: %s)", g_getThemeClass ? L"sí" : L"no");
    g_isExplorer = IsExplorerProcess();
    ApplyClassicMenu(g_classicExplorer);
    return TRUE;
}

void Wh_ModUninit() {
    // La clave del menú clásico se deja: el mod también se descarga al
    // reiniciar el Explorador, y quitarla ahí haría que el Explorador nuevo
    // arranque con el menú de Windows 11. Se quita al apagar la opción.
}

void Wh_ModSettingsChanged() {
    LoadSettings();
    ApplyClassicMenu(g_classicExplorer);
}
