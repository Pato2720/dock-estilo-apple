// ==WindhawkMod==
// @id              menu-estilo-apple
// @name            Menús estilo Apple
// @description     Menús del clic derecho y desplegables con el estilo de macOS, muy personalizables: paletas, formas del resaltado, marcas vectoriales, animaciones y más
// @version         2.0
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
otro. Con la paleta Grafito usa los mismos colores que el menú de **Dock
estilo Apple**.

## Qué hace

* **Fondo liso** claro u oscuro, sin la franja de los iconos a la izquierda
  (se puede dejar).
* **Resaltado** de la opción bajo el mouse en **gris claro** (o el color de
  acento, o uno propio), con varias formas: pastilla redondeada, contorno,
  barra lateral o el renglón completo. Redondeo, margen e intensidad a gusto.
* **Paletas**: Grafito (la del Dock), Arena, Hielo y Alto contraste, o colores
  propios para el fondo, el resaltado y el borde.
* **Marcas vectoriales** con antialias: palomita, punto o casilla; flecha de
  submenú fina, triángulo o ninguna.
* **Separadores** en línea fina, línea gruesa, puntos o solo espacio.
* **Atajos de teclado en gris** (Ctrl+C…), como en macOS.
* **Esquinas** redondeadas y **borde** fino, del color que quieras.
* Las opciones desactivadas se ven tenues y no se resaltan.

## Funciones

* **Tema** automático (el de cada menú) o siempre claro u oscuro.
* **Animación** de los menús: la del sistema, desvanecer, deslizar o ninguna.
* **Retraso de los submenús**: cuánto esperan antes de abrirse.
* **Subrayar siempre las letras de acceso** (la A de _A_rchivo).
* **Apps excluidas**: una lista de programas donde los menús quedan como
  estaban.
* **Menú clásico** en el escritorio y el Explorador (ver abajo).

Las tres primeras se aplican a todo Windows mientras el mod está activo y
vuelven como estaban al desactivarlo.

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
* Con el tema forzado, si en alguna app el texto no se lee, vuelve a
  Automático o excluye esa app.
* En el registro de Windhawk (Registros del mod) aparece "Primer menú con
  estilo" en cada proceso donde ya se aplicó.
*/
// ==/WindhawkModReadme==

// ==WindhawkModSettings==
/*
- theme: auto
  $name: Tema del menú
  $options:
  - auto: Automático (el de cada menú)
  - light: Siempre claro
  - dark: Siempre oscuro
- palette: graphite
  $name: Paleta de colores
  $options:
  - graphite: Grafito (la del Dock)
  - sand: Arena (cálida)
  - ice: Hielo (fría)
  - contrast: Alto contraste
- lightBackground: ""
  $name: Fondo propio para el tema claro
  $description: Vacío = el de la paleta. Ejemplo #FAFAFA
- darkBackground: ""
  $name: Fondo propio para el tema oscuro
  $description: Vacío = el de la paleta. Ejemplo #1E1E22
- highlight: gray
  $name: Color del resaltado
  $options:
  - gray: Gris claro
  - accent: Color de acento de Windows, con el texto en blanco
  - custom: Color propio
- highlightColor: "#0A84FF"
  $name: Color propio del resaltado
  $description: Solo con "Color propio". El texto encima se pone blanco o negro según se lea mejor
- highlightIntensity: 100
  $name: Intensidad del resaltado (%)
  $description: 100 = normal; más, más marcado (hasta 300)
- highlightShape: pill
  $name: Forma del resaltado
  $options:
  - pill: Pastilla redondeada
  - outline: Contorno
  - bar: Barra lateral
  - full: Renglón completo
- highlightRadius: 6
  $name: Redondeo del resaltado
  $description: En píxeles a escala de 100 % (0 a 14)
- highlightMargin: 5
  $name: Separación del resaltado con los bordes
  $description: En píxeles a escala de 100 % (0 a 12)
- separator: line
  $name: Separadores
  $options:
  - line: Línea fina
  - thick: Línea gruesa
  - dots: Puntos
  - space: Solo espacio
- checkStyle: check
  $name: Opciones marcadas
  $options:
  - check: Palomita
  - dot: Punto
  - box: Casilla
- arrowStyle: chevron
  $name: Flecha de los submenús
  $options:
  - chevron: Fina
  - triangle: Triángulo
  - none: Ninguna
- dimShortcuts: true
  $name: Atajos de teclado en gris
  $description: Ctrl+C, Ctrl+V… más tenues que el nombre, como en macOS
- showGutter: false
  $name: Mostrar la franja de los iconos
- corners: round
  $name: Esquinas del menú
  $options:
  - round: Redondeadas
  - small: Poco redondeadas
  - square: Rectas
- border: true
  $name: Borde fino alrededor del menú
- borderColor: ""
  $name: Color del borde
  $description: Vacío = según la paleta. Ejemplo #8E8E93
- menuBar: true
  $name: Resaltar también la barra de menús de las apps
  $description: Archivo, Edición… en las apps clásicas
- animation: system
  $name: Animación al abrir los menús
  $description: Para todo Windows mientras el mod está activo
  $options:
  - system: La del sistema
  - fade: Desvanecer
  - slide: Deslizar
  - none: Ninguna (aparecen al instante)
- submenuDelay: -1
  $name: Retraso de los submenús (ms)
  $description: Cuánto esperan antes de abrirse. -1 = el del sistema (Windows usa 400)
- accessKeys: false
  $name: Subrayar siempre las letras de acceso
  $description: La A de Archivo, sin tener que pulsar Alt
- excludedApps: ""
  $name: Apps excluidas
  $description: Programas donde los menús quedan como estaban, separados por punto y coma. Ejemplo chrome.exe; code.exe
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
#include <cwctype>
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
// Configuración
// ---------------------------------------------------------------------------
// Se lee en el hilo de Windhawk y se usa al dibujar en los hilos de cada app:
// se guarda entera bajo un candado y se copia al usarla (son pocos bytes).

enum class ThemeMode { Auto, Light, Dark };
enum class PaletteKind { Graphite, Sand, Ice, Contrast };
enum class HiliteColor { Gray, Accent, Custom };
enum class HiliteShape { Pill, Outline, Bar, Full };
enum class SepStyle { Line, Thick, Dots, Space };
enum class CheckStyle { Check, Dot, Box };
enum class ArrowStyle { Chevron, Triangle, None };
enum class Animation { System, Fade, Slide, None };

constexpr COLORREF kNoColor = CLR_INVALID;

struct Style {
    ThemeMode theme = ThemeMode::Auto;
    PaletteKind palette = PaletteKind::Graphite;
    COLORREF lightBg = kNoColor, darkBg = kNoColor;
    HiliteColor hilite = HiliteColor::Gray;
    COLORREF hiliteColor = RGB(10, 132, 255);
    float intensity = 1.0f;
    HiliteShape shape = HiliteShape::Pill;
    float radius = 6, margin = 5;  // px a 100 %
    SepStyle separator = SepStyle::Line;
    CheckStyle check = CheckStyle::Check;
    ArrowStyle arrow = ArrowStyle::Chevron;
    bool dimShortcuts = true;
    bool gutter = false;
    DWORD corners = kCornerRound;
    bool border = true;
    COLORREF borderColor = kNoColor;
    bool menuBar = true;
    // Funciones (solo las aplica el Explorador)
    Animation animation = Animation::System;
    int submenuDelay = -1;
    bool accessKeys = false;
    bool classicExplorer = true;
    UINT gen = 1;  // cambia con cada configuración nueva
};

static SRWLOCK g_styleLock = SRWLOCK_INIT;
static Style g_style;

static Style CurrentStyle() {
    AcquireSRWLockShared(&g_styleLock);
    Style st = g_style;
    ReleaseSRWLockShared(&g_styleLock);
    return st;
}

static std::wstring StringSetting(PCWSTR name) {
    PCWSTR s = Wh_GetStringSetting(name);
    std::wstring r = s ? s : L"";
    if (s) Wh_FreeStringSetting(s);
    return r;
}

// "#RRGGBB", "RRGGBB" o "#RGB"; kNoColor si está vacío o no se entiende.
static COLORREF ParseColor(std::wstring s) {
    s.erase(std::remove_if(s.begin(), s.end(), [](wchar_t c) { return c == L' ' || c == L'#'; }), s.end());
    if (s.size() == 3) s = {s[0], s[0], s[1], s[1], s[2], s[2]};
    if (s.size() != 6 || !std::all_of(s.begin(), s.end(), iswxdigit)) return kNoColor;
    unsigned v = (unsigned)wcstoul(s.c_str(), nullptr, 16);
    return RGB((v >> 16) & 0xFF, (v >> 8) & 0xFF, v & 0xFF);
}

static void LoadSettings() {
    Style st;
    std::wstring t = StringSetting(L"theme");
    st.theme = t == L"light" ? ThemeMode::Light : t == L"dark" ? ThemeMode::Dark : ThemeMode::Auto;
    std::wstring pal = StringSetting(L"palette");
    st.palette = pal == L"sand"       ? PaletteKind::Sand
                 : pal == L"ice"      ? PaletteKind::Ice
                 : pal == L"contrast" ? PaletteKind::Contrast
                                      : PaletteKind::Graphite;
    st.lightBg = ParseColor(StringSetting(L"lightBackground"));
    st.darkBg = ParseColor(StringSetting(L"darkBackground"));
    std::wstring h = StringSetting(L"highlight");
    st.hilite = h == L"accent" ? HiliteColor::Accent : h == L"custom" ? HiliteColor::Custom : HiliteColor::Gray;
    COLORREF hc = ParseColor(StringSetting(L"highlightColor"));
    if (hc != kNoColor) st.hiliteColor = hc;
    st.intensity = std::clamp(Wh_GetIntSetting(L"highlightIntensity"), 10, 300) / 100.0f;
    std::wstring sh = StringSetting(L"highlightShape");
    st.shape = sh == L"outline" ? HiliteShape::Outline
               : sh == L"bar"   ? HiliteShape::Bar
               : sh == L"full"  ? HiliteShape::Full
                                : HiliteShape::Pill;
    st.radius = (float)std::clamp(Wh_GetIntSetting(L"highlightRadius"), 0, 14);
    st.margin = (float)std::clamp(Wh_GetIntSetting(L"highlightMargin"), 0, 12);
    std::wstring sp = StringSetting(L"separator");
    st.separator = sp == L"thick" ? SepStyle::Thick : sp == L"dots" ? SepStyle::Dots
                 : sp == L"space" ? SepStyle::Space : SepStyle::Line;
    std::wstring ck = StringSetting(L"checkStyle");
    st.check = ck == L"dot" ? CheckStyle::Dot : ck == L"box" ? CheckStyle::Box : CheckStyle::Check;
    std::wstring ar = StringSetting(L"arrowStyle");
    st.arrow = ar == L"triangle" ? ArrowStyle::Triangle : ar == L"none" ? ArrowStyle::None : ArrowStyle::Chevron;
    st.dimShortcuts = Wh_GetIntSetting(L"dimShortcuts") != 0;
    st.gutter = Wh_GetIntSetting(L"showGutter") != 0;
    std::wstring c = StringSetting(L"corners");
    st.corners = c == L"square" ? kCornerSquare : c == L"small" ? kCornerSmall : kCornerRound;
    st.border = Wh_GetIntSetting(L"border") != 0;
    st.borderColor = ParseColor(StringSetting(L"borderColor"));
    st.menuBar = Wh_GetIntSetting(L"menuBar") != 0;
    std::wstring an = StringSetting(L"animation");
    st.animation = an == L"fade"    ? Animation::Fade
                   : an == L"slide" ? Animation::Slide
                   : an == L"none"  ? Animation::None
                                    : Animation::System;
    st.submenuDelay = std::clamp(Wh_GetIntSetting(L"submenuDelay"), -1, 5000);
    st.accessKeys = Wh_GetIntSetting(L"accessKeys") != 0;
    st.classicExplorer = Wh_GetIntSetting(L"classicExplorer") != 0;
    AcquireSRWLockExclusive(&g_styleLock);
    st.gen = g_style.gen + 1;
    g_style = st;
    ReleaseSRWLockExclusive(&g_styleLock);
}

// ---------------------------------------------------------------------------
// Colores
// ---------------------------------------------------------------------------

struct Palette {
    bool dark;
    COLORREF bg, text, dim, hotText;
    COLORREF hilite;
    float hiliteA;
    COLORREF sep;     // línea del separador, ya mezclada con el fondo
    COLORREF gutter;  // franja de los iconos (si se muestra)
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

// Fondo, texto y texto tenue de cada paleta, en claro y en oscuro.
struct BaseColors {
    COLORREF bg, text, dim;
};

static BaseColors PaletteColors(PaletteKind k, bool dark) {
    switch (k) {
        case PaletteKind::Sand:
            return dark ? BaseColors{RGB(40, 36, 32), RGB(244, 238, 228), RGB(150, 140, 128)}
                        : BaseColors{RGB(250, 247, 240), RGB(40, 34, 28), RGB(160, 150, 138)};
        case PaletteKind::Ice:
            return dark ? BaseColors{RGB(28, 32, 40), RGB(236, 242, 250), RGB(130, 140, 156)}
                        : BaseColors{RGB(244, 247, 251), RGB(24, 30, 40), RGB(140, 150, 166)};
        case PaletteKind::Contrast:
            return dark ? BaseColors{RGB(14, 14, 16), RGB(255, 255, 255), RGB(165, 165, 170)}
                        : BaseColors{RGB(255, 255, 255), RGB(0, 0, 0), RGB(105, 105, 110)};
        default:  // Grafito: los del menú del Dock
            return dark ? BaseColors{RGB(36, 36, 40), RGB(242, 242, 245), RGB(140, 140, 146)}
                        : BaseColors{RGB(246, 246, 248), RGB(28, 28, 30), RGB(150, 150, 156)};
    }
}

static Palette MakePalette(const Style& st, bool dark) {
    // Un fondo propio decide por sí mismo si el texto va claro u oscuro.
    COLORREF own = dark ? st.darkBg : st.lightBg;
    if (own != kNoColor) dark = Luma(own) < 128;
    BaseColors b = PaletteColors(st.palette, dark);
    Palette p;
    p.dark = dark;
    p.bg = own != kNoColor ? own : b.bg;
    p.text = b.text;
    p.dim = b.dim;
    p.hotText = p.text;
    switch (st.hilite) {
        case HiliteColor::Accent:
            p.hilite = AccentColor();
            p.hiliteA = std::min(1.0f, st.intensity);
            p.hotText = RGB(255, 255, 255);
            break;
        case HiliteColor::Custom: {
            p.hilite = st.hiliteColor;
            p.hiliteA = std::min(1.0f, st.intensity);
            // Blanco o negro encima, según se lea mejor.
            p.hotText = Luma(Mix(p.bg, p.hilite, p.hiliteA)) > 150 ? RGB(20, 20, 22) : RGB(255, 255, 255);
            break;
        }
        default:
            // Gris claro: el del menú del Dock (negro o blanco muy suave
            // encima del fondo).
            p.hilite = dark ? RGB(255, 255, 255) : RGB(0, 0, 0);
            p.hiliteA = std::min(1.0f, (dark ? 40 / 255.0f : 24 / 255.0f) * st.intensity);
            break;
    }
    // Con contorno o barra el texto queda sobre el fondo: su color normal.
    if (st.shape == HiliteShape::Outline || st.shape == HiliteShape::Bar) p.hotText = p.text;
    p.sep = Mix(p.bg, p.text, dark ? 0.16f : 0.12f);
    p.gutter = Mix(p.bg, p.text, 0.035f);
    p.border = st.borderColor != kNoColor ? st.borderColor : Mix(p.bg, p.text, dark ? 0.2f : 0.18f);
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
    if (!info.menu) return false;
    if (!g_loggedFirst.exchange(true)) Wh_Log(L"Primer menú con estilo (tema %s)", info.dark ? L"oscuro" : L"claro");
    ThemeMode mode = CurrentStyle().theme;
    dark = mode == ThemeMode::Auto ? info.dark : mode == ThemeMode::Dark;
    return true;
}

// ---------------------------------------------------------------------------
// Ventanas de menú: esquinas y borde (DWM)
// ---------------------------------------------------------------------------

static SRWLOCK g_wndLock = SRWLOCK_INIT;
static std::unordered_map<HWND, UINT> g_styled;  // ventana -> configuración con la que se arregló

static void StyleMenuWindow(HWND h, bool dark) {
    Style st = CurrentStyle();
    UINT want = st.gen * 2 + (dark ? 1 : 0);
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
    DWORD corner = st.corners;
    DwmSetWindowAttribute(h, kDwmCornerPreference, &corner, sizeof(corner));
    COLORREF border = st.border ? MakePalette(st, dark).border : kDwmColorNone;
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

// Contorno de un rectángulo redondeado (por dentro de su borde).
static void StrokeRoundRect(Canvas& cv, float x0, float y0, float w, float h, float r, float width, COLORREF c,
                            float alpha) {
    if (!cv.bits || w <= 0 || h <= 0) return;
    float cx = x0 + w / 2, cy = y0 + h / 2, hw = w / 2, hh = h / 2;
    r = std::min(r, std::min(hw, hh));
    int ix0 = std::max(0, (int)std::floor(x0)), ix1 = std::min(cv.w, (int)std::ceil(x0 + w));
    int iy0 = std::max(0, (int)std::floor(y0)), iy1 = std::min(cv.h, (int)std::ceil(y0 + h));
    for (int y = iy0; y < iy1; y++) {
        for (int x = ix0; x < ix1; x++) {
            float d = RoundRectSdf(x + 0.5f, y + 0.5f, cx, cy, hw, hh, r);
            float cov = std::clamp(width / 2 + 0.5f - std::fabs(d + width / 2), 0.0f, 1.0f);
            Plot(cv, x, y, cov * alpha, c);
        }
    }
}

// Triángulo relleno, con antialias por muestreo (4 × 4 por píxel).
static void FillTriangle(Canvas& cv, const float* p, COLORREF c) {
    if (!cv.bits) return;
    auto edge = [](float ax, float ay, float bx, float by, float x, float y) {
        return (bx - ax) * (y - ay) - (by - ay) * (x - ax);
    };
    float area = edge(p[0], p[1], p[2], p[3], p[4], p[5]);
    if (std::fabs(area) < 1e-3f) return;
    float sign = area > 0 ? 1.0f : -1.0f;
    int x0 = std::max(0, (int)std::floor(std::min({p[0], p[2], p[4]})));
    int x1 = std::min(cv.w, (int)std::ceil(std::max({p[0], p[2], p[4]})));
    int y0 = std::max(0, (int)std::floor(std::min({p[1], p[3], p[5]})));
    int y1 = std::min(cv.h, (int)std::ceil(std::max({p[1], p[3], p[5]})));
    for (int y = y0; y < y1; y++) {
        for (int x = x0; x < x1; x++) {
            int in = 0;
            for (int sy = 0; sy < 4; sy++) {
                for (int sx = 0; sx < 4; sx++) {
                    float px = x + (sx + 0.5f) / 4, py = y + (sy + 0.5f) / 4;
                    in += sign * edge(p[0], p[1], p[2], p[3], px, py) >= 0 &&
                          sign * edge(p[2], p[3], p[4], p[5], px, py) >= 0 &&
                          sign * edge(p[4], p[5], p[0], p[1], px, py) >= 0;
                }
            }
            Plot(cv, x, y, in / 16.0f, c);
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

// Franja de los iconos que se pintó en este hilo (para respetarla al pintar
// cada opción encima, si se muestra).
struct GutterSpan {
    DWORD tid = 0;
    HDC dc = nullptr;
    LONG left = 0, right = 0;
};
static SRWLOCK g_gutterLock = SRWLOCK_INIT;
static GutterSpan g_gutterSpan;

static void RememberGutter(HDC hdc, const RECT& rc) {
    AcquireSRWLockExclusive(&g_gutterLock);
    g_gutterSpan = GutterSpan{GetCurrentThreadId(), hdc, rc.left, rc.right};
    ReleaseSRWLockExclusive(&g_gutterLock);
}

// Fondo de una opción; con la franja visible, su tramo va en su color.
static void FillItemBackground(HDC hdc, const RECT& rc, const Style& st, const Palette& p) {
    FillSolid(hdc, rc, p.bg);
    if (!st.gutter) return;
    AcquireSRWLockShared(&g_gutterLock);
    GutterSpan g = g_gutterSpan;
    ReleaseSRWLockShared(&g_gutterLock);
    if (g.tid != GetCurrentThreadId() || g.dc != hdc) return;
    RECT band{std::max(rc.left, g.left), rc.top, std::min(rc.right, g.right), rc.bottom};
    if (band.right > band.left) FillSolid(hdc, band, p.gutter);
}

// Resaltado de la opción bajo el mouse, con la forma elegida.
static void DrawHilite(HDC hdc, const RECT& rc, float s, const Style& st, const Palette& p) {
    int w = rc.right - rc.left, h = rc.bottom - rc.top;
    if (w <= 0 || h <= 0) return;
    Canvas cv(w, h);
    bool full = st.shape == HiliteShape::Full;
    float ix = full ? 0 : std::min(st.margin * s, w / 4.0f), iy = full ? 0 : std::min(1.0f * s, h / 4.0f);
    float bw = w - 2 * ix, bh = h - 2 * iy, r = full ? 0 : st.radius * s;
    switch (st.shape) {
        case HiliteShape::Outline:
            StrokeRoundRect(cv, ix, iy, bw, bh, r, std::max(1.0f, 1.25f * s), p.hilite, std::min(1.0f, p.hiliteA * 3));
            break;
        case HiliteShape::Bar: {
            // Un tinte suave y una barrita vertical a la izquierda.
            FillRoundRect(cv, ix, iy, bw, bh, r, p.hilite, st.hilite == HiliteColor::Gray ? p.hiliteA : p.hiliteA * 0.18f);
            float barW = std::max(2.0f, 3.0f * s), pad = std::min(4.0f * s, bh / 4);
            COLORREF bar = st.hilite == HiliteColor::Gray ? p.text : p.hilite;
            FillRoundRect(cv, ix + 3.0f * s, iy + pad, barW, bh - 2 * pad, barW / 2, bar,
                          st.hilite == HiliteColor::Gray ? 0.55f : 1.0f);
            break;
        }
        default:
            FillRoundRect(cv, ix, iy, bw, bh, r, p.hilite, p.hiliteA);
            break;
    }
    cv.Present(hdc, rc.left, rc.top);
}

// Separador con el estilo elegido (el fondo ya está pintado).
static void DrawSeparator(HDC hdc, const RECT& rc, float s, const Style& st, const Palette& p) {
    int inset = (int)std::lround(10 * s);
    RECT span{rc.left + inset, rc.top, rc.right - inset, rc.bottom};
    if (span.right <= span.left) return;
    switch (st.separator) {
        case SepStyle::Space:
            return;
        case SepStyle::Dots: {
            Canvas cv(span.right - span.left, rc.bottom - rc.top);
            float step = std::max(4.0f, 5.0f * s), r = std::max(0.8f, 0.9f * s), cy = cv.h / 2.0f;
            for (float x = r + 0.5f; x < cv.w - r; x += step) FillDot(cv, x, cy, r, Mix(p.bg, p.text, 0.35f));
            cv.Present(hdc, span.left, span.top);
            return;
        }
        default: {
            bool thick = st.separator == SepStyle::Thick;
            int th = thick ? std::max(2, (int)std::lround(2 * s)) : std::max(1, (int)std::lround(s));
            int y = (rc.top + rc.bottom - th) / 2;
            FillSolid(hdc, RECT{span.left, y, span.right, y + th}, thick ? Mix(p.bg, p.text, 0.22f) : p.sep);
            return;
        }
    }
}

// Opción marcada (palomita, punto o casilla) o de grupo (punto), centrada.
static void DrawCheckMark(HDC hdc, const RECT& rc, float s, const Style& st, bool bullet, COLORREF c, COLORREF bg) {
    int w = rc.right - rc.left, h = rc.bottom - rc.top;
    if (w <= 0 || h <= 0) return;
    Canvas cv(w, h);
    float cx = w / 2.0f, cy = h / 2.0f, g = std::min(10.0f * s, std::min(w, h) * 0.62f);
    auto check = [&](float k, COLORREF col) {
        float pts[] = {cx - g * 0.42f * k, cy + g * 0.02f * k, cx - g * 0.12f * k, cy + g * 0.32f * k,
                       cx + g * 0.44f * k, cy - g * 0.34f * k};
        StrokePolyline(cv, pts, 3, std::max(1.5f * s, g * 0.16f) * std::min(1.0f, k + 0.1f), col);
    };
    switch (st.check) {
        case CheckStyle::Box:
            if (bullet) {
                FillDot(cv, cx, cy, g * 0.55f, c);
                FillDot(cv, cx, cy, g * 0.2f, bg);
            } else {
                float side = g * 1.15f;
                FillRoundRect(cv, cx - side / 2, cy - side / 2, side, side, side * 0.24f, c, 1.0f);
                check(0.72f, bg);
            }
            break;
        case CheckStyle::Dot:
            FillDot(cv, cx, cy, g * (bullet ? 0.26f : 0.3f), c);
            break;
        default:
            if (bullet) FillDot(cv, cx, cy, g * 0.26f, c);
            else check(1.0f, c);
            break;
    }
    cv.Present(hdc, rc.left, rc.top);
}

// Flecha de submenú: un ángulo fino (como en macOS) o un triángulo.
static void DrawArrow(HDC hdc, const RECT& rc, float s, const Style& st, COLORREF c) {
    int w = rc.right - rc.left, h = rc.bottom - rc.top;
    if (w <= 0 || h <= 0 || st.arrow == ArrowStyle::None) return;
    Canvas cv(w, h);
    float cx = w / 2.0f, cy = h / 2.0f, g = std::min(9.0f * s, std::min(w, h) * 0.6f);
    if (st.arrow == ArrowStyle::Triangle) {
        float pts[] = {cx - g * 0.22f, cy - g * 0.42f, cx + g * 0.3f, cy, cx - g * 0.22f, cy + g * 0.42f};
        FillTriangle(cv, pts, c);
    } else {
        float pts[] = {cx - g * 0.18f, cy - g * 0.40f, cx + g * 0.22f, cy, cx - g * 0.18f, cy + g * 0.40f};
        StrokePolyline(cv, pts, 3, std::max(1.4f * s, g * 0.15f), c);
    }
    cv.Present(hdc, rc.left, rc.top);
}

// Pinta una parte del menú con el estilo propio. false = que la dibuje el tema.
static bool PaintMenuPart(HDC hdc, int part, int state, const RECT& rc, const RECT* clip, bool dark) {
    if (rc.right <= rc.left || rc.bottom <= rc.top) return false;
    Style st = CurrentStyle();
    Palette p = MakePalette(st, dark);
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
        case kPopupGutter:
            RememberGutter(hdc, rc);
            FillSolid(hdc, rc, st.gutter ? p.gutter : p.bg);
            return true;
        case kPopupCheckBackground:  // sin recuadro detrás de la palomita
            return true;
        case kPopupSeparator:
            FillItemBackground(hdc, rc, st, p);
            DrawSeparator(hdc, rc, s, st, p);
            return true;
        case kPopupItem:
        case kPopupItemFocusable: {
            FillItemBackground(hdc, rc, st, p);
            if (state == 2) {  // las desactivadas no se resaltan (como en macOS)
                DrawHilite(hdc, rc, s, st, p);
                SetHotItem(hdc, &rc);
            } else if (OnHotItem(hdc, rc)) {
                SetHotItem(hdc, nullptr);
            }
            return true;
        }
        case kPopupCheck: {
            bool disabled = state == 2 || state == 4;
            bool hot = OnHotItem(hdc, rc);
            COLORREF c = disabled ? p.dim : hot ? p.hotText : p.text;
            // La casilla "hueca" toma el color de lo que tiene detrás.
            COLORREF behind = hot && p.hotText != p.text ? Mix(p.bg, p.hilite, p.hiliteA) : p.bg;
            DrawCheckMark(hdc, rc, s, st, state >= 3, c, behind);
            return true;
        }
        case kPopupSubmenu: {
            COLORREF c = state == 2 ? p.dim : OnHotItem(hdc, rc) ? p.hotText : p.text;
            DrawArrow(hdc, rc, s, st, c);
            return true;
        }
        case kBarItem: {
            // Barra de menús: solo el resaltado (gris claro, en pastilla),
            // sobre el fondo que ya tenga.
            if (!st.menuBar || (state != 2 && state != 3)) return false;
            Style bar = st;
            bar.hilite = HiliteColor::Gray;
            bar.shape = HiliteShape::Pill;
            bar.margin = 2;
            DrawHilite(hdc, rc, s, bar, MakePalette(bar, dark));
            return true;
        }
    }
    return false;
}

static bool IsStyledPart(int part) {
    return (part >= kBarItem && part <= kPopupSubmenu) || part == kPopupItemFocusable;
}

// Color del texto de una opción según su estado. shortcut = el atajo de
// teclado (Windows lo dibuja aparte, alineado a la derecha).
static COLORREF ItemTextColor(int state, bool dark, bool shortcut) {
    Style st = CurrentStyle();
    Palette p = MakePalette(st, dark);
    bool dimmed = shortcut && st.dimShortcuts;
    switch (state) {
        case 2:
            return p.hotText != p.text ? p.hotText : dimmed ? p.dim : p.text;
        case 3:
        case 4:
            return p.dim;
        default:
            return dimmed ? p.dim : p.text;
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
        o.crText = ItemTextColor(state, dark, (flags & DT_RIGHT) != 0);
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
        o.crText = ItemTextColor(state, dark, (flags & DT_RIGHT) != 0);
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

// Nombre del programa de este proceso, en minúsculas ("explorer.exe").
static std::wstring ProcessName() {
    wchar_t path[MAX_PATH] = {};
    GetModuleFileNameW(nullptr, path, ARRAYSIZE(path));
    const wchar_t* slash = wcsrchr(path, L'\\');
    std::wstring name = slash ? slash + 1 : path;
    CharLowerBuffW(name.data(), (DWORD)name.size());
    return name;
}

// ¿Está el programa en la lista de apps excluidas? ("chrome.exe; code")
static bool IsExcluded(const std::wstring& list, const std::wstring& process) {
    size_t pos = 0;
    while (pos < list.size()) {
        size_t end = list.find_first_of(L";,", pos);
        if (end == std::wstring::npos) end = list.size();
        std::wstring item = list.substr(pos, end - pos);
        pos = end + 1;
        size_t a = item.find_first_not_of(L" \t\"'"), b = item.find_last_not_of(L" \t\"'");
        if (a == std::wstring::npos) continue;
        item = item.substr(a, b - a + 1);
        CharLowerBuffW(item.data(), (DWORD)item.size());
        if (item.find(L'.') == std::wstring::npos) item += L".exe";
        if (item == process) return true;
    }
    return false;
}

// ---------------------------------------------------------------------------
// Animación, retraso de los submenús y letras de acceso
// ---------------------------------------------------------------------------
// Son ajustes de todo Windows. Los aplica el Explorador sin guardarlos en el
// perfil (vuelven solos al cerrar sesión) y se recuerdan los originales para
// devolverlos al desactivar el mod o la opción.

static bool GetSpiBool(UINT action) {
    BOOL v = FALSE;
    SystemParametersInfoW(action, 0, &v, 0);
    return v != FALSE;
}

static void SetSpiBool(UINT get, UINT set, bool v) {
    if (GetSpiBool(get) != v) SystemParametersInfoW(set, 0, (PVOID)(UINT_PTR)(v ? TRUE : FALSE), SPIF_SENDCHANGE);
}

static void SetSubmenuDelay(int ms) {
    DWORD now = 0;
    SystemParametersInfoW(SPI_GETMENUSHOWDELAY, 0, &now, 0);
    if ((int)now != ms) SystemParametersInfoW(SPI_SETMENUSHOWDELAY, (UINT)ms, nullptr, SPIF_SENDCHANGE);
}

static void RestoreSystemMenuSettings() {
    if (!Wh_GetIntValue(L"sysSaved", 0)) return;
    SetSpiBool(SPI_GETMENUANIMATION, SPI_SETMENUANIMATION, Wh_GetIntValue(L"origMenuAnimation", 1) != 0);
    SetSpiBool(SPI_GETMENUFADE, SPI_SETMENUFADE, Wh_GetIntValue(L"origMenuFade", 1) != 0);
    SetSubmenuDelay(Wh_GetIntValue(L"origMenuDelay", 400));
    SetSpiBool(SPI_GETKEYBOARDCUES, SPI_SETKEYBOARDCUES, Wh_GetIntValue(L"origKeyboardCues", 0) != 0);
    Wh_SetIntValue(L"sysSaved", 0);
}

static void ApplySystemMenuSettings(const Style& st) {
    if (!g_isExplorer) return;
    bool touches = st.animation != Animation::System || st.submenuDelay >= 0 || st.accessKeys;
    if (!touches) {
        RestoreSystemMenuSettings();
        return;
    }
    if (!Wh_GetIntValue(L"sysSaved", 0)) {
        DWORD delay = 400;
        SystemParametersInfoW(SPI_GETMENUSHOWDELAY, 0, &delay, 0);
        Wh_SetIntValue(L"origMenuAnimation", GetSpiBool(SPI_GETMENUANIMATION));
        Wh_SetIntValue(L"origMenuFade", GetSpiBool(SPI_GETMENUFADE));
        Wh_SetIntValue(L"origMenuDelay", (int)delay);
        Wh_SetIntValue(L"origKeyboardCues", GetSpiBool(SPI_GETKEYBOARDCUES));
        Wh_SetIntValue(L"sysSaved", 1);
    }
    bool anim = Wh_GetIntValue(L"origMenuAnimation", 1) != 0, fade = Wh_GetIntValue(L"origMenuFade", 1) != 0;
    switch (st.animation) {
        case Animation::Fade:
            anim = fade = true;
            break;
        case Animation::Slide:
            anim = true;
            fade = false;
            break;
        case Animation::None:
            anim = false;
            break;
        default:
            break;
    }
    SetSpiBool(SPI_GETMENUANIMATION, SPI_SETMENUANIMATION, anim);
    SetSpiBool(SPI_GETMENUFADE, SPI_SETMENUFADE, fade);
    SetSubmenuDelay(st.submenuDelay >= 0 ? st.submenuDelay : Wh_GetIntValue(L"origMenuDelay", 400));
    SetSpiBool(SPI_GETKEYBOARDCUES, SPI_SETKEYBOARDCUES,
               st.accessKeys || Wh_GetIntValue(L"origKeyboardCues", 0) != 0);
}

static bool OwnsTaskbar() {
    HWND tray = FindWindowW(L"Shell_TrayWnd", nullptr);
    DWORD pid = 0;
    if (tray) GetWindowThreadProcessId(tray, &pid);
    return pid == GetCurrentProcessId();
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

static void ApplyExplorerOptions() {
    Style st = CurrentStyle();
    ApplyClassicMenu(st.classicExplorer);
    ApplySystemMenuSettings(st);
}

BOOL Wh_ModInit() {
    LoadSettings();
    std::wstring process = ProcessName();
    g_isExplorer = process == L"explorer.exe";
    if (IsExcluded(StringSetting(L"excludedApps"), process)) {
        // Se queda cargado sin tocar nada: la lista se vuelve a mirar al
        // abrir de nuevo la app.
        Wh_Log(L"%s está en las apps excluidas", process.c_str());
        return TRUE;
    }
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
    ApplyExplorerOptions();
    return TRUE;
}

void Wh_ModUninit() {
    // La animación y los demás ajustes de Windows vuelven como estaban (solo
    // el Explorador del escritorio: no una ventana de carpeta en otro
    // proceso). La clave del menú clásico se deja: el mod también se
    // descarga al reiniciar el Explorador, y quitarla ahí haría que el nuevo
    // arranque con el menú de Windows 11. Se quita al apagar la opción.
    if (g_isExplorer && OwnsTaskbar()) RestoreSystemMenuSettings();
}

void Wh_ModSettingsChanged() {
    LoadSettings();
    ApplyExplorerOptions();
}
