// ==WindhawkMod==
// @id              dock-estilo-apple
// @name            Dock estilo Apple
// @description     Reemplaza la barra de tareas por un Dock estilo macOS: ampliación fluida a la frecuencia del monitor, cristal líquido, arrastrar para ordenar y la papelera de Windows
// @version         3.0
// @author          local
// @include         explorer.exe
// @architecture    x86-64
// @compilerOptions -lole32 -loleaut32 -lshell32 -lgdi32 -luser32 -ladvapi32 -ldwmapi -lgdiplus -lshcore -lversion -luuid
// ==/WindhawkMod==

// ==WindhawkModReadme==
/*
# Dock estilo Apple

Un Dock como el de macOS abajo de la pantalla principal, que reemplaza a la
barra de tareas. Hace pareja con **Muesca superior estilo Apple** (barra de
menús, reloj y bandeja arriba; el Dock abajo).

## Qué hace

* **Apps ancladas y abiertas.** Al primer arranque toma las apps ancladas a
  la barra de tareas, incluidas las de la Tienda (Configuración,
  Calculadora…). Las apps abiertas sin anclar van a la derecha; un punto
  debajo indica que están abiertas.
* **Ampliación fluida.** Cada cuadro se sincroniza con el refresco real del
  monitor (60, 120, 144 Hz…). Los iconos se dibujan con precisión de
  subpíxel, así que se mueven sin saltos.
* **Rebotes discretos** al abrir una app y cuando una ventana pide atención.
* **Clic:** abre la app o trae su ventana; en la app activa la minimiza (o
  pasa a su siguiente ventana). Mayús + clic o clic central abre otra.
* **Clic derecho:** ventanas, Mantener en el Dock, Mostrar en el Explorador,
  Salir.
* **Arrastrar para ordenar.** Arrastrar un icono hacia arriba, o soltarlo en
  la papelera, lo quita del Dock.
* **Soltar archivos:** en la papelera los manda a la Papelera de reciclaje
  (la de Windows, con deshacer); en una app los abre con ella; una app
  (.exe o acceso directo) soltada entre los iconos se ancla.
* **Inicio** a la izquierda con el logo de Windows del sistema (clic = menú
  Inicio, clic derecho = menú Win + X) y la **Papelera** de Windows al final.
* **Se esconde** con una ventana maximizada o en pantalla completa; aparece
  al llevar el mouse al borde de abajo, encima de las ventanas y ocupando
  solo su propio espacio.

## Notas

* Ocultar la barra de tareas la pone en ocultar automáticamente y la
  esconde; al desactivar el mod vuelve como estaba.
* Las insignias de número y las barras de progreso de la barra de tareas no
  se muestran en el Dock.
*/
// ==/WindhawkModReadme==

// ==WindhawkModSettings==
/*
- iconSize: 48
  $name: Tamaño de los iconos
  $description: En píxeles a escala de 100 % (32 a 96)
- spacing: 5
  $name: Separación entre apps
  $description: En píxeles a escala de 100 % (0 a 40)
- magnification: true
  $name: Ampliar los iconos al pasar el mouse
- magnifiedSize: 80
  $name: Tamaño ampliado
  $description: En píxeles a escala de 100 % (48 a 160)
- magnificationRange: 3
  $name: Alcance de la ampliación
  $description: Cuántos iconos a cada lado crecen (1 a 6)
- style: glass
  $name: Fondo del Dock
  $options:
  - glass: Cristal líquido (fondo de pantalla desenfocado)
  - translucent: Translúcido según el tema de Windows
  - dark: Oscuro
  - light: Claro
- backgroundOpacity: 100
  $name: Opacidad del fondo (%)
  $description: 0 = sin fondo, solo los iconos
- cornerRadius: 100
  $name: Redondeo de las esquinas (%)
- bottomMargin: 7
  $name: Distancia al borde de abajo
  $description: En píxeles a escala de 100 % (0 a 40)
- autoHide: maximized
  $name: Esconder el Dock
  $description: Para mostrarlo, lleva el mouse al borde de abajo
  $options:
  - maximized: Con una ventana maximizada o en pantalla completa
  - always: Siempre
  - never: Nunca (reserva una franja abajo para las ventanas)
- revealDelay: 80
  $name: Espera para aparecer (ms)
  $description: Tiempo con el mouse en el borde de abajo
- hideDelay: 700
  $name: Espera para esconderse (ms)
- hideTaskbar: true
  $name: Ocultar la barra de tareas de Windows
- showStart: true
  $name: Botón de Inicio a la izquierda
- startIcon: ""
  $name: Icono de Inicio
  $description: Vacío = el logo de Windows del sistema. O una ruta a .ico / .png, o archivo.exe,índice
- showTrash: true
  $name: Papelera al final
- trashIcon: windows
  $name: Icono de la papelera
  $options:
  - windows: El de Windows
  - vector: Vectorial (estilo Apple)
- showUnpinned: true
  $name: Mostrar las apps abiertas que no están ancladas
- indicator: dot
  $name: Indicador de app abierta
  $options:
  - dot: Un punto
  - dots: Un punto por ventana (hasta 3)
  - line: Una línea
  - none: Ninguno
- showLabels: true
  $name: Nombre de la app al pasar el mouse
- iconShadows: true
  $name: Sombra suave bajo los iconos
- windowPreviews: thumbnails
  $name: Al pasar el mouse sobre una app abierta
  $options:
  - thumbnails: Ver sus ventanas en miniatura
  - list: Ver la lista de sus ventanas
  - off: Nada (solo el nombre)
- previewDelay: 450
  $name: Espera para ver las ventanas (ms)
- clickMinimizes: true
  $name: Clic en la app activa la minimiza
  $description: Desactivado = pasa a su siguiente ventana
- middleClick: newWindow
  $name: Clic central sobre una app
  $options:
  - newWindow: Abrir otra ventana
  - close: Cerrar sus ventanas
  - none: Nada
- scrollAction: cycle
  $name: Rueda del mouse sobre una app
  $options:
  - cycle: Pasar entre sus ventanas (abajo = minimizar)
  - none: Nada
- lockItems: false
  $name: Bloquear el orden de los iconos
  $description: No se pueden arrastrar ni quitar del Dock
- bounceOnLaunch: true
  $name: Rebotar al abrir una app
- bounceOnAttention: true
  $name: Rebotar cuando una app pide atención
- animationBounce: false
  $name: Pequeño rebote al aparecer y al soltar iconos
  $description: Desactivado = movimientos sin rebote, más tranquilos
- animationSpeed: normal
  $name: Velocidad de las animaciones
  $options:
  - slow: Lenta
  - normal: Normal
  - fast: Rápida
- font: Segoe UI Variable Text
  $name: Fuente de los textos
*/
// ==/WindhawkModSettings==

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <windowsx.h>
#include <dwmapi.h>
#include <shellapi.h>
#include <shlobj.h>
#include <shobjidl.h>
#include <shellscalingapi.h>
#include <propsys.h>
#include <commoncontrols.h>
#include <knownfolders.h>

#include <algorithm>
#include <atomic>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <deque>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

namespace Gdiplus {
using std::max;
using std::min;
}  // namespace Gdiplus
#include <gdiplus.h>
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

#ifndef CREATE_WAITABLE_TIMER_HIGH_RESOLUTION
#define CREATE_WAITABLE_TIMER_HIGH_RESOLUTION 0x00000002
#endif

// ---------------------------------------------------------------------------
// Constantes
// ---------------------------------------------------------------------------

constexpr double kPi = 3.14159265358979323846;

enum : UINT {
    WM_APP_FRAME = WM_APP + 1,  // lParam = QPC del refresco (vblank)
    WM_APP_EXIT,
    WM_APP_SETTINGS,
    WM_APP_JOBS,      // terminó trabajo del hilo de iconos
    WM_APP_WINDOWS,   // nuevo listado de ventanas
    WM_APP_APPBAR,
    WM_APP_TRASH,
    WM_APP_LAUNCHFAIL,
    WM_APP_TRAYSHOW,
    WM_APP_SNAP,      // llegó una foto de ventana
};

enum : UINT_PTR {
    TIMER_POLL = 1,  // mouse: entrar/salir, borde de abajo, esconderse
    TIMER_SLOW = 2,  // papelera, barra de tareas, fondo de pantalla
    TIMER_TOPMOST = 3,
};

static const wchar_t kDockClass[] = L"WindhawkAppleDock";
static const wchar_t kMenuHostClass[] = L"WindhawkAppleDockMenuHost";
static const wchar_t kDragClass[] = L"WindhawkAppleDockDrag";
static const wchar_t kAppsFolderPrefix[] = L"::{4234D49B-0245-4DF3-B780-3893943456E1}\\";

// ---------------------------------------------------------------------------
// Configuración
// ---------------------------------------------------------------------------

enum class DockStyle { Glass, Translucent, Dark, Light };
enum class AutoHide { Never, Maximized, Always };

enum class Indicator { Dot, Dots, Line, None };
enum class Previews { Thumbnails, List, Off };

struct Settings {
    int iconSize = 48;
    int spacing = 5;
    bool magnify = true;
    int magSize = 80;
    int magRange = 3;
    double bgOpacity = 1.0;
    double cornerRadius = 1.0;
    int bottomMargin = 7;
    double revealDelay = 0.08;
    double hideDelay = 0.7;
    bool vectorTrash = false;
    bool showUnpinned = true;
    Indicator indicator = Indicator::Dot;
    Previews previews = Previews::Thumbnails;
    double previewDelay = 0.45;
    int middleClick = 0;  // 0 nueva ventana, 1 cerrar, 2 nada
    bool scrollCycle = true;
    bool lockItems = false;
    DockStyle style = DockStyle::Glass;
    AutoHide autoHide = AutoHide::Maximized;
    bool hideTaskbar = true;
    bool showStart = true;
    std::wstring startIcon;
    bool showTrash = true;
    bool indicators = true;
    bool labels = true;
    bool shadows = true;
    bool clickMinimizes = true;
    bool bounceLaunch = true;
    bool bounceAttention = true;
    bool animBounce = false;
    double speed = 1.0;  // multiplica la duración de los resortes
    std::wstring font = L"Segoe UI Variable Text";
};

static Settings g_set;

static std::wstring StringSetting(PCWSTR name) {
    PCWSTR s = Wh_GetStringSetting(name);
    std::wstring r = s ? s : L"";
    if (s) Wh_FreeStringSetting(s);
    return r;
}

static void LoadSettings() {
    Settings s;
    s.iconSize = std::clamp(Wh_GetIntSetting(L"iconSize"), 32, 96);
    s.spacing = std::clamp(Wh_GetIntSetting(L"spacing"), 0, 40);
    s.magnify = Wh_GetIntSetting(L"magnification") != 0;
    s.magSize = std::clamp(Wh_GetIntSetting(L"magnifiedSize"), 48, 160);
    if (s.magSize < s.iconSize) s.magSize = s.iconSize;
    s.magRange = std::clamp(Wh_GetIntSetting(L"magnificationRange"), 1, 6);
    s.bgOpacity = std::clamp(Wh_GetIntSetting(L"backgroundOpacity"), 0, 100) / 100.0;
    s.cornerRadius = std::clamp(Wh_GetIntSetting(L"cornerRadius"), 0, 100) / 100.0;
    s.bottomMargin = std::clamp(Wh_GetIntSetting(L"bottomMargin"), 0, 40);
    s.revealDelay = std::clamp(Wh_GetIntSetting(L"revealDelay"), 0, 2000) / 1000.0;
    s.hideDelay = std::clamp(Wh_GetIntSetting(L"hideDelay"), 100, 5000) / 1000.0;
    s.vectorTrash = StringSetting(L"trashIcon") == L"vector";
    s.showUnpinned = Wh_GetIntSetting(L"showUnpinned") != 0;
    std::wstring ind = StringSetting(L"indicator");
    s.indicator = ind == L"dots" ? Indicator::Dots : ind == L"line" ? Indicator::Line
                : ind == L"none" ? Indicator::None : Indicator::Dot;
    std::wstring pv = StringSetting(L"windowPreviews");
    s.previews = pv == L"list" ? Previews::List : pv == L"off" ? Previews::Off : Previews::Thumbnails;
    s.previewDelay = std::clamp(Wh_GetIntSetting(L"previewDelay"), 0, 3000) / 1000.0;
    std::wstring mc = StringSetting(L"middleClick");
    s.middleClick = mc == L"close" ? 1 : mc == L"none" ? 2 : 0;
    s.scrollCycle = StringSetting(L"scrollAction") != L"none";
    s.lockItems = Wh_GetIntSetting(L"lockItems") != 0;
    std::wstring style = StringSetting(L"style");
    s.style = style == L"translucent" ? DockStyle::Translucent
              : style == L"dark"      ? DockStyle::Dark
              : style == L"light"     ? DockStyle::Light
                                      : DockStyle::Glass;
    std::wstring hide = StringSetting(L"autoHide");
    s.autoHide = hide == L"always"  ? AutoHide::Always
                 : hide == L"never" ? AutoHide::Never
                                    : AutoHide::Maximized;
    s.hideTaskbar = Wh_GetIntSetting(L"hideTaskbar") != 0;
    s.showStart = Wh_GetIntSetting(L"showStart") != 0;
    s.startIcon = StringSetting(L"startIcon");
    s.showTrash = Wh_GetIntSetting(L"showTrash") != 0;
    s.indicators = s.indicator != Indicator::None;
    s.labels = Wh_GetIntSetting(L"showLabels") != 0;
    s.shadows = Wh_GetIntSetting(L"iconShadows") != 0;
    s.clickMinimizes = Wh_GetIntSetting(L"clickMinimizes") != 0;
    s.bounceLaunch = Wh_GetIntSetting(L"bounceOnLaunch") != 0;
    s.bounceAttention = Wh_GetIntSetting(L"bounceOnAttention") != 0;
    s.animBounce = Wh_GetIntSetting(L"animationBounce") != 0;
    std::wstring speed = StringSetting(L"animationSpeed");
    s.speed = speed == L"slow" ? 1.4 : speed == L"fast" ? 0.72 : 1.0;
    s.font = StringSetting(L"font");
    if (s.font.empty()) s.font = L"Segoe UI Variable Text";
    g_set = s;
}

// ---------------------------------------------------------------------------
// Estado general y utilidades
// ---------------------------------------------------------------------------

static HANDLE g_thread = nullptr;
static std::atomic<bool> g_unloading{false};
static std::atomic<HWND> g_wnd{nullptr};
static HWND g_menuHost = nullptr;
static double g_qpcFreq = 1;

static RECT g_mon{};  // monitor principal (coordenadas de pantalla)
static HMONITOR g_monitor = nullptr;
static std::wstring g_monDevice;
static UINT g_dpi = 96;
static bool g_light = false;

// El lienzo cubre todo el ancho del monitor (coordenadas "del lienzo"); la
// ventana real es solo la parte que ocupa el Dock (g_envL..g_envR).
static int g_winX = 0, g_winY = 0, g_winW = 0, g_winH = 0;
static int g_envL = 0, g_envR = 0;
static double g_envShrinkSince = -1;

static HDC g_memDC = nullptr;
static HBITMAP g_dib = nullptr;
static HGDIOBJ g_oldBmp = nullptr;
static uint32_t* g_bits = nullptr;
static bool g_fullRedraw = true;
static RECT g_prevBounds{};

static UINT g_msgShellHook = 0;
static UINT g_msgTaskbarCreated = 0;

// Solo para pruebas: fuerza el periodo de cuadro (ms). 0 = el del monitor.
static double g_forcedPeriodMs = 0;
// Solo para pruebas: cuánto tarda cada mensaje en el hilo del Dock.
static bool g_profileOn = false;
// Contadores para diagnóstico (se ven en las pruebas).
static int g_dbgUlwFail = 0;
static POINT g_dbgDownPt{};
static int g_dbgDownIndex = -2, g_dbgPollRelease = 0, g_dbgUp = 0;
static int g_dbgDockDown = 0, g_dbgPopDown = 0, g_dbgHookOn = 0, g_dbgHookOff = 0, g_dbgKeys = 0;
static std::map<UINT, std::pair<double, int>> g_profileMax;  // msg -> (máx ms, cuántos > 4 ms)

static double Now() {
    LARGE_INTEGER c;
    QueryPerformanceCounter(&c);
    return (double)c.QuadPart / g_qpcFreq;
}

static int S(double v) { return (int)std::lround(v * g_dpi / 96.0); }

static std::wstring Lower(std::wstring s) {
    if (!s.empty()) CharLowerBuffW(s.data(), (DWORD)s.size());
    return s;
}

static bool EqI(const std::wstring& a, const std::wstring& b) {
    return _wcsicmp(a.c_str(), b.c_str()) == 0;
}

static bool StartsWithI(const std::wstring& s, const wchar_t* prefix) {
    size_t n = wcslen(prefix);
    return s.size() >= n && _wcsnicmp(s.c_str(), prefix, n) == 0;
}

static bool EndsWithI(const std::wstring& s, const wchar_t* suffix) {
    size_t n = wcslen(suffix);
    return s.size() >= n && _wcsicmp(s.c_str() + s.size() - n, suffix) == 0;
}

static std::wstring FileNameNoExt(const std::wstring& path) {
    size_t slash = path.find_last_of(L"\\/");
    std::wstring name = slash == std::wstring::npos ? path : path.substr(slash + 1);
    size_t dot = name.find_last_of(L'.');
    if (dot != std::wstring::npos && dot > 0) name.resize(dot);
    return name;
}

static std::wstring ExpandEnv(const std::wstring& s) {
    if (s.find(L'%') == std::wstring::npos) return s;
    wchar_t buf[2048];
    DWORD n = ExpandEnvironmentStringsW(s.c_str(), buf, ARRAYSIZE(buf));
    return n && n <= ARRAYSIZE(buf) ? std::wstring(buf) : s;
}

static bool FileExists(const std::wstring& p) {
    return !p.empty() && GetFileAttributesW(p.c_str()) != INVALID_FILE_ATTRIBUTES;
}

static bool ReadThemeIsLight() {
    DWORD v = 0, size = sizeof(v);
    if (RegGetValueW(HKEY_CURRENT_USER,
                     L"Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize",
                     L"SystemUsesLightTheme", RRF_RT_REG_DWORD, nullptr, &v,
                     &size) == ERROR_SUCCESS) {
        return v != 0;
    }
    return false;
}

static bool IsCloaked(HWND h) {
    BOOL cloaked = FALSE;
    return SUCCEEDED(DwmGetWindowAttribute(h, DWMWA_CLOAKED, &cloaked, sizeof(cloaked))) &&
           cloaked;
}

static std::wstring ClassOf(HWND h) {
    wchar_t buf[128] = {};
    GetClassNameW(h, buf, ARRAYSIZE(buf));
    return buf;
}

// ---------------------------------------------------------------------------
// Resortes (integración a 600 Hz, como en la muesca)
// ---------------------------------------------------------------------------

struct Spring {
    double x = 0;
    double v = 0;
    double target = 0;
    bool settled = true;

    void Snap(double value) {
        x = target = value;
        v = 0;
        settled = true;
    }

    void SetTarget(double t) {
        if (t != target) {
            target = t;
            settled = false;
        }
    }

    // response: segundos de un ciclo; damping: 1 = sin rebote, < 1 = rebote
    void Step(double dt, double response, double damping, double eps) {
        if (settled) return;
        double k = (2 * kPi / response) * (2 * kPi / response);
        double c = 4 * kPi * damping / response;
        const double h = 1.0 / 600.0;
        while (dt > 1e-9) {
            double s = std::min(dt, h);
            double a = -k * (x - target) - c * v;
            v += a * s;
            x += v * s;
            dt -= s;
        }
        if (std::fabs(x - target) < eps && std::fabs(v) < eps * 10) {
            x = target;
            v = 0;
            settled = true;
        }
    }
};

// Rebote solo si se pidió; si no, todo llega sin pasarse (más tranquilo).
static double Bouncy() { return g_set.animBounce ? 0.82 : 1.0; }

// ---------------------------------------------------------------------------
// Imágenes en memoria (ARGB premultiplicado) y dibujo directo en píxeles
// ---------------------------------------------------------------------------

struct Img {
    int w = 0, h = 0;
    std::vector<uint32_t> px;
    bool empty() const { return w <= 0 || h <= 0 || px.empty(); }
};

// Un icono listo para dibujar a cualquier tamaño: mipmaps sin sombra y
// mipmaps con la sombra ya horneada (un solo dibujo por icono).
// Las imágenes llevan un borde transparente de 1 px (muestreo sin revisar
// bordes).
struct IconSet {
    std::vector<Img> mips;       // mips[0] = tamaño ampliado; cada uno la mitad
    std::vector<Img> shadowed;   // con sombra; más grandes por el margen
    double padFrac = 0;          // margen de la sombra / tamaño del icono
};

struct Surface {
    uint32_t* bits = nullptr;
    int w = 0, h = 0;  // h = filas; el ancho de fila es w
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
    *dst = ((uint32_t)oa << 24) | ((uint32_t)std::min(orr, oa) << 16) |
           ((uint32_t)std::min(og, oa) << 8) | (uint32_t)std::min(ob, oa);
}

// Escala por promedio de área (para reducir) o bilineal (para ampliar).
static Img Resample(const Img& s, int W, int H) {
    Img d;
    d.w = W;
    d.h = H;
    d.px.assign((size_t)W * H, 0);
    if (s.empty() || W <= 0 || H <= 0) return d;
    double sx = (double)s.w / W, sy = (double)s.h / H;
    if (sx >= 1.0 && sy >= 1.0) {
        for (int y = 0; y < H; y++) {
            double y0 = y * sy, y1 = y0 + sy;
            for (int x = 0; x < W; x++) {
                double x0 = x * sx, x1 = x0 + sx;
                double acc[4] = {0, 0, 0, 0}, area = 0;
                for (int yy = (int)y0; yy < (int)std::ceil(y1) && yy < s.h; yy++) {
                    double wy = std::min(y1, yy + 1.0) - std::max(y0, (double)yy);
                    if (wy <= 0) continue;
                    for (int xx = (int)x0; xx < (int)std::ceil(x1) && xx < s.w; xx++) {
                        double wx = std::min(x1, xx + 1.0) - std::max(x0, (double)xx);
                        if (wx <= 0) continue;
                        double wgt = wx * wy;
                        uint32_t p = s.px[(size_t)yy * s.w + xx];
                        acc[0] += (p >> 24) * wgt;
                        acc[1] += ((p >> 16) & 0xFF) * wgt;
                        acc[2] += ((p >> 8) & 0xFF) * wgt;
                        acc[3] += (p & 0xFF) * wgt;
                        area += wgt;
                    }
                }
                if (area <= 0) continue;
                uint32_t a = (uint32_t)std::lround(acc[0] / area);
                uint32_t r = std::min(a, (uint32_t)std::lround(acc[1] / area));
                uint32_t g = std::min(a, (uint32_t)std::lround(acc[2] / area));
                uint32_t b = std::min(a, (uint32_t)std::lround(acc[3] / area));
                d.px[(size_t)y * W + x] = (a << 24) | (r << 16) | (g << 8) | b;
            }
        }
        return d;
    }
    for (int y = 0; y < H; y++) {
        double fy = std::clamp((y + 0.5) * sy - 0.5, 0.0, s.h - 1.0);
        int y0 = (int)fy, y1 = std::min(y0 + 1, s.h - 1);
        double ty = fy - y0;
        for (int x = 0; x < W; x++) {
            double fx = std::clamp((x + 0.5) * sx - 0.5, 0.0, s.w - 1.0);
            int x0 = (int)fx, x1 = std::min(x0 + 1, s.w - 1);
            double tx = fx - x0;
            uint32_t q[4] = {s.px[(size_t)y0 * s.w + x0], s.px[(size_t)y0 * s.w + x1],
                             s.px[(size_t)y1 * s.w + x0], s.px[(size_t)y1 * s.w + x1]};
            uint32_t out = 0;
            for (int sh = 0; sh <= 24; sh += 8) {
                double a = ((q[0] >> sh) & 0xFF) * (1 - tx) + ((q[1] >> sh) & 0xFF) * tx;
                double b = ((q[2] >> sh) & 0xFF) * (1 - tx) + ((q[3] >> sh) & 0xFF) * tx;
                out |= (uint32_t)std::clamp((int)std::lround(a * (1 - ty) + b * ty), 0, 255) << sh;
            }
            d.px[(size_t)y * W + x] = out;
        }
    }
    return d;
}

// Encaja la imagen en un cuadrado de px × px, centrada.
static Img FitSquare(const Img& s, int px) {
    Img out;
    out.w = out.h = px;
    out.px.assign((size_t)px * px, 0);
    if (s.empty()) return out;
    double k = std::min((double)px / s.w, (double)px / s.h);
    int w = std::max(1, (int)std::lround(s.w * k)), h = std::max(1, (int)std::lround(s.h * k));
    Img r = Resample(s, w, h);
    int ox = (px - w) / 2, oy = (px - h) / 2;
    for (int y = 0; y < h; y++) {
        memcpy(&out.px[(size_t)(oy + y) * px + ox], &r.px[(size_t)y * w], (size_t)w * 4);
    }
    return out;
}

static void BoxBlurAlpha(std::vector<float>& a, int w, int h, int r) {
    if (r <= 0) return;
    std::vector<float> tmp(a.size());
    for (int pass = 0; pass < 3; pass++) {
        for (int y = 0; y < h; y++) {
            float sum = 0;
            for (int k = -r; k <= r; k++) sum += a[(size_t)y * w + std::clamp(k, 0, w - 1)];
            for (int x = 0; x < w; x++) {
                tmp[(size_t)y * w + x] = sum / (2 * r + 1);
                sum += a[(size_t)y * w + std::clamp(x + r + 1, 0, w - 1)] -
                       a[(size_t)y * w + std::clamp(x - r, 0, w - 1)];
            }
        }
        for (int x = 0; x < w; x++) {
            float sum = 0;
            for (int k = -r; k <= r; k++) sum += tmp[(size_t)std::clamp(k, 0, h - 1) * w + x];
            for (int y = 0; y < h; y++) {
                a[(size_t)y * w + x] = sum / (2 * r + 1);
                sum += tmp[(size_t)std::clamp(y + r + 1, 0, h - 1) * w + x] -
                       tmp[(size_t)std::clamp(y - r, 0, h - 1) * w + x];
            }
        }
    }
}

static Img Half(const Img& s) {
    return Resample(s, std::max(1, s.w / 2), std::max(1, s.h / 2));
}

// Copia con un borde transparente de 1 px.
static Img Bordered(const Img& s) {
    Img o;
    o.w = s.w + 2;
    o.h = s.h + 2;
    o.px.assign((size_t)o.w * o.h, 0);
    for (int y = 0; y < s.h; y++) memcpy(&o.px[(size_t)(y + 1) * o.w + 1], &s.px[(size_t)y * s.w], (size_t)s.w * 4);
    return o;
}

static std::vector<Img> MipChain(Img top) {
    std::vector<Img> raw;
    raw.push_back(std::move(top));
    while (raw.back().w > 16) raw.push_back(Half(raw.back()));
    std::vector<Img> out;
    for (auto& m : raw) out.push_back(Bordered(m));
    return out;
}

// Arma los mipmaps y la versión con sombra (silueta desenfocada debajo).
// Trabajo pesado: se hace en el hilo de iconos.
static std::shared_ptr<IconSet> MakeIconSet(const Img& src, int px) {
    if (src.empty() || px <= 0) return nullptr;
    auto set = std::make_shared<IconSet>();
    Img icon = FitSquare(src, px);
    int blur = std::max(1, px / 26), off = std::max(1, (int)std::lround(px * 0.045));
    int pad = blur * 3 + off + 1;
    int W = px + 2 * pad;
    std::vector<float> a((size_t)W * W, 0.0f);
    for (int y = 0; y < px; y++)
        for (int x = 0; x < px; x++) a[(size_t)(y + pad + off) * W + (x + pad)] = (float)(icon.px[(size_t)y * px + x] >> 24);
    BoxBlurAlpha(a, W, W, blur);
    Img sh;
    sh.w = sh.h = W;
    sh.px.resize(a.size());
    const float k = 0.36f;
    for (size_t i = 0; i < a.size(); i++) sh.px[i] = (uint32_t)std::clamp((int)std::lround(a[i] * k), 0, 255) << 24;
    // Icono encima de su sombra (premultiplicado).
    for (int y = 0; y < px; y++) {
        for (int x = 0; x < px; x++) {
            uint32_t s = icon.px[(size_t)y * px + x];
            uint32_t& d = sh.px[(size_t)(y + pad) * W + (x + pad)];
            uint32_t sa = s >> 24, inv = 255 - sa;
            uint32_t da = d >> 24;
            uint32_t oa = sa + (da * inv + 127) / 255;
            uint32_t orr = ((s >> 16) & 0xFF), og = ((s >> 8) & 0xFF), ob = (s & 0xFF);  // sombra negra
            d = (std::min(oa, 255u) << 24) | (orr << 16) | (og << 8) | ob;
        }
    }
    set->padFrac = (double)pad / px;
    set->mips = MipChain(std::move(icon));
    set->shadowed = MipChain(std::move(sh));
    return set;
}

// Interpolación de dos píxeles ARGB (w = 0..256), dos canales a la vez.
static inline uint32_t Lerp32(uint32_t a, uint32_t b, uint32_t w) {
    uint32_t iw = 256 - w;
    uint32_t rb = (((a & 0x00FF00FFu) * iw + (b & 0x00FF00FFu) * w) >> 8) & 0x00FF00FFu;
    uint32_t ag = (((a >> 8) & 0x00FF00FFu) * iw + ((b >> 8) & 0x00FF00FFu) * w) & 0xFF00FF00u;
    return rb | ag;
}

// Multiplica los cuatro canales por m (0..256).
static inline uint32_t Scale32(uint32_t p, uint32_t m) {
    uint32_t rb = (((p & 0x00FF00FFu) * m) >> 8) & 0x00FF00FFu;
    uint32_t ag = (((p >> 8) & 0x00FF00FFu) * m) & 0xFF00FF00u;
    return rb | ag;
}

// src (premultiplicado) encima de dst.
static inline uint32_t Over32(uint32_t s, uint32_t d) {
    uint32_t inv = 255 - (s >> 24);
    if (inv == 255) return d;
    if (inv == 0) return s;
    return s + Scale32(d, inv);
}

// Dibuja un icono escalado a size × size en (x, y) con precisión de
// subpíxel: bilineal desde el mipmap adecuado, en enteros, sin revisar
// bordes por píxel (las imágenes traen borde transparente).
static void BlitScaled(Surface& dst, const RECT& clip, const std::vector<Img>& mips, double x, double y,
                       double size, double alpha, double bright) {
    if (mips.empty() || size < 0.5 || alpha <= 0.003) return;
    const Img* src = &mips[0];
    for (const Img& m : mips) {
        if (m.w - 2 >= size * 0.98) src = &m;
    }
    const int iw = src->w - 2, ih = src->h - 2;
    if (iw <= 0 || ih <= 0) return;
    const double scale = iw / size;
    int x0 = std::max<int>({(int)clip.left, (int)std::floor(x), 0});
    int x1 = std::min<int>({(int)clip.right, (int)std::ceil(x + size), dst.w});
    int y0 = std::max<int>({(int)clip.top, (int)std::floor(y), 0});
    int y1 = std::min<int>({(int)clip.bottom, (int)std::ceil(y + size), dst.h});
    if (x0 >= x1 || y0 >= y1) return;
    // Columnas: índice del texel (ya con el borde) y peso, una sola vez.
    static thread_local std::vector<int> colIdx;
    static thread_local std::vector<uint32_t> colW;
    colIdx.resize(x1 - x0);
    colW.resize(x1 - x0);
    for (int ix = x0; ix < x1; ix++) {
        double u = (ix + 0.5 - x) * scale - 0.5;
        int t = (int)std::floor(u);
        uint32_t w = (uint32_t)std::lround((u - t) * 256);
        if (t < -1) { t = -1; w = 0; }
        if (t > iw - 1) { t = iw - 1; w = 256; }
        if (w >= 256) { t++; w = 0; if (t > iw) { t = iw; } }
        colIdx[ix - x0] = t + 1;
        colW[ix - x0] = w;
    }
    const uint32_t mA = (uint32_t)std::clamp((int)std::lround(alpha * 256), 0, 256);
    const uint32_t mC = (uint32_t)std::clamp((int)std::lround(alpha * bright * 256), 0, 256);
    const bool plain = mA == 256 && mC == 256;
    const int stride = src->w;
    const uint32_t* sp = src->px.data();
    for (int iy = y0; iy < y1; iy++) {
        double v = (iy + 0.5 - y) * scale - 0.5;
        int ty = (int)std::floor(v);
        uint32_t wy = (uint32_t)std::lround((v - ty) * 256);
        if (ty < -1) { ty = -1; wy = 0; }
        if (ty > ih - 1) { ty = ih - 1; wy = 256; }
        if (wy >= 256) { ty++; wy = 0; if (ty > ih) ty = ih; }
        const uint32_t* r0 = sp + (size_t)(ty + 1) * stride;
        const uint32_t* r1 = (ty + 2 < src->h) ? r0 + stride : r0;
        uint32_t* row = dst.bits + (size_t)iy * dst.w;
        for (int ix = x0; ix < x1; ix++) {
            int c = colIdx[ix - x0];
            int c1 = c + 1 < stride ? c + 1 : c;
            uint32_t wx = colW[ix - x0];
            uint32_t p = Lerp32(Lerp32(r0[c], r0[c1], wx), Lerp32(r1[c], r1[c1], wx), wy);
            if (!(p >> 24)) continue;
            if (!plain) {
                uint32_t a = (((p >> 24) * mA) >> 8);
                uint32_t col = Scale32(p, mC) & 0x00FFFFFFu;
                p = (a << 24) | col;
            }
            row[ix] = Over32(p, row[ix]);
        }
    }
}

// Dibuja un icono (con o sin su sombra horneada) en el cuadro del icono.
static void BlitIcon(Surface& dst, const RECT& clip, const IconSet& set, double x, double y, double size,
                     double alpha, double bright, bool shadow) {
    if (shadow && !set.shadowed.empty()) {
        double pad = set.padFrac * size;
        BlitScaled(dst, clip, set.shadowed, x - pad, y - pad, size + 2 * pad, alpha, bright);
    } else {
        BlitScaled(dst, clip, set.mips, x, y, size, alpha, bright);
    }
}

// Copia una imagen (sin escalar) con opacidad, en una posición entera.
static void BlitImage(Surface& dst, const RECT& clip, const Img& img, int x, int y, double alpha) {
    if (img.empty() || alpha <= 0.003) return;
    int x0 = std::max<int>({(int)clip.left, x, 0}), x1 = std::min<int>({(int)clip.right, x + img.w, dst.w});
    int y0 = std::max<int>({(int)clip.top, y, 0}), y1 = std::min<int>({(int)clip.bottom, y + img.h, dst.h});
    float fa = (float)alpha;
    for (int iy = y0; iy < y1; iy++) {
        const uint32_t* s = &img.px[(size_t)(iy - y) * img.w];
        uint32_t* row = dst.bits + (size_t)iy * dst.w;
        for (int ix = x0; ix < x1; ix++) {
            uint32_t p = s[ix - x];
            if (!p) continue;
            BlendOver(row + ix, (p >> 24) * fa / 255.0f, ((p >> 16) & 0xFF) * fa,
                      ((p >> 8) & 0xFF) * fa, (p & 0xFF) * fa);
        }
    }
}

// Círculo con antialias.
static void FillCircle(Surface& dst, const RECT& clip, double cx, double cy, double r, double a,
                       int R, int G, int B) {
    int x0 = std::max<int>(clip.left, (int)std::floor(cx - r - 1)), x1 = std::min<int>(clip.right, (int)std::ceil(cx + r + 1));
    int y0 = std::max<int>(clip.top, (int)std::floor(cy - r - 1)), y1 = std::min<int>(clip.bottom, (int)std::ceil(cy + r + 1));
    x0 = std::max(x0, 0);
    y0 = std::max(y0, 0);
    x1 = std::min(x1, dst.w);
    y1 = std::min(y1, dst.h);
    for (int y = y0; y < y1; y++) {
        for (int x = x0; x < x1; x++) {
            double d = std::hypot(x + 0.5 - cx, y + 0.5 - cy) - r;
            float cov = (float)std::clamp(0.5 - d, 0.0, 1.0) * (float)a;
            if (cov > 0.002f) BlendOver(dst.bits + (size_t)y * dst.w + x, cov, R * cov, G * cov, B * cov);
        }
    }
}

// Distancia con signo a un rectángulo redondeado (negativa adentro).
static inline float RoundRectSdf(float px, float py, float cx, float cy, float hw, float hh, float r) {
    float qx = std::fabs(px - cx) - (hw - r);
    float qy = std::fabs(py - cy) - (hh - r);
    float ox = std::max(qx, 0.0f), oy = std::max(qy, 0.0f);
    return std::sqrt(ox * ox + oy * oy) + std::min(std::max(qx, qy), 0.0f) - r;
}

// Rectángulo redondeado con antialias, sombra opcional y filo.
struct RoundStyle {
    float fillA = 1, fr = 0, fg = 0, fb = 0;
    float shadowA = 0, shadowR = 0, shadowOff = 0;
    float rimA = 0;      // filo claro de 1 px por dentro
    float outerA = 0;    // línea oscura por fuera
};

static void FillRoundRect(Surface& dst, const RECT& clip, float x0, float y0, float w, float h, float r,
                          const RoundStyle& st) {
    if (w <= 1 || h <= 1) return;
    float cx = x0 + w / 2, cy = y0 + h / 2, hw = w / 2, hh = h / 2;
    r = std::min(r, std::min(hw, hh));
    float m = st.shadowR + st.shadowOff + 2;
    int ix0 = std::max<int>({(int)clip.left, (int)std::floor(x0 - m), 0});
    int ix1 = std::min<int>({(int)clip.right, (int)std::ceil(x0 + w + m), dst.w});
    int iy0 = std::max<int>({(int)clip.top, (int)std::floor(y0 - m), 0});
    int iy1 = std::min<int>({(int)clip.bottom, (int)std::ceil(y0 + h + m), dst.h});
    for (int y = iy0; y < iy1; y++) {
        float py = y + 0.5f;
        uint32_t* row = dst.bits + (size_t)y * dst.w;
        for (int x = ix0; x < ix1; x++) {
            float px = x + 0.5f;
            float d = RoundRectSdf(px, py, cx, cy, hw, hh, r);
            if (st.shadowA > 0 && d > -1.0f) {
                float ds = RoundRectSdf(px, py, cx, cy + st.shadowOff, hw, hh, r);
                float t = std::clamp(1.0f - ds / std::max(1.0f, st.shadowR), 0.0f, 1.0f);
                float sa = st.shadowA * t * t * std::clamp(d + 1.0f, 0.0f, 1.0f);
                if (sa > 0.002f) BlendOver(row + x, sa, 0, 0, 0);
            }
            float cov = std::clamp(0.5f - d, 0.0f, 1.0f);
            if (cov > 0) {
                float a = st.fillA * cov;
                BlendOver(row + x, a, st.fr * a, st.fg * a, st.fb * a);
                float rc = st.rimA * std::clamp(1.0f - std::fabs(d + 1.0f), 0.0f, 1.0f) * cov;
                if (rc > 0.002f) BlendOver(row + x, rc, 255 * rc, 255 * rc, 255 * rc);
            }
            float oc = st.outerA * std::clamp(1.0f - std::fabs(d - 0.5f), 0.0f, 1.0f) * (1.0f - cov);
            if (oc > 0.002f) BlendOver(row + x, oc, 0, 0, 0);
        }
    }
}

// ---------------------------------------------------------------------------
// Elementos del Dock
// ---------------------------------------------------------------------------

enum class Kind { Start, App, Sep, Trash, End };

struct DockItem {
    uint64_t uid = 0;
    Kind kind = Kind::App;
    bool pinned = false;
    // 'L' acceso directo, 'E' ejecutable, 'A' app empaquetada (AUMID), 'P' archivo/carpeta
    wchar_t type = 'E';
    std::wstring value;
    std::wstring aumid;  // para reconocer sus ventanas
    std::wstring exe;    // ruta del ejecutable en minúsculas
    std::wstring name;
    bool nameFinal = false;
    std::vector<std::wstring> iconSrcs;  // fuentes del icono, en orden
    std::wstring iconKey;
    std::vector<HWND> windows;  // la más reciente primero
    std::vector<DWORD> pids;    // procesos de sus ventanas

    Spring presence;  // 0 → 1 al aparecer, 1 → 0 al irse
    Spring slot;      // 0 mientras se arrastra (deja su lugar)
    Spring gap;       // hueco antes de este icono (arrastres)
    Spring dot;       // punto de app abierta
    Spring hilite;    // resaltado al soltar algo encima
    Spring flyX, flyY;

    bool removing = false;
    double bounceStart = -1;
    int bounceCycles = 0;
    double bounceAmp = 0.4;
    bool launching = false;
    double launchUntil = 0;
};

static std::vector<std::unique_ptr<DockItem>> g_items;
static uint64_t g_nextUid = 1;

static std::unique_ptr<DockItem> NewItem(Kind kind) {
    auto it = std::make_unique<DockItem>();
    it->uid = g_nextUid++;
    it->kind = kind;
    it->presence.Snap(1);
    it->slot.Snap(1);
    return it;
}

static int IndexOf(const DockItem* item) {
    for (size_t i = 0; i < g_items.size(); i++) {
        if (g_items[i].get() == item) return (int)i;
    }
    return -1;
}

static DockItem* FindByUid(uint64_t uid) {
    for (auto& it : g_items) {
        if (it->uid == uid) return it.get();
    }
    return nullptr;
}

static DockItem* FindKind(Kind k) {
    for (auto& it : g_items) {
        if (it->kind == k) return it.get();
    }
    return nullptr;
}

static int PinnedEnd() {
    for (size_t i = 0; i < g_items.size(); i++) {
        auto* it = g_items[i].get();
        if (it->kind == Kind::Start) continue;
        if (it->kind != Kind::App || !it->pinned) return (int)i;
    }
    return (int)g_items.size();
}

static int AppsEnd() {
    for (size_t i = 0; i < g_items.size(); i++) {
        Kind k = g_items[i]->kind;
        if (k == Kind::Sep || k == Kind::Trash || k == Kind::End) return (int)i;
    }
    return (int)g_items.size();
}

// Índice justo después de la última app anclada (sin contar exclude).
static int AfterLastPinned(const DockItem* exclude) {
    int pos = -1;
    for (size_t i = 0; i < g_items.size(); i++) {
        auto* it = g_items[i].get();
        if (it == exclude) continue;
        if (it->kind == Kind::Start) pos = std::max(pos, (int)i);
        if (it->kind == Kind::App && it->pinned) pos = (int)i;
    }
    return pos + 1;
}

// ---------------------------------------------------------------------------
// Identidad de las apps (AUMID, ejecutable)
// ---------------------------------------------------------------------------

static const PROPERTYKEY kPKEY_AppUserModel_ID = {
    {0x9F4C2855, 0x9F79, 0x4B39, {0xA8, 0xD0, 0xE1, 0xD4, 0x2D, 0xE1, 0xD5, 0xF3}}, 5};

static std::wstring StoreAumid(IPropertyStore* store) {
    std::wstring r;
    PROPVARIANT pv;
    PropVariantInit(&pv);
    if (SUCCEEDED(store->GetValue(kPKEY_AppUserModel_ID, &pv)) && pv.vt == VT_LPWSTR && pv.pwszVal) {
        r = pv.pwszVal;
    }
    PropVariantClear(&pv);
    return r;
}

static std::wstring WindowAumid(HWND h) {
    std::wstring r;
    IPropertyStore* store = nullptr;
    if (SUCCEEDED(SHGetPropertyStoreForWindow(h, IID_PPV_ARGS(&store))) && store) {
        r = StoreAumid(store);
        store->Release();
    }
    return r;
}

using GetAumidFn = LONG(WINAPI*)(HANDLE, UINT32*, PWSTR);

static std::wstring PackagedAumid(HANDLE proc) {
    static GetAumidFn fn = (GetAumidFn)(void*)GetProcAddress(GetModuleHandleW(L"kernel32.dll"),
                                                             "GetApplicationUserModelId");
    if (!fn || !proc) return L"";
    wchar_t buf[512];
    UINT32 len = ARRAYSIZE(buf);
    if (fn(proc, &len, buf) != ERROR_SUCCESS) return L"";
    return buf;
}

struct ProcInfo {
    std::wstring exe;  // minúsculas
    std::wstring packaged;
    FILETIME created{};
};

static ProcInfo QueryProc(DWORD pid) {
    ProcInfo info;
    HANDLE proc = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pid);
    if (!proc) return info;
    wchar_t buf[MAX_PATH * 2];
    DWORD size = ARRAYSIZE(buf);
    if (QueryFullProcessImageNameW(proc, 0, buf, &size)) info.exe = Lower(buf);
    info.packaged = PackagedAumid(proc);
    FILETIME e, k, u;
    GetProcessTimes(proc, &info.created, &e, &k, &u);
    CloseHandle(proc);
    return info;
}

struct WinInfo {
    HWND h = nullptr;
    std::wstring aumid;
    std::wstring exe;
    bool packaged = false;
    DWORD pid = 0;  // proceso real de la app (no el de ApplicationFrameHost)
};

static HWND g_dragWnd = nullptr;

static bool IsDockWindow(HWND h) {
    if (!IsWindowVisible(h)) return false;
    if (h == g_wnd.load() || h == g_menuHost || h == g_dragWnd) return false;
    LONG_PTR ex = GetWindowLongPtrW(h, GWL_EXSTYLE);
    if (!(ex & WS_EX_APPWINDOW)) {
        if (GetWindow(h, GW_OWNER)) return false;
        if (ex & WS_EX_TOOLWINDOW) return false;
        if (ex & WS_EX_NOACTIVATE) return false;
    }
    if (IsCloaked(h)) return false;
    std::wstring cls = ClassOf(h);
    static const wchar_t* kSkip[] = {L"Progman", L"WorkerW", L"Shell_TrayWnd",
                                     L"Shell_SecondaryTrayWnd", L"Windows.UI.Core.CoreWindow",
                                     L"WindhawkTopNotchWidget"};
    for (auto* s : kSkip) {
        if (cls == s) return false;
    }
    return true;
}

static bool ItemMatches(const DockItem& it, const WinInfo& w, bool exact) {
    if (it.kind != Kind::App || it.removing) return false;
    if (exact) return !it.aumid.empty() && !w.aumid.empty() && EqI(it.aumid, w.aumid);
    if (!it.aumid.empty() && !w.aumid.empty()) return false;
    return !it.exe.empty() && it.exe == w.exe;
}

// ---------------------------------------------------------------------------
// Hilo de ventanas: averiguar quién está abierto sin frenar el dibujo
// ---------------------------------------------------------------------------

static std::mutex g_scanMx;
static std::vector<WinInfo> g_scanResult;
static bool g_scanFresh = false;
static HANDLE g_scanEvent = nullptr;
static HANDLE g_scanThread = nullptr;
static std::atomic<bool> g_scanStop{false};

struct ScanCtx {
    std::vector<WinInfo>* out;
    std::map<DWORD, ProcInfo>* procs;
    std::map<DWORD, bool>* seen;
};

static const ProcInfo& ProcFor(ScanCtx* ctx, DWORD pid) {
    (*ctx->seen)[pid] = true;
    auto it = ctx->procs->find(pid);
    if (it != ctx->procs->end()) {
        // ¿Sigue siendo el mismo proceso? (los PID se reciclan)
        HANDLE p = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pid);
        FILETIME c{}, e, k, u;
        bool same = p && GetProcessTimes(p, &c, &e, &k, &u) &&
                    CompareFileTime(&c, &it->second.created) == 0;
        if (p) CloseHandle(p);
        if (same) return it->second;
        ctx->procs->erase(it);
    }
    return ctx->procs->emplace(pid, QueryProc(pid)).first->second;
}

static BOOL CALLBACK FindCoreWindowProc(HWND h, LPARAM lParam) {
    if (ClassOf(h) == L"Windows.UI.Core.CoreWindow") {
        *(HWND*)lParam = h;
        return FALSE;
    }
    return TRUE;
}

static BOOL CALLBACK ScanWindowsProc(HWND h, LPARAM lParam) {
    auto* ctx = (ScanCtx*)lParam;
    if (!IsDockWindow(h)) return TRUE;
    WinInfo w;
    w.h = h;
    DWORD pid = 0;
    GetWindowThreadProcessId(h, &pid);
    const ProcInfo& p = ProcFor(ctx, pid);
    w.exe = p.exe;
    w.pid = pid;
    if (!p.packaged.empty()) {
        w.aumid = p.packaged;
        w.packaged = true;
    } else {
        w.aumid = WindowAumid(h);
    }
    // Apps de la Tienda (Configuración, Calculadora…): la ventana es de
    // ApplicationFrameHost.exe; la app real es la CoreWindow de adentro.
    if (EndsWithI(w.exe, L"\\applicationframehost.exe")) {
        HWND core = nullptr;
        EnumChildWindows(h, FindCoreWindowProc, (LPARAM)&core);
        if (core) {
            DWORD cpid = 0;
            GetWindowThreadProcessId(core, &cpid);
            const ProcInfo& cp = ProcFor(ctx, cpid);
            if (!cp.packaged.empty()) w.aumid = cp.packaged;
            if (!cp.exe.empty()) w.exe = cp.exe;
            w.pid = cpid;
        } else {
            w.pid = 0;  // nunca cerrar ApplicationFrameHost
        }
        if (w.aumid.empty()) return TRUE;  // marco vacío (app suspendida)
        w.packaged = true;                 // se abre por su AUMID, no por el .exe
    }
    ctx->out->push_back(std::move(w));
    return TRUE;
}

static DWORD WINAPI ScanThread(LPVOID) {
    CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
    std::map<DWORD, ProcInfo> procs;
    while (!g_scanStop) {
        DWORD r = MsgWaitForMultipleObjects(1, &g_scanEvent, FALSE, 1500, QS_ALLINPUT);
        if (g_scanStop) break;
        if (r == WAIT_OBJECT_0 + 1) {
            MSG m;
            while (PeekMessageW(&m, nullptr, 0, 0, PM_REMOVE)) DispatchMessageW(&m);
            continue;
        }
        if (r == WAIT_OBJECT_0) Sleep(25);  // juntar varios avisos seguidos
        std::vector<WinInfo> wins;
        std::map<DWORD, bool> seen;
        ScanCtx ctx{&wins, &procs, &seen};
        EnumWindows(ScanWindowsProc, (LPARAM)&ctx);
        for (auto it = procs.begin(); it != procs.end();) {
            it = seen.count(it->first) ? std::next(it) : procs.erase(it);
        }
        {
            std::lock_guard<std::mutex> lock(g_scanMx);
            g_scanResult = std::move(wins);
            g_scanFresh = true;
        }
        HWND h = g_wnd.load();
        if (h) PostMessageW(h, WM_APP_WINDOWS, 0, 0);
    }
    CoUninitialize();
    return 0;
}

static void RequestScan() {
    if (g_scanEvent) SetEvent(g_scanEvent);
}

// ---------------------------------------------------------------------------
// Anclados
// ---------------------------------------------------------------------------

struct LnkInfo {
    std::wstring target;
    std::wstring aumid;
};

static LnkInfo ReadLnk(const std::wstring& path) {
    LnkInfo r;
    IShellLinkW* sl = nullptr;
    if (FAILED(CoCreateInstance(CLSID_ShellLink, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&sl))) || !sl) {
        return r;
    }
    IPersistFile* pf = nullptr;
    if (SUCCEEDED(sl->QueryInterface(IID_PPV_ARGS(&pf))) && pf) {
        if (SUCCEEDED(pf->Load(path.c_str(), STGM_READ))) {
            wchar_t buf[MAX_PATH * 2] = {};
            if (SUCCEEDED(sl->GetPath(buf, ARRAYSIZE(buf), nullptr, 0)) && buf[0]) {
                r.target = ExpandEnv(buf);
            }
            IPropertyStore* ps = nullptr;
            if (SUCCEEDED(sl->QueryInterface(IID_PPV_ARGS(&ps))) && ps) {
                r.aumid = StoreAumid(ps);
                ps->Release();
            }
        }
        pf->Release();
    }
    sl->Release();
    return r;
}

static std::unique_ptr<DockItem> MakeApp(wchar_t type, const std::wstring& value,
                                         const std::wstring& aumid, const std::wstring& exe) {
    auto it = NewItem(Kind::App);
    it->type = type;
    it->value = value;
    it->aumid = aumid;
    it->exe = Lower(exe);
    switch (type) {
        case 'L':
            it->iconSrcs = {value};
            if (!it->exe.empty()) it->iconSrcs.push_back(it->exe);
            it->name = FileNameNoExt(value);
            it->nameFinal = true;
            break;
        case 'A':
            it->iconSrcs = {L"shell:AppsFolder\\" + value};
            if (!it->exe.empty()) it->iconSrcs.push_back(it->exe);
            it->name = value;
            break;
        default:
            // Ventanas con AUMID propio (Chrome, apps web…): el icono de su
            // acceso en Inicio es el correcto; si no hay, el del ejecutable.
            if (!aumid.empty()) it->iconSrcs.push_back(L"shell:AppsFolder\\" + aumid);
            it->iconSrcs.push_back(value);
            it->name = FileNameNoExt(value);
            break;
    }
    it->iconKey = Lower(it->iconSrcs.front());
    return it;
}

static std::unique_ptr<DockItem> MakePinFromPath(const std::wstring& path) {
    if (EndsWithI(path, L".lnk")) {
        LnkInfo li = ReadLnk(path);
        std::wstring exe = EndsWithI(li.target, L".exe") ? li.target : L"";
        return MakeApp('L', path, li.aumid, exe);
    }
    if (EndsWithI(path, L".exe")) return MakeApp('E', path, L"", path);
    return MakeApp('P', path, L"", L"");
}

static void SavePins() {
    std::wstring all;
    for (auto& it : g_items) {
        if (it->kind != Kind::App || !it->pinned) continue;
        all += it->type;
        all += L'\t';
        all += it->value;
        all += L'\t';
        all += it->aumid;
        all += L'\t';
        all += it->exe;
        all += L'\n';
    }
    Wh_SetStringValue(L"pins", all.c_str());
    Wh_SetIntValue(L"pinsInit", 1);
}

// Anclados de la barra de Windows, en su orden, incluidas las apps de la
// Tienda (que no tienen acceso directo). Están como listas de elementos del
// Explorador (IDList) dentro de un valor binario; cada candidata se valida
// pidiéndole su nombre al propio Explorador.
static std::vector<std::wstring> TaskbandPins() {
    std::vector<std::wstring> out;
    DWORD size = 0;
    const wchar_t* key = L"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Taskband";
    if (RegGetValueW(HKEY_CURRENT_USER, key, L"Favorites", RRF_RT_REG_BINARY, nullptr, nullptr,
                     &size) != ERROR_SUCCESS ||
        size < 24 || size > 1024 * 1024) {
        return out;
    }
    std::vector<BYTE> d(size);
    if (RegGetValueW(HKEY_CURRENT_USER, key, L"Favorites", RRF_RT_REG_BINARY, nullptr, d.data(),
                     &size) != ERROR_SUCCESS) {
        return out;
    }
    size_t n = size;
    auto rd16 = [&](size_t i) { return (WORD)(d[i] | (d[i + 1] << 8)); };
    for (size_t i = 0; i + 22 < n;) {
        // Una IDList desde el escritorio empieza con un elemento raíz: 20
        // bytes, tipo 0x1F (carpeta con GUID).
        if (rd16(i) == 0x14 && d[i + 2] == 0x1F) {
            size_t j = i;
            int count = 0;
            bool ok = false;
            while (j + 2 <= n && count <= 32) {
                WORD cb = rd16(j);
                if (cb == 0) {
                    ok = count > 0;
                    break;
                }
                if (cb < 3 || j + cb > n) break;
                j += cb;
                count++;
            }
            if (ok) {
                size_t len = j + 2 - i;
                auto* pidl = (PIDLIST_ABSOLUTE)CoTaskMemAlloc(len);
                if (pidl) {
                    memcpy(pidl, &d[i], len);
                    PWSTR name = nullptr;
                    if (SUCCEEDED(SHGetNameFromIDList(pidl, SIGDN_DESKTOPABSOLUTEPARSING, &name)) && name) {
                        out.push_back(name);
                        CoTaskMemFree(name);
                    }
                    CoTaskMemFree(pidl);
                }
                i = j + 2;
                continue;
            }
        }
        i++;
    }
    return out;
}

static std::unique_ptr<DockItem> PinFromParsingName(const std::wstring& name) {
    if (StartsWithI(name, kAppsFolderPrefix)) {
        std::wstring aumid = name.substr(wcslen(kAppsFolderPrefix));
        if (aumid.empty()) return nullptr;
        // Apps de escritorio que aparecen por su ruta dentro de Inicio.
        if (aumid.find(L'\\') != std::wstring::npos && FileExists(ExpandEnv(aumid))) {
            return MakePinFromPath(ExpandEnv(aumid));
        }
        return MakeApp('A', aumid, aumid, L"");
    }
    if ((EndsWithI(name, L".lnk") || EndsWithI(name, L".exe")) && FileExists(name)) {
        return MakePinFromPath(name);
    }
    return nullptr;
}

static bool SamePin(const DockItem& a, const DockItem& b) {
    if (a.type == b.type && EqI(a.value, b.value)) return true;
    if (!a.aumid.empty() && !b.aumid.empty() && EqI(a.aumid, b.aumid)) return true;
    return !a.exe.empty() && a.exe == b.exe;
}

static std::vector<std::unique_ptr<DockItem>> SeedPins() {
    std::vector<std::unique_ptr<DockItem>> out;
    auto add = [&](std::unique_ptr<DockItem> it) {
        if (!it) return;
        for (auto& o : out) {
            if (SamePin(*o, *it)) return;
        }
        out.push_back(std::move(it));
    };
    for (auto& name : TaskbandPins()) add(PinFromParsingName(name));
    if (out.empty()) {
        std::wstring dir = ExpandEnv(
            L"%APPDATA%\\Microsoft\\Internet Explorer\\Quick Launch\\User Pinned\\TaskBar");
        std::vector<std::unique_ptr<DockItem>> found;
        WIN32_FIND_DATAW fd;
        HANDLE f = FindFirstFileW((dir + L"\\*.lnk").c_str(), &fd);
        if (f != INVALID_HANDLE_VALUE) {
            do {
                if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                    found.push_back(MakePinFromPath(dir + L"\\" + fd.cFileName));
                }
            } while (FindNextFileW(f, &fd));
            FindClose(f);
        }
        auto isExplorer = [](const std::unique_ptr<DockItem>& it) {
            return EndsWithI(it->exe, L"\\explorer.exe");
        };
        std::sort(found.begin(), found.end(), [&](const auto& a, const auto& b) {
            if (isExplorer(a) != isExplorer(b)) return isExplorer(a);
            return _wcsicmp(a->name.c_str(), b->name.c_str()) < 0;
        });
        for (auto& it : found) add(std::move(it));
    }
    if (out.empty()) {
        std::wstring ex = ExpandEnv(L"%WINDIR%\\explorer.exe");
        add(MakeApp('E', ex, L"", ex));
    }
    return out;
}

static std::vector<std::unique_ptr<DockItem>> LoadPins() {
    std::vector<std::unique_ptr<DockItem>> out;
    if (!Wh_GetIntValue(L"pinsInit", 0)) {
        Wh_SetIntValue(L"pinsV2", 1);
        return SeedPins();
    }
    std::vector<wchar_t> buf(65536);
    Wh_GetStringValue(L"pins", buf.data(), buf.size());
    buf.back() = L'\0';
    std::wstring all(buf.data());
    size_t pos = 0;
    while (pos < all.size()) {
        size_t end = all.find(L'\n', pos);
        if (end == std::wstring::npos) end = all.size();
        std::wstring line = all.substr(pos, end - pos);
        pos = end + 1;
        std::vector<std::wstring> f;
        size_t p = 0;
        while (true) {
            size_t t = line.find(L'\t', p);
            f.push_back(line.substr(p, t == std::wstring::npos ? std::wstring::npos : t - p));
            if (t == std::wstring::npos) break;
            p = t + 1;
        }
        if (f.size() < 2 || f[0].size() != 1 || f[1].empty()) continue;
        wchar_t type = f[0][0];
        if (type != 'L' && type != 'E' && type != 'A' && type != 'P') continue;
        std::wstring exe = f.size() > 3 ? f[3] : L"";
        // Versión 1 guardaba apps de la Tienda como ApplicationFrameHost.exe.
        if (type == 'E' && EndsWithI(exe, L"\\applicationframehost.exe")) {
            if (f.size() > 2 && !f[2].empty()) {
                out.push_back(MakeApp('A', f[2], f[2], L""));
            }
            continue;
        }
        out.push_back(MakeApp(type, f[1], f.size() > 2 ? f[2] : L"", exe));
    }
    // Una sola vez: sumar las apps de la Tienda ancladas en Windows que la
    // versión anterior no podía leer.
    if (!Wh_GetIntValue(L"pinsV2", 0)) {
        for (auto& name : TaskbandPins()) {
            auto it = PinFromParsingName(name);
            if (!it || it->type != 'A') continue;
            bool dup = false;
            for (auto& o : out) dup = dup || SamePin(*o, *it);
            if (!dup) out.push_back(std::move(it));
        }
        Wh_SetIntValue(L"pinsV2", 1);
    }
    return out;
}

// ---------------------------------------------------------------------------
// Hilo de iconos: carga, escala, mipmaps y sombras sin frenar el dibujo
// ---------------------------------------------------------------------------

enum class JobKind { App, Start, Trash, Glass };

struct Job {
    JobKind kind = JobKind::App;
    std::wstring key;
    std::vector<std::wstring> srcs;
    int px = 64;
    bool wantName = false;
    bool trashFull = false;
    bool vectorTrash = false;
    std::wstring startIcon;
    // cristal
    int winW = 0, winH = 0;
    RECT mon{};
    bool light = false;
    bool force = false;
};

struct JobResult {
    JobKind kind = JobKind::App;
    std::wstring key;
    std::shared_ptr<IconSet> set;
    std::wstring name;
    std::shared_ptr<std::vector<uint32_t>> glass;
    std::wstring glassKey;
    int winW = 0, winH = 0;
};

struct IconEntry {
    int state = 0;  // 0 sin pedir, 1 pedido, 2 listo, 3 falló
    int px = 0;
    std::shared_ptr<IconSet> set;
};

static std::map<std::wstring, IconEntry> g_icons;
static std::mutex g_jobMx;
static std::deque<Job> g_jobs;
static std::deque<JobResult> g_results;
static HANDLE g_jobEvent = nullptr;
static HANDLE g_jobThread = nullptr;
static std::atomic<bool> g_jobStop{false};

static std::wstring FileDescription(const std::wstring& path) {
    DWORD handle = 0;
    DWORD size = GetFileVersionInfoSizeW(path.c_str(), &handle);
    if (!size) return L"";
    std::vector<BYTE> data(size);
    if (!GetFileVersionInfoW(path.c_str(), 0, size, data.data())) return L"";
    struct LangCp {
        WORD lang, cp;
    }* tr = nullptr;
    UINT len = 0;
    if (!VerQueryValueW(data.data(), L"\\VarFileInfo\\Translation", (void**)&tr, &len) ||
        len < sizeof(LangCp)) {
        return L"";
    }
    wchar_t q[80];
    wsprintfW(q, L"\\StringFileInfo\\%04x%04x\\FileDescription", tr[0].lang, tr[0].cp);
    wchar_t* v = nullptr;
    UINT vl = 0;
    if (VerQueryValueW(data.data(), q, (void**)&v, &vl) && v && vl > 1) return v;
    return L"";
}

// Recorta el contenido si el icono es chico dentro de un cuadro grande.
static Img TrimIcon(std::vector<uint32_t>&& px, int w, int h) {
    Img r;
    int x0 = w, y0 = h, x1 = -1, y1 = -1;
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            if ((px[(size_t)y * w + x] >> 24) > 8) {
                x0 = std::min(x0, x);
                x1 = std::max(x1, x);
                y0 = std::min(y0, y);
                y1 = std::max(y1, y);
            }
        }
    }
    if (x1 < 0) return r;  // vacío
    int side = std::max(x1 - x0 + 1, y1 - y0 + 1);
    if (side < w * 6 / 10) {
        int cx = (x0 + x1) / 2, cy = (y0 + y1) / 2;
        side = std::min(side + 2, std::min(w, h));
        int sx = std::clamp(cx - side / 2, 0, w - side);
        int sy = std::clamp(cy - side / 2, 0, h - side);
        r.w = r.h = side;
        r.px.resize((size_t)side * side);
        for (int y = 0; y < side; y++) {
            memcpy(&r.px[(size_t)y * side], &px[(size_t)(sy + y) * w + sx], (size_t)side * 4);
        }
        return r;
    }
    r.w = w;
    r.h = h;
    r.px = std::move(px);
    return r;
}

static Img HBitmapToImg(HBITMAP hbm) {
    BITMAP bm{};
    if (!GetObjectW(hbm, sizeof(bm), &bm)) return {};
    int w = bm.bmWidth, h = std::abs(bm.bmHeight);
    if (w <= 0 || h <= 0 || w > 1024 || h > 1024) return {};
    std::vector<uint32_t> px((size_t)w * h);
    BITMAPINFO bi{};
    bi.bmiHeader.biSize = sizeof(bi.bmiHeader);
    bi.bmiHeader.biWidth = w;
    bi.bmiHeader.biHeight = -h;
    bi.bmiHeader.biPlanes = 1;
    bi.bmiHeader.biBitCount = 32;
    bi.bmiHeader.biCompression = BI_RGB;
    HDC dc = GetDC(nullptr);
    int lines = GetDIBits(dc, hbm, 0, h, px.data(), &bi, DIB_RGB_COLORS);
    ReleaseDC(nullptr, dc);
    if (lines != h) return {};
    bool hasAlpha = std::any_of(px.begin(), px.end(), [](uint32_t p) { return (p >> 24) != 0; });
    if (!hasAlpha) {
        bool blank = std::all_of(px.begin(), px.end(), [](uint32_t p) { return (p & 0xFFFFFF) == 0; });
        if (blank) return {};
        for (auto& p : px) p |= 0xFF000000u;
    }
    return TrimIcon(std::move(px), w, h);
}

static Img HIconToImg(HICON icon, int size) {
    if (!icon || size <= 0 || size > 1024) return {};
    Img result;
    BITMAPINFO bi{};
    bi.bmiHeader.biSize = sizeof(bi.bmiHeader);
    bi.bmiHeader.biWidth = size;
    bi.bmiHeader.biHeight = -size;
    bi.bmiHeader.biPlanes = 1;
    bi.bmiHeader.biBitCount = 32;
    bi.bmiHeader.biCompression = BI_RGB;
    void* bits = nullptr;
    void* maskBits = nullptr;
    HDC dc = CreateCompatibleDC(nullptr);
    HBITMAP dib = CreateDIBSection(dc, &bi, DIB_RGB_COLORS, &bits, nullptr, 0);
    HBITMAP mdib = CreateDIBSection(dc, &bi, DIB_RGB_COLORS, &maskBits, nullptr, 0);
    if (dc && dib && mdib && bits && maskBits) {
        size_t n = (size_t)size * size;
        HGDIOBJ old = SelectObject(dc, dib);
        memset(bits, 0, n * 4);
        DrawIconEx(dc, 0, 0, icon, size, size, 0, nullptr, DI_NORMAL);
        SelectObject(dc, mdib);
        memset(maskBits, 0, n * 4);
        DrawIconEx(dc, 0, 0, icon, size, size, 0, nullptr, DI_MASK);
        SelectObject(dc, old);
        GdiFlush();
        std::vector<uint32_t> px((const uint32_t*)bits, (const uint32_t*)bits + n);
        const uint32_t* mask = (const uint32_t*)maskBits;
        bool hasAlpha = std::any_of(px.begin(), px.end(), [](uint32_t p) { return (p >> 24) != 0; });
        if (!hasAlpha) {
            for (size_t i = 0; i < n; i++) px[i] = (mask[i] & 0xFFFFFF) ? 0 : (px[i] | 0xFF000000u);
        }
        result = TrimIcon(std::move(px), size, size);
    }
    if (dib) DeleteObject(dib);
    if (mdib) DeleteObject(mdib);
    if (dc) DeleteDC(dc);
    return result;
}

static const GUID kIID_IImageList = {
    0x46EB5926, 0x582E, 0x4017, {0x9F, 0xDF, 0xE8, 0x99, 0x8D, 0xAA, 0x09, 0x50}};

static Img SystemImageListIcon(const std::wstring& src, int want) {
    PIDLIST_ABSOLUTE pidl = nullptr;
    if (FAILED(SHParseDisplayName(src.c_str(), nullptr, &pidl, 0, nullptr)) || !pidl) return {};
    SHFILEINFOW sfi{};
    DWORD_PTR ok = SHGetFileInfoW((LPCWSTR)pidl, 0, &sfi, sizeof(sfi), SHGFI_PIDL | SHGFI_SYSICONINDEX);
    ILFree(pidl);
    if (!ok) return {};
    const int lists[] = {want > 48 ? SHIL_JUMBO : SHIL_EXTRALARGE, SHIL_EXTRALARGE, SHIL_LARGE};
    for (int list : lists) {
        IImageList* il = nullptr;
        if (FAILED(SHGetImageList(list, kIID_IImageList, (void**)&il)) || !il) continue;
        HICON icon = nullptr;
        int cx = 0, cy = 0;
        il->GetIconSize(&cx, &cy);
        Img r;
        if (SUCCEEDED(il->GetIcon(sfi.iIcon, ILD_TRANSPARENT, &icon)) && icon) {
            r = HIconToImg(icon, std::max(cx, cy));
            DestroyIcon(icon);
        }
        il->Release();
        if (!r.empty()) return r;
    }
    return {};
}

static Img ShellIcon(const std::wstring& src, int px) {
    Img r;
    IShellItem* item = nullptr;
    if (SUCCEEDED(SHCreateItemFromParsingName(src.c_str(), nullptr, IID_PPV_ARGS(&item))) && item) {
        IShellItemImageFactory* f = nullptr;
        if (SUCCEEDED(item->QueryInterface(IID_PPV_ARGS(&f))) && f) {
            const SIIGBF tries[] = {SIIGBF_ICONONLY, (SIIGBF)(SIIGBF_ICONONLY | SIIGBF_BIGGERSIZEOK),
                                    SIIGBF_RESIZETOFIT};
            for (SIIGBF fl : tries) {
                HBITMAP hbm = nullptr;
                if (SUCCEEDED(f->GetImage(SIZE{px, px}, fl, &hbm)) && hbm) {
                    r = HBitmapToImg(hbm);
                    DeleteObject(hbm);
                }
                if (!r.empty()) break;
            }
            f->Release();
        }
        item->Release();
    }
    if (r.empty()) r = SystemImageListIcon(src, px);
    return r;
}

static std::wstring ShellName(const std::wstring& src) {
    if (EndsWithI(src, L".exe")) {
        std::wstring d = FileDescription(src);
        if (!d.empty()) return d;
    }
    std::wstring r;
    IShellItem* item = nullptr;
    if (SUCCEEDED(SHCreateItemFromParsingName(src.c_str(), nullptr, IID_PPV_ARGS(&item))) && item) {
        LPWSTR dn = nullptr;
        if (SUCCEEDED(item->GetDisplayName(SIGDN_NORMALDISPLAY, &dn)) && dn) {
            r = dn;
            CoTaskMemFree(dn);
        }
        item->Release();
    }
    return r;
}

// Imagen desde archivo (png/jpg/ico/bmp) o "archivo,índice".
static Img ImageFromSpec(const std::wstring& specIn, int px) {
    std::wstring spec = ExpandEnv(specIn);
    if (spec.size() >= 2 && spec.front() == L'"' && spec.back() == L'"') spec = spec.substr(1, spec.size() - 2);
    int index = 0;
    size_t comma = spec.find_last_of(L',');
    std::wstring path = spec;
    if (comma != std::wstring::npos && comma + 1 < spec.size()) {
        bool num = true;
        for (size_t i = comma + 1; i < spec.size(); i++) num = num && (iswdigit(spec[i]) || spec[i] == L'-');
        if (num) {
            index = _wtoi(spec.c_str() + comma + 1);
            path = spec.substr(0, comma);
        }
    }
    if (!FileExists(path)) return {};
    bool image = EndsWithI(path, L".png") || EndsWithI(path, L".jpg") || EndsWithI(path, L".jpeg") ||
                 EndsWithI(path, L".bmp") || EndsWithI(path, L".gif");
    if (image) {
        std::unique_ptr<Gdiplus::Bitmap> b(Gdiplus::Bitmap::FromFile(path.c_str()));
        if (!b || b->GetLastStatus() != Gdiplus::Ok) return {};
        Img r;
        r.w = (int)b->GetWidth();
        r.h = (int)b->GetHeight();
        if (r.w <= 0 || r.h <= 0 || r.w > 2048 || r.h > 2048) return {};
        r.px.resize((size_t)r.w * r.h);
        Gdiplus::BitmapData bd{};
        Gdiplus::Rect rc(0, 0, r.w, r.h);
        if (b->LockBits(&rc, Gdiplus::ImageLockModeRead, PixelFormat32bppPARGB, &bd) != Gdiplus::Ok) return {};
        for (int y = 0; y < r.h; y++) memcpy(&r.px[(size_t)y * r.w], (BYTE*)bd.Scan0 + (size_t)y * bd.Stride, (size_t)r.w * 4);
        b->UnlockBits(&bd);
        return r;
    }
    HICON icon = nullptr;
    if (SUCCEEDED(SHDefExtractIconW(path.c_str(), index, 0, &icon, nullptr, MAKELONG(px, 16))) && icon) {
        Img r = HIconToImg(icon, px);
        DestroyIcon(icon);
        return r;
    }
    return {};
}

// --- Inicio: el logo de Windows tal como lo trae la fuente de iconos del
// sistema. No se conoce de antemano su código, así que se busca entre los
// glifos el que tiene la forma de cuatro paneles (2 × 2) y se recuerda.
static int ScoreLogoGlyph(const BYTE* bm, int w, int h, int pitch) {
    if (w < 24 || h < 24) return 0;
    double aspect = (double)w / h;
    if (aspect < 0.88 || aspect > 1.14) return 0;
    auto at = [&](int x, int y) { return bm[(size_t)y * pitch + x] / 64.0; };
    std::vector<double> col(w, 0), row(h, 0);
    for (int y = 0; y < h; y++)
        for (int x = 0; x < w; x++) {
            double v = at(x, y);
            col[x] += v / h;
            row[y] += v / w;
        }
    auto gapIn = [](const std::vector<double>& p, int& g0, int& g1) {
        int n = (int)p.size();
        g0 = -1;
        g1 = -1;
        for (int i = n * 35 / 100; i <= n * 65 / 100; i++) {
            if (p[i] < 0.15) {
                if (g0 < 0) g0 = i;
                g1 = i;
            }
        }
        return g0 >= 0 && (g1 - g0 + 1) <= n / 6;
    };
    int cx0, cx1, ry0, ry1;
    if (!gapIn(col, cx0, cx1) || !gapIn(row, ry0, ry1)) return 0;
    // Cada cuadrante, lleno.
    double minFill = 1;
    int qx[2][2] = {{0, cx0}, {cx1 + 1, w}}, qy[2][2] = {{0, ry0}, {ry1 + 1, h}};
    for (auto& xs : qx)
        for (auto& ys : qy) {
            double s = 0;
            int cnt = 0;
            for (int y = ys[0] + 1; y < ys[1] - 1; y++)
                for (int x = xs[0] + 1; x < xs[1] - 1; x++) {
                    s += at(x, y);
                    cnt++;
                }
            if (cnt <= 0) return 0;
            minFill = std::min(minFill, s / cnt);
        }
    return minFill >= 0.9 ? (int)(minFill * 1000) : 0;
}

static Img LogoFromSystemFont(int px) {
    const wchar_t* fonts[] = {L"Segoe Fluent Icons", L"Segoe MDL2 Assets"};
    int cachedFont = Wh_GetIntValue(L"startGlyphFont", -1);
    int cachedCp = Wh_GetIntValue(L"startGlyph", 0);
    HDC dc = CreateCompatibleDC(nullptr);
    if (!dc) return {};
    Img result;
    MAT2 mat = {{0, 1}, {0, 0}, {0, 0}, {0, 1}};
    for (int fi = 0; fi < 2 && result.empty(); fi++) {
        LOGFONTW lf{};
        lf.lfHeight = -64;
        lf.lfCharSet = DEFAULT_CHARSET;
        wcscpy_s(lf.lfFaceName, fonts[fi]);
        HFONT font = CreateFontIndirectW(&lf);
        if (!font) continue;
        HGDIOBJ old = SelectObject(dc, font);
        wchar_t face[LF_FACESIZE] = {};
        GetTextFaceW(dc, LF_FACESIZE, face);
        if (_wcsicmp(face, fonts[fi]) != 0) {  // no está instalada
            SelectObject(dc, old);
            DeleteObject(font);
            continue;
        }
        int found = 0;
        if (cachedFont == fi && cachedCp > 0) {
            found = cachedCp;
        } else {
            int best = 0;
            std::vector<BYTE> buf;
            for (int cp = 0xE700; cp <= 0xF8FF; cp++) {
                wchar_t ch = (wchar_t)cp;
                WORD gi = 0;
                if (GetGlyphIndicesW(dc, &ch, 1, &gi, GGI_MARK_NONEXISTING_GLYPHS) == GDI_ERROR || gi == 0xFFFF) continue;
                GLYPHMETRICS gm{};
                DWORD need = GetGlyphOutlineW(dc, cp, GGO_GRAY8_BITMAP, &gm, 0, nullptr, &mat);
                if (need == GDI_ERROR || need == 0 || need > 64 * 1024) continue;
                buf.resize(need);
                if (GetGlyphOutlineW(dc, cp, GGO_GRAY8_BITMAP, &gm, need, buf.data(), &mat) == GDI_ERROR) continue;
                int pitch = ((int)gm.gmBlackBoxX + 3) & ~3;
                int sc = ScoreLogoGlyph(buf.data(), (int)gm.gmBlackBoxX, (int)gm.gmBlackBoxY, pitch);
                if (sc > best) {
                    best = sc;
                    found = cp;
                }
            }
            if (found) {
                Wh_SetIntValue(L"startGlyphFont", fi);
                Wh_SetIntValue(L"startGlyph", found);
            }
        }
        SelectObject(dc, old);
        DeleteObject(font);
        if (!found) continue;
        // Dibujarlo grande y colorearlo con el azul de Windows.
        lf.lfHeight = -(int)std::lround(px * 0.86);
        font = CreateFontIndirectW(&lf);
        old = SelectObject(dc, font);
        GLYPHMETRICS gm{};
        DWORD need = GetGlyphOutlineW(dc, found, GGO_GRAY8_BITMAP, &gm, 0, nullptr, &mat);
        if (need != GDI_ERROR && need > 0) {
            std::vector<BYTE> buf(need);
            if (GetGlyphOutlineW(dc, found, GGO_GRAY8_BITMAP, &gm, need, buf.data(), &mat) != GDI_ERROR) {
                int w = (int)gm.gmBlackBoxX, h = (int)gm.gmBlackBoxY, pitch = (w + 3) & ~3;
                result.w = w;
                result.h = h;
                result.px.assign((size_t)w * h, 0);
                for (int y = 0; y < h; y++) {
                    for (int x = 0; x < w; x++) {
                        double cov = buf[(size_t)y * pitch + x] / 64.0;
                        if (cov <= 0) continue;
                        double t = std::clamp((x / (double)w + y / (double)h) / 2, 0.0, 1.0);
                        double r = 0x4C + (0x00 - 0x4C) * t, g = 0xC2 + (0x67 - 0xC2) * t,
                               b = 0xFF + (0xC0 - 0xFF) * t;
                        uint32_t a = (uint32_t)std::lround(cov * 255);
                        result.px[(size_t)y * w + x] = (a << 24) | ((uint32_t)std::lround(r * cov) << 16) |
                                                       ((uint32_t)std::lround(g * cov) << 8) |
                                                       (uint32_t)std::lround(b * cov);
                    }
                }
            }
        }
        SelectObject(dc, old);
        DeleteObject(font);
    }
    DeleteDC(dc);
    if (result.empty()) return result;
    // Margen como el de los demás iconos.
    Img padded;
    int side = (int)std::lround(std::max(result.w, result.h) / 0.70);
    padded.w = padded.h = side;
    padded.px.assign((size_t)side * side, 0);
    int ox = (side - result.w) / 2, oy = (side - result.h) / 2;
    for (int y = 0; y < result.h; y++) memcpy(&padded.px[(size_t)(oy + y) * side + ox], &result.px[(size_t)y * result.w], (size_t)result.w * 4);
    return padded;
}

static Img StockIcon(SHSTOCKICONID id, int px) {
    SHSTOCKICONINFO sii{};
    sii.cbSize = sizeof(sii);
    if (SUCCEEDED(SHGetStockIconInfo(id, SHGSI_ICONLOCATION, &sii)) && sii.szPath[0]) {
        HICON icon = nullptr;
        if (SUCCEEDED(SHDefExtractIconW(sii.szPath, sii.iIcon, 0, &icon, nullptr, MAKELONG(px, 16))) && icon) {
            Img r = HIconToImg(icon, px);
            DestroyIcon(icon);
            if (!r.empty()) return r;
        }
    }
    sii = SHSTOCKICONINFO{};
    sii.cbSize = sizeof(sii);
    if (SUCCEEDED(SHGetStockIconInfo(id, SHGSI_ICON | SHGSI_LARGEICON, &sii)) && sii.hIcon) {
        Img r = HIconToImg(sii.hIcon, 48);
        DestroyIcon(sii.hIcon);
        return r;
    }
    return {};
}

// --- Cristal líquido (lo arma el hilo de iconos) ---
struct WallpaperInfo {
    std::wstring path;
    DESKTOP_WALLPAPER_POSITION pos = DWPOS_FILL;
    COLORREF bg = RGB(0, 0, 0);
};

static std::wstring TranscodedWallpaperPath() {
    std::wstring p = ExpandEnv(L"%APPDATA%\\Microsoft\\Windows\\Themes\\TranscodedWallpaper");
    return FileExists(p) ? p : L"";
}

static WallpaperInfo ReadWallpaper(const RECT& mon) {
    WallpaperInfo w;
    bool known = false;
    IDesktopWallpaper* dw = nullptr;
    if (SUCCEEDED(CoCreateInstance(__uuidof(DesktopWallpaper), nullptr, CLSCTX_ALL, IID_PPV_ARGS(&dw))) && dw) {
        UINT count = 0;
        dw->GetMonitorDevicePathCount(&count);
        for (UINT i = 0; i < count; i++) {
            LPWSTR id = nullptr;
            if (FAILED(dw->GetMonitorDevicePathAt(i, &id)) || !id) continue;
            RECT r{};
            bool match = SUCCEEDED(dw->GetMonitorRECT(id, &r)) && EqualRect(&r, &mon);
            if (match) {
                LPWSTR path = nullptr;
                if (SUCCEEDED(dw->GetWallpaper(id, &path))) {
                    known = true;
                    if (path) {
                        w.path = path;
                        CoTaskMemFree(path);
                    }
                }
            }
            CoTaskMemFree(id);
            if (match) break;
        }
        dw->GetPosition(&w.pos);
        dw->GetBackgroundColor(&w.bg);
        dw->Release();
    }
    if (!known || (!w.path.empty() && !FileExists(w.path))) w.path = TranscodedWallpaperPath();
    return w;
}

static std::wstring FileStamp(const std::wstring& path) {
    WIN32_FILE_ATTRIBUTE_DATA fa{};
    if (path.empty() || !GetFileAttributesExW(path.c_str(), GetFileExInfoStandard, &fa)) return L"";
    return std::to_wstring(fa.ftLastWriteTime.dwLowDateTime) + L"." + std::to_wstring(fa.nFileSizeLow);
}

static void BoxBlurRgb(std::vector<float>& c, int w, int h, int r) {
    if (r <= 0 || w <= 0 || h <= 0) return;
    std::vector<float> tmp(c.size());
    for (int pass = 0; pass < 3; pass++) {
        for (int y = 0; y < h; y++)
            for (int ch = 0; ch < 3; ch++) {
                float sum = 0;
                for (int k = -r; k <= r; k++) sum += c[((size_t)y * w + std::clamp(k, 0, w - 1)) * 3 + ch];
                for (int x = 0; x < w; x++) {
                    tmp[((size_t)y * w + x) * 3 + ch] = sum / (2 * r + 1);
                    sum += c[((size_t)y * w + std::clamp(x + r + 1, 0, w - 1)) * 3 + ch] -
                           c[((size_t)y * w + std::clamp(x - r, 0, w - 1)) * 3 + ch];
                }
            }
        for (int x = 0; x < w; x++)
            for (int ch = 0; ch < 3; ch++) {
                float sum = 0;
                for (int k = -r; k <= r; k++) sum += tmp[((size_t)std::clamp(k, 0, h - 1) * w + x) * 3 + ch];
                for (int y = 0; y < h; y++) {
                    c[((size_t)y * w + x) * 3 + ch] = sum / (2 * r + 1);
                    sum += tmp[((size_t)std::clamp(y + r + 1, 0, h - 1) * w + x) * 3 + ch] -
                           tmp[((size_t)std::clamp(y - r, 0, h - 1) * w + x) * 3 + ch];
                }
            }
    }
}

static std::wstring g_glassKeyWorker;  // solo el hilo de iconos

static bool BuildGlassPixels(const Job& job, std::vector<uint32_t>& out, std::wstring& keyOut) {
    WallpaperInfo wp = ReadWallpaper(job.mon);
    std::wstring key = wp.path + L"|" + FileStamp(wp.path) + L"|" + std::to_wstring(wp.pos) + L"|" +
                       std::to_wstring(wp.bg) + L"|" + std::to_wstring(job.winW) + L"x" +
                       std::to_wstring(job.winH) + L"|" + (job.light ? L"l" : L"d");
    if (!job.force && key == g_glassKeyWorker) return false;
    g_glassKeyWorker = key;
    keyOut = key;
    constexpr int kScale = 8;
    int monW = job.mon.right - job.mon.left, monH = job.mon.bottom - job.mon.top;
    int sw = std::max(4, job.winW / kScale + 2), sh = std::max(4, job.winH / kScale + 2);
    std::vector<float> c((size_t)sw * sh * 3, 0.0f);
    {
        Gdiplus::Bitmap small(sw, sh, PixelFormat32bppARGB);
        {
            Gdiplus::Graphics g(&small);
            g.Clear(Gdiplus::Color(255, GetRValue(wp.bg), GetGValue(wp.bg), GetBValue(wp.bg)));
            g.SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBilinear);
            g.SetPixelOffsetMode(Gdiplus::PixelOffsetModeHighQuality);
            std::unique_ptr<Gdiplus::Bitmap> img;
            auto load = [&](const std::wstring& p) {
                if (p.empty()) return;
                img.reset(Gdiplus::Bitmap::FromFile(p.c_str()));
                if (img && img->GetLastStatus() != Gdiplus::Ok) img.reset();
            };
            load(wp.path);
            if (!img && !wp.path.empty()) load(TranscodedWallpaperPath());
            if (img && img->GetWidth() > 0 && img->GetHeight() > 0) {
                double iw = img->GetWidth(), ih = img->GetHeight(), dw = iw, dh = ih;
                switch (wp.pos) {
                    case DWPOS_STRETCH:
                        dw = monW;
                        dh = monH;
                        break;
                    case DWPOS_FIT: {
                        double k = std::min(monW / iw, monH / ih);
                        dw = iw * k;
                        dh = ih * k;
                        break;
                    }
                    case DWPOS_CENTER:
                    case DWPOS_TILE:
                        break;
                    default: {
                        double k = std::max(monW / iw, monH / ih);
                        dw = iw * k;
                        dh = ih * k;
                        break;
                    }
                }
                double ox = wp.pos == DWPOS_TILE ? 0 : (monW - dw) / 2;
                double oy = (wp.pos == DWPOS_TILE ? 0 : (monH - dh) / 2) - (monH - job.winH);
                Gdiplus::ImageAttributes attrs;
                attrs.SetWrapMode(wp.pos == DWPOS_TILE ? Gdiplus::WrapModeTile : Gdiplus::WrapModeTileFlipXY);
                if (wp.pos == DWPOS_TILE) {
                    double ty0 = -std::fmod((double)(monH - job.winH), ih);
                    for (double ty = ty0; ty < sh * kScale; ty += ih)
                        for (double tx = 0; tx < sw * kScale; tx += iw)
                            g.DrawImage(img.get(), Gdiplus::RectF((float)(tx / kScale), (float)(ty / kScale), (float)(iw / kScale), (float)(ih / kScale)),
                                        0, 0, (float)iw, (float)ih, Gdiplus::UnitPixel, &attrs);
                } else {
                    g.DrawImage(img.get(), Gdiplus::RectF((float)(ox / kScale), (float)(oy / kScale), (float)(dw / kScale), (float)(dh / kScale)),
                                0, 0, (float)iw, (float)ih, Gdiplus::UnitPixel, &attrs);
                }
            }
        }
        Gdiplus::BitmapData bd{};
        Gdiplus::Rect rc(0, 0, sw, sh);
        if (small.LockBits(&rc, Gdiplus::ImageLockModeRead, PixelFormat32bppARGB, &bd) == Gdiplus::Ok) {
            for (int y = 0; y < sh; y++) {
                const uint32_t* row = (const uint32_t*)((const BYTE*)bd.Scan0 + (size_t)y * bd.Stride);
                for (int x = 0; x < sw; x++) {
                    float* o = &c[((size_t)y * sw + x) * 3];
                    o[0] = (float)((row[x] >> 16) & 0xFF);
                    o[1] = (float)((row[x] >> 8) & 0xFF);
                    o[2] = (float)(row[x] & 0xFF);
                }
            }
            small.UnlockBits(&bd);
        }
    }
    BoxBlurRgb(c, sw, sh, 3);
    std::vector<uint32_t> small((size_t)sw * sh);
    const double tint = job.light ? 0.46 : 0.36, tc = job.light ? 250.0 : 28.0;
    for (size_t i = 0; i < small.size(); i++) {
        const float* p = &c[i * 3];
        double l = 0.299 * p[0] + 0.587 * p[1] + 0.114 * p[2];
        uint32_t o = 0xFF000000u;
        for (int ch = 0; ch < 3; ch++) {
            double v = l + (p[ch] - l) * 1.55;
            v = v + (tc - v) * tint;
            o |= (uint32_t)std::clamp((int)std::lround(v), 0, 255) << (16 - ch * 8);
        }
        small[i] = o;
    }
    out.assign((size_t)job.winW * job.winH, 0xFF000000u);
    for (int y = 0; y < job.winH; y++) {
        double fy = std::clamp((y + 0.5) / kScale - 0.5, 0.0, sh - 1.0);
        int y0 = (int)fy, y1 = std::min(y0 + 1, sh - 1);
        double ty = fy - y0;
        for (int x = 0; x < job.winW; x++) {
            double fx = std::clamp((x + 0.5) / kScale - 0.5, 0.0, sw - 1.0);
            int x0 = (int)fx, x1 = std::min(x0 + 1, sw - 1);
            double tx = fx - x0;
            uint32_t p00 = small[(size_t)y0 * sw + x0], p10 = small[(size_t)y0 * sw + x1];
            uint32_t p01 = small[(size_t)y1 * sw + x0], p11 = small[(size_t)y1 * sw + x1];
            uint32_t o = 0xFF000000u;
            for (int s8 = 0; s8 <= 16; s8 += 8) {
                double a = ((p00 >> s8) & 0xFF) * (1 - tx) + ((p10 >> s8) & 0xFF) * tx;
                double b = ((p01 >> s8) & 0xFF) * (1 - tx) + ((p11 >> s8) & 0xFF) * tx;
                o |= (uint32_t)std::lround(a * (1 - ty) + b * ty) << s8;
            }
            out[(size_t)y * job.winW + x] = o;
        }
    }
    return true;
}

static std::shared_ptr<IconSet> VectorTrash(int px, bool full);

static void RunJob(const Job& job, JobResult& r) {
    r.kind = job.kind;
    r.key = job.key;
    switch (job.kind) {
        case JobKind::App: {
            Img img;
            for (auto& s : job.srcs) {
                img = ShellIcon(s, job.px);
                if (!img.empty()) break;
            }
            if (!img.empty()) r.set = MakeIconSet(img, job.px);
            if (job.wantName) {
                for (auto& s : job.srcs) {
                    r.name = ShellName(s);
                    if (!r.name.empty()) break;
                }
            }
            break;
        }
        case JobKind::Start: {
            Img img;
            if (!job.startIcon.empty()) img = ImageFromSpec(job.startIcon, job.px);
            if (img.empty()) img = LogoFromSystemFont(job.px);
            if (img.empty()) img = StockIcon(SIID_APPLICATION, job.px);
            if (!img.empty()) r.set = MakeIconSet(img, job.px);
            break;
        }
        case JobKind::Trash: {
            Img img;
            if (!job.vectorTrash) img = StockIcon(job.trashFull ? SIID_RECYCLERFULL : SIID_RECYCLER, job.px);
            if (!img.empty()) r.set = MakeIconSet(img, job.px);
            else r.set = VectorTrash(job.px, job.trashFull);
            break;
        }
        case JobKind::Glass: {
            auto px = std::make_shared<std::vector<uint32_t>>();
            if (BuildGlassPixels(job, *px, r.glassKey)) {
                r.glass = px;
                r.winW = job.winW;
                r.winH = job.winH;
            }
            break;
        }
    }
}

static DWORD WINAPI JobThread(LPVOID) {
    CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
    while (!g_jobStop) {
        Job job;
        bool have = false;
        {
            std::lock_guard<std::mutex> lock(g_jobMx);
            if (!g_jobs.empty()) {
                job = std::move(g_jobs.front());
                g_jobs.pop_front();
                have = true;
            }
        }
        if (!have) {
            DWORD r = MsgWaitForMultipleObjects(1, &g_jobEvent, FALSE, INFINITE, QS_ALLINPUT);
            if (r == WAIT_OBJECT_0 + 1) {
                MSG m;
                while (PeekMessageW(&m, nullptr, 0, 0, PM_REMOVE)) {
                    TranslateMessage(&m);
                    DispatchMessageW(&m);
                }
            }
            continue;
        }
        JobResult res;
        RunJob(job, res);
        {
            std::lock_guard<std::mutex> lock(g_jobMx);
            g_results.push_back(std::move(res));
        }
        HWND h = g_wnd.load();
        if (h) PostMessageW(h, WM_APP_JOBS, 0, 0);
    }
    CoUninitialize();
    return 0;
}

static void PostJob(Job job) {
    {
        std::lock_guard<std::mutex> lock(g_jobMx);
        g_jobs.push_back(std::move(job));
    }
    if (g_jobEvent) SetEvent(g_jobEvent);
}

static int IconPx() { return std::max(64, S(std::max(g_set.magSize, g_set.iconSize))); }

static void RequestIcon(DockItem& it) {
    if (it.iconKey.empty() || it.kind != Kind::App) return;
    IconEntry& e = g_icons[it.iconKey];
    bool wantName = !it.nameFinal;
    if (e.state == 1 || (e.state >= 2 && e.px == IconPx() && !wantName)) return;
    e.state = 1;
    Job j;
    j.kind = JobKind::App;
    j.key = it.iconKey;
    j.srcs = it.iconSrcs;
    j.px = IconPx();
    j.wantName = wantName;
    PostJob(std::move(j));
}

static bool g_trashFull = false;

static void RequestFixedIcon(DockItem& it) {
    Job j;
    j.px = IconPx();
    if (it.kind == Kind::Start) {
        it.iconKey = L"::start";
        j.kind = JobKind::Start;
        j.startIcon = g_set.startIcon;
    } else if (it.kind == Kind::Trash) {
        it.iconKey = g_trashFull ? L"::trash1" : L"::trash0";
        j.kind = JobKind::Trash;
        j.trashFull = g_trashFull;
        j.vectorTrash = g_set.vectorTrash;
    } else {
        return;
    }
    j.key = it.iconKey;
    IconEntry& e = g_icons[it.iconKey];
    if (e.state == 1 || (e.state == 2 && e.px == IconPx())) return;
    e.state = 1;
    PostJob(std::move(j));
}

// ---------------------------------------------------------------------------
// Dibujos de respaldo (GDI+, una sola vez)
// ---------------------------------------------------------------------------

static void AddRoundRect(Gdiplus::GraphicsPath& p, float x, float y, float w, float h, float r) {
    r = std::min(r, std::min(w, h) / 2);
    if (r < 0.5f) {
        p.AddRectangle(Gdiplus::RectF(x, y, w, h));
        return;
    }
    float d = r * 2;
    p.AddArc(x, y, d, d, 180, 90);
    p.AddArc(x + w - d, y, d, d, 270, 90);
    p.AddArc(x + w - d, y + h - d, d, d, 0, 90);
    p.AddArc(x, y + h - d, d, d, 90, 90);
    p.CloseFigure();
}

static Img ImgFromBitmap(Gdiplus::Bitmap& b) {
    Img r;
    r.w = (int)b.GetWidth();
    r.h = (int)b.GetHeight();
    r.px.resize((size_t)r.w * r.h);
    Gdiplus::BitmapData bd{};
    Gdiplus::Rect rc(0, 0, r.w, r.h);
    if (b.LockBits(&rc, Gdiplus::ImageLockModeRead, PixelFormat32bppPARGB, &bd) != Gdiplus::Ok) return {};
    for (int y = 0; y < r.h; y++) memcpy(&r.px[(size_t)y * r.w], (BYTE*)bd.Scan0 + (size_t)y * bd.Stride, (size_t)r.w * 4);
    b.UnlockBits(&bd);
    return r;
}

static std::shared_ptr<IconSet> VectorTrash(int px, bool full) {
    Gdiplus::Bitmap bmp(px, px, PixelFormat32bppPARGB);
    {
        Gdiplus::Graphics g(&bmp);
        g.Clear(Gdiplus::Color(0, 0, 0, 0));
        g.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
        g.SetPixelOffsetMode(Gdiplus::PixelOffsetModeHighQuality);
        float s = (float)px;
        float top = s * 0.25f, bottom = s * 0.93f, topW = s * 0.66f, botW = s * 0.52f, cx = s / 2;
        float rimH = s * 0.10f, br = s * 0.07f;
        // Interior oscuro que se ve por la boca
        Gdiplus::SolidBrush inner(Gdiplus::Color(210, 58, 62, 72));
        g.FillEllipse(&inner, cx - topW / 2, top - rimH / 2, topW, rimH);
        if (full) {
            // Papeles arrugados asomando
            Gdiplus::Color pc[] = {Gdiplus::Color(255, 250, 250, 248), Gdiplus::Color(255, 236, 240, 246),
                                   Gdiplus::Color(255, 246, 244, 236)};
            float bx[] = {-0.15f, 0.06f, 0.18f}, by[] = {-0.05f, -0.09f, -0.02f}, rr[] = {0.11f, 0.12f, 0.09f};
            Gdiplus::Pen pe(Gdiplus::Color(200, 170, 174, 182), std::max(1.0f, s * 0.012f));
            for (int i = 0; i < 3; i++) {
                Gdiplus::SolidBrush b(pc[i]);
                float r = rr[i] * s;
                g.FillEllipse(&b, cx + bx[i] * s - r, top + by[i] * s - r, 2 * r, 2 * r);
                g.DrawEllipse(&pe, cx + bx[i] * s - r, top + by[i] * s - r, 2 * r, 2 * r);
            }
        }
        Gdiplus::GraphicsPath body;
        body.AddLine(cx - topW / 2, top, cx + topW / 2, top);
        body.AddLine(cx + topW / 2, top, cx + botW / 2, bottom - br);
        body.AddArc(cx + botW / 2 - 2 * br, bottom - 2 * br, 2 * br, 2 * br, 0, 90);
        body.AddArc(cx - botW / 2, bottom - 2 * br, 2 * br, 2 * br, 90, 90);
        body.CloseFigure();
        // Vidrio esmerilado: degradado horizontal con brillo a la izquierda
        Gdiplus::LinearGradientBrush bb(Gdiplus::PointF(cx - topW / 2, 0), Gdiplus::PointF(cx + topW / 2, 0),
                                        Gdiplus::Color(215, 200, 205, 214), Gdiplus::Color(215, 160, 166, 178));
        Gdiplus::REAL pos[] = {0.0f, 0.35f, 1.0f};
        Gdiplus::Color cols[] = {Gdiplus::Color(215, 196, 201, 212), Gdiplus::Color(230, 244, 246, 250),
                                 Gdiplus::Color(215, 150, 157, 170)};
        bb.SetInterpolationColors(cols, pos, 3);
        g.FillPath(&bb, &body);
        // Malla: costillas verticales y una banda horizontal
        g.SetClip(&body);
        Gdiplus::Pen rib(Gdiplus::Color(120, 110, 116, 132), std::max(1.0f, s * 0.013f));
        const int ribs = 8;
        for (int i = 1; i < ribs; i++) {
            float t = (float)i / ribs;
            g.DrawLine(&rib, cx - topW / 2 + topW * t, top, cx - botW / 2 + botW * t, bottom);
        }
        Gdiplus::Pen band(Gdiplus::Color(90, 255, 255, 255), std::max(1.0f, s * 0.02f));
        g.DrawLine(&band, cx - topW / 2, top + (bottom - top) * 0.5f, cx + topW / 2, top + (bottom - top) * 0.5f);
        g.ResetClip();
        Gdiplus::Pen edge(Gdiplus::Color(210, 120, 126, 140), std::max(1.0f, s * 0.016f));
        g.DrawPath(&edge, &body);
        // Aro de la boca
        Gdiplus::Pen rim(Gdiplus::Color(255, 238, 240, 244), std::max(1.5f, s * 0.038f));
        g.DrawEllipse(&rim, cx - topW / 2, top - rimH / 2, topW, rimH);
        Gdiplus::Pen rimEdge(Gdiplus::Color(180, 116, 122, 136), std::max(1.0f, s * 0.012f));
        g.DrawEllipse(&rimEdge, cx - topW / 2 - s * 0.017f, top - rimH / 2 - s * 0.017f, topW + s * 0.034f,
                      rimH + s * 0.034f);
    }
    return MakeIconSet(ImgFromBitmap(bmp), px);
}

// Mientras llega el icono: un cuadro con la inicial (uno por letra).
static std::shared_ptr<IconSet> PlaceholderIcon(int px, wchar_t ch) {
    Gdiplus::Bitmap bmp(px, px, PixelFormat32bppPARGB);
    {
        Gdiplus::Graphics g(&bmp);
        g.Clear(Gdiplus::Color(0, 0, 0, 0));
        g.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
        g.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);
        float s = (float)px;
        Gdiplus::GraphicsPath p;
        AddRoundRect(p, s * 0.08f, s * 0.08f, s * 0.84f, s * 0.84f, s * 0.22f);
        Gdiplus::LinearGradientBrush b(Gdiplus::PointF(0, 0), Gdiplus::PointF(0, s),
                                       Gdiplus::Color(245, 150, 156, 168), Gdiplus::Color(245, 98, 104, 116));
        g.FillPath(&b, &p);
        Gdiplus::FontFamily fam(L"Segoe UI");
        Gdiplus::Font f(fam.GetLastStatus() == Gdiplus::Ok ? &fam : Gdiplus::FontFamily::GenericSansSerif(),
                        s * 0.42f, Gdiplus::FontStyleBold, Gdiplus::UnitPixel);
        Gdiplus::StringFormat sf;
        sf.SetAlignment(Gdiplus::StringAlignmentCenter);
        sf.SetLineAlignment(Gdiplus::StringAlignmentCenter);
        Gdiplus::SolidBrush tb(Gdiplus::Color(255, 255, 255, 255));
        wchar_t text[2] = {ch, 0};
        g.DrawString(text, 1, &f, Gdiplus::RectF(0, 0, s, s), &sf, &tb);
    }
    return MakeIconSet(ImgFromBitmap(bmp), px);
}

static const IconSet* IconFor(DockItem& it) {
    auto found = g_icons.find(it.iconKey);
    if (found != g_icons.end() && found->second.set) return found->second.set.get();
    wchar_t ch = it.name.empty() ? L'?' : (wchar_t)(ULONG_PTR)CharUpperW((LPWSTR)(ULONG_PTR)(WORD)it.name[0]);
    if (it.kind == Kind::Start || it.kind == Kind::Trash) return nullptr;  // aún no llega: nada
    std::wstring key = std::wstring(L"::ph") + ch;
    IconEntry& ph = g_icons[key];
    if (!ph.set || ph.px != IconPx()) {
        ph.set = PlaceholderIcon(IconPx(), ch);
        ph.px = IconPx();
        ph.state = 2;
    }
    return ph.set.get();
}

// ---------------------------------------------------------------------------
// Medidas y diseño
// ---------------------------------------------------------------------------

struct Metrics {
    double base = 48, maxS = 80, gap = 6, padV = 6, margin = 6, sepW = 16, bgH = 60;
};

static Metrics g_m;

static void UpdateMetrics() {
    Metrics m;
    m.base = S(g_set.iconSize);
    m.maxS = g_set.magnify ? std::max(m.base, (double)S(g_set.magSize)) : m.base;
    m.gap = S(g_set.spacing);
    m.padV = std::max(3.0, std::round(m.base * 0.13));
    m.margin = S(g_set.bottomMargin);
    m.sepW = std::max(6.0, std::round(m.base * 0.26));
    m.bgH = m.base + 2 * m.padV;
    g_m = m;
}

static int DockAreaHeight() {
    return (int)std::ceil(g_m.margin + g_m.bgH + (g_m.maxS - g_m.base) + g_m.base * 0.55 + S(56));
}

struct Geo {
    bool shown = false;
    double x = 0, y = 0, size = 0;
    double slotL = 0, slotR = 0;
    double cx = 0;
    double alpha = 1;
};

struct Layout {
    std::vector<Geo> g;
    double bgL = 0, bgR = 0, bgT = 0, bgB = 0;
    double base = 48, maxS = 80, pad = 6;
    double topMost = 0;
    double hideOff = 0;
    double baseL = 0, baseR = 0;
};

static Spring g_mag;      // 0..1 ampliación
static Spring g_hide;     // 0 visible, 1 escondido
static Spring g_labelA;   // opacidad del nombre
static Spring g_labelX;   // centro del nombre (se desliza entre iconos)
static Spring g_anchor;   // x del mouse, suavizada
static double g_mouseX = 0, g_mouseY = 0;  // crudos, en coordenadas del lienzo
static bool g_hover = false;
static DockItem* g_labelItem = nullptr;

static DockItem* g_press = nullptr;
static POINT g_pressPt{};
static DockItem* g_drag = nullptr;
static double g_dragGrabX = 0.5, g_dragGrabY = 0.5;
static double g_dragSize = 48;
static bool g_dragRemove = false;
static bool g_dragOverTrash = false;
static DockItem* g_insertBefore = nullptr;

static bool g_fileDrag = false;
static bool g_fileDragPins = false;
static DockItem* g_fileTarget = nullptr;

static bool g_menuOpen = false;
static DockItem* g_menuItem = nullptr;

static void ClosePopup(bool instant);
static void OpenPreview(DockItem* it);
static bool PopupOpen();
static bool StepPopup(double dt);
static bool CursorInPopup(POINT pt);
static uint64_t g_prevSuppress = 0;  // no volver a abrir la vista de esta app hasta salir de ella
static DockItem* g_prevCand = nullptr;
static double g_prevSince = 0;

static double BounceOffset(DockItem& it, double now, double base) {
    if (it.bounceStart < 0) return 0;
    double period = 0.58 * g_set.speed;
    double t = now - it.bounceStart;
    if (t < 0) return 0;
    if (!it.launching && it.bounceCycles > 0 && t >= it.bounceCycles * period) {
        it.bounceStart = -1;
        return 0;
    }
    double p = std::fmod(t, period) / period;
    // Salto suave: sube con desaceleración, baja con aceleración (parábola),
    // y se aplana un poco en el piso para que no se vea brusco.
    double h = 4 * p * (1 - p);
    return it.bounceAmp * base * h * h * (3 - 2 * h);
}

static bool IsIconKind(Kind k) { return k == Kind::App || k == Kind::Start || k == Kind::Trash; }

static Layout ComputeLayout(double now) {
    Layout L;
    size_t n = g_items.size();
    L.g.resize(n);
    double base = g_m.base, maxS = g_m.maxS, gap = g_m.gap;
    auto iconW0 = [&](const DockItem& it, double b) {
        switch (it.kind) {
            case Kind::Sep:
                return g_m.sepW * b / g_m.base;
            case Kind::End:
                return 0.0;
            default:
                return b;
        }
    };
    auto slotScale = [](const DockItem& it) {
        return std::max(0.0, it.presence.x) * std::clamp(it.slot.x, 0.0, 1.2);
    };
    {
        double need = 0;
        for (auto& it : g_items) {
            if (it->kind != Kind::End) need += (iconW0(*it, base) + gap) * slotScale(*it);
        }
        double avail = g_winW - S(24) - 2 * g_m.padV;
        if (need > avail && need > 0) {
            double k = std::max(0.3, avail / need);
            base *= k;
            maxS = std::max(base, maxS * k);
            gap *= k;
        }
    }
    L.base = base;
    L.maxS = maxS;
    double pad = g_m.padV * base / g_m.base;
    L.pad = pad;
    double bgH = base + 2 * pad;

    std::vector<double> slot0(n), left0(n), center0(n);
    double total0 = 0;
    for (size_t i = 0; i < n; i++) {
        auto& it = *g_items[i];
        double gb = std::max(0.0, it.gap.x) * (base + gap);
        slot0[i] = (iconW0(it, base) + gap) * slotScale(it) + gb;
        total0 += slot0[i];
    }
    double centerX = g_winW / 2.0;
    double start0 = centerX - total0 / 2;
    double cur = start0;
    for (size_t i = 0; i < n; i++) {
        auto& it = *g_items[i];
        double gb = std::max(0.0, it.gap.x) * (base + gap);
        left0[i] = cur;
        center0[i] = cur + gb + (iconW0(it, base) + gap) * slotScale(it) / 2;
        cur += slot0[i];
    }

    double ax = std::clamp(g_anchor.x, start0, start0 + total0);
    double mag = g_set.magnify ? std::clamp(g_mag.x, 0.0, 1.0) : 0.0;
    // Curva suave (coseno) de unos tres iconos a cada lado.
    const double range = g_set.magRange + 0.0;
    std::vector<double> iconW(n), slotW(n);
    double total = 0;
    for (size_t i = 0; i < n; i++) {
        auto& it = *g_items[i];
        double w0 = iconW0(it, base), w = w0;
        if (IsIconKind(it.kind) && mag > 0) {
            double d = std::fabs(ax - center0[i]) / (base + gap);
            double f = d < range ? (std::cos(kPi * d / range) + 1) / 2 : 0;
            w = w0 + (maxS - base) * mag * f;
        }
        double gb = std::max(0.0, it.gap.x) * (base + gap);
        iconW[i] = w;
        slotW[i] = (w + gap) * slotScale(it) + gb;
        total += slotW[i];
    }
    // El punto bajo el mouse se queda bajo el mouse; con la ampliación en 0
    // esto da exactamente la posición normal (sin saltos al entrar o salir).
    double start = start0;
    if (mag > 0 && n > 0) {
        size_t k = n - 1;
        double t = 0;
        for (size_t i = 0; i < n; i++) {
            if (ax < left0[i] + slot0[i] || i == n - 1) {
                k = i;
                t = slot0[i] > 0 ? std::clamp((ax - left0[i]) / slot0[i], 0.0, 1.0) : 0;
                break;
            }
        }
        double before = 0;
        for (size_t i = 0; i < k; i++) before += slotW[i];
        start = ax - (before + t * slotW[k]);
    }
    double edge = S(4) + pad;
    if (start < edge) start = edge;
    if (start + total > g_winW - edge) start = std::max(edge, g_winW - edge - total);

    L.hideOff = std::clamp(g_hide.x, 0.0, 1.2) * (bgH + g_m.margin + S(14));
    L.bgB = g_winH - g_m.margin + L.hideOff;
    L.bgT = L.bgB - bgH;
    L.bgL = start - pad + gap / 2;
    L.bgR = start + total + pad - gap / 2;
    L.baseL = start0 - pad + gap / 2;
    L.baseR = start0 + total0 + pad - gap / 2;
    L.topMost = L.bgT;

    cur = start;
    for (size_t i = 0; i < n; i++) {
        auto& it = *g_items[i];
        Geo& g = L.g[i];
        double s = slotScale(it);
        double gb = std::max(0.0, it.gap.x) * (base + gap);
        g.slotL = cur;
        g.slotR = cur + slotW[i];
        double pres = std::max(0.0, it.presence.x);
        double size = iconW[i] * std::min(1.0, pres) * (pres > 1 ? pres : 1.0);
        if (it.kind == Kind::Sep) size = iconW[i] * s;
        g.size = size;
        g.cx = cur + gb + (iconW[i] + gap) * s / 2;
        g.x = g.cx - size / 2;
        g.y = L.bgB - pad - size - BounceOffset(it, now, base);
        g.alpha = std::clamp(pres, 0.0, 1.0);
        g.shown = it.kind != Kind::End && s > 0.001 && size > 0.5 && &it != g_drag;
        if (it.kind != Kind::Sep && g.shown) L.topMost = std::min(L.topMost, g.y);
        cur += slotW[i];
    }
    return L;
}

static int ItemAtX(const Layout& L, double x) {
    for (size_t i = 0; i < g_items.size() && i < L.g.size(); i++) {
        if (!L.g[i].shown || !IsIconKind(g_items[i]->kind)) continue;
        if (x >= L.g[i].slotL && x < L.g[i].slotR) return (int)i;
    }
    return -1;
}

static int ItemAt(const Layout& L, double x, double y) {
    int i = ItemAtX(L, x);
    if (i < 0) return -1;
    if (y < L.g[i].y - S(4) || y > g_winH) return -1;
    return i;
}

// ---------------------------------------------------------------------------
// Ritmo de los cuadros: uno por refresco real del monitor
// ---------------------------------------------------------------------------
// Se espera el vblank del monitor con D3DKMTWaitForVerticalBlankEvent (lo
// que usan los navegadores). Si no se puede, DwmFlush; y si tampoco espera,
// un temporizador de alta resolución con el periodo del monitor. Cada cuadro
// lleva la hora exacta de su refresco, así las animaciones avanzan en pasos
// iguales aunque el hilo se atrase un poco: sin trompicones.

struct KmtOpenFromHdc {
    HDC hDc;
    UINT hAdapter;
    LUID adapterLuid;
    UINT vidPnSourceId;
};
struct KmtWaitVblank {
    UINT hAdapter;
    UINT hDevice;
    UINT vidPnSourceId;
};
struct KmtClose {
    UINT hAdapter;
};
using PfnKmtOpen = LONG(APIENTRY*)(KmtOpenFromHdc*);
using PfnKmtWait = LONG(APIENTRY*)(const KmtWaitVblank*);
using PfnKmtClose = LONG(APIENTRY*)(const KmtClose*);

static HANDLE g_vsyncThread = nullptr;
static HANDLE g_vsyncWake = nullptr;
static std::atomic<bool> g_vsyncRun{false};
static std::atomic<bool> g_vsyncStop{false};
static std::atomic<bool> g_framePosted{false};
static std::atomic<bool> g_vsyncReopen{true};
static std::atomic<int> g_vsyncMode{0};  // 1 vblank, 2 DwmFlush, 3 temporizador
static std::mutex g_vsyncMx;
static std::wstring g_vsyncDevice;
static double g_lastFrame = 0;
static double g_lastActive = 0;

static double MonitorPeriod(const std::wstring& device) {
    if (g_forcedPeriodMs > 0) return g_forcedPeriodMs / 1000.0;
    DEVMODEW dm{};
    dm.dmSize = sizeof(dm);
    if (!device.empty() && EnumDisplaySettingsW(device.c_str(), ENUM_CURRENT_SETTINGS, &dm) &&
        dm.dmDisplayFrequency > 1) {
        return 1.0 / dm.dmDisplayFrequency;
    }
    DWM_TIMING_INFO ti{};
    ti.cbSize = sizeof(ti);
    if (SUCCEEDED(DwmGetCompositionTimingInfo(nullptr, &ti)) && ti.rateRefresh.uiNumerator) {
        return (double)ti.rateRefresh.uiDenominator / ti.rateRefresh.uiNumerator;
    }
    return 1.0 / 60;
}

static DWORD WINAPI VsyncThread(LPVOID) {
    HMODULE gdi = GetModuleHandleW(L"gdi32.dll");
    auto kOpen = (PfnKmtOpen)(void*)GetProcAddress(gdi, "D3DKMTOpenAdapterFromHdc");
    auto kWait = (PfnKmtWait)(void*)GetProcAddress(gdi, "D3DKMTWaitForVerticalBlankEvent");
    auto kClose = (PfnKmtClose)(void*)GetProcAddress(gdi, "D3DKMTCloseAdapter");
    HANDLE timer = CreateWaitableTimerExW(nullptr, nullptr, CREATE_WAITABLE_TIMER_HIGH_RESOLUTION,
                                          TIMER_ALL_ACCESS);
    if (!timer) timer = CreateWaitableTimerW(nullptr, TRUE, nullptr);
    KmtWaitVblank wait{};
    bool haveKmt = false;
    int kmtFails = 0;
    double period = 1.0 / 60, next = 0;

    auto waitUntil = [&](double t) {
        double ms = (t - Now()) * 1000.0;
        if (ms <= 0) return;
        if (timer && ms > 1.2) {
            LARGE_INTEGER due;
            due.QuadPart = -(LONGLONG)((ms - 0.6) * 10000.0);
            if (SetWaitableTimer(timer, &due, 0, nullptr, nullptr, FALSE)) WaitForSingleObject(timer, 100);
        }
        while (Now() < t) YieldProcessor();
    };

    while (!g_vsyncStop) {
        if (!g_vsyncRun) {
            WaitForSingleObject(g_vsyncWake, INFINITE);
            next = 0;
            continue;
        }
        if (g_vsyncReopen.exchange(false)) {
            if (haveKmt && kClose) {
                KmtClose c{wait.hAdapter};
                kClose(&c);
            }
            haveKmt = false;
            std::wstring dev;
            {
                std::lock_guard<std::mutex> lock(g_vsyncMx);
                dev = g_vsyncDevice;
            }
            period = MonitorPeriod(dev);
            if (g_forcedPeriodMs <= 0 && kOpen && kWait && !dev.empty() && kmtFails < 3) {
                HDC dc = CreateDCW(nullptr, dev.c_str(), nullptr, nullptr);
                if (dc) {
                    KmtOpenFromHdc o{};
                    o.hDc = dc;
                    if (kOpen(&o) == 0) {
                        wait.hAdapter = o.hAdapter;
                        wait.hDevice = 0;
                        wait.vidPnSourceId = o.vidPnSourceId;
                        haveKmt = true;
                    }
                    DeleteDC(dc);
                }
            }
        }
        bool waited = false;
        if (haveKmt) {
            double t0 = Now();
            if (kWait(&wait) == 0 && Now() - t0 > 0.0005) {
                waited = true;
                g_vsyncMode = 1;
                kmtFails = 0;
            } else if (++kmtFails >= 3) {
                if (kClose) {
                    KmtClose c{wait.hAdapter};
                    kClose(&c);
                }
                haveKmt = false;
            }
        }
        if (!waited && g_forcedPeriodMs <= 0) {
            double t0 = Now();
            if (SUCCEEDED(DwmFlush()) && Now() - t0 > 0.001) {
                waited = true;
                g_vsyncMode = 2;
            }
        }
        if (!waited) {
            // Reloj propio en fase: sin acumular atraso.
            double now = Now();
            if (next <= 0 || next < now - period) next = now + period;
            waitUntil(next);
            next += period;
            g_vsyncMode = 3;
        }
        LARGE_INTEGER q;
        QueryPerformanceCounter(&q);
        if (g_vsyncRun && !g_framePosted.exchange(true)) {
            HWND h = g_wnd.load();
            if (!h || !PostMessageW(h, WM_APP_FRAME, 0, (LPARAM)q.QuadPart)) g_framePosted = false;
        }
    }
    if (haveKmt && kClose) {
        KmtClose c{wait.hAdapter};
        kClose(&c);
    }
    if (timer) CloseHandle(timer);
    return 0;
}

static void Kick() {
    g_lastActive = Now();
    if (!g_vsyncRun) {
        g_lastFrame = Now();
        g_vsyncRun = true;
        if (g_vsyncWake) SetEvent(g_vsyncWake);
    }
}

// ---------------------------------------------------------------------------
// Lienzo
// ---------------------------------------------------------------------------

static Layout g_lastLayout;
static std::vector<uint32_t> g_glass;  // fondo desenfocado del tamaño del lienzo

static void FreeBuffers() {
    if (g_memDC) {
        if (g_oldBmp) SelectObject(g_memDC, g_oldBmp);
        DeleteDC(g_memDC);
    }
    if (g_dib) DeleteObject(g_dib);
    g_memDC = nullptr;
    g_dib = nullptr;
    g_oldBmp = nullptr;
    g_bits = nullptr;
}

static bool CreateBuffers() {
    FreeBuffers();
    if (g_winW <= 0 || g_winH <= 0) return false;
    BITMAPINFO bi{};
    bi.bmiHeader.biSize = sizeof(bi.bmiHeader);
    bi.bmiHeader.biWidth = g_winW;
    bi.bmiHeader.biHeight = -g_winH;
    bi.bmiHeader.biPlanes = 1;
    bi.bmiHeader.biBitCount = 32;
    bi.bmiHeader.biCompression = BI_RGB;
    void* bits = nullptr;
    g_memDC = CreateCompatibleDC(nullptr);
    g_dib = CreateDIBSection(g_memDC, &bi, DIB_RGB_COLORS, &bits, nullptr, 0);
    if (!g_memDC || !g_dib || !bits) {
        FreeBuffers();
        return false;
    }
    g_oldBmp = SelectObject(g_memDC, g_dib);
    g_bits = (uint32_t*)bits;
    memset(g_bits, 0, (size_t)g_winW * g_winH * 4);
    g_fullRedraw = true;
    return true;
}

static std::unique_ptr<Gdiplus::FontFamily> g_family;
static std::unique_ptr<Gdiplus::Font> g_font;
static std::unique_ptr<Gdiplus::Font> g_fontBoldPtr, g_fontSmallPtr;

static void RecreateFont() {
    g_font.reset();
    g_family = std::make_unique<Gdiplus::FontFamily>(g_set.font.c_str());
    if (g_family->GetLastStatus() != Gdiplus::Ok) g_family = std::make_unique<Gdiplus::FontFamily>(L"Segoe UI");
    g_font = std::make_unique<Gdiplus::Font>(g_family.get(), (Gdiplus::REAL)S(13), Gdiplus::FontStyleRegular,
                                             Gdiplus::UnitPixel);
    g_fontBoldPtr = std::make_unique<Gdiplus::Font>(g_family.get(), (Gdiplus::REAL)S(13), Gdiplus::FontStyleBold,
                                                    Gdiplus::UnitPixel);
    g_fontSmallPtr = std::make_unique<Gdiplus::Font>(g_family.get(), (Gdiplus::REAL)S(11), Gdiplus::FontStyleRegular,
                                                     Gdiplus::UnitPixel);
}

static bool DarkUi() {
    switch (g_set.style) {
        case DockStyle::Dark:
            return true;
        case DockStyle::Light:
            return false;
        default:
            return !g_light;
    }
}

// --- Nombres: se dibujan una vez por texto y luego solo se copian ---
struct LabelImg {
    std::wstring text;
    bool dark = true;
    UINT dpi = 96;
    Img img;
    int pad = 0;  // margen de la sombra alrededor de la burbuja
};
static std::deque<LabelImg> g_labelCache;

static const LabelImg* GetLabelImg(const std::wstring& text) {
    bool dark = DarkUi();
    for (auto& l : g_labelCache) {
        if (l.text == text && l.dark == dark && l.dpi == g_dpi) return &l;
    }
    if (!g_font) return nullptr;
    Gdiplus::RectF tb;
    {
        HDC dc = CreateCompatibleDC(nullptr);
        Gdiplus::Graphics mg(dc);
        mg.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);
        mg.MeasureString(text.c_str(), (INT)text.size(), g_font.get(), Gdiplus::PointF(0, 0), &tb);
        DeleteDC(dc);
    }
    int padX = S(11), padY = S(5), sp = S(8);
    int bw = std::min((int)std::ceil(tb.Width) + 2 * padX, S(380));
    int bh = (int)std::ceil(tb.Height) + 2 * padY;
    LabelImg li;
    li.text = text;
    li.dark = dark;
    li.dpi = g_dpi;
    li.pad = sp;
    li.img.w = bw + 2 * sp;
    li.img.h = bh + 2 * sp;
    li.img.px.assign((size_t)li.img.w * li.img.h, 0);
    Surface s{li.img.px.data(), li.img.w, li.img.h};
    RECT all{0, 0, li.img.w, li.img.h};
    RoundStyle st;
    st.fillA = dark ? 0.86f : 0.93f;
    st.fr = dark ? 38.f : 248.f;
    st.fg = dark ? 38.f : 248.f;
    st.fb = dark ? 42.f : 250.f;
    st.shadowA = dark ? 0.32f : 0.16f;
    st.shadowR = (float)S(7);
    st.shadowOff = (float)S(2);
    st.rimA = dark ? 0.16f : 0.6f;
    st.outerA = dark ? 0.35f : 0.08f;
    FillRoundRect(s, all, (float)sp, (float)sp, (float)bw, (float)bh, (float)S(8), st);
    {
        Gdiplus::Bitmap b(li.img.w, li.img.h, li.img.w * 4, PixelFormat32bppPARGB, (BYTE*)li.img.px.data());
        Gdiplus::Graphics g(&b);
        g.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);
        Gdiplus::StringFormat sf;
        sf.SetAlignment(Gdiplus::StringAlignmentCenter);
        sf.SetLineAlignment(Gdiplus::StringAlignmentCenter);
        sf.SetTrimming(Gdiplus::StringTrimmingEllipsisCharacter);
        sf.SetFormatFlags(Gdiplus::StringFormatFlagsNoWrap);
        Gdiplus::SolidBrush brush(dark ? Gdiplus::Color(255, 246, 246, 248) : Gdiplus::Color(255, 28, 28, 30));
        g.DrawString(text.c_str(), (INT)text.size(), g_font.get(),
                     Gdiplus::RectF((float)sp, (float)sp, (float)bw, (float)bh), &sf, &brush);
        g.Flush(Gdiplus::FlushIntentionSync);
    }
    g_labelCache.push_front(std::move(li));
    if (g_labelCache.size() > 32) g_labelCache.pop_back();
    return &g_labelCache.front();
}

static std::wstring ItemLabel(const DockItem& it) {
    switch (it.kind) {
        case Kind::Start:
            return L"Inicio";
        case Kind::Trash:
            return L"Papelera de reciclaje";
        default:
            return it.name;
    }
}

struct LabelGeo {
    const LabelImg* li = nullptr;
    int x = 0, y = 0;
    double alpha = 0;
};

static LabelGeo ComputeLabel(const Layout& L) {
    LabelGeo lg;
    if (!g_labelItem || g_labelA.x < 0.01 || g_drag) return lg;
    int idx = IndexOf(g_labelItem);
    if (idx < 0 || !L.g[idx].shown) return lg;
    lg.li = GetLabelImg(ItemLabel(*g_labelItem));
    if (!lg.li) return lg;
    double cx = g_labelX.x;
    int w = lg.li->img.w;
    lg.x = (int)std::lround(std::clamp(cx - w / 2.0, (double)S(2) - lg.li->pad, (double)(g_winW - w + lg.li->pad - S(2))));
    lg.y = (int)std::lround(L.topMost - S(10) - (lg.li->img.h - lg.li->pad));
    lg.alpha = std::clamp(g_labelA.x, 0.0, 1.0);
    return lg;
}

// ---------------------------------------------------------------------------
// Fondo del Dock: cristal líquido con antialias analítico, en una pasada
// ---------------------------------------------------------------------------

static void DrawBackground(const Layout& L, const RECT& clip) {
    float x0 = (float)L.bgL, y0 = (float)L.bgT, w = (float)(L.bgR - L.bgL), h = (float)(L.bgB - L.bgT);
    if (w <= 1 || h <= 1 || !g_bits) return;
    if (g_set.bgOpacity <= 0.001) return;
    float r = std::min(h / 2, (float)(L.base * 0.38 + S(5))) * (float)g_set.cornerRadius;
    r = std::max(r, 0.5f);
    float cx = x0 + w / 2, cy = y0 + h / 2, hw = w / 2, hh = h / 2;
    bool dark = DarkUi();
    float sr = (float)S(18), sOff = (float)S(4);
    float shadowA = dark ? 0.34f : 0.20f;
    float fr = 0, fg = 0, fb = 0, fa = 1;
    bool glass = g_set.style == DockStyle::Glass && (int)g_glass.size() == g_winW * g_winH;
    switch (g_set.style) {
        case DockStyle::Glass:
            fa = glass ? 0.90f : (dark ? 0.62f : 0.66f);
            fr = fg = dark ? 34.f : 246.f;
            fb = dark ? 38.f : 248.f;
            break;
        case DockStyle::Translucent:
            fa = dark ? 0.56f : 0.60f;
            fr = fg = dark ? 30.f : 246.f;
            fb = dark ? 34.f : 248.f;
            break;
        case DockStyle::Dark:
            fa = 0.88f;
            fr = fg = 30.f;
            fb = 33.f;
            break;
        case DockStyle::Light:
            fa = 0.90f;
            fr = fg = 241.f;
            fb = 244.f;
            break;
    }
    float shineTop = dark ? 0.08f : 0.22f;
    float rimTop = dark ? 0.42f : 0.85f, rimBottom = dark ? 0.12f : 0.35f;
    float glowA = dark ? 0.07f : 0.12f;  // brillo interior junto al borde
    float outerA = dark ? 0.50f : 0.12f;
    {
        float op = (float)g_set.bgOpacity;
        fa *= op;
        shadowA *= op;
        shineTop *= op;
        rimTop *= op;
        rimBottom *= op;
        glowA *= op;
        outerA *= op;
    }

    int ix0 = std::max<int>(clip.left, (int)std::floor(x0 - sr - 2));
    int ix1 = std::min<int>(clip.right, (int)std::ceil(x0 + w + sr + 2));
    int iy0 = std::max<int>(clip.top, (int)std::floor(y0 - sr - 2));
    int iy1 = std::min<int>(clip.bottom, (int)std::ceil(y0 + h + sr + sOff + 2));
    ix0 = std::max(ix0, 0);
    iy0 = std::max(iy0, 0);
    ix1 = std::min(ix1, g_winW);
    iy1 = std::min(iy1, g_winH);
    const float glowW = (float)S(5);
    const float inner = glowW + 2.5f;  // más adentro de esto: solo relleno
    // Entre los centros de las esquinas la distancia depende solo de la fila.
    const float midL = cx - (hw - r), midR = cx + (hw - r);

    auto shade = [&](uint32_t* dst, float px, float d, float ds, float shine, float rimV, uint32_t gp) {
        if (d > -1.0f) {
            float t = std::clamp(1.0f - ds / sr, 0.0f, 1.0f);
            float sa = shadowA * t * t * t * std::clamp(d + 1.0f, 0.0f, 1.0f);
            if (sa > 0.002f) BlendOver(dst, sa, 0, 0, 0);
        }
        float cov = std::clamp(0.5f - d, 0.0f, 1.0f);
        if (cov <= 0.0f) {
            float oc = outerA * std::clamp(1.0f - std::fabs(d - 0.5f), 0.0f, 1.0f);
            if (oc > 0.002f) BlendOver(dst, oc, 0, 0, 0);
            return;
        }
        float sr8 = fr, sg8 = fg, sb8 = fb;
        if (glass) {
            sr8 = (float)((gp >> 16) & 0xFF);
            sg8 = (float)((gp >> 8) & 0xFF);
            sb8 = (float)(gp & 0xFF);
        }
        float glow = glowA * std::clamp(1.0f + d / glowW, 0.0f, 1.0f);
        float lift = std::min(1.0f, shine + glow);
        sr8 += (255 - sr8) * lift;
        sg8 += (255 - sg8) * lift;
        sb8 += (255 - sb8) * lift;
        float a = std::min(1.0f, fa + (1 - fa) * lift) * cov;
        BlendOver(dst, a, sr8 * a, sg8 * a, sb8 * a);
        float hx = 1.0f - 0.35f * std::fabs((px - cx) / hw);
        float rc = rimV * hx * std::clamp(1.0f - std::fabs(d + 1.0f), 0.0f, 1.0f) * cov;
        if (rc > 0.002f) BlendOver(dst, rc, 255 * rc, 255 * rc, 255 * rc);
        float oc = outerA * std::clamp(1.0f - std::fabs(d - 0.5f), 0.0f, 1.0f) * (1.0f - cov);
        if (oc > 0.002f) BlendOver(dst, oc, 0, 0, 0);
    };

    for (int y = iy0; y < iy1; y++) {
        float py = y + 0.5f;
        float vt = std::clamp((py - y0) / h, 0.0f, 1.0f);
        float shine = shineTop * std::max(0.0f, 1.0f - vt / 0.55f);
        float rimV = rimTop + (rimBottom - rimTop) * vt;
        uint32_t* row = g_bits + (size_t)y * g_winW;
        const uint32_t* grow = glass ? &g_glass[(size_t)y * g_winW] : nullptr;
        // Distancias de esta fila en el tramo recto (sin raíces).
        float dMid = std::fabs(py - cy) - hh;
        float dsMid = std::fabs(py - (cy + sOff)) - hh;
        // Tramo interior: todo relleno, se escribe directo.
        int inL = 1, inR = 0;
        if (dMid <= -inner) {
            float hwI = hw - inner, hhI = hh - inner, rI = std::max(0.0f, r - inner);
            float qy = std::fabs(py - cy) - (hhI - rI);
            float half = qy <= 0 ? hwI : (qy < rI ? hwI - rI + std::sqrt(rI * rI - qy * qy) : -1);
            if (half > 0) {
                inL = (int)std::ceil(cx - half - 0.5f);
                inR = (int)std::floor(cx + half - 0.5f);
            }
        }
        float aIn = std::min(1.0f, fa + (1 - fa) * shine);
        uint32_t aIn8 = (uint32_t)std::lround(aIn * 255);
        uint32_t shine256 = (uint32_t)std::lround(shine * 256);
        uint32_t solid = 0;
        if (!glass) {
            float r8 = fr + (255 - fr) * shine, g8 = fg + (255 - fg) * shine, b8 = fb + (255 - fb) * shine;
            solid = (aIn8 << 24) | ((uint32_t)std::lround(r8 * aIn) << 16) | ((uint32_t)std::lround(g8 * aIn) << 8) |
                    (uint32_t)std::lround(b8 * aIn);
        }
        for (int x = ix0; x < ix1; x++) {
            uint32_t* dst = row + x;
            if (x >= inL && x <= inR) {
                uint32_t c;
                if (glass) {
                    uint32_t gp = grow[x];
                    // gp + (blanco - gp) * brillo, luego por la opacidad
                    uint32_t lifted = gp + Scale32(~gp & 0x00FFFFFFu, shine256);
                    c = (Scale32(lifted, aIn8 + 1) & 0x00FFFFFFu) | (aIn8 << 24);
                } else {
                    c = solid;
                }
                *dst = Over32(c, *dst);
                continue;
            }
            float px = x + 0.5f;
            float d, ds;
            if (px >= midL && px <= midR) {
                d = dMid;
                ds = dsMid;
            } else {
                d = RoundRectSdf(px, py, cx, cy, hw, hh, r);
                ds = d > -1.0f ? RoundRectSdf(px, py, cx, cy + sOff, hw, hh, r) : 0;
            }
            if (d > sr + 2 && ds > sr) continue;
            shade(dst, px, d, ds, shine, rimV, grow ? grow[x] : 0);
        }
    }
}

// ---------------------------------------------------------------------------
// Dibujo de un cuadro
// ---------------------------------------------------------------------------

static RECT RectOf(double l, double t, double r, double b) {
    return RECT{(LONG)std::floor(l), (LONG)std::floor(t), (LONG)std::ceil(r), (LONG)std::ceil(b)};
}

static void UnionInto(RECT& acc, const RECT& r) {
    if (r.right <= r.left || r.bottom <= r.top) return;
    if (acc.right <= acc.left || acc.bottom <= acc.top) {
        acc = r;
        return;
    }
    UnionRect(&acc, &acc, &r);
}

// La ventana ocupa solo lo que puede llegar a ocupar el Dock (con la
// ampliación máxima y el nombre), no todo el ancho de la pantalla.
static bool UpdateEnvelope(const Layout& L, const LabelGeo& lg, double now) {
    double delta = g_set.magnify ? (L.maxS - L.base) * (0.6 + 0.6 * g_set.magRange) : 0;
    double l = std::min(L.baseL - delta, L.bgL) - S(30);
    double r = std::max(L.baseR + delta, L.bgR) + S(30);
    if (lg.li) {
        l = std::min(l, (double)lg.x);
        r = std::max(r, (double)(lg.x + lg.li->img.w));
    }
    int nl = std::clamp((int)std::floor(l / 32) * 32, 0, g_winW);
    int nr = std::clamp((int)std::ceil(r / 32) * 32, 0, g_winW);
    if (nr - nl < 64) {
        nl = std::max(0, nl - 32);
        nr = std::min(g_winW, nr + 32);
    }
    bool change = false;
    if (nl < g_envL || nr > g_envR || g_envR <= g_envL) {
        change = true;
        g_envShrinkSince = -1;
    } else if ((nl - g_envL) + (g_envR - nr) > S(160)) {
        if (g_envShrinkSince < 0) g_envShrinkSince = now;
        if (now - g_envShrinkSince > 0.8 && !g_hover) change = true;
    } else {
        g_envShrinkSince = -1;
    }
    if (change) {
        g_envL = nl;
        g_envR = nr;
        g_envShrinkSince = -1;
    }
    return change;
}

static void Render(double now) {
    HWND hwnd = g_wnd.load();
    if (!hwnd || !g_bits) return;
    Layout L = ComputeLayout(now);
    LabelGeo lg = ComputeLabel(L);
    g_lastLayout = L;
    if (UpdateEnvelope(L, lg, now)) g_fullRedraw = true;
    RECT win{g_envL, 0, g_envR, g_winH};

    RECT cur{};
    UnionInto(cur, RectOf(L.bgL - S(22), L.bgT - S(22), L.bgR + S(22), g_winH));
    UnionInto(cur, RectOf(L.bgL - S(6), L.topMost - S(8), L.bgR + S(6), g_winH));
    if (lg.li) UnionInto(cur, RectOf(lg.x, lg.y, lg.x + lg.li->img.w, lg.y + lg.li->img.h));
    for (size_t i = 0; i < g_items.size(); i++) {
        auto& it = *g_items[i];
        const Geo& g = L.g[i];
        if (!g.shown) continue;
        double fx = it.flyX.x, fy = it.flyY.x;
        double pad = g.size * 0.2 + 3;
        UnionInto(cur, RectOf(g.x + fx - pad, g.y + fy - pad, g.x + fx + g.size + pad, g.y + fy + g.size + pad));
    }
    IntersectRect(&cur, &cur, &win);
    RECT dirty = cur;
    UnionInto(dirty, g_prevBounds);
    if (g_fullRedraw) dirty = win;
    IntersectRect(&dirty, &dirty, &win);
    g_prevBounds = cur;
    if (IsRectEmpty(&dirty) && !g_fullRedraw) return;

    for (int y = dirty.top; y < dirty.bottom; y++) {
        memset(g_bits + (size_t)y * g_winW + dirty.left, 0, (size_t)(dirty.right - dirty.left) * 4);
    }
    Surface s{g_bits, g_winW, g_winH};

    // Zona casi invisible (alfa 1) para que el mouse no "caiga" entre los
    // iconos ampliados ni en el borde de abajo.
    {
        double top = (g_hover || g_mag.x > 0.02) ? L.topMost - S(4) : L.bgB;
        int hx0 = std::max<int>(dirty.left, (int)std::floor(L.bgL)), hx1 = std::min<int>(dirty.right, (int)std::ceil(L.bgR));
        int hy0 = std::max<int>(dirty.top, (int)std::floor(top)), hy1 = dirty.bottom;
        for (int y = hy0; y < hy1; y++)
            for (int x = hx0; x < hx1; x++) g_bits[(size_t)y * g_winW + x] = 0x01000000u;
    }

    DrawBackground(L, dirty);

    bool dark = DarkUi();
    for (size_t i = 0; i < g_items.size(); i++) {
        auto& it = *g_items[i];
        const Geo& ge = L.g[i];
        if (!ge.shown) continue;
        if (it.kind == Kind::Sep) {
            // Línea fina que se desvanece en las puntas
            double h = L.base * 0.74, x = ge.cx, yb = L.bgB - (L.bgB - L.bgT - h) / 2;
            int ix = (int)std::floor(x);
            float frac = (float)(x - ix);
            for (int y = std::max<int>(dirty.top, (int)(yb - h)); y < std::min<int>(dirty.bottom, (int)yb); y++) {
                double t = (y + 0.5 - (yb - h)) / h;
                float a = (float)((dark ? 0.32 : 0.22) * ge.alpha * std::sin(kPi * std::clamp(t, 0.0, 1.0)));
                int c = dark ? 255 : 0;
                for (int k = 0; k < 2; k++) {
                    int xx = ix + k;
                    if (xx < dirty.left || xx >= dirty.right) continue;
                    float cov = (k == 0 ? 1.0f - frac : frac) * a;
                    BlendOver(g_bits + (size_t)y * g_winW + xx, cov, c * cov, c * cov, c * cov);
                }
            }
            continue;
        }
        const IconSet* set = IconFor(it);
        double x = ge.x + it.flyX.x, y = ge.y + it.flyY.x, size = ge.size;
        // Resaltado al soltar algo encima: un poco más grande y claro.
        double hl = std::clamp(it.hilite.x, 0.0, 1.0);
        if (hl > 0.001) {
            double grow = size * 0.08 * hl;
            x -= grow / 2;
            y -= grow;
            size += grow;
        }
        if (set) {
            double bright = (&it == g_press && !g_drag) ? 0.62 : 1.0;
            if (g_fileDrag && &it == g_fileTarget) bright = 0.7;
            BlitIcon(s, dirty, *set, x, y, size, ge.alpha, bright, g_set.shadows);
        }
        if (g_set.indicators && it.kind == Kind::App && it.dot.x > 0.01) {
            double r = std::max(1.6, S(2.2) * L.base / g_m.base);
            double cy = L.bgB - std::max(r + 1.5, L.pad / 2);
            double a = it.dot.x * ge.alpha;
            int R = dark ? 255 : 20, G = dark ? 255 : 20, B = dark ? 255 : 24;
            double core = dark ? 0.92 : 0.66;
            auto dot = [&](double x) {
                if (dark) FillCircle(s, dirty, x, cy, r + 1.6, 0.18 * a, 255, 255, 255);  // halo
                FillCircle(s, dirty, x, cy, r, core * a, R, G, B);
            };
            if (g_set.indicator == Indicator::Line) {
                RoundStyle st;
                st.fillA = (float)(core * a);
                st.fr = (float)R;
                st.fg = (float)G;
                st.fb = (float)B;
                double lw = std::max(S(10), (int)(ge.size * 0.32)), lh = std::max(2.0, r * 1.1);
                FillRoundRect(s, dirty, (float)(ge.cx - lw / 2), (float)(cy - lh / 2), (float)lw, (float)lh,
                              (float)(lh / 2), st);
            } else if (g_set.indicator == Indicator::Dots && it.windows.size() > 1) {
                int n = (int)std::min<size_t>(3, it.windows.size());
                double step = r * 3.2;
                for (int k = 0; k < n; k++) dot(ge.cx + (k - (n - 1) / 2.0) * step);
            } else {
                dot(ge.cx);
            }
        }
    }
    if (lg.li) BlitImage(s, dirty, lg.li->img, lg.x, lg.y, lg.alpha);
    GdiFlush();

    POINT dst{g_winX + g_envL, g_winY};
    SIZE size{g_envR - g_envL, g_winH};
    POINT src{g_envL, 0};
    BLENDFUNCTION bf{AC_SRC_OVER, 0, 255, AC_SRC_ALPHA};
    RECT rel{dirty.left - g_envL, dirty.top, dirty.right - g_envL, dirty.bottom};
    UPDATELAYEREDWINDOWINFO info{};
    info.cbSize = sizeof(info);
    info.hdcSrc = g_memDC;
    info.pptDst = &dst;
    info.psize = &size;
    info.pptSrc = &src;
    info.pblend = &bf;
    info.dwFlags = ULW_ALPHA;
    info.prcDirty = g_fullRedraw ? nullptr : &rel;
    if (!UpdateLayeredWindowIndirect(hwnd, &info)) {
        g_dbgUlwFail++;
        info.prcDirty = nullptr;
        if (!UpdateLayeredWindowIndirect(hwnd, &info)) {
            // Que al menos la ventana quede donde el Dock cree que está.
            SetWindowPos(hwnd, nullptr, dst.x, dst.y, size.cx, size.cy,
                         SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOOWNERZORDER);
        }
    }
    g_fullRedraw = false;
}

// ---------------------------------------------------------------------------
// Ventanita que sigue al cursor al arrastrar un icono (por toda la pantalla)
// ---------------------------------------------------------------------------

struct Puff {
    double x, y, size, t0;
};

static HDC g_dragDC = nullptr;
static HBITMAP g_dragDib = nullptr;
static HGDIOBJ g_dragOld = nullptr;
static uint32_t* g_dragBits = nullptr;
static int g_dragW = 0, g_dragH = 0;
static int g_dragState = -1;
static bool g_dragPuffOn = false;
static Puff g_dragPuff{};

static void FreeDragBuffers() {
    if (g_dragDC) {
        if (g_dragOld) SelectObject(g_dragDC, g_dragOld);
        DeleteDC(g_dragDC);
    }
    if (g_dragDib) DeleteObject(g_dragDib);
    g_dragDC = nullptr;
    g_dragDib = nullptr;
    g_dragOld = nullptr;
    g_dragBits = nullptr;
    g_dragW = g_dragH = 0;
}

static bool EnsureDragWindow(int w, int h) {
    if (!g_dragWnd) {
        g_dragWnd = CreateWindowExW(WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW | WS_EX_TOPMOST | WS_EX_NOACTIVATE,
                                    kDragClass, L"", WS_POPUP, 0, 0, 1, 1, nullptr, nullptr,
                                    GetModuleHandleW(nullptr), nullptr);
        if (!g_dragWnd) return false;
    }
    if (w == g_dragW && h == g_dragH && g_dragBits) return true;
    FreeDragBuffers();
    BITMAPINFO bi{};
    bi.bmiHeader.biSize = sizeof(bi.bmiHeader);
    bi.bmiHeader.biWidth = w;
    bi.bmiHeader.biHeight = -h;
    bi.bmiHeader.biPlanes = 1;
    bi.bmiHeader.biBitCount = 32;
    bi.bmiHeader.biCompression = BI_RGB;
    void* bits = nullptr;
    g_dragDC = CreateCompatibleDC(nullptr);
    g_dragDib = CreateDIBSection(g_dragDC, &bi, DIB_RGB_COLORS, &bits, nullptr, 0);
    if (!g_dragDC || !g_dragDib || !bits) {
        FreeDragBuffers();
        return false;
    }
    g_dragOld = SelectObject(g_dragDC, g_dragDib);
    g_dragBits = (uint32_t*)bits;
    g_dragW = w;
    g_dragH = h;
    g_dragState = -1;
    return true;
}

static void PresentDragWindow(int x, int y) {
    POINT dst{x, y};
    SIZE size{g_dragW, g_dragH};
    POINT src{0, 0};
    BLENDFUNCTION bf{AC_SRC_OVER, 0, 255, AC_SRC_ALPHA};
    UpdateLayeredWindow(g_dragWnd, nullptr, &dst, &size, g_dragDC, &src, 0, &bf, ULW_ALPHA);
    if (!IsWindowVisible(g_dragWnd)) {
        SetWindowPos(g_dragWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_SHOWWINDOW);
    }
}

static void HideDragWindow() {
    if (g_dragWnd) ShowWindow(g_dragWnd, SW_HIDE);
    g_dragState = -1;
    g_dragPuffOn = false;
}

static void UpdateDragWindow(double now) {
    if (g_drag) {
        int size = (int)std::ceil(g_dragSize);
        int labelH = S(44);
        int w = std::max(size + S(20), S(260));
        int h = size + labelH + S(10);
        if (!EnsureDragWindow(w, h)) return;
        int state = g_dragRemove ? 1 : 0;
        int iconX = (w - size) / 2, iconY = h - size - S(6);
        if (state != g_dragState) {
            memset(g_dragBits, 0, (size_t)w * h * 4);
            Surface s{g_dragBits, w, h};
            RECT all{0, 0, w, h};
            const IconSet* set = IconFor(*g_drag);
            if (set) BlitIcon(s, all, *set, iconX, iconY, size, g_dragRemove ? 0.5 : 0.95, 1, g_set.shadows);
            if (g_dragRemove && g_set.labels) {
                const LabelImg* li = GetLabelImg(L"Quitar del Dock");
                if (li) BlitImage(s, all, li->img, (w - li->img.w) / 2, iconY - S(8) - (li->img.h - li->pad), 1.0);
            }
            GdiFlush();
            g_dragState = state;
        }
        int sx = g_winX + (int)std::lround(g_mouseX - g_dragGrabX * g_dragSize) - iconX;
        int sy = g_winY + (int)std::lround(g_mouseY - g_dragGrabY * g_dragSize) - iconY;
        PresentDragWindow(sx, sy);
        return;
    }
    if (g_dragPuffOn) {
        double t = (now - g_dragPuff.t0) / 0.42;
        if (t >= 1) {
            HideDragWindow();
            return;
        }
        int w = (int)std::ceil(g_dragPuff.size * 2);
        if (!EnsureDragWindow(w, w)) return;
        memset(g_dragBits, 0, (size_t)w * w * 4);
        Surface s{g_dragBits, w, w};
        RECT all{0, 0, w, w};
        double c = w / 2.0, sz = g_dragPuff.size;
        double e = 1 - (1 - t) * (1 - t);  // desacelera
        for (int k = 0; k < 7; k++) {
            double ang = k * 2 * kPi / 7 + 0.4;
            double dist = sz * 0.16 + sz * 0.30 * e;
            double rr = sz * (0.13 + 0.14 * e);
            FillCircle(s, all, c + std::cos(ang) * dist, c + std::sin(ang) * dist, rr, 0.85 * (1 - t), 236, 236, 240);
        }
        GdiFlush();
        g_dragState = -1;
        PresentDragWindow((int)std::lround(g_dragPuff.x - w / 2.0), (int)std::lround(g_dragPuff.y - w / 2.0));
    }
}

// ---------------------------------------------------------------------------
// Animación por cuadro
// ---------------------------------------------------------------------------

static void ClearRefs(DockItem* it) {
    if (g_press == it) g_press = nullptr;
    if (g_drag == it) g_drag = nullptr;
    if (g_labelItem == it) g_labelItem = nullptr;
    if (g_insertBefore == it) g_insertBefore = nullptr;
    if (g_fileTarget == it) g_fileTarget = nullptr;
    if (g_menuItem == it) g_menuItem = nullptr;
}

static void ReleaseForward(DockItem* it);

static bool StepAnimations(double dt, double now) {
    bool active = false;
    double sp = g_set.speed;
    auto step = [&](Spring& s, double resp, double damp, double eps) {
        s.Step(dt, resp * sp, damp, eps);
        if (!s.settled) active = true;
    };
    step(g_anchor, 0.05, 1.0, 0.05);
    step(g_mag, 0.24, 1.0, 0.0008);
    step(g_hide, 0.34, 1.0, 0.0008);
    step(g_labelA, 0.13, 1.0, 0.01);
    step(g_labelX, 0.11, 1.0, 0.1);
    for (auto& p : g_items) {
        auto& it = *p;
        step(it.presence, 0.40, Bouncy(), 0.002);
        step(it.slot, 0.30, 1.0, 0.002);
        step(it.gap, 0.26, 1.0, 0.002);
        step(it.dot, 0.30, 1.0, 0.01);
        step(it.hilite, 0.16, 1.0, 0.01);
        step(it.flyX, 0.34, Bouncy(), 0.3);
        step(it.flyY, 0.34, Bouncy(), 0.3);
        if (it.launching && now > it.launchUntil) {
            it.launching = false;
            it.bounceCycles = 0;
        }
        if (it.bounceStart >= 0) {
            if (!it.launching && it.bounceCycles <= 0) {
                double period = 0.58 * g_set.speed;
                it.bounceCycles = std::max(1, (int)std::ceil((now - it.bounceStart) / period));
            }
            active = true;
        }
    }
    if (g_dragPuffOn) active = true;
    for (size_t i = 0; i < g_items.size();) {
        auto* it = g_items[i].get();
        if (it->removing && it->presence.settled && it->presence.x <= 0.001) {
            ReleaseForward(it);
            ClearRefs(it);
            g_items.erase(g_items.begin() + i);
            continue;
        }
        i++;
    }
    return active;
}

static void UpdateLabelTarget(bool snap);

static void OnFrame(LONGLONG vblankQpc) {
    g_framePosted = false;
    double t = vblankQpc ? (double)vblankQpc / g_qpcFreq : Now();
    double dt = std::clamp(t - g_lastFrame, 0.0, 0.05);
    g_lastFrame = t;
    // El mouse se lee justo al dibujar: lo más fresco posible.
    if (g_hover && !g_menuOpen && !g_fileDrag) {
        POINT pt;
        if (GetCursorPos(&pt) && !CursorInPopup(pt)) {
            g_mouseX = pt.x - g_winX;
            g_mouseY = pt.y - g_winY;
            g_anchor.SetTarget(g_mouseX);
        }
    }
    bool active = StepAnimations(dt, t);
    if (StepPopup(dt)) active = true;
    UpdateLabelTarget(false);
    Render(t);
    if (g_drag || g_dragPuffOn) UpdateDragWindow(t);
    if (active) g_lastActive = t;
    // Se sigue al ritmo del monitor un momento más, para que un nuevo
    // movimiento arranque sin esperar.
    if (!active && t - g_lastActive > 0.35) g_vsyncRun = false;
}

// ---------------------------------------------------------------------------
// Aplicar el listado de ventanas
// ---------------------------------------------------------------------------

static void StartBounce(DockItem& it, double amp, int cycles, bool launching) {
    double now = Now();
    if (it.bounceStart < 0) it.bounceStart = now;
    it.bounceAmp = amp;
    it.launching = launching;
    if (launching) {
        it.launchUntil = now + 3.5;  // como mucho unos seis saltos
        it.bounceCycles = 0;
    } else {
        double period = 0.58 * g_set.speed;
        it.bounceCycles = (int)std::floor((now - it.bounceStart) / period) + cycles;
    }
    Kick();
}

static void ApplyWindows(const std::vector<WinInfo>& wins) {
    std::map<DockItem*, size_t> before;
    for (auto& it : g_items) {
        before[it.get()] = it->windows.size();
        it->windows.clear();
        it->pids.clear();
    }
    bool changed = false;
    for (auto& w : wins) {
        DockItem* match = nullptr;
        for (int pass = 0; pass < 2 && !match; pass++) {
            for (auto& it : g_items) {
                if (ItemMatches(*it, w, pass == 0)) {
                    match = it.get();
                    break;
                }
            }
        }
        if (!match && !g_set.showUnpinned) continue;
        if (!match) {
            std::unique_ptr<DockItem> it =
                w.packaged ? MakeApp('A', w.aumid, w.aumid, w.exe) : MakeApp('E', w.exe, w.aumid, w.exe);
            it->pinned = false;
            it->presence.Snap(0);
            it->presence.SetTarget(1);
            it->dot.Snap(0);
            match = it.get();
            RequestIcon(*it);
            g_items.insert(g_items.begin() + AppsEnd(), std::move(it));
            changed = true;
        }
        match->windows.push_back(w.h);
        if (w.pid && std::find(match->pids.begin(), match->pids.end(), w.pid) == match->pids.end()) {
            match->pids.push_back(w.pid);
        }
    }
    for (auto& p : g_items) {
        auto& it = *p;
        if (it.kind != Kind::App) continue;
        bool open = !it.windows.empty();
        it.dot.SetTarget(open ? 1 : 0);
        if (!it.dot.settled) changed = true;
        if (open && it.launching) {
            it.launching = false;
            it.bounceCycles = 0;
            changed = true;
        }
        if (!it.pinned && !open && !it.removing && &it != g_drag) {
            it.removing = true;
            it.presence.SetTarget(0);
            changed = true;
        }
        auto b = before.find(&it);
        if (b != before.end() && b->second != it.windows.size()) changed = true;
    }
    if (changed) Kick();
}

static void TakeScanResult();

// ---------------------------------------------------------------------------
// Fotos de ventanas: Windows no dibuja en vivo las ventanas minimizadas, así
// que se guarda una foto de cada una mientras está a la vista y la vista de
// ventanas la muestra cuando está minimizada.
// ---------------------------------------------------------------------------

#ifndef PW_RENDERFULLCONTENT
#define PW_RENDERFULLCONTENT 0x00000002
#endif

struct Snapshot {
    std::vector<Img> mips;  // con borde de 1 px (para BlitScaled)
    int w = 0, h = 0;       // tamaño visible de la ventana al tomarla
};

static std::map<HWND, std::shared_ptr<Snapshot>> g_snaps;  // hilo del Dock
static std::mutex g_snapMx;
struct SnapJob {
    HWND h = nullptr;
    bool minimizeAfter = false;
};
static std::deque<SnapJob> g_snapJobs;
static std::vector<std::pair<HWND, std::shared_ptr<Snapshot>>> g_snapDone;
static HANDLE g_snapEvent = nullptr;
static HANDLE g_snapThread = nullptr;
static std::atomic<bool> g_snapStop{false};

// Foto de una ventana visible, reducida a un tamaño de miniatura. nullptr si
// no se pudo (minimizada, colgada o salió en negro).
static std::shared_ptr<Snapshot> CaptureWindow(HWND h) {
    if (!IsWindow(h) || IsIconic(h) || !IsWindowVisible(h) || IsHungAppWindow(h)) return nullptr;
    RECT wr{}, fr{};
    if (!GetWindowRect(h, &wr)) return nullptr;
    if (FAILED(DwmGetWindowAttribute(h, DWMWA_EXTENDED_FRAME_BOUNDS, &fr, sizeof(fr)))) fr = wr;
    int ww = wr.right - wr.left, wh = wr.bottom - wr.top;
    if (ww < 16 || wh < 16 || ww > 8192 || wh > 8192) return nullptr;
    BITMAPINFO bi{};
    bi.bmiHeader.biSize = sizeof(bi.bmiHeader);
    bi.bmiHeader.biWidth = ww;
    bi.bmiHeader.biHeight = -wh;
    bi.bmiHeader.biPlanes = 1;
    bi.bmiHeader.biBitCount = 32;
    bi.bmiHeader.biCompression = BI_RGB;
    void* bits = nullptr;
    HDC dc = CreateCompatibleDC(nullptr);
    HBITMAP dib = dc ? CreateDIBSection(dc, &bi, DIB_RGB_COLORS, &bits, nullptr, 0) : nullptr;
    std::shared_ptr<Snapshot> out;
    if (dib && bits) {
        HGDIOBJ old = SelectObject(dc, dib);
        // Primero como la compone Windows (apps modernas); si sale en negro,
        // pidiéndole a la app que se dibuje (apps clásicas).
        auto blackish = [&] {
            const uint32_t* p = (const uint32_t*)bits;
            size_t n = (size_t)ww * wh, step = std::max<size_t>(1, n / 4096), dark = 0, total = 0;
            for (size_t i = 0; i < n; i += step, total++) dark += (p[i] & 0xFFFFFF) == 0;
            return dark >= total * 97 / 100;
        };
        BOOL ok = FALSE;
        for (UINT flags : {(UINT)PW_RENDERFULLCONTENT, 0u}) {
            memset(bits, 0, (size_t)ww * wh * 4);
            ok = PrintWindow(h, dc, flags);
            GdiFlush();
            if (ok && !blackish()) break;
            ok = FALSE;
        }
        if (!ok && h == GetForegroundWindow()) {
            // Último recurso: lo que se ve en pantalla (está al frente).
            HDC screen = GetDC(nullptr);
            if (screen) {
                memset(bits, 0, (size_t)ww * wh * 4);
                ok = BitBlt(dc, 0, 0, ww, wh, screen, wr.left, wr.top, SRCCOPY | CAPTUREBLT);
                GdiFlush();
                ReleaseDC(nullptr, screen);
                if (ok && blackish()) ok = FALSE;
            }
        }
        SelectObject(dc, old);
        if (ok) {
            // Solo la parte visible (sin los bordes invisibles de redimensionar).
            int cx = std::clamp((int)(fr.left - wr.left), 0, ww - 1), cy = std::clamp((int)(fr.top - wr.top), 0, wh - 1);
            int cw = std::clamp((int)(fr.right - fr.left), 1, ww - cx), ch = std::clamp((int)(fr.bottom - fr.top), 1, wh - cy);
            Img full;
            full.w = cw;
            full.h = ch;
            full.px.resize((size_t)cw * ch);
            const uint32_t* src = (const uint32_t*)bits;
            size_t dark = 0;
            for (int y = 0; y < ch; y++) {
                for (int x = 0; x < cw; x++) {
                    uint32_t p = src[(size_t)(cy + y) * ww + cx + x] | 0xFF000000u;
                    full.px[(size_t)y * cw + x] = p;
                    if ((p & 0xFFFFFF) == 0) dark++;
                }
            }
            if (dark < full.px.size() * 97 / 100) {
                // Al doble del tamaño de la miniatura, para que se vea nítida.
                double k = std::min(1.0, std::min(440.0 * g_dpi / 96 / cw, 270.0 * g_dpi / 96 / ch));
                int tw = std::max(1, (int)std::lround(cw * k)), th = std::max(1, (int)std::lround(ch * k));
                Img top = Resample(full, tw, th);
                out = std::make_shared<Snapshot>();
                out->w = cw;
                out->h = ch;
                out->mips.push_back(Bordered(top));
                Img half = top;
                while (half.w > 64 && half.h > 64) {
                    half = Half(half);
                    out->mips.push_back(Bordered(half));
                }
            }
        }
    }
    if (dib) DeleteObject(dib);
    if (dc) DeleteDC(dc);
    return out;
}

static DWORD WINAPI SnapThread(LPVOID) {
    while (!g_snapStop) {
        SnapJob job;
        {
            std::lock_guard<std::mutex> lock(g_snapMx);
            if (!g_snapJobs.empty()) {
                job = g_snapJobs.front();
                g_snapJobs.pop_front();
            }
        }
        HWND h = job.h;
        if (!h) {
            WaitForSingleObject(g_snapEvent, INFINITE);
            continue;
        }
        auto snap = CaptureWindow(h);
        if (job.minimizeAfter && IsWindow(h) && !IsIconic(h)) {
            if (!PostMessageW(h, WM_SYSCOMMAND, SC_MINIMIZE, 0)) ShowWindowAsync(h, SW_MINIMIZE);
        }
        if (!snap) continue;
        {
            std::lock_guard<std::mutex> lock(g_snapMx);
            g_snapDone.emplace_back(h, snap);
        }
        HWND d = g_wnd.load();
        if (d) PostMessageW(d, WM_APP_SNAP, 0, 0);
    }
    return 0;
}

static bool IsDockAppWindow(HWND h) {
    if (!h) return false;
    for (auto& it : g_items) {
        if (it->kind != Kind::App) continue;
        for (HWND w : it->windows) {
            if (w == h) return true;
        }
    }
    return false;
}

// Pide una foto (en el hilo de fotos) de una ventana del Dock.
static bool QueueSnapshot(HWND h, bool minimizeAfter = false) {
    if (g_set.previews == Previews::Off || !g_snapEvent || !IsDockAppWindow(h) || IsIconic(h)) return false;
    {
        std::lock_guard<std::mutex> lock(g_snapMx);
        for (auto& j : g_snapJobs) {
            if (j.h == h) {
                j.minimizeAfter = j.minimizeAfter || minimizeAfter;
                SetEvent(g_snapEvent);
                return true;
            }
        }
        g_snapJobs.push_back({h, minimizeAfter});
    }
    SetEvent(g_snapEvent);
    return true;
}

static void TakeSnapshots() {
    std::vector<std::pair<HWND, std::shared_ptr<Snapshot>>> done;
    {
        std::lock_guard<std::mutex> lock(g_snapMx);
        done.swap(g_snapDone);
    }
    for (auto& d : done) {
        if (IsWindow(d.first)) g_snaps[d.first] = d.second;
    }
}

// Las fotos de ventanas que ya no existen se tiran.
static void PruneSnapshots() {
    for (auto it = g_snaps.begin(); it != g_snaps.end();) {
        it = IsWindow(it->first) ? std::next(it) : g_snaps.erase(it);
    }
}

static std::shared_ptr<Snapshot> SnapshotOf(HWND h) {
    auto it = g_snaps.find(h);
    return it == g_snaps.end() ? nullptr : it->second;
}

// ---------------------------------------------------------------------------
// Acciones
// ---------------------------------------------------------------------------

struct LaunchJob {
    std::wstring file, params, dir;
    uint64_t uid = 0;
};

static DWORD WINAPI LaunchThread(LPVOID p) {
    std::unique_ptr<LaunchJob> job((LaunchJob*)p);
    CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    SHELLEXECUTEINFOW sei{sizeof(sei)};
    sei.fMask = SEE_MASK_FLAG_NO_UI | SEE_MASK_NOASYNC;
    sei.lpFile = job->file.c_str();
    sei.lpParameters = job->params.empty() ? nullptr : job->params.c_str();
    sei.lpDirectory = job->dir.empty() ? nullptr : job->dir.c_str();
    sei.nShow = SW_SHOWNORMAL;
    BOOL ok = ShellExecuteExW(&sei);
    if (sei.hProcess) CloseHandle(sei.hProcess);
    CoUninitialize();
    if (!ok) {
        HWND h = g_wnd.load();
        if (h) PostMessageW(h, WM_APP_LAUNCHFAIL, 0, (LPARAM)job->uid);
    }
    return 0;
}

static void ShellOpenAsync(const std::wstring& file, const std::wstring& params = L"",
                           const std::wstring& dir = L"", uint64_t uid = 0) {
    AllowSetForegroundWindow(ASFW_ANY);
    auto* job = new LaunchJob{file, params, dir, uid};
    HANDLE t = CreateThread(nullptr, 0, LaunchThread, job, 0, nullptr);
    if (t) CloseHandle(t);
    else delete job;
}

static std::wstring LaunchTarget(const DockItem& it) {
    return it.type == 'A' ? L"shell:AppsFolder\\" + it.value : it.value;
}

static void LaunchItem(DockItem& it, bool bounce) {
    std::wstring dir;
    if (it.type == 'E') {
        size_t slash = it.value.find_last_of(L"\\/");
        if (slash != std::wstring::npos) dir = it.value.substr(0, slash);
    }
    ShellOpenAsync(LaunchTarget(it), L"", dir, it.uid);
    if (bounce && g_set.bounceLaunch) {
        if (it.windows.empty()) StartBounce(it, 0.38, 0, true);
        else StartBounce(it, 0.32, 1, false);
    }
    RequestScan();
}

// Traer una ventana al frente aunque el Dock no tenga el foco. Windows solo
// deja cambiar el primer plano al proceso que lo tiene; compartiendo por un
// instante la cola de entrada con la app activa (lo que hacen las barras de
// tareas alternativas), el cambio sí se permite.
static void ActivateWindow(HWND h) {
    if (!IsWindow(h)) return;
    if (IsIconic(h)) ShowWindow(h, SW_RESTORE);
    HWND fg = GetForegroundWindow();
    if (fg == h) return;
    DWORD me = GetCurrentThreadId();
    DWORD fgTid = fg ? GetWindowThreadProcessId(fg, nullptr) : 0;
    DWORD tgtTid = GetWindowThreadProcessId(h, nullptr);
    bool a1 = fgTid && fgTid != me && AttachThreadInput(me, fgTid, TRUE);
    bool a2 = tgtTid && tgtTid != me && tgtTid != fgTid && AttachThreadInput(me, tgtTid, TRUE);
    BringWindowToTop(h);
    BOOL ok = SetForegroundWindow(h);
    if (a2) AttachThreadInput(me, tgtTid, FALSE);
    if (a1) AttachThreadInput(me, fgTid, FALSE);
    if (!ok || GetForegroundWindow() != h) {
        // Último recurso: el mismo cambio que hace Alt+Tab.
        SwitchToThisWindow(h, TRUE);
    }
    Wh_Log(L"Activar %p: %s", h, GetForegroundWindow() == h ? L"ok" : L"no se pudo");
}

static void MinimizeWindow(HWND h) {
    if (!IsWindow(h) || IsIconic(h)) return;
    // Foto primero (en su hilo, que luego la minimiza). Una app colgada se
    // minimiza directo, sin esperar la foto.
    if (!IsHungAppWindow(h) && QueueSnapshot(h, true)) return;
    // WM_SYSCOMMAND respeta la animación y lo que la app haga al minimizar.
    if (!PostMessageW(h, WM_SYSCOMMAND, SC_MINIMIZE, 0)) ShowWindowAsync(h, SW_MINIMIZE);
}

static HWND RootOf(HWND h) {
    HWND r = GetAncestor(h, GA_ROOTOWNER);
    return r ? r : h;
}

static bool OwnsWindow(const DockItem& it, HWND h) {
    if (!h) return false;
    HWND root = RootOf(h);
    for (HWND w : it.windows) {
        if (w == h || w == root || RootOf(w) == root) return true;
    }
    return false;
}

static void SendKeys(std::initializer_list<WORD> keys) {
    std::vector<INPUT> in;
    for (WORD k : keys) {
        INPUT i{};
        i.type = INPUT_KEYBOARD;
        i.ki.wVk = k;
        in.push_back(i);
    }
    for (auto it = keys.end(); it != keys.begin();) {
        --it;
        INPUT i{};
        i.type = INPUT_KEYBOARD;
        i.ki.wVk = *it;
        i.ki.dwFlags = KEYEVENTF_KEYUP;
        in.push_back(i);
    }
    SendInput((UINT)in.size(), in.data(), sizeof(INPUT));
}

static void OnItemClick(DockItem& it, bool newInstance) {
    ClosePopup(true);
    g_prevSuppress = it.uid;
    g_prevCand = nullptr;
    Wh_Log(L"Clic en %s (%u ventanas)", it.name.c_str(), (unsigned)it.windows.size());
    switch (it.kind) {
        case Kind::Start:
            SendKeys({VK_LWIN});
            return;
        case Kind::Trash:
            ShellOpenAsync(L"shell:RecycleBinFolder");
            return;
        case Kind::App:
            break;
        default:
            return;
    }
    if (newInstance || it.windows.empty()) {
        LaunchItem(it, true);
        return;
    }
    HWND fg = GetForegroundWindow();
    bool anyVisible = std::any_of(it.windows.begin(), it.windows.end(), [](HWND w) { return !IsIconic(w); });
    if (!anyVisible) {
        ActivateWindow(it.windows.front());
        return;
    }
    if (OwnsWindow(it, fg) && !IsIconic(fg)) {
        if (g_set.clickMinimizes) {
            for (HWND w : it.windows) MinimizeWindow(w);
        } else {
            ActivateWindow(it.windows.back());
        }
        return;
    }
    ActivateWindow(it.windows.front());
}

static void MoveItem(DockItem* item, int newIndex) {
    int from = IndexOf(item);
    if (from < 0) return;
    auto holder = std::move(g_items[from]);
    g_items.erase(g_items.begin() + from);
    if (newIndex > from) newIndex--;
    newIndex = std::clamp(newIndex, 0, (int)g_items.size());
    g_items.insert(g_items.begin() + newIndex, std::move(holder));
}

static void PinItem(DockItem& it) {
    if (it.pinned) return;
    it.pinned = true;
    it.removing = false;
    it.presence.SetTarget(1);
    MoveItem(&it, AfterLastPinned(&it));
    SavePins();
    Kick();
}

static void UnpinItem(DockItem& it, bool instant) {
    if (!it.pinned) return;
    it.pinned = false;
    if (it.windows.empty()) {
        it.removing = true;
        if (instant) it.presence.Snap(0);
        else it.presence.SetTarget(0);
    } else {
        MoveItem(&it, AfterLastPinned(&it));
    }
    SavePins();
    Kick();
}

// ---------------------------------------------------------------------------
// Papelera
// ---------------------------------------------------------------------------

static std::atomic<bool> g_trashQuerying{false};

static DWORD WINAPI TrashQueryThread(LPVOID) {
    SHQUERYRBINFO info{sizeof(info)};
    bool full = SUCCEEDED(SHQueryRecycleBinW(nullptr, &info)) && info.i64NumItems > 0;
    HWND h = g_wnd.load();
    if (h) PostMessageW(h, WM_APP_TRASH, full ? 1 : 0, 0);
    g_trashQuerying = false;
    return 0;
}

static void QueryTrashAsync() {
    if (!g_set.showTrash || g_trashQuerying.exchange(true)) return;
    HANDLE t = CreateThread(nullptr, 0, TrashQueryThread, nullptr, 0, nullptr);
    if (t) CloseHandle(t);
    else g_trashQuerying = false;
}

// Respaldo si el Explorador no ofrece su destino de soltar.
struct FileJob {
    int kind = 0;  // 0 abrir con, 1 a la papelera
    std::vector<std::wstring> files;
    std::wstring exe, aumid;
};

static DWORD WINAPI FileJobThread(LPVOID p) {
    std::unique_ptr<FileJob> job((FileJob*)p);
    CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    if (job->kind == 1) {
        std::wstring list;
        for (auto& f : job->files) {
            list += f;
            list.push_back(L'\0');
        }
        list.push_back(L'\0');
        SHFILEOPSTRUCTW op{};
        op.wFunc = FO_DELETE;
        op.pFrom = list.c_str();
        op.fFlags = FOF_ALLOWUNDO | FOF_NOCONFIRMATION;
        SHFileOperationW(&op);
        HWND h = g_wnd.load();
        if (h) PostMessageW(h, WM_APP_TRASH, 2, 0);
    } else if (!job->exe.empty()) {
        std::wstring params;
        for (auto& f : job->files) {
            if (!params.empty()) params += L' ';
            params += L"\"" + f + L"\"";
        }
        SHELLEXECUTEINFOW sei{sizeof(sei)};
        sei.fMask = SEE_MASK_FLAG_NO_UI | SEE_MASK_NOASYNC;
        sei.lpFile = job->exe.c_str();
        sei.lpParameters = params.c_str();
        sei.nShow = SW_SHOWNORMAL;
        ShellExecuteExW(&sei);
    } else if (!job->aumid.empty()) {
        std::vector<PIDLIST_ABSOLUTE> pidls;
        for (auto& f : job->files) {
            PIDLIST_ABSOLUTE pidl = ILCreateFromPathW(f.c_str());
            if (pidl) pidls.push_back(pidl);
        }
        IShellItemArray* arr = nullptr;
        if (!pidls.empty() &&
            SUCCEEDED(SHCreateShellItemArrayFromIDLists((UINT)pidls.size(), (PCIDLIST_ABSOLUTE_ARRAY)pidls.data(), &arr)) && arr) {
            IApplicationActivationManager* aam = nullptr;
            if (SUCCEEDED(CoCreateInstance(CLSID_ApplicationActivationManager, nullptr, CLSCTX_LOCAL_SERVER,
                                           IID_PPV_ARGS(&aam))) && aam) {
                DWORD pid = 0;
                aam->ActivateForFile(job->aumid.c_str(), arr, L"open", &pid);
                aam->Release();
            }
            arr->Release();
        }
        for (auto pidl : pidls) ILFree(pidl);
    }
    CoUninitialize();
    return 0;
}

static void RunFileJob(FileJob* job) {
    AllowSetForegroundWindow(ASFW_ANY);
    HANDLE t = CreateThread(nullptr, 0, FileJobThread, job, 0, nullptr);
    if (t) CloseHandle(t);
    else delete job;
}

// ---------------------------------------------------------------------------
// Ventanas emergentes propias: menú contextual y vista de ventanas.
// Vectoriales, del tamaño exacto en pantalla, y sin tomar el foco: funcionan
// igual con el escritorio o con cualquier app en primer plano. Se cierran
// con un clic fuera o con Esc (ganchos de bajo nivel solo mientras están
// abiertas).
// ---------------------------------------------------------------------------

static const wchar_t kPopupClass[] = L"WindhawkAppleDockPopup";

enum class PopKind { None, Menu, Preview };

enum : UINT {
    M_WINDOW = 1,
    M_OPEN,
    M_SHOWALL,
    M_HIDE,
    M_HIDEOTHERS,
    M_PIN,
    M_STARTUP,
    M_SHOWFILE,
    M_RUNAS,
    M_QUIT,
    M_KILL,
    M_TRASH_OPEN,
    M_TRASH_EMPTY,
    M_APPS,
    M_TASKMGR,
    M_SETTINGS,
    M_EXPLORER,
    M_RUN,
    M_DESKTOP,
    M_LOCK,
    M_SIGNOUT,
    M_SLEEP,
    M_RESTART,
    M_SHUTDOWN,
    M_TASKBARSET,
};

struct MenuEntry {
    std::wstring text;
    UINT id = 0;
    bool sep = false, checked = false, disabled = false, danger = false;
    HWND hwnd = nullptr;
    RECT rc{};
};

struct PrevEntry {
    HWND hwnd = nullptr;
    std::wstring title;
    HTHUMBNAIL thumb = nullptr;
    bool iconic = false;
    std::shared_ptr<Snapshot> snap;  // foto guardada (si está minimizada)
    Img snapFit;                     // la foto al tamaño exacto de la miniatura
    RECT card{}, thumbRc{}, closeRc{};
};

struct Popup {
    PopKind kind = PopKind::None;
    HWND wnd = nullptr;
    uint64_t itemUid = 0;
    std::vector<MenuEntry> menu;
    std::vector<PrevEntry> prev;
    std::wstring title;
    bool list = false;
    int hot = -1;
    bool hotClose = false;
    int x = 0, y = 0, w = 0, h = 0;  // ventana en pantalla (incluye el margen de la sombra)
    int margin = 0;
    Spring alpha, rise;
    bool closing = false;
    HDC dc = nullptr;
    HBITMAP dib = nullptr;
    HGDIOBJ old = nullptr;
    uint32_t* bits = nullptr;
    int bw = 0, bh = 0;
    double leaveSince = -1;
};

static Popup g_pop;
static HHOOK g_llMouse = nullptr, g_llKey = nullptr;
#define g_fontBold g_fontBoldPtr
#define g_fontSmall g_fontSmallPtr

enum : UINT { WM_APP_POPCLOSE = WM_APP + 40, WM_APP_POPKEY };

static bool PopupOpen() { return g_pop.kind != PopKind::None && !g_pop.closing; }

static RECT PopupPanelScreen() {
    return RECT{g_pop.x + g_pop.margin, g_pop.y + g_pop.margin, g_pop.x + g_pop.w - g_pop.margin,
                g_pop.y + g_pop.h - g_pop.margin};
}

static bool CursorInPopup(POINT pt) {
    if (!PopupOpen()) return false;
    RECT pr = PopupPanelScreen();
    InflateRect(&pr, S(6), S(10));
    return PtInRect(&pr, pt) != FALSE;
}

static Gdiplus::SizeF MeasureText(const std::wstring& t, Gdiplus::Font* f) {
    if (t.empty() || !f) return Gdiplus::SizeF(0, 0);
    HDC dc = CreateCompatibleDC(nullptr);
    Gdiplus::RectF b;
    {
        Gdiplus::Graphics g(dc);
        g.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);
        Gdiplus::StringFormat sf;
        sf.SetFormatFlags(Gdiplus::StringFormatFlagsNoWrap);
        g.MeasureString(t.c_str(), (INT)t.size(), f, Gdiplus::PointF(0, 0), &sf, &b);
    }
    DeleteDC(dc);
    return Gdiplus::SizeF(b.Width, b.Height);
}

static Gdiplus::Color AccentColor() {
    DWORD c = 0;
    BOOL opaque = FALSE;
    if (SUCCEEDED(DwmGetColorizationColor(&c, &opaque))) {
        BYTE r = (c >> 16) & 0xFF, g = (c >> 8) & 0xFF, b = c & 0xFF;
        // Muy oscuro o muy claro no sirve para resaltar: azul de sistema.
        int l = (r * 299 + g * 587 + b * 114) / 1000;
        if (l > 40 && l < 215) return Gdiplus::Color(255, r, g, b);
    }
    return Gdiplus::Color(255, 10, 132, 255);
}

static void FreePopupBuffers() {
    if (g_pop.dc) {
        if (g_pop.old) SelectObject(g_pop.dc, g_pop.old);
        DeleteDC(g_pop.dc);
    }
    if (g_pop.dib) DeleteObject(g_pop.dib);
    g_pop.dc = nullptr;
    g_pop.dib = nullptr;
    g_pop.old = nullptr;
    g_pop.bits = nullptr;
    g_pop.bw = g_pop.bh = 0;
}

static bool EnsurePopupBuffers(int w, int h) {
    if (w == g_pop.bw && h == g_pop.bh && g_pop.bits) return true;
    FreePopupBuffers();
    BITMAPINFO bi{};
    bi.bmiHeader.biSize = sizeof(bi.bmiHeader);
    bi.bmiHeader.biWidth = w;
    bi.bmiHeader.biHeight = -h;
    bi.bmiHeader.biPlanes = 1;
    bi.bmiHeader.biBitCount = 32;
    bi.bmiHeader.biCompression = BI_RGB;
    void* bits = nullptr;
    g_pop.dc = CreateCompatibleDC(nullptr);
    g_pop.dib = CreateDIBSection(g_pop.dc, &bi, DIB_RGB_COLORS, &bits, nullptr, 0);
    if (!g_pop.dc || !g_pop.dib || !bits) {
        FreePopupBuffers();
        return false;
    }
    g_pop.old = SelectObject(g_pop.dc, g_pop.dib);
    g_pop.bits = (uint32_t*)bits;
    g_pop.bw = w;
    g_pop.bh = h;
    return true;
}

// --- Dibujos vectoriales pequeños (palomita, cruz) ---
static void DrawCheck(Gdiplus::Graphics& g, float cx, float cy, float s, Gdiplus::Color c) {
    Gdiplus::Pen pen(c, std::max(1.5f, s * 0.16f));
    pen.SetStartCap(Gdiplus::LineCapRound);
    pen.SetEndCap(Gdiplus::LineCapRound);
    pen.SetLineJoin(Gdiplus::LineJoinRound);
    Gdiplus::PointF pts[3] = {{cx - s * 0.42f, cy + s * 0.02f}, {cx - s * 0.12f, cy + s * 0.32f},
                              {cx + s * 0.44f, cy - s * 0.34f}};
    g.DrawLines(&pen, pts, 3);
}

static void DrawCross(Gdiplus::Graphics& g, float cx, float cy, float s, Gdiplus::Color c) {
    Gdiplus::Pen pen(c, std::max(1.4f, s * 0.15f));
    pen.SetStartCap(Gdiplus::LineCapRound);
    pen.SetEndCap(Gdiplus::LineCapRound);
    g.DrawLine(&pen, cx - s * 0.32f, cy - s * 0.32f, cx + s * 0.32f, cy + s * 0.32f);
    g.DrawLine(&pen, cx - s * 0.32f, cy + s * 0.32f, cx + s * 0.32f, cy - s * 0.32f);
}

static void FillRoundG(Gdiplus::Graphics& g, const RECT& r, float rad, Gdiplus::Color c) {
    Gdiplus::GraphicsPath p;
    AddRoundRect(p, (float)r.left, (float)r.top, (float)(r.right - r.left), (float)(r.bottom - r.top), rad);
    Gdiplus::SolidBrush b(c);
    g.FillPath(&b, &p);
}

// Dibuja todo el contenido de la ventana emergente.
static void RenderPopup() {
    if (!EnsurePopupBuffers(g_pop.w, g_pop.h)) return;
    memset(g_pop.bits, 0, (size_t)g_pop.w * g_pop.h * 4);
    bool dark = DarkUi();
    Surface s{g_pop.bits, g_pop.w, g_pop.h};
    RECT all{0, 0, g_pop.w, g_pop.h};
    int M = g_pop.margin;
    RoundStyle st;
    st.fillA = dark ? 0.94f : 0.96f;
    st.fr = dark ? 36.f : 246.f;
    st.fg = dark ? 36.f : 246.f;
    st.fb = dark ? 40.f : 248.f;
    st.shadowA = dark ? 0.42f : 0.22f;
    st.shadowR = (float)(M - S(2));
    st.shadowOff = (float)S(3);
    st.rimA = dark ? 0.18f : 0.7f;
    st.outerA = dark ? 0.55f : 0.14f;
    FillRoundRect(s, all, (float)M, (float)M, (float)(g_pop.w - 2 * M), (float)(g_pop.h - 2 * M), (float)S(11), st);

    Gdiplus::Bitmap bmp(g_pop.w, g_pop.h, g_pop.w * 4, PixelFormat32bppPARGB, (BYTE*)g_pop.bits);
    Gdiplus::Graphics g(&bmp);
    g.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
    g.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);
    g.SetPixelOffsetMode(Gdiplus::PixelOffsetModeHalf);
    Gdiplus::Color text = dark ? Gdiplus::Color(255, 242, 242, 245) : Gdiplus::Color(255, 28, 28, 30);
    Gdiplus::Color dim = dark ? Gdiplus::Color(255, 140, 140, 146) : Gdiplus::Color(255, 150, 150, 156);
    Gdiplus::Color red = Gdiplus::Color(255, 255, 69, 58);
    (void)AccentColor;
    Gdiplus::StringFormat sf;
    sf.SetLineAlignment(Gdiplus::StringAlignmentCenter);
    sf.SetTrimming(Gdiplus::StringTrimmingEllipsisCharacter);
    sf.SetFormatFlags(Gdiplus::StringFormatFlagsNoWrap);

    if (g_pop.kind == PopKind::Menu) {
        for (size_t i = 0; i < g_pop.menu.size(); i++) {
            const MenuEntry& e = g_pop.menu[i];
            const RECT& r = e.rc;
            if (e.sep) {
                float y = (r.top + r.bottom) / 2.0f;
                Gdiplus::Pen pen(dark ? Gdiplus::Color(40, 255, 255, 255) : Gdiplus::Color(30, 0, 0, 0), 1.0f);
                g.DrawLine(&pen, (float)r.left + S(8), y, (float)r.right - S(8), y);
                continue;
            }
            bool hot = (int)i == g_pop.hot && !e.disabled;
            // Resaltado gris neutro (no el color de acento de Windows).
            if (hot) FillRoundG(g, r, (float)S(6), dark ? Gdiplus::Color(40, 255, 255, 255) : Gdiplus::Color(24, 0, 0, 0));
            Gdiplus::Color c = e.disabled ? dim : e.danger ? red : text;
            if (e.checked) DrawCheck(g, r.left + S(14.0), (r.top + r.bottom) / 2.0f, (float)S(11), c);
            Gdiplus::SolidBrush tb(c);
            Gdiplus::RectF tr((float)r.left + S(26), (float)r.top, (float)(r.right - r.left - S(34)),
                              (float)(r.bottom - r.top));
            g.DrawString(e.text.c_str(), (INT)e.text.size(), e.hwnd ? g_font.get() : g_font.get(), tr, &sf, &tb);
        }
    } else if (g_pop.kind == PopKind::Preview) {
        DockItem* item = FindByUid(g_pop.itemUid);
        Gdiplus::SolidBrush tb(text);
        Gdiplus::RectF titleRc((float)M + S(12), (float)M + S(4), (float)(g_pop.w - 2 * M - S(24)), (float)S(28));
        g.DrawString(g_pop.title.c_str(), (INT)g_pop.title.size(), g_fontBold.get(), titleRc, &sf, &tb);
        const IconSet* set = item ? IconFor(*item) : nullptr;
        for (size_t i = 0; i < g_pop.prev.size(); i++) {
            const PrevEntry& e = g_pop.prev[i];
            bool hot = (int)i == g_pop.hot;
            if (hot) FillRoundG(g, e.card, (float)S(8), dark ? Gdiplus::Color(34, 255, 255, 255) : Gdiplus::Color(22, 0, 0, 0));
            if (!g_pop.list) {
                // Marco de la miniatura; debajo, el icono (se ve si no hay miniatura).
                FillRoundG(g, e.thumbRc, (float)S(5), dark ? Gdiplus::Color(50, 0, 0, 0) : Gdiplus::Color(18, 0, 0, 0));
                if (set) {
                    g.Flush(Gdiplus::FlushIntentionSync);
                    double is = std::min(S(56), (int)std::min(e.thumbRc.right - e.thumbRc.left, e.thumbRc.bottom - e.thumbRc.top) - S(8));
                    double ix = (e.thumbRc.left + e.thumbRc.right - is) / 2, iy = (e.thumbRc.top + e.thumbRc.bottom - is) / 2;
                    BlitIcon(s, all, *set, ix, iy, is, 1, 1, true);
                }
                if (!e.snapFit.empty()) {
                    // La foto guardada, ya ajustada al tamaño de la miniatura.
                    g.Flush(Gdiplus::FlushIntentionSync);
                    BlitImage(s, all, e.snapFit, e.thumbRc.left, e.thumbRc.top, 1.0);
                }
                if (e.iconic) {
                    // Etiqueta "Minimizada" en una pastilla abajo de la miniatura.
                    std::wstring m = L"Minimizada";
                    Gdiplus::SizeF ts = MeasureText(m, g_fontSmall.get());
                    float pw = ts.Width + S(14), ph = ts.Height + S(4);
                    float pxl = (e.thumbRc.left + e.thumbRc.right - pw) / 2, pyl = (float)e.thumbRc.bottom - ph - S(6);
                    RECT pr{(LONG)pxl, (LONG)pyl, (LONG)(pxl + pw), (LONG)(pyl + ph)};
                    FillRoundG(g, pr, ph / 2, Gdiplus::Color(170, 20, 20, 24));
                    Gdiplus::SolidBrush db(Gdiplus::Color(255, 235, 235, 240));
                    Gdiplus::StringFormat cf;
                    cf.SetAlignment(Gdiplus::StringAlignmentCenter);
                    cf.SetLineAlignment(Gdiplus::StringAlignmentCenter);
                    g.DrawString(m.c_str(), (INT)m.size(), g_fontSmall.get(),
                                 Gdiplus::RectF(pxl, pyl, pw, ph), &cf, &db);
                }
                Gdiplus::RectF tr((float)e.card.left + S(8), (float)e.thumbRc.bottom + S(4),
                                  (float)(e.card.right - e.card.left - S(16)), (float)S(22));
                Gdiplus::StringFormat cf;
                cf.SetAlignment(Gdiplus::StringAlignmentCenter);
                cf.SetLineAlignment(Gdiplus::StringAlignmentCenter);
                cf.SetTrimming(Gdiplus::StringTrimmingEllipsisCharacter);
                cf.SetFormatFlags(Gdiplus::StringFormatFlagsNoWrap);
                g.DrawString(e.title.c_str(), (INT)e.title.size(), g_font.get(), tr, &cf, &tb);
            } else {
                if (set) {
                    g.Flush(Gdiplus::FlushIntentionSync);
                    double is = S(20);
                    BlitIcon(s, all, *set, e.card.left + S(8), (e.card.top + e.card.bottom - is) / 2, is, 1, 1, false);
                }
                Gdiplus::RectF tr((float)e.card.left + S(36), (float)e.card.top, (float)(e.card.right - e.card.left - S(66)),
                                  (float)(e.card.bottom - e.card.top));
                Gdiplus::SolidBrush rb(e.iconic ? dim : text);
                g.DrawString(e.title.c_str(), (INT)e.title.size(), g_font.get(), tr, &sf, &rb);
            }
            if (hot) {
                bool hc = g_pop.hotClose;
                const RECT& c = e.closeRc;
                float cx = (c.left + c.right) / 2.0f, cy = (c.top + c.bottom) / 2.0f, rr = (c.right - c.left) / 2.0f;
                Gdiplus::SolidBrush cb(hc ? red : (dark ? Gdiplus::Color(220, 70, 70, 76) : Gdiplus::Color(230, 225, 225, 230)));
                g.FillEllipse(&cb, cx - rr, cy - rr, 2 * rr, 2 * rr);
                DrawCross(g, cx, cy, rr * 1.15f, hc ? Gdiplus::Color(255, 255, 255, 255) : text);
            }
        }
    }
    g.Flush(Gdiplus::FlushIntentionSync);
    GdiFlush();
}

static void PresentPopup() {
    if (!g_pop.wnd || !g_pop.bits) return;
    double a = std::clamp(g_pop.alpha.x, 0.0, 1.0);
    POINT dst{g_pop.x, g_pop.y + (int)std::lround(g_pop.rise.x)};
    SIZE size{g_pop.w, g_pop.h};
    POINT src{0, 0};
    BLENDFUNCTION bf{AC_SRC_OVER, 0, (BYTE)std::lround(a * 255), AC_SRC_ALPHA};
    UpdateLayeredWindow(g_pop.wnd, nullptr, &dst, &size, g_pop.dc, &src, 0, &bf, ULW_ALPHA);
    for (auto& e : g_pop.prev) {
        if (!e.thumb) continue;
        DWM_THUMBNAIL_PROPERTIES tp{};
        tp.dwFlags = DWM_TNP_OPACITY;
        tp.opacity = (BYTE)std::lround(a * 255);
        DwmUpdateThumbnailProperties(e.thumb, &tp);
    }
}

static void UnregisterThumbs() {
    for (auto& e : g_pop.prev) {
        if (e.thumb) DwmUnregisterThumbnail(e.thumb);
        e.thumb = nullptr;
    }
}

static LRESULT CALLBACK LLMouseProc(int code, WPARAM w, LPARAM l) {
    if (code == HC_ACTION && (w == WM_LBUTTONDOWN || w == WM_RBUTTONDOWN || w == WM_MBUTTONDOWN)) {
        auto* m = (MSLLHOOKSTRUCT*)l;
        RECT r = PopupPanelScreen();
        if (!PtInRect(&r, m->pt)) {
            HWND h = g_wnd.load();
            if (h) PostMessageW(h, WM_APP_POPCLOSE, 0, 0);
        }
    }
    return CallNextHookEx(nullptr, code, w, l);
}

static LRESULT CALLBACK LLKeyProc(int code, WPARAM w, LPARAM l) {
    if (code == HC_ACTION && (w == WM_KEYDOWN || w == WM_SYSKEYDOWN) && g_pop.kind == PopKind::Menu) {
        auto* k = (KBDLLHOOKSTRUCT*)l;
        switch (k->vkCode) {
            case VK_ESCAPE:
            case VK_UP:
            case VK_DOWN:
            case VK_RETURN:
            case VK_SPACE:
            case VK_HOME:
            case VK_END: {
                HWND h = g_wnd.load();
                g_dbgKeys++;
                if (h) PostMessageW(h, WM_APP_POPKEY, k->vkCode, 0);
                return 1;  // la tecla es del menú
            }
        }
    }
    return CallNextHookEx(nullptr, code, w, l);
}

static void InstallPopupHooks(bool keys) {
    HINSTANCE inst = GetModuleHandleW(nullptr);
    if (!g_llMouse) g_llMouse = SetWindowsHookExW(WH_MOUSE_LL, LLMouseProc, inst, 0);
    if (keys && !g_llKey) {
        g_llKey = SetWindowsHookExW(WH_KEYBOARD_LL, LLKeyProc, inst, 0);
        g_dbgHookOn++;
    }
    if (!keys && g_llKey) {
        UnhookWindowsHookEx(g_llKey);
        g_llKey = nullptr;
    }
}

static void RemovePopupHooks() {
    if (g_llMouse) UnhookWindowsHookEx(g_llMouse);
    if (g_llKey) {
        UnhookWindowsHookEx(g_llKey);
        g_dbgHookOff++;
    }
    g_llMouse = g_llKey = nullptr;
}

static void ClosePopup(bool instant) {
    if (g_pop.kind == PopKind::None) return;
    // La vista de esa app no vuelve a abrirse hasta que el mouse pase a otra.
    if (g_pop.itemUid) g_prevSuppress = g_pop.itemUid;
    else if (g_lastLayout.g.size() == g_items.size()) {
        int i = ItemAtX(g_lastLayout, g_mouseX);
        if (i >= 0) g_prevSuppress = g_items[i]->uid;
    }
    RemovePopupHooks();
    UnregisterThumbs();
    if (instant || !g_pop.wnd) {
        if (g_pop.wnd) ShowWindow(g_pop.wnd, SW_HIDE);
        g_pop.kind = PopKind::None;
        g_pop.closing = false;
        g_pop.menu.clear();
        g_pop.prev.clear();
        g_pop.alpha.Snap(0);
    } else {
        g_pop.closing = true;
        g_pop.alpha.SetTarget(0);
        g_pop.rise.SetTarget(S(4));
    }
    g_menuOpen = false;
    g_prevCand = nullptr;
    Kick();
}

static void FinishPopupClose() {
    if (g_pop.wnd) ShowWindow(g_pop.wnd, SW_HIDE);
    g_pop.kind = PopKind::None;
    g_pop.closing = false;
    g_pop.menu.clear();
    g_pop.prev.clear();
}

static LRESULT CALLBACK PopupWndProc(HWND, UINT, WPARAM, LPARAM);

static bool EnsurePopupWindow() {
    if (g_pop.wnd) return true;
    g_pop.wnd = CreateWindowExW(WS_EX_LAYERED | WS_EX_TOOLWINDOW | WS_EX_TOPMOST | WS_EX_NOACTIVATE, kPopupClass, L"",
                                WS_POPUP, 0, 0, 1, 1, nullptr, nullptr, GetModuleHandleW(nullptr), nullptr);
    return g_pop.wnd != nullptr;
}

// Coloca la ventana emergente encima del icono (o del punto dado).
static void PlacePopup(double anchorX, double bottomY) {
    MONITORINFO mi{sizeof(mi)};
    GetMonitorInfoW(g_monitor, &mi);
    RECT mr = mi.rcMonitor;
    int x = (int)std::lround(anchorX - g_pop.w / 2.0);
    x = std::clamp(x, (int)mr.left + S(4) - g_pop.margin, (int)mr.right - S(4) - g_pop.w + g_pop.margin);
    int y = (int)std::lround(bottomY - g_pop.h + g_pop.margin);
    y = std::max(y, (int)mr.top + S(4) - g_pop.margin);
    g_pop.x = x;
    g_pop.y = y;
}

static void ShowPopupWindow() {
    if (!EnsurePopupWindow()) return;
    RenderPopup();
    g_pop.alpha.Snap(0);
    g_pop.alpha.SetTarget(1);
    g_pop.rise.Snap(S(6));
    g_pop.rise.SetTarget(0);
    g_pop.closing = false;
    g_pop.leaveSince = -1;
    PresentPopup();
    SetWindowPos(g_pop.wnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_SHOWWINDOW);
    Kick();
}

// --- Menú ---
static void LayoutMenu() {
    int M = S(16);
    g_pop.margin = M;
    int pad = S(6), rowH = S(28), sepH = S(9);
    // Mismo ancho para todos los menús: el de la opción más larga que puede
    // tener cualquier menú del Dock. Los títulos de ventana largos se cortan
    // con "…" en lugar de ensanchar el menú.
    static const wchar_t* kAll[] = {L"Configuración de la barra de tareas", L"Ejecutar como administrador",
                                    L"Cerrar todas las ventanas", L"Mostrar todas las ventanas",
                                    L"Mostrar en el Explorador", L"Aplicaciones instaladas",
                                    L"Administrador de tareas", L"Abrir al iniciar sesión", L"Mantener en el Dock"};
    float maxW = 0;
    for (const wchar_t* t : kAll) maxW = std::max(maxW, MeasureText(t, g_font.get()).Width);
    // Texto + margen izquierdo (palomita) + margen derecho + relleno de la fila
    int w = std::clamp((int)std::ceil(maxW) + S(26) + S(22) + S(12), S(210), S(460));
    int y = M + pad;
    for (auto& e : g_pop.menu) {
        int hh = e.sep ? sepH : rowH;
        e.rc = RECT{M + pad, y, M + w - pad, y + hh};
        y += hh;
    }
    g_pop.w = w + 2 * M;
    g_pop.h = y + pad + M;
}

static std::wstring StartupLinkPath(const DockItem& it) {
    std::wstring dir = ExpandEnv(L"%APPDATA%\\Microsoft\\Windows\\Start Menu\\Programs\\Startup");
    std::wstring name = it.name.empty() ? FileNameNoExt(it.value) : it.name;
    for (auto& c : name) {
        if (wcschr(L"\\/:*?\"<>|", c)) c = L'_';
    }
    return dir + L"\\" + name + L".lnk";
}

static bool CanStartup(const DockItem& it) { return it.kind == Kind::App && (it.type != 'P'); }

static void ToggleStartup(const DockItem& it) {
    std::wstring link = StartupLinkPath(it);
    if (FileExists(link)) {
        DeleteFileW(link.c_str());
        return;
    }
    if (it.type == 'L') {
        CopyFileW(it.value.c_str(), link.c_str(), FALSE);
        return;
    }
    IShellLinkW* sl = nullptr;
    if (FAILED(CoCreateInstance(CLSID_ShellLink, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&sl))) || !sl) return;
    if (it.type == 'A') {
        sl->SetPath(ExpandEnv(L"%WINDIR%\\explorer.exe").c_str());
        sl->SetArguments((L"shell:AppsFolder\\" + it.value).c_str());
    } else {
        sl->SetPath(it.value.c_str());
    }
    IPersistFile* pf = nullptr;
    if (SUCCEEDED(sl->QueryInterface(IID_PPV_ARGS(&pf))) && pf) {
        pf->Save(link.c_str(), TRUE);
        pf->Release();
    }
    sl->Release();
}

static bool IsExplorerItem(const DockItem& it) { return EndsWithI(it.exe, L"\\explorer.exe"); }

static void BuildAppMenu(DockItem& it) {
    auto add = [&](const std::wstring& t, UINT id, bool checked = false, bool disabled = false, bool danger = false) {
        MenuEntry e;
        e.text = t;
        e.id = id;
        e.checked = checked;
        e.disabled = disabled;
        e.danger = danger;
        g_pop.menu.push_back(e);
    };
    auto sep = [&] {
        if (!g_pop.menu.empty() && !g_pop.menu.back().sep) {
            MenuEntry e;
            e.sep = true;
            g_pop.menu.push_back(e);
        }
    };
    HWND fg = GetForegroundWindow();
    int n = 0;
    for (HWND w : it.windows) {
        if (n++ >= 10) break;
        wchar_t buf[256] = {};
        InternalGetWindowText(w, buf, ARRAYSIZE(buf));
        MenuEntry e;
        e.text = buf[0] ? buf : it.name;
        e.id = M_WINDOW;
        e.hwnd = w;
        e.checked = w == fg;
        g_pop.menu.push_back(e);
    }
    sep();
    add(it.windows.empty() ? L"Abrir" : L"Nueva ventana", M_OPEN);
    if (!it.windows.empty()) {
        bool anyMin = std::any_of(it.windows.begin(), it.windows.end(), [](HWND w) { return IsIconic(w); });
        bool anyVis = std::any_of(it.windows.begin(), it.windows.end(), [](HWND w) { return !IsIconic(w); });
        add(L"Mostrar todas las ventanas", M_SHOWALL, false, !anyMin);
        add(L"Ocultar", M_HIDE, false, !anyVis);
    }
    add(L"Ocultar las demás", M_HIDEOTHERS);
    sep();
    add(L"Mantener en el Dock", M_PIN, it.pinned, g_set.lockItems);
    if (CanStartup(it)) add(L"Abrir al iniciar sesión", M_STARTUP, FileExists(StartupLinkPath(it)));
    if (it.type != 'A' && !it.value.empty()) add(L"Mostrar en el Explorador", M_SHOWFILE);
    if (it.type == 'E' || it.type == 'L') add(L"Ejecutar como administrador", M_RUNAS);
    if (!it.windows.empty()) {
        sep();
        add(it.windows.size() > 1 ? L"Cerrar todas las ventanas" : L"Salir", M_QUIT);
        add(L"Finalizar tarea", M_KILL, false, it.pids.empty() || IsExplorerItem(it), true);
    }
}

static void OpenMenu(DockItem* it, double anchorX) {
    ClosePopup(true);
    g_pop.menu.clear();
    g_pop.prev.clear();
    g_pop.kind = PopKind::Menu;
    g_pop.itemUid = it ? it->uid : 0;
    auto add = [&](const std::wstring& t, UINT id, bool disabled = false, bool danger = false) {
        MenuEntry e;
        e.text = t;
        e.id = id;
        e.disabled = disabled;
        e.danger = danger;
        g_pop.menu.push_back(e);
    };
    auto sep = [&] {
        MenuEntry e;
        e.sep = true;
        g_pop.menu.push_back(e);
    };
    if (it && it->kind == Kind::App) {
        BuildAppMenu(*it);
    } else if (it && it->kind == Kind::Trash) {
        add(L"Abrir", M_TRASH_OPEN);
        add(L"Vaciar papelera…", M_TRASH_EMPTY, !g_trashFull);
    } else if (it && it->kind == Kind::Start) {
        add(L"Aplicaciones instaladas", M_APPS);
        add(L"Administrador de tareas", M_TASKMGR);
        add(L"Configuración", M_SETTINGS);
        add(L"Explorador de archivos", M_EXPLORER);
        add(L"Ejecutar…", M_RUN);
        add(L"Mostrar el escritorio", M_DESKTOP);
        sep();
        add(L"Bloquear", M_LOCK);
        add(L"Cerrar sesión", M_SIGNOUT);
        add(L"Suspender", M_SLEEP);
        add(L"Reiniciar", M_RESTART);
        add(L"Apagar", M_SHUTDOWN, false, true);
    } else {
        add(L"Mostrar el escritorio", M_DESKTOP);
        add(L"Administrador de tareas", M_TASKMGR);
        add(L"Configuración de la barra de tareas", M_TASKBARSET);
    }
    while (!g_pop.menu.empty() && g_pop.menu.back().sep) g_pop.menu.pop_back();
    LayoutMenu();
    const Layout& L = g_lastLayout;
    double top = g_winY + std::min(L.topMost, L.bgT) - S(8);
    PlacePopup(anchorX, top);
    g_pop.hot = -1;
    g_menuOpen = true;
    g_labelA.SetTarget(0);
    ShowPopupWindow();
    InstallPopupHooks(true);
}

static DWORD WINAPI KillThread(LPVOID p) {
    std::unique_ptr<std::vector<DWORD>> pids((std::vector<DWORD>*)p);
    std::wstring denied;
    for (DWORD pid : *pids) {
        HANDLE h = OpenProcess(PROCESS_TERMINATE, FALSE, pid);
        if (h && TerminateProcess(h, 1)) {
            CloseHandle(h);
            continue;
        }
        if (h) CloseHandle(h);
        denied += L" /PID " + std::to_wstring(pid);
    }
    if (!denied.empty()) {
        // Procesos de administrador: lo mismo, pidiendo permiso.
        SHELLEXECUTEINFOW sei{sizeof(sei)};
        sei.fMask = SEE_MASK_NOASYNC;
        sei.lpVerb = L"runas";
        sei.lpFile = L"taskkill.exe";
        std::wstring args = L"/F" + denied;
        sei.lpParameters = args.c_str();
        sei.nShow = SW_HIDE;
        ShellExecuteExW(&sei);
    }
    return 0;
}

static void PowerAction(UINT id) {
    switch (id) {
        case M_LOCK:
            LockWorkStation();
            break;
        case M_SIGNOUT:
            ExitWindowsEx(EWX_LOGOFF, SHTDN_REASON_FLAG_PLANNED);
            break;
        case M_SLEEP: {
            using SetSuspendFn = BOOLEAN(WINAPI*)(BOOLEAN, BOOLEAN, BOOLEAN);
            HMODULE pp = LoadLibraryW(L"powrprof.dll");
            auto fn = pp ? (SetSuspendFn)(void*)GetProcAddress(pp, "SetSuspendState") : nullptr;
            if (fn) fn(FALSE, FALSE, FALSE);
            break;
        }
        case M_RESTART:
            ShellOpenAsync(L"shutdown.exe", L"/r /t 0");
            break;
        case M_SHUTDOWN:
            ShellOpenAsync(L"shutdown.exe", L"/s /hybrid /t 0");
            break;
    }
}

static void ExecuteMenu(size_t index) {
    if (index >= g_pop.menu.size()) return;
    MenuEntry e = g_pop.menu[index];
    if (e.sep || e.disabled) return;
    DockItem* it = FindByUid(g_pop.itemUid);
    ClosePopup(true);
    Wh_Log(L"Menú: %s", e.text.c_str());
    switch (e.id) {
        case M_WINDOW:
            ActivateWindow(e.hwnd);
            break;
        case M_OPEN:
            if (it) LaunchItem(*it, true);
            break;
        case M_SHOWALL:
            if (it) {
                for (auto w = it->windows.rbegin(); w != it->windows.rend(); ++w) {
                    if (IsIconic(*w)) ShowWindowAsync(*w, SW_RESTORE);
                }
                if (!it->windows.empty()) ActivateWindow(it->windows.front());
            }
            break;
        case M_HIDE:
            if (it) for (HWND w : it->windows) MinimizeWindow(w);
            break;
        case M_HIDEOTHERS:
            for (auto& o : g_items) {
                if (o.get() == it || o->kind != Kind::App) continue;
                for (HWND w : o->windows) MinimizeWindow(w);
            }
            if (it && !it->windows.empty()) ActivateWindow(it->windows.front());
            break;
        case M_PIN:
            if (it) {
                if (it->pinned) UnpinItem(*it, false);
                else PinItem(*it);
            }
            break;
        case M_STARTUP:
            if (it) ToggleStartup(*it);
            break;
        case M_SHOWFILE:
            if (it) ShellOpenAsync(L"explorer.exe", L"/select,\"" + (!it->exe.empty() ? it->exe : it->value) + L"\"");
            break;
        case M_RUNAS:
            if (it) {
                AllowSetForegroundWindow(ASFW_ANY);
                std::wstring target = it->value;
                HANDLE t = CreateThread(nullptr, 0, [](LPVOID p) -> DWORD {
                    std::unique_ptr<std::wstring> f((std::wstring*)p);
                    CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
                    SHELLEXECUTEINFOW sei{sizeof(sei)};
                    sei.fMask = SEE_MASK_NOASYNC;
                    sei.lpVerb = L"runas";
                    sei.lpFile = f->c_str();
                    sei.nShow = SW_SHOWNORMAL;
                    ShellExecuteExW(&sei);
                    CoUninitialize();
                    return 0;
                }, new std::wstring(target), 0, nullptr);
                if (t) CloseHandle(t);
            }
            break;
        case M_QUIT:
            if (it) for (HWND w : it->windows) PostMessageW(w, WM_CLOSE, 0, 0);
            break;
        case M_KILL:
            if (it && !it->pids.empty() && !IsExplorerItem(*it)) {
                auto* pids = new std::vector<DWORD>(it->pids);
                HANDLE t = CreateThread(nullptr, 0, KillThread, pids, 0, nullptr);
                if (t) CloseHandle(t);
                else delete pids;
                RequestScan();
            }
            break;
        case M_TRASH_OPEN:
            ShellOpenAsync(L"shell:RecycleBinFolder");
            break;
        case M_TRASH_EMPTY:
            SHEmptyRecycleBinW(nullptr, nullptr, 0);
            QueryTrashAsync();
            break;
        case M_APPS:
            ShellOpenAsync(L"ms-settings:appsfeatures");
            break;
        case M_TASKMGR:
            ShellOpenAsync(L"taskmgr.exe");
            break;
        case M_SETTINGS:
            ShellOpenAsync(L"ms-settings:");
            break;
        case M_EXPLORER:
            ShellOpenAsync(L"explorer.exe");
            break;
        case M_RUN:
            SendKeys({VK_LWIN, 'R'});
            break;
        case M_DESKTOP:
            SendKeys({VK_LWIN, 'D'});
            break;
        case M_TASKBARSET:
            ShellOpenAsync(L"ms-settings:taskbar");
            break;
        default:
            PowerAction(e.id);
            break;
    }
    RequestScan();
}

static void MenuKey(UINT vk) {
    if (g_pop.kind != PopKind::Menu) return;
    auto selectable = [](const MenuEntry& e) { return !e.sep && !e.disabled; };
    int n = (int)g_pop.menu.size();
    if (vk == VK_ESCAPE) {
        ClosePopup(false);
        return;
    }
    if ((vk == VK_RETURN || vk == VK_SPACE) && g_pop.hot >= 0) {
        ExecuteMenu((size_t)g_pop.hot);
        return;
    }
    int dir = (vk == VK_UP) ? -1 : 1;
    int i = g_pop.hot;
    if (vk == VK_HOME) i = -1, dir = 1;
    if (vk == VK_END) i = n, dir = -1;
    for (int k = 0; k < n; k++) {
        i = (i + dir + n) % n;
        if (selectable(g_pop.menu[i])) break;
    }
    if (i >= 0 && i < n && selectable(g_pop.menu[i]) && i != g_pop.hot) {
        g_pop.hot = i;
        RenderPopup();
        PresentPopup();
    }
}

// --- Vista de las ventanas de una app ---
static bool LayoutPreview(DockItem& it) {
    g_pop.prev.clear();
    for (HWND w : it.windows) {
        if (!IsWindow(w)) continue;
        PrevEntry e;
        e.hwnd = w;
        wchar_t buf[256] = {};
        InternalGetWindowText(w, buf, ARRAYSIZE(buf));
        e.title = buf[0] ? buf : it.name;
        e.iconic = IsIconic(w) != FALSE;
        if (e.iconic) e.snap = SnapshotOf(w);
        g_pop.prev.push_back(e);
        if (g_pop.prev.size() >= 12) break;
    }
    if (g_pop.prev.empty()) return false;
    g_pop.title = it.name;
    int M = S(16);
    g_pop.margin = M;
    MONITORINFO mi{sizeof(mi)};
    GetMonitorInfoW(g_monitor, &mi);
    int monW = mi.rcMonitor.right - mi.rcMonitor.left;
    int pad = S(10), headH = S(34);
    int cardW = S(214), thumbW = S(198), thumbH = S(122), cardH = thumbH + S(16) + S(26);
    int maxCols = std::max(1, (monW - 2 * M - S(40)) / (cardW + S(6)));
    g_pop.list = g_set.previews == Previews::List || (int)g_pop.prev.size() > maxCols;
    if (!g_pop.list) {
        int n = (int)g_pop.prev.size();
        int w = pad * 2 + n * cardW + (n - 1) * S(6);
        int x = M + pad, y = M + headH;
        for (auto& e : g_pop.prev) {
            e.card = RECT{x, y, x + cardW, y + cardH};
            // Proporción real de la ventana
            RECT wr{};
            if (e.iconic && e.snap) {
                wr = RECT{0, 0, e.snap->w, e.snap->h};  // como se veía al guardarla
            } else if (e.iconic) {
                WINDOWPLACEMENT wp{sizeof(wp)};
                GetWindowPlacement(e.hwnd, &wp);
                wr = wp.rcNormalPosition;
            } else if (FAILED(DwmGetWindowAttribute(e.hwnd, DWMWA_EXTENDED_FRAME_BOUNDS, &wr, sizeof(wr)))) {
                GetWindowRect(e.hwnd, &wr);
            }
            double ww = std::max<LONG>(1, wr.right - wr.left), wh = std::max<LONG>(1, wr.bottom - wr.top);
            double k = std::min(thumbW / ww, thumbH / wh);
            int tw = std::max(S(40), (int)std::lround(ww * k)), th = std::max(S(30), (int)std::lround(wh * k));
            int tx = x + (cardW - tw) / 2, ty = y + S(8) + (thumbH - th) / 2;
            e.thumbRc = RECT{tx, ty, tx + tw, ty + th};
            if (e.snap && !e.snap->mips.empty()) {
                // El nivel más chico que todavía alcance, sin su borde.
                const Img* best = &e.snap->mips[0];
                for (auto& m : e.snap->mips) {
                    if (m.w - 2 >= tw && m.h - 2 >= th) best = &m;
                }
                Img core;
                core.w = best->w - 2;
                core.h = best->h - 2;
                core.px.resize((size_t)core.w * core.h);
                for (int yy = 0; yy < core.h; yy++) {
                    memcpy(&core.px[(size_t)yy * core.w], &best->px[(size_t)(yy + 1) * best->w + 1], (size_t)core.w * 4);
                }
                e.snapFit = Resample(core, tw, th);
            }
            int cs = S(20);
            e.closeRc = RECT{x + cardW - cs - S(4), y + S(4), x + cardW - S(4), y + S(4) + cs};
            x += cardW + S(6);
        }
        g_pop.w = w + 2 * M;
        g_pop.h = headH + cardH + pad + 2 * M;
    } else {
        float maxT = MeasureText(g_pop.title, g_fontBold.get()).Width;
        for (auto& e : g_pop.prev) maxT = std::max(maxT, MeasureText(e.title, g_font.get()).Width + S(70));
        int w = std::clamp((int)std::ceil(maxT) + 2 * pad, S(240), S(460));
        int rowH = S(32);
        int y = M + headH;
        for (auto& e : g_pop.prev) {
            e.card = RECT{M + pad / 2, y, M + w - pad / 2, y + rowH};
            e.thumbRc = RECT{};
            int cs = S(18);
            e.closeRc = RECT{e.card.right - cs - S(8), y + (rowH - cs) / 2, e.card.right - S(8), y + (rowH + cs) / 2};
            y += rowH;
        }
        g_pop.w = w + 2 * M;
        g_pop.h = y + pad + M;
    }
    return true;
}

static void RegisterThumbs() {
    if (g_pop.list || !g_pop.wnd) return;
    for (auto& e : g_pop.prev) {
        if (e.snap || e.thumb) continue;
        HTHUMBNAIL th = nullptr;
        if (FAILED(DwmRegisterThumbnail(g_pop.wnd, e.hwnd, &th)) || !th) continue;
        DWM_THUMBNAIL_PROPERTIES tp{};
        tp.dwFlags = DWM_TNP_RECTDESTINATION | DWM_TNP_VISIBLE | DWM_TNP_OPACITY | DWM_TNP_SOURCECLIENTAREAONLY;
        tp.rcDestination = e.thumbRc;
        tp.fVisible = TRUE;
        tp.opacity = (BYTE)std::lround(std::clamp(g_pop.alpha.x, 0.0, 1.0) * 255);
        tp.fSourceClientAreaOnly = FALSE;
        if (SUCCEEDED(DwmUpdateThumbnailProperties(th, &tp))) {
            e.thumb = th;
        } else {
            DwmUnregisterThumbnail(th);
        }
    }
}

static void OpenPreview(DockItem* it) {
    if (!it || it->windows.empty() || g_set.previews == Previews::Off) return;
    bool wasOpen = g_pop.kind == PopKind::Preview && !g_pop.closing;
    ClosePopup(true);
    g_prevSuppress = 0;
    g_pop.kind = PopKind::Preview;
    g_pop.itemUid = it->uid;
    if (!LayoutPreview(*it)) {
        g_pop.kind = PopKind::None;
        return;
    }
    int idx = IndexOf(it);
    const Layout& L = g_lastLayout;
    double cx = idx >= 0 && idx < (int)L.g.size() ? L.g[idx].cx : g_mouseX;
    double top = g_winY + std::min(L.topMost, L.bgT) - S(6);
    PlacePopup(g_winX + cx, top);
    g_pop.hot = -1;
    g_labelA.SetTarget(0);
    ShowPopupWindow();
    if (wasOpen) {
        // Al pasar de una app a otra, sin volver a aparecer desde cero.
        g_pop.alpha.Snap(1);
        g_pop.rise.Snap(0);
        PresentPopup();
    }
    RegisterThumbs();
    InstallPopupHooks(false);
}

// Hit test en coordenadas de la ventana emergente.
static void PopupHit(POINT pt, int& index, bool& onClose) {
    index = -1;
    onClose = false;
    if (g_pop.kind == PopKind::Menu) {
        for (size_t i = 0; i < g_pop.menu.size(); i++) {
            if (!g_pop.menu[i].sep && PtInRect(&g_pop.menu[i].rc, pt)) index = (int)i;
        }
    } else if (g_pop.kind == PopKind::Preview) {
        for (size_t i = 0; i < g_pop.prev.size(); i++) {
            if (PtInRect(&g_pop.prev[i].card, pt)) {
                index = (int)i;
                onClose = PtInRect(&g_pop.prev[i].closeRc, pt) != FALSE;
            }
        }
    }
}

static void PreviewCloseWindow(size_t i) {
    if (i >= g_pop.prev.size()) return;
    HWND w = g_pop.prev[i].hwnd;
    PostMessageW(w, WM_CLOSE, 0, 0);
    DockItem* it = FindByUid(g_pop.itemUid);
    if (it) {
        it->windows.erase(std::remove(it->windows.begin(), it->windows.end(), w), it->windows.end());
        if (it->windows.empty()) {
            ClosePopup(false);
        } else {
            OpenPreview(it);
        }
    }
    RequestScan();
}

static LRESULT CALLBACK PopupWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (msg == WM_LBUTTONDOWN) g_dbgPopDown++;
    switch (msg) {
        case WM_MOUSEACTIVATE:
            return MA_NOACTIVATE;
        case WM_MOUSEMOVE: {
            TRACKMOUSEEVENT tme{sizeof(tme), TME_LEAVE, hwnd, 0};
            TrackMouseEvent(&tme);
            POINT pt{GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
            int i;
            bool c;
            PopupHit(pt, i, c);
            if (g_pop.kind == PopKind::Menu && i >= 0 && g_pop.menu[i].disabled) i = -1;
            if (i != g_pop.hot || c != g_pop.hotClose) {
                g_pop.hot = i;
                g_pop.hotClose = c;
                RenderPopup();
                PresentPopup();
            }
            return 0;
        }
        case WM_MOUSELEAVE:
            if (g_pop.kind == PopKind::Preview && g_pop.hot != -1) {
                g_pop.hot = -1;
                g_pop.hotClose = false;
                RenderPopup();
                PresentPopup();
            }
            return 0;
        case WM_LBUTTONUP:
        case WM_RBUTTONUP: {
            POINT pt{GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
            int i;
            bool c;
            PopupHit(pt, i, c);
            if (i < 0) return 0;
            if (g_pop.kind == PopKind::Menu) {
                ExecuteMenu((size_t)i);
            } else if (g_pop.kind == PopKind::Preview && msg == WM_LBUTTONUP) {
                if (c) {
                    PreviewCloseWindow((size_t)i);
                } else {
                    HWND w = g_pop.prev[i].hwnd;
                    ClosePopup(true);
                    ActivateWindow(w);
                }
            }
            return 0;
        }
        case WM_MBUTTONUP: {
            POINT pt{GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
            int i;
            bool c;
            PopupHit(pt, i, c);
            if (g_pop.kind == PopKind::Preview && i >= 0) PreviewCloseWindow((size_t)i);
            return 0;
        }
    }
    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

// Avanza la animación de la ventana emergente; true si sigue moviéndose.
static bool StepPopup(double dt) {
    if (g_pop.kind == PopKind::None) return false;
    double sp = g_set.speed;
    bool moving = false;
    if (!g_pop.alpha.settled || !g_pop.rise.settled) {
        g_pop.alpha.Step(dt, 0.16 * sp, 1.0, 0.004);
        g_pop.rise.Step(dt, 0.20 * sp, 1.0, 0.05);
        moving = true;
        if (g_pop.closing && g_pop.alpha.x <= 0.01) {
            FinishPopupClose();
            return false;
        }
        PresentPopup();
    }
    return moving;
}

static void TakeScanResult() {
    std::vector<WinInfo> wins;
    {
        std::lock_guard<std::mutex> lock(g_scanMx);
        if (!g_scanFresh) return;
        wins = std::move(g_scanResult);
        g_scanFresh = false;
    }
    size_t before = 0;
    DockItem* shown = g_pop.kind == PopKind::Preview && !g_pop.closing ? FindByUid(g_pop.itemUid) : nullptr;
    if (shown) before = shown->windows.size();
    ApplyWindows(wins);
    if (shown && FindByUid(g_pop.itemUid) == shown && shown->windows.size() != before) {
        if (shown->windows.empty()) ClosePopup(false);
        else OpenPreview(shown);
    }
}

// ---------------------------------------------------------------------------
// Arrastrar iconos para ordenar o quitar
// ---------------------------------------------------------------------------

static void UpdateInsertion(const Layout& L, double mx, bool pinsOnly) {
    DockItem* target = nullptr;
    int end = PinnedEnd();
    for (int i = 0; i < (int)g_items.size() && i < (int)L.g.size(); i++) {
        auto* it = g_items[i].get();
        if (it == g_drag || it->kind == Kind::Start) continue;
        if (i >= end || mx < L.g[i].cx) {
            target = it;
            break;
        }
    }
    if (pinsOnly && !target && !g_items.empty()) target = g_items.back().get();
    g_insertBefore = target;
    for (auto& it : g_items) it->gap.SetTarget(it.get() == target ? 1 : 0);
}

static void ClearInsertion() {
    g_insertBefore = nullptr;
    for (auto& it : g_items) it->gap.SetTarget(0);
}

static void SetTrashHilite(bool on) {
    if (DockItem* t = FindKind(Kind::Trash)) t->hilite.SetTarget(on ? 1 : 0);
}

static void BeginDrag(DockItem* it, const Layout& L) {
    int idx = IndexOf(it);
    if (idx < 0) return;
    g_drag = it;
    ClosePopup(true);
    SetCapture(g_wnd.load());  // ahora sí: hay que seguir al mouse fuera del Dock
    g_dragSize = std::max(8.0, L.g[idx].size);
    g_dragGrabX = std::clamp((g_pressPt.x - L.g[idx].x) / g_dragSize, 0.0, 1.0);
    g_dragGrabY = std::clamp((g_pressPt.y - L.g[idx].y) / g_dragSize, 0.0, 1.0);
    g_dragRemove = false;
    g_dragOverTrash = false;
    it->slot.Snap(0);
    if (idx + 1 < (int)g_items.size()) g_items[idx + 1]->gap.Snap(1);
    UpdateInsertion(L, g_mouseX, false);
    g_labelA.SetTarget(0);
    UpdateDragWindow(Now());
    Kick();
}

static void EndDrag(bool cancel) {
    DockItem* it = g_drag;
    if (!it) return;
    double dropX = g_mouseX - g_dragGrabX * g_dragSize;
    double dropY = g_mouseY - g_dragGrabY * g_dragSize;
    g_drag = nullptr;
    bool remove = !cancel && (g_dragRemove || g_dragOverTrash);
    if (remove) {
        bool wasOpen = !it->windows.empty();
        if (!wasOpen && !g_dragOverTrash) {
            g_dragPuff = {g_winX + dropX + g_dragSize / 2, g_winY + dropY + g_dragSize / 2, g_dragSize, Now()};
            g_dragPuffOn = true;
        }
        if (it->pinned) UnpinItem(*it, true);
        if (wasOpen) {
            it->slot.SetTarget(1);
        } else {
            it->removing = true;
            it->presence.Snap(0);
            it->slot.Snap(0);
        }
    } else if (!cancel && g_insertBefore) {
        int to = IndexOf(g_insertBefore);
        double gapX = g_insertBefore->gap.x;
        g_insertBefore->gap.Snap(0);
        MoveItem(it, to);
        it->slot.Snap(std::clamp(gapX, 0.0, 1.0));
        it->slot.SetTarget(1);
        it->pinned = true;
        it->removing = false;
        SavePins();
    } else {
        it->slot.SetTarget(1);
    }
    g_dragRemove = false;
    g_dragOverTrash = false;
    SetTrashHilite(false);
    ClearInsertion();
    if (g_dragPuffOn) UpdateDragWindow(Now());
    else HideDragWindow();
    if (IndexOf(it) >= 0 && !it->removing) {
        Layout L = ComputeLayout(Now());
        int idx = IndexOf(it);
        it->flyX.Snap(dropX - (L.g[idx].cx - L.base / 2));
        it->flyY.Snap(dropY - (L.bgB - L.pad - L.base));
        it->flyX.SetTarget(0);
        it->flyY.SetTarget(0);
    }
    Kick();
}

// ---------------------------------------------------------------------------
// Soltar archivos: se reenvía al destino real del Explorador (la Papelera
// de Windows, el programa o el acceso directo), como en el escritorio.
// ---------------------------------------------------------------------------

static std::vector<std::wstring> g_dropFiles;
static IDataObject* g_dropData = nullptr;
static DWORD g_dropAllowed = 0;
static IDropTarget* g_fwd = nullptr;
static DockItem* g_fwdItem = nullptr;

static std::vector<std::wstring> FilesFrom(IDataObject* data) {
    std::vector<std::wstring> files;
    FORMATETC fmt{CF_HDROP, nullptr, DVASPECT_CONTENT, -1, TYMED_HGLOBAL};
    STGMEDIUM med{};
    if (FAILED(data->GetData(&fmt, &med))) return files;
    HDROP drop = (HDROP)GlobalLock(med.hGlobal);
    if (drop) {
        UINT n = DragQueryFileW(drop, 0xFFFFFFFF, nullptr, 0);
        for (UINT i = 0; i < n && i < 1024; i++) {
            UINT len = DragQueryFileW(drop, i, nullptr, 0);
            std::wstring f(len + 1, L'\0');
            DragQueryFileW(drop, i, f.data(), len + 1);
            f.resize(len);
            files.push_back(f);
        }
        GlobalUnlock(med.hGlobal);
    }
    ReleaseStgMedium(&med);
    return files;
}

static IDropTarget* ShellDropTargetFor(DockItem* it) {
    PIDLIST_ABSOLUTE pidl = nullptr;
    if (it->kind == Kind::Trash) {
        if (FAILED(SHGetKnownFolderIDList(FOLDERID_RecycleBinFolder, 0, nullptr, &pidl))) pidl = nullptr;
    } else if (it->kind == Kind::App) {
        std::wstring name = it->type == 'A' ? L"shell:AppsFolder\\" + it->value : it->value;
        if (FAILED(SHParseDisplayName(name.c_str(), nullptr, &pidl, 0, nullptr))) pidl = nullptr;
    }
    if (!pidl) return nullptr;
    IDropTarget* dt = nullptr;
    IShellFolder* parent = nullptr;
    PCUITEMID_CHILD child = nullptr;
    if (SUCCEEDED(SHBindToParent(pidl, IID_PPV_ARGS(&parent), &child)) && parent) {
        if (FAILED(parent->GetUIObjectOf(g_wnd.load(), 1, &child, IID_IDropTarget, nullptr, (void**)&dt))) dt = nullptr;
        parent->Release();
    }
    ILFree(pidl);
    return dt;
}

static void ReleaseForward(DockItem* it) {
    if (it && it != g_fwdItem) return;
    if (g_fwd) {
        g_fwd->DragLeave();
        g_fwd->Release();
    }
    g_fwd = nullptr;
    g_fwdItem = nullptr;
}

// Elige el destino bajo el cursor y le pasa el arrastre.
static DWORD UpdateFileDrag(DWORD keys, POINTL ptl) {
    double mx = ptl.x - g_winX, my = ptl.y - g_winY;
    g_mouseX = mx;
    g_mouseY = my;
    g_anchor.SetTarget(mx);
    const Layout& L = g_lastLayout;
    int i = ItemAtX(L, mx);
    DockItem* under = i >= 0 ? g_items[i].get() : nullptr;
    DockItem* target = nullptr;
    DWORD effect = DROPEFFECT_NONE;
    if (under && under->kind == Kind::Trash) {
        target = under;
        ClearInsertion();
    } else if (g_fileDragPins) {
        UpdateInsertion(L, mx, true);
        effect = DROPEFFECT_LINK & g_dropAllowed;
    } else if (under && under->kind == Kind::App) {
        target = under;
        ClearInsertion();
    } else {
        ClearInsertion();
    }
    if (target != g_fwdItem) {
        ReleaseForward(nullptr);
        if (target) {
            g_fwd = ShellDropTargetFor(target);
            g_fwdItem = target;
            if (g_fwd) {
                DWORD e = g_dropAllowed;
                if (FAILED(g_fwd->DragEnter(g_dropData, keys, ptl, &e))) {
                    g_fwd->Release();
                    g_fwd = nullptr;
                }
            }
        }
    }
    if (target) {
        if (g_fwd) {
            effect = g_dropAllowed;
            if (FAILED(g_fwd->DragOver(keys, ptl, &effect))) effect = DROPEFFECT_NONE;
        } else if (target->kind == Kind::Trash) {
            effect = DROPEFFECT_MOVE & g_dropAllowed;
        } else if (!target->exe.empty() || !target->aumid.empty()) {
            effect = DROPEFFECT_COPY & g_dropAllowed;
        }
    }
    g_fileTarget = effect != DROPEFFECT_NONE ? target : nullptr;
    for (auto& it : g_items) it->hilite.SetTarget(it.get() == g_fileTarget ? 1 : 0);
    g_labelItem = under;
    g_labelA.SetTarget(under && g_set.labels ? 1 : 0);
    Kick();
    return effect;
}

static void EndFileDrag() {
    g_fileDrag = false;
    g_fileTarget = nullptr;
    for (auto& it : g_items) it->hilite.SetTarget(0);
    ClearInsertion();
    if (g_dropData) g_dropData->Release();
    g_dropData = nullptr;
    Kick();
}

class DockDropTarget : public IDropTarget {
   public:
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppv) override {
        if (riid == IID_IUnknown || riid == IID_IDropTarget) {
            *ppv = static_cast<IDropTarget*>(this);
            AddRef();
            return S_OK;
        }
        *ppv = nullptr;
        return E_NOINTERFACE;
    }
    ULONG STDMETHODCALLTYPE AddRef() override { return ++m_ref; }
    ULONG STDMETHODCALLTYPE Release() override { return --m_ref; }

    HRESULT STDMETHODCALLTYPE DragEnter(IDataObject* data, DWORD keys, POINTL pt, DWORD* effect) override {
        g_dropFiles = FilesFrom(data);
        if (g_dropFiles.empty()) {
            *effect = DROPEFFECT_NONE;
            return S_OK;
        }
        if (g_dropData) g_dropData->Release();
        g_dropData = data;
        g_dropData->AddRef();
        g_dropAllowed = *effect;
        g_fileDrag = true;
        g_fileDragPins = std::all_of(g_dropFiles.begin(), g_dropFiles.end(), [](const std::wstring& f) {
            return EndsWithI(f, L".exe") || EndsWithI(f, L".lnk");
        });
        if (!g_hover) g_anchor.Snap(pt.x - g_winX);
        g_hover = true;
        g_mag.SetTarget(1);
        *effect = UpdateFileDrag(keys, pt);
        return S_OK;
    }
    HRESULT STDMETHODCALLTYPE DragOver(DWORD keys, POINTL pt, DWORD* effect) override {
        if (!g_fileDrag) {
            *effect = DROPEFFECT_NONE;
            return S_OK;
        }
        g_dropAllowed = *effect;
        *effect = UpdateFileDrag(keys, pt);
        return S_OK;
    }
    HRESULT STDMETHODCALLTYPE DragLeave() override {
        ReleaseForward(nullptr);
        EndFileDrag();
        return S_OK;
    }
    HRESULT STDMETHODCALLTYPE Drop(IDataObject* data, DWORD keys, POINTL pt, DWORD* effect) override {
        if (!g_fileDrag) {
            *effect = DROPEFFECT_NONE;
            return S_OK;
        }
        g_dropAllowed = *effect;
        DWORD e = UpdateFileDrag(keys, pt);
        auto files = g_dropFiles;
        DockItem* target = g_fwdItem;
        if (g_fwd) {
            // El Explorador hace el trabajo (y su "deshacer").
            IDropTarget* fwd = g_fwd;
            g_fwd = nullptr;
            g_fwdItem = nullptr;
            DWORD fe = e;
            fwd->Drop(data, keys, pt, &fe);
            fwd->Release();
            *effect = fe;
            if (target && target->kind == Kind::Trash) QueryTrashAsync();
            if (target && target->kind == Kind::App && g_set.bounceLaunch) StartBounce(*target, 0.32, 1, false);
        } else if (g_fileDragPins && !(target && target->kind == Kind::Trash)) {
            int to = g_insertBefore ? IndexOf(g_insertBefore) : PinnedEnd();
            for (auto& f : files) {
                auto it = MakePinFromPath(f);
                bool dup = false;
                for (auto& o : g_items) dup = dup || (o->kind == Kind::App && o->pinned && SamePin(*o, *it));
                if (dup) continue;
                it->pinned = true;
                it->presence.Snap(0);
                it->presence.SetTarget(1);
                RequestIcon(*it);
                to = std::clamp(to, 0, (int)g_items.size());
                g_items.insert(g_items.begin() + to, std::move(it));
                to++;
            }
            SavePins();
            RequestScan();
            *effect = DROPEFFECT_LINK & g_dropAllowed;
        } else if (target && target->kind == Kind::Trash) {
            auto* job = new FileJob;
            job->kind = 1;
            job->files = files;
            RunFileJob(job);
            // Ya lo hicimos nosotros: que el origen no intente moverlos.
            *effect = DROPEFFECT_NONE;
        } else if (target && target->kind == Kind::App) {
            auto* job = new FileJob;
            job->files = files;
            if (target->type == 'A') job->aumid = target->aumid;
            else if (!target->exe.empty()) job->exe = target->exe;
            else job->aumid = target->aumid;
            if (g_set.bounceLaunch) StartBounce(*target, 0.32, 1, false);
            RunFileJob(job);
            *effect = DROPEFFECT_COPY & g_dropAllowed;
        } else {
            *effect = DROPEFFECT_NONE;
        }
        EndFileDrag();
        return S_OK;
    }

   private:
    ULONG m_ref = 1;
};

static DockDropTarget g_dropTarget;
static bool g_dropRegistered = false;

// ---------------------------------------------------------------------------
// Monitor, ventana, barra de tareas y AppBar
// ---------------------------------------------------------------------------

static void UpdateMonitor() {
    POINT origin{0, 0};
    g_monitor = MonitorFromPoint(origin, MONITOR_DEFAULTTOPRIMARY);
    MONITORINFOEXW mi{};
    mi.cbSize = sizeof(mi);
    if (GetMonitorInfoW(g_monitor, &mi)) {
        g_mon = mi.rcMonitor;
        g_monDevice = mi.szDevice;
    } else {
        g_mon = RECT{0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)};
        g_monDevice.clear();
    }
    UINT dx = 96, dy = 96;
    if (SUCCEEDED(GetDpiForMonitor(g_monitor, MDT_EFFECTIVE_DPI, &dx, &dy)) && dx) g_dpi = dx;
    {
        std::lock_guard<std::mutex> lock(g_vsyncMx);
        g_vsyncDevice = g_monDevice;
    }
    g_vsyncReopen = true;
}

static void PlaceWindow() {
    UpdateMetrics();
    int w = g_mon.right - g_mon.left;
    int h = DockAreaHeight();
    bool resize = w != g_winW || h != g_winH;
    g_winX = g_mon.left;
    g_winY = g_mon.bottom - h;
    g_winW = w;
    g_winH = h;
    if (resize || !g_bits) {
        CreateBuffers();
        g_envL = g_envR = 0;
    }
    g_fullRedraw = true;
}

static bool g_tbHidden = false;
static LPARAM g_tbPrevState = 0;

static BOOL CALLBACK HideTrayProc(HWND h, LPARAM lParam) {
    std::wstring cls = ClassOf(h);
    if (cls == L"Shell_TrayWnd" || cls == L"Shell_SecondaryTrayWnd") {
        DWORD pid = 0;
        GetWindowThreadProcessId(h, &pid);
        if (pid == GetCurrentProcessId()) {
            bool hide = lParam != 0;
            if (hide && IsWindowVisible(h)) ShowWindowAsync(h, SW_HIDE);
            if (!hide && !IsWindowVisible(h)) ShowWindowAsync(h, SW_SHOWNA);
        }
    }
    return TRUE;
}

static void EnsureTaskbarAutoHide() {
    if (!g_tbHidden) return;
    HWND tray = FindWindowW(L"Shell_TrayWnd", nullptr);
    if (!tray) return;
    APPBARDATA abd{sizeof(abd)};
    abd.hWnd = tray;
    if (!(SHAppBarMessage(ABM_GETSTATE, &abd) & ABS_AUTOHIDE)) {
        abd.lParam = g_tbPrevState | ABS_AUTOHIDE;
        SHAppBarMessage(ABM_SETSTATE, &abd);
    }
    EnumWindows(HideTrayProc, 1);
}

static void ApplyTaskbar(bool hide) {
    HWND tray = FindWindowW(L"Shell_TrayWnd", nullptr);
    if (!tray) return;
    APPBARDATA abd{sizeof(abd)};
    abd.hWnd = tray;
    if (hide && !g_tbHidden) {
        g_tbPrevState = (LPARAM)SHAppBarMessage(ABM_GETSTATE, &abd);
        abd.lParam = g_tbPrevState | ABS_AUTOHIDE;
        SHAppBarMessage(ABM_SETSTATE, &abd);
        Wh_SetIntValue(L"tbPrevState", (int)g_tbPrevState);
        Wh_SetIntValue(L"tbHidden", 1);
        g_tbHidden = true;
    } else if (!hide && g_tbHidden) {
        abd.lParam = g_tbPrevState;
        SHAppBarMessage(ABM_SETSTATE, &abd);
        Wh_SetIntValue(L"tbHidden", 0);
        g_tbHidden = false;
    }
    EnumWindows(HideTrayProc, hide ? 1 : 0);
}

static bool g_appBar = false;
static RECT g_appBarRect{};

static void UpdateAppBar(bool force = false) {
    HWND hwnd = g_wnd.load();
    if (!hwnd) return;
    bool want = g_set.autoHide == AutoHide::Never;
    APPBARDATA abd{sizeof(abd)};
    abd.hWnd = hwnd;
    if (!want) {
        if (g_appBar) {
            SHAppBarMessage(ABM_REMOVE, &abd);
            g_appBar = false;
            g_appBarRect = RECT{};
        }
        return;
    }
    if (!g_appBar) {
        abd.uCallbackMessage = WM_APP_APPBAR;
        g_appBar = SHAppBarMessage(ABM_NEW, &abd) != 0;
        if (!g_appBar) return;
        force = true;
    }
    int h = (int)std::ceil(g_m.bgH + g_m.margin);
    RECT rc{g_mon.left, g_mon.bottom - h, g_mon.right, g_mon.bottom};
    if (!force && EqualRect(&rc, &g_appBarRect)) return;
    g_appBarRect = rc;
    abd.uEdge = ABE_BOTTOM;
    abd.rc = rc;
    SHAppBarMessage(ABM_QUERYPOS, &abd);
    abd.rc.top = abd.rc.bottom - h;
    SHAppBarMessage(ABM_SETPOS, &abd);
}

static void RemoveAppBar() {
    HWND hwnd = g_wnd.load();
    if (!g_appBar || !hwnd) return;
    APPBARDATA abd{sizeof(abd)};
    abd.hWnd = hwnd;
    SHAppBarMessage(ABM_REMOVE, &abd);
    g_appBar = false;
    g_appBarRect = RECT{};
}

// --- Pantalla completa y ventanas maximizadas ---
static bool g_fgMaximized = false;
static bool g_fgFullscreen = false;
static bool g_exclusive = false;
static bool g_revealed = false;
static double g_edgeSince = -1;
static double g_lastInside = 0;
static bool g_windowHidden = false;
static double g_lastQuns = 0;

static bool IsShellClass(const std::wstring& cls) {
    static const wchar_t* kClasses[] = {
        L"Progman", L"WorkerW", L"Shell_TrayWnd", L"Shell_SecondaryTrayWnd", L"Windows.UI.Core.CoreWindow",
        L"XamlExplorerHostIslandWindow", L"TopLevelWindowForOverflowXamlIsland", L"MultitaskingViewFrame",
        L"ForegroundStaging", L"Shell_InputSwitchTopLevelWindow", L"NotifyIconOverflowWindow",
        L"WindhawkTopNotchWidget", L"#32768"};
    for (auto* c : kClasses) {
        if (cls == c) return true;
    }
    return cls == kDockClass || cls == kMenuHostClass || cls == kDragClass || cls == L"WindhawkAppleDockPopup";
}

// ¿Hay una ventana maximizada a la vista en el monitor? (la de arriba de
// todo que no esté minimizada; la que tiene el foco cuenta primero)
static BOOL CALLBACK TopVisibleProc(HWND h, LPARAM lParam) {
    if (!IsWindowVisible(h) || IsIconic(h) || IsCloaked(h)) return TRUE;
    LONG_PTR ex = GetWindowLongPtrW(h, GWL_EXSTYLE);
    if (ex & (WS_EX_TOOLWINDOW | WS_EX_NOACTIVATE)) return TRUE;
    if (GetWindow(h, GW_OWNER) && !(ex & WS_EX_APPWINDOW)) return TRUE;
    std::wstring cls = ClassOf(h);
    if (IsShellClass(cls)) {
        if (cls == L"Progman" || cls == L"WorkerW") {
            *(HWND*)lParam = nullptr;
            return FALSE;
        }
        return TRUE;
    }
    if (MonitorFromWindow(h, MONITOR_DEFAULTTONULL) != g_monitor) return TRUE;
    *(HWND*)lParam = h;
    return FALSE;
}

static void UpdateScreenState() {
    double now = Now();
    if (now - g_lastQuns > 0.5) {
        g_lastQuns = now;
        QUERY_USER_NOTIFICATION_STATE qs{};
        g_exclusive = SUCCEEDED(SHQueryUserNotificationState(&qs)) &&
                      (qs == QUNS_RUNNING_D3D_FULL_SCREEN || qs == QUNS_PRESENTATION_MODE);
    }
    HWND fg = GetForegroundWindow();
    std::wstring fgClass = fg ? ClassOf(fg) : L"";
    bool desktop = !fg || fg == GetDesktopWindow() || fg == GetShellWindow() || fgClass == L"#32769" ||
                   fgClass == L"Progman" || fgClass == L"WorkerW";
    if (desktop) {
        g_fgMaximized = false;
        g_fgFullscreen = false;
        return;
    }
    if (IsShellClass(fgClass)) return;  // Inicio, Alt+Tab, menús: como estaba
    HWND top = fg;
    if (MonitorFromWindow(fg, MONITOR_DEFAULTTONULL) != g_monitor || IsIconic(fg) || IsCloaked(fg)) {
        top = nullptr;
        EnumWindows(TopVisibleProc, (LPARAM)&top);
    }
    if (!top) {
        g_fgMaximized = false;
        g_fgFullscreen = false;
        return;
    }
    RECT r{};
    GetWindowRect(top, &r);
    LONG_PTR style = GetWindowLongPtrW(top, GWL_STYLE);
    bool covers = r.left <= g_mon.left && r.top <= g_mon.top && r.right >= g_mon.right && r.bottom >= g_mon.bottom;
    g_fgFullscreen = covers && (style & WS_CAPTION) != WS_CAPTION;
    // Maximizada, o una ventana que llega hasta abajo de la pantalla a lo
    // ancho (las que se "maximizan" a mano o por Snap).
    RECT work{};
    MONITORINFO mi{sizeof(mi)};
    GetMonitorInfoW(g_monitor, &mi);
    work = mi.rcWork;
    bool fills = r.left <= work.left + 8 && r.right >= work.right - 8 && r.bottom >= work.bottom - 8 &&
                 r.top <= work.top + 8;
    g_fgMaximized = IsZoomed(top) != FALSE || fills;
}

static bool WantHidden() {
    if (g_drag || g_menuOpen || g_fileDrag || g_revealed || PopupOpen()) return false;
    switch (g_set.autoHide) {
        case AutoHide::Always:
            return true;
        case AutoHide::Maximized:
            return g_fgMaximized || g_fgFullscreen;
        default:
            return g_fgFullscreen;
    }
}

static void ApplyVisibility() {
    HWND hwnd = g_wnd.load();
    if (!hwnd) return;
    bool hideWindow = g_exclusive;
    if (hideWindow != g_windowHidden) {
        g_windowHidden = hideWindow;
        ShowWindow(hwnd, hideWindow ? SW_HIDE : SW_SHOWNOACTIVATE);
        if (!hideWindow) {
            SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
            g_fullRedraw = true;
        }
    }
    double target = WantHidden() ? 1.0 : 0.0;
    if (g_hide.target != target) {
        g_hide.SetTarget(target);
        Kick();
    }
}

// ---------------------------------------------------------------------------
// Mouse: entrar, salir, nombre, borde de abajo
// ---------------------------------------------------------------------------

static UINT g_pollMs = 0;

static void SetPollRate(UINT ms) {
    HWND hwnd = g_wnd.load();
    if (!hwnd || ms == g_pollMs) return;
    g_pollMs = ms;
    SetTimer(hwnd, TIMER_POLL, ms, nullptr);
}

static void UpdateLabelTarget(bool snap) {
    if (!g_labelItem) return;
    int idx = IndexOf(g_labelItem);
    if (idx < 0 || idx >= (int)g_lastLayout.g.size()) return;
    double cx = g_lastLayout.g[idx].cx + g_labelItem->flyX.x;
    if (snap || g_labelA.x < 0.05) g_labelX.Snap(cx);
    else g_labelX.SetTarget(cx);
}

static void UpdateHover(POINT screenPt) {
    double now = Now();
    double mx = screenPt.x - g_winX, my = screenPt.y - g_winY;
    const Layout& L = g_lastLayout;
    bool visible = g_hide.x < 0.5 && !g_windowHidden;
    double top = g_hover ? L.topMost - S(10) : L.bgT;
    double tol = g_hover ? std::max((double)S(4), L.base * 0.35) : S(2);
    double left = std::min(L.bgL, L.baseL) - tol;
    double right = std::max(L.bgR, L.baseR) + tol;
    bool inside = visible && mx >= left && mx <= right && my >= top && screenPt.y <= g_mon.bottom;
    if (g_drag || g_press) inside = true;
    // La ventana emergente cuenta como parte del Dock.
    bool inPopup = CursorInPopup(screenPt);

    if (g_hide.target > 0.5 && !g_windowHidden) {
        bool atEdge = screenPt.y >= g_mon.bottom - 2 && screenPt.x >= g_mon.left && screenPt.x < g_mon.right &&
                      mx >= L.baseL - L.base * 2 && mx <= L.baseR + L.base * 2;
        if (atEdge) {
            if (g_edgeSince < 0) g_edgeSince = now;
            if (now - g_edgeSince >= g_set.revealDelay) {
                g_revealed = true;
                g_lastInside = now;
                g_edgeSince = -1;
                ApplyVisibility();
            }
        } else {
            g_edgeSince = -1;
        }
    }
    if (g_revealed) {
        if (inside || inPopup || g_menuOpen || g_fileDrag) g_lastInside = now;
        else if (now - g_lastInside > g_set.hideDelay) {
            g_revealed = false;
            ApplyVisibility();
        }
    }

    bool hover = inside || inPopup || g_fileDrag || (g_menuOpen && g_hover);
    if (hover != g_hover) {
        if (hover) {
            // Al entrar, la curva arranca donde está el mouse (sin barrido), y
            // el Dock se asegura de estar encima de todo.
            g_anchor.Snap(mx);
            SetWindowPos(g_wnd.load(), HWND_TOPMOST, 0, 0, 0, 0,
                         SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_NOOWNERZORDER);
        }
        g_hover = hover;
        g_mag.SetTarget(hover ? 1 : 0);
        Kick();
    }

    // Vista de las ventanas de la app bajo el mouse.
    if (g_pop.kind == PopKind::Preview && !g_pop.closing) {
        if (inPopup) {
            g_pop.leaveSince = -1;
        } else if (inside && !g_drag && !g_press) {
            g_pop.leaveSince = -1;
            int i = ItemAtX(L, mx);
            DockItem* it = i >= 0 ? g_items[i].get() : nullptr;
            if (it && it->uid != g_pop.itemUid) {
                if (it->kind == Kind::App && !it->windows.empty()) OpenPreview(it);
                else ClosePopup(false);
            }
        } else {
            if (g_pop.leaveSince < 0) g_pop.leaveSince = now;
            if (now - g_pop.leaveSince > 0.3) ClosePopup(false);
        }
    } else if (g_set.previews != Previews::Off && inside && !inPopup && !g_drag && !g_press && !g_menuOpen &&
               !g_fileDrag && g_hide.x < 0.1) {
        int i = ItemAtX(L, mx);
        DockItem* it = i >= 0 ? g_items[i].get() : nullptr;
        if (it && it->uid != g_prevSuppress) g_prevSuppress = 0;
        if (!it) g_prevSuppress = 0;
        if (!(it && it->kind == Kind::App && !it->windows.empty())) it = nullptr;
        if (it && it->uid == g_prevSuppress) it = nullptr;
        if (it != g_prevCand) {
            g_prevCand = it;
            g_prevSince = now;
        } else if (it && now - g_prevSince >= g_set.previewDelay) {
            OpenPreview(it);
        }
    } else {
        g_prevCand = nullptr;
        if (!inside && !inPopup) g_prevSuppress = 0;
    }

    if (hover && !g_menuOpen && !g_fileDrag && !inPopup) {
        if (std::fabs(g_mouseX - mx) > 0.01 || std::fabs(g_mouseY - my) > 0.01) {
            g_mouseX = mx;
            g_mouseY = my;
            g_anchor.SetTarget(mx);
            Kick();
        }
    }
    if (hover && !g_menuOpen && !g_fileDrag && !inPopup && !PopupOpen()) {
        int i = ItemAtX(L, mx);
        DockItem* item = (i >= 0) ? g_items[i].get() : nullptr;
        if (g_drag || (g_press && !g_drag)) item = nullptr;
        if (item && item != g_labelItem) {
            bool wasHidden = g_labelA.x < 0.05;
            g_labelItem = item;
            UpdateLabelTarget(wasHidden);
        }
        double la = (item && g_set.labels) ? 1 : 0;
        if (g_labelA.target != la) {
            g_labelA.SetTarget(la);
            Kick();
        }
    } else if (!hover && g_labelA.target != 0) {
        g_labelA.SetTarget(0);
        Kick();
    }
    SetPollRate((hover || g_revealed || g_hide.target > 0.5) ? 30 : 120);
}

// ---------------------------------------------------------------------------
// Elementos fijos e iconos
// ---------------------------------------------------------------------------

static void RebuildFixedItems() {
    for (size_t i = 0; i < g_items.size();) {
        if (g_items[i]->kind != Kind::App) {
            ReleaseForward(g_items[i].get());
            ClearRefs(g_items[i].get());
            g_items.erase(g_items.begin() + i);
            continue;
        }
        i++;
    }
    if (g_set.showStart) {
        auto s = NewItem(Kind::Start);
        s->iconKey = L"::start";
        g_items.insert(g_items.begin(), std::move(s));
    }
    if (g_set.showTrash) {
        g_items.push_back(NewItem(Kind::Sep));
        auto t = NewItem(Kind::Trash);
        t->iconKey = g_trashFull ? L"::trash1" : L"::trash0";
        g_items.push_back(std::move(t));
    }
    g_items.push_back(NewItem(Kind::End));
}

static void ReloadAllIcons() {
    g_icons.clear();
    for (auto& it : g_items) {
        if (it->kind == Kind::App) RequestIcon(*it);
        else RequestFixedIcon(*it);
    }
}

static void RequestGlass(bool force) {
    if (g_set.style != DockStyle::Glass) {
        g_glass.clear();
        return;
    }
    Job j;
    j.kind = JobKind::Glass;
    j.winW = g_winW;
    j.winH = g_winH;
    j.mon = g_mon;
    j.light = g_light;
    j.force = force;
    PostJob(std::move(j));
}

static void ApplyJobResults() {
    std::deque<JobResult> results;
    {
        std::lock_guard<std::mutex> lock(g_jobMx);
        results.swap(g_results);
    }
    bool redraw = false;
    for (auto& r : results) {
        if (r.kind == JobKind::Glass) {
            if (r.glass && r.winW == g_winW && r.winH == g_winH) {
                g_glass = std::move(*r.glass);
                redraw = true;
            }
            continue;
        }
        IconEntry& e = g_icons[r.key];
        e.px = IconPx();
        if (r.set) {
            e.set = r.set;
            e.state = 2;
        } else if (!e.set) {
            e.state = 3;
        }
        redraw = true;
        if (!r.name.empty()) {
            for (auto& it : g_items) {
                if (it->iconKey == r.key && !it->nameFinal) {
                    it->name = r.name;
                    it->nameFinal = true;
                }
            }
        }
    }
    if (redraw) {
        g_fullRedraw = true;
        Kick();
    }
}

static void ApplySettings(bool first) {
    int oldPx = IconPx();
    std::wstring oldStart = g_set.startIcon;
    bool oldVectorTrash = g_set.vectorTrash;
    LoadSettings();
    g_light = ReadThemeIsLight();
    UpdateMonitor();
    PlaceWindow();
    RecreateFont();
    g_labelCache.clear();
    ClosePopup(true);
    RebuildFixedItems();
    if (first || oldPx != IconPx()) {
        ReloadAllIcons();
    } else {
        if (oldStart != g_set.startIcon) g_icons.erase(L"::start");
        if (oldVectorTrash != g_set.vectorTrash) {
            g_icons.erase(L"::trash0");
            g_icons.erase(L"::trash1");
        }
        for (auto& it : g_items) {
            if (it->kind != Kind::App) RequestFixedIcon(*it);
        }
    }
    RequestGlass(true);
    ApplyTaskbar(g_set.hideTaskbar);
    UpdateAppBar();
    UpdateScreenState();
    ApplyVisibility();
    g_hide.Snap(g_hide.target);
    g_lastLayout = ComputeLayout(Now());
    g_fullRedraw = true;
    Kick();
}

// ---------------------------------------------------------------------------
// Ventana del Dock
// ---------------------------------------------------------------------------

static HWINEVENTHOOK g_showHook = nullptr;
static HWINEVENTHOOK g_fgHook = nullptr;
static HWINEVENTHOOK g_minHook = nullptr;

static void CALLBACK WinEventProc(HWINEVENTHOOK, DWORD event, HWND h, LONG idObject, LONG, DWORD, DWORD) {
    HWND dock = g_wnd.load();
    if (!dock) return;
    if (event == EVENT_OBJECT_SHOW && idObject == OBJID_WINDOW && h && g_tbHidden) {
        std::wstring cls = ClassOf(h);
        if (cls == L"Shell_TrayWnd" || cls == L"Shell_SecondaryTrayWnd") PostMessageW(dock, WM_APP_TRAYSHOW, 0, 0);
    } else if (event == EVENT_SYSTEM_FOREGROUND || event == EVENT_SYSTEM_MINIMIZEEND ||
               event == EVENT_SYSTEM_MINIMIZESTART || event == EVENT_SYSTEM_MOVESIZEEND) {
        UpdateScreenState();
        ApplyVisibility();
        static HWND s_lastFg = nullptr;
        if (event == EVENT_SYSTEM_FOREGROUND) {
            SetTimer(dock, TIMER_TOPMOST, 250, nullptr);
            // La que deja el frente sigue a la vista: buena foto.
            if (s_lastFg && s_lastFg != h) QueueSnapshot(s_lastFg);
            s_lastFg = h;
        } else if (event == EVENT_SYSTEM_MOVESIZEEND) {
            QueueSnapshot(h);
        }
    }
}

// Coordenadas del lienzo a partir de las del cliente, con la posición real
// de la ventana (no la que se supone).
static POINT ClientToCanvas(LPARAM lParam) {
    POINT pt{GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
    ClientToScreen(g_wnd.load(), &pt);
    return POINT{pt.x - g_winX, pt.y - g_winY};
}

static LRESULT CALLBACK DockWndProcImpl(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

static LRESULT CALLBACK DockWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (!g_profileOn) return DockWndProcImpl(hwnd, msg, wParam, lParam);
    double t0 = Now();
    LRESULT r = DockWndProcImpl(hwnd, msg, wParam, lParam);
    double ms = (Now() - t0) * 1000;
    UINT key = msg == WM_TIMER ? 0x10000 + (UINT)wParam : msg;
    auto& e = g_profileMax[key];
    e.first = std::max(e.first, ms);
    if (ms > 4) e.second++;
    return r;
}

static LRESULT CALLBACK DockWndProcImpl(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (g_msgShellHook && msg == g_msgShellHook) {
        HWND h = (HWND)lParam;
        switch (wParam) {
            case HSHELL_FLASH:
                if (g_set.bounceAttention) {
                    for (auto& it : g_items) {
                        if (it->kind == Kind::App && OwnsWindow(*it, h)) {
                            StartBounce(*it, 0.48, 2, false);
                            break;
                        }
                    }
                }
                break;
            case HSHELL_WINDOWCREATED:
            case HSHELL_WINDOWDESTROYED:
            case HSHELL_WINDOWACTIVATED:
            case HSHELL_RUDEAPPACTIVATED:
            case HSHELL_WINDOWREPLACED:
                RequestScan();
                break;
        }
        return 0;
    }
    if (g_msgTaskbarCreated && msg == g_msgTaskbarCreated) {
        if (g_set.hideTaskbar) EnsureTaskbarAutoHide();
        if (g_appBar) UpdateAppBar(true);
        return 0;
    }

    switch (msg) {
        case WM_APP_FRAME:
            OnFrame((LONGLONG)lParam);
            return 0;

        case WM_APP_JOBS:
            ApplyJobResults();
            return 0;

        case WM_APP_WINDOWS:
            TakeScanResult();
            return 0;

        case WM_APP_SNAP:
            TakeSnapshots();
            return 0;

        case WM_APP_TRASH: {
            if (wParam == 2) {
                QueryTrashAsync();
                return 0;
            }
            bool full = wParam != 0;
            if (full != g_trashFull) {
                g_trashFull = full;
                for (auto& it : g_items) {
                    if (it->kind == Kind::Trash) {
                        it->iconKey = full ? L"::trash1" : L"::trash0";
                        RequestFixedIcon(*it);
                    }
                }
                g_fullRedraw = true;
                Kick();
            }
            return 0;
        }

        case WM_APP_LAUNCHFAIL: {
            DockItem* it = FindByUid((uint64_t)lParam);
            if (it) {
                it->launching = false;
                it->bounceCycles = 0;
            }
            return 0;
        }

        case WM_APP_TRAYSHOW:
            if (g_tbHidden) EnumWindows(HideTrayProc, 1);
            return 0;

        case WM_APP_APPBAR:
            if (wParam == ABN_POSCHANGED) UpdateAppBar();
            if (wParam == ABN_FULLSCREENAPP) {
                UpdateScreenState();
                ApplyVisibility();
            }
            return 0;

        case WM_APP_SETTINGS:
            ApplySettings(false);
            RequestScan();
            return 0;

        case WM_TIMER:
            switch (wParam) {
                case TIMER_POLL: {
                    POINT pt;
                    GetCursorPos(&pt);
                    if (g_press || g_drag) {
                        int vk = GetSystemMetrics(SM_SWAPBUTTON) ? VK_RBUTTON : VK_LBUTTON;
                        if (!(GetAsyncKeyState(vk) & 0x8000)) {
                            g_dbgPollRelease++;
                            DockItem* pressed = g_press;
                            g_press = nullptr;
                            g_mouseX = pt.x - g_winX;
                            g_mouseY = pt.y - g_winY;
                            if (g_drag) {
                                EndDrag(false);
                            } else if (pressed) {
                                // Se soltó sin que llegara el mensaje: si sigue
                                // encima del mismo icono, es un clic.
                                int i = ItemAt(g_lastLayout, g_mouseX, g_mouseY);
                                if (i >= 0 && g_items[i].get() == pressed) {
                                    OnItemClick(*pressed, (GetKeyState(VK_SHIFT) & 0x8000) != 0);
                                }
                            }
                            if (GetCapture() == hwnd) ReleaseCapture();
                            Kick();
                        }
                    }
                    UpdateHover(pt);
                    UpdateScreenState();
                    ApplyVisibility();
                    // Si una ventana de la vista se minimizó o volvió, rearmarla.
                    if (g_pop.kind == PopKind::Preview && !g_pop.closing) {
                        bool changed = false;
                        for (auto& e : g_pop.prev) changed = changed || (e.iconic != (IsIconic(e.hwnd) != FALSE));
                        if (changed) {
                            TakeSnapshots();
                            if (DockItem* it = FindByUid(g_pop.itemUid)) OpenPreview(it);
                        }
                    }
                    return 0;
                }
                case TIMER_TOPMOST:
                    KillTimer(hwnd, TIMER_TOPMOST);
                    if (!g_press && !g_drag && !g_menuOpen && !g_fileDrag) {
                        SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0,
                                     SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_NOOWNERZORDER);
                    }
                    return 0;
                case TIMER_SLOW: {
                    static int tick = 0;
                    tick++;
                    if (tick % 2 == 0) {
                        QueryTrashAsync();
                        QueueSnapshot(GetForegroundWindow());
                    }
                    if (tick % 20 == 0) PruneSnapshots();
                    if (g_tbHidden) EnumWindows(HideTrayProc, 1);
                    if (tick % 40 == 0) RequestGlass(false);
                    return 0;
                }
            }
            break;

        case WM_MOUSEACTIVATE:
            return MA_NOACTIVATE;

        case WM_MOUSEMOVE: {
            POINT pt{GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
            ClientToScreen(hwnd, &pt);
            UpdateHover(pt);
            double mx = pt.x - g_winX, my = pt.y - g_winY;
            g_mouseX = mx;
            g_mouseY = my;
            if (g_press && !g_drag && g_press->kind == Kind::App && !g_set.lockItems) {
                double dx = mx - g_pressPt.x, dy = my - g_pressPt.y;
                if (dx * dx + dy * dy > S(6) * S(6)) BeginDrag(g_press, g_lastLayout);
            }
            if (g_drag) {
                const Layout& L = g_lastLayout;
                int ti = ItemAt(L, mx, my);
                bool overTrash = ti >= 0 && g_items[ti]->kind == Kind::Trash;
                bool remove = !overTrash && my < L.bgT - std::max(S(70), (int)L.base);
                if (overTrash != g_dragOverTrash) {
                    g_dragOverTrash = overTrash;
                    SetTrashHilite(overTrash);
                }
                g_dragRemove = remove || overTrash;
                if (g_dragRemove) ClearInsertion();
                else UpdateInsertion(L, mx, false);
                UpdateDragWindow(Now());
                Kick();
            }
            return 0;
        }

        case WM_LBUTTONDOWN: {
            g_dbgDockDown++;
            POINT p = ClientToCanvas(lParam);
            int i = ItemAt(g_lastLayout, p.x, p.y);
            g_press = i >= 0 ? g_items[i].get() : nullptr;
            g_dbgDownIndex = i;
            g_dbgDownPt = p;
            g_pressPt = p;
            // Sin capturar el mouse: con otra app en primer plano la captura
            // es limitada y podía perder el clic. Solo se captura al arrastrar.
            if (g_press) g_labelA.SetTarget(0);
            Kick();
            return 0;
        }

        case WM_LBUTTONUP: {
            g_dbgUp++;
            DockItem* pressed = g_press;
            g_press = nullptr;
            if (g_drag) {
                EndDrag(false);
                if (GetCapture() == hwnd) ReleaseCapture();
                return 0;
            }
            if (GetCapture() == hwnd) ReleaseCapture();
            POINT p = ClientToCanvas(lParam);
            int i = ItemAt(g_lastLayout, p.x, p.y);
            if (pressed && i >= 0 && g_items[i].get() == pressed) {
                OnItemClick(*pressed, (GetKeyState(VK_SHIFT) & 0x8000) != 0);
            }
            Kick();
            return 0;
        }

        case WM_CAPTURECHANGED:
            if ((HWND)lParam != hwnd && g_drag) {
                EndDrag(true);
                g_press = nullptr;
                Kick();
            }
            return 0;

        case WM_MBUTTONUP: {
            POINT p = ClientToCanvas(lParam);
            int i = ItemAt(g_lastLayout, p.x, p.y);
            if (i < 0 || g_items[i]->kind != Kind::App) return 0;
            DockItem& it = *g_items[i];
            ClosePopup(true);
            if (g_set.middleClick == 0) LaunchItem(it, true);
            else if (g_set.middleClick == 1) for (HWND w : it.windows) PostMessageW(w, WM_CLOSE, 0, 0);
            return 0;
        }

        case WM_MOUSEWHEEL: {
            if (!g_set.scrollCycle || g_drag) return 0;
            POINT pt{GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};  // ya en pantalla
            int i = ItemAtX(g_lastLayout, pt.x - g_winX);
            if (i < 0 || g_items[i]->kind != Kind::App || g_items[i]->windows.empty()) return 0;
            DockItem& it = *g_items[i];
            ClosePopup(true);
            short delta = GET_WHEEL_DELTA_WPARAM(wParam);
            if (delta < 0) {
                // Abajo: minimizar la ventana de la app que está al frente.
                HWND fg = GetForegroundWindow();
                if (OwnsWindow(it, fg) && !IsIconic(fg)) MinimizeWindow(fg);
                else for (HWND w : it.windows) if (!IsIconic(w)) { MinimizeWindow(w); break; }
            } else {
                // Arriba: primero vuelve una minimizada; si no hay, pasa a la
                // siguiente ventana (la más vieja de la lista).
                HWND next = nullptr;
                for (HWND w : it.windows) {
                    if (IsIconic(w)) {
                        next = w;
                        break;
                    }
                }
                if (!next) {
                    HWND fg = GetForegroundWindow();
                    next = OwnsWindow(it, fg) && it.windows.size() > 1 ? it.windows.back() : it.windows.front();
                }
                ActivateWindow(next);
            }
            return 0;
        }

        case WM_RBUTTONUP: {
            if (g_drag) return 0;
            POINT p = ClientToCanvas(lParam);
            int i = ItemAt(g_lastLayout, p.x, p.y);
            DockItem* it = i >= 0 ? g_items[i].get() : nullptr;
            double ax = it ? g_winX + g_lastLayout.g[i].cx : g_winX + p.x;
            OpenMenu(it, ax);
            return 0;
        }

        case WM_APP_POPCLOSE:
            ClosePopup(false);
            return 0;

        case WM_APP_POPKEY:
            MenuKey((UINT)wParam);
            return 0;

        case WM_DISPLAYCHANGE:
        case WM_DPICHANGED:
            ApplySettings(false);
            return 0;

        case WM_SETTINGCHANGE:
            if (lParam && !wcscmp((LPCWSTR)lParam, L"ImmersiveColorSet")) {
                bool light = ReadThemeIsLight();
                if (light != g_light) {
                    g_light = light;
                    g_labelCache.clear();
                    RequestGlass(true);
                    g_fullRedraw = true;
                    Kick();
                }
            }
            if (wParam == SPI_SETDESKWALLPAPER) RequestGlass(true);
            if (wParam == SPI_SETWORKAREA && g_set.hideTaskbar) EnsureTaskbarAutoHide();
            return 0;

        case WM_APP_EXIT:
            DestroyWindow(hwnd);
            return 0;

        case WM_DESTROY: {
            for (UINT_PTR t : {TIMER_POLL, TIMER_SLOW, TIMER_TOPMOST}) KillTimer(hwnd, t);
            DeregisterShellHookWindow(hwnd);
            RemovePopupHooks();
            ReleaseForward(nullptr);
            if (g_dropData) g_dropData->Release();
            g_dropData = nullptr;
            if (g_dropRegistered) RevokeDragDrop(hwnd);
            g_dropRegistered = false;
            RemoveAppBar();
            g_vsyncRun = false;
            g_wnd = nullptr;
            PostQuitMessage(0);
            return 0;
        }
    }
    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

// ---------------------------------------------------------------------------
// Hilo principal del mod
// ---------------------------------------------------------------------------

static DWORD WINAPI DockThread(LPVOID) {
    HWND tray = nullptr;
    for (int i = 0; i < 1200 && !g_unloading; i++) {
        tray = FindWindowW(L"Shell_TrayWnd", nullptr);
        if (tray) break;
        Sleep(100);
    }
    if (!tray || g_unloading) return 0;
    DWORD pid = 0;
    GetWindowThreadProcessId(tray, &pid);
    if (pid != GetCurrentProcessId()) return 0;

    SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
    HRESULT oleInit = OleInitialize(nullptr);
    LARGE_INTEGER f;
    QueryPerformanceFrequency(&f);
    g_qpcFreq = (double)f.QuadPart;

    Gdiplus::GdiplusStartupInput gdiplusInput;
    ULONG_PTR gdiplusToken = 0;
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusInput, nullptr);

    if (Wh_GetIntValue(L"tbHidden", 0)) {
        g_tbHidden = true;
        g_tbPrevState = (LPARAM)Wh_GetIntValue(L"tbPrevState", 0);
    }

    HINSTANCE hInstance = GetModuleHandleW(nullptr);
    WNDCLASSEXW wc{sizeof(wc)};
    wc.lpfnWndProc = DockWndProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursorW(nullptr, IDC_ARROW);
    wc.lpszClassName = kDockClass;
    RegisterClassExW(&wc);
    WNDCLASSEXW dc{sizeof(dc)};
    dc.lpfnWndProc = DefWindowProcW;
    dc.hInstance = hInstance;
    dc.lpszClassName = kDragClass;
    RegisterClassExW(&dc);
    WNDCLASSEXW pc{sizeof(pc)};
    pc.lpfnWndProc = PopupWndProc;
    pc.hInstance = hInstance;
    pc.hCursor = LoadCursorW(nullptr, IDC_ARROW);
    pc.lpszClassName = kPopupClass;
    RegisterClassExW(&pc);

    WNDCLASSEXW mc{sizeof(mc)};
    mc.lpfnWndProc = DefWindowProcW;
    mc.hInstance = hInstance;
    mc.lpszClassName = kMenuHostClass;
    RegisterClassExW(&mc);

    LoadSettings();
    UpdateMonitor();
    UpdateMetrics();
    g_winH = DockAreaHeight();
    g_winW = g_mon.right - g_mon.left;
    g_winX = g_mon.left;
    g_winY = g_mon.bottom - g_winH;

    HWND hwnd = CreateWindowExW(WS_EX_LAYERED | WS_EX_TOOLWINDOW | WS_EX_TOPMOST | WS_EX_NOACTIVATE, kDockClass,
                                L"Dock", WS_POPUP, g_winX, g_winY, 1, 1, nullptr, nullptr, hInstance, nullptr);
    g_menuHost = CreateWindowExW(WS_EX_TOOLWINDOW, kMenuHostClass, L"", WS_POPUP, 0, 0, 0, 0, nullptr, nullptr,
                                 hInstance, nullptr);
    if (!hwnd) {
        Wh_Log(L"No se pudo crear la ventana del Dock");
        if (g_menuHost) DestroyWindow(g_menuHost);
        g_menuHost = nullptr;
        UnregisterClassW(kDockClass, hInstance);
        UnregisterClassW(kMenuHostClass, hInstance);
        UnregisterClassW(kDragClass, hInstance);
    UnregisterClassW(L"WindhawkAppleDockPopup", hInstance);
        Gdiplus::GdiplusShutdown(gdiplusToken);
        if (SUCCEEDED(oleInit)) OleUninitialize();
        return 0;
    }
    g_wnd = hwnd;

    g_vsyncStop = false;
    g_vsyncWake = CreateEventW(nullptr, FALSE, FALSE, nullptr);
    g_vsyncThread = CreateThread(nullptr, 0, VsyncThread, nullptr, 0, nullptr);
    if (g_vsyncThread) SetThreadPriority(g_vsyncThread, THREAD_PRIORITY_TIME_CRITICAL);
    g_jobStop = false;
    g_jobEvent = CreateEventW(nullptr, FALSE, FALSE, nullptr);
    g_jobThread = CreateThread(nullptr, 0, JobThread, nullptr, 0, nullptr);
    if (g_jobThread) SetThreadPriority(g_jobThread, THREAD_PRIORITY_BELOW_NORMAL);
    g_scanStop = false;
    g_scanEvent = CreateEventW(nullptr, FALSE, FALSE, nullptr);
    g_scanThread = CreateThread(nullptr, 0, ScanThread, nullptr, 0, nullptr);
    g_snapStop = false;
    g_snapEvent = CreateEventW(nullptr, FALSE, FALSE, nullptr);
    g_snapThread = CreateThread(nullptr, 0, SnapThread, nullptr, 0, nullptr);
    if (g_snapThread) SetThreadPriority(g_snapThread, THREAD_PRIORITY_BELOW_NORMAL);

    for (auto& it : LoadPins()) {
        it->pinned = true;
        g_items.push_back(std::move(it));
    }
    g_mag.Snap(0);
    g_hide.Snap(0);
    g_labelA.Snap(0);
    ApplySettings(true);
    SavePins();
    QueryTrashAsync();

    // Primer listado de ventanas sin esperar al hilo (para no ver el Dock
    // "llenarse" al arrancar).
    {
        std::vector<WinInfo> wins;
        std::map<DWORD, ProcInfo> procs;
        std::map<DWORD, bool> seen;
        ScanCtx ctx{&wins, &procs, &seen};
        EnumWindows(ScanWindowsProc, (LPARAM)&ctx);
        ApplyWindows(wins);
        for (auto& it : g_items) {
            it->presence.Snap(it->removing ? 0 : 1);
            it->dot.Snap(it->windows.empty() ? 0 : 1);
        }
    }

    g_msgShellHook = RegisterWindowMessageW(L"SHELLHOOK");
    g_msgTaskbarCreated = RegisterWindowMessageW(L"TaskbarCreated");
    RegisterShellHookWindow(hwnd);
    g_dropRegistered = SUCCEEDED(RegisterDragDrop(hwnd, &g_dropTarget));

    g_showHook = SetWinEventHook(EVENT_OBJECT_SHOW, EVENT_OBJECT_SHOW, nullptr, WinEventProc,
                                 GetCurrentProcessId(), 0, WINEVENT_OUTOFCONTEXT);
    g_fgHook = SetWinEventHook(EVENT_SYSTEM_FOREGROUND, EVENT_SYSTEM_MOVESIZEEND, nullptr, WinEventProc, 0, 0,
                               WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS);
    g_minHook = SetWinEventHook(EVENT_SYSTEM_MINIMIZESTART, EVENT_SYSTEM_MINIMIZEEND, nullptr, WinEventProc, 0, 0,
                                WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS);

    g_lastLayout = ComputeLayout(Now());
    g_fullRedraw = true;
    Render(Now());
    ShowWindow(hwnd, SW_SHOWNOACTIVATE);
    SetPollRate(120);
    SetTimer(hwnd, TIMER_SLOW, 1500, nullptr);
    Kick();

    MSG m;
    while (GetMessageW(&m, nullptr, 0, 0) > 0) {
        TranslateMessage(&m);
        DispatchMessageW(&m);
    }

    // Limpieza
    for (HWINEVENTHOOK* h : {&g_showHook, &g_fgHook, &g_minHook}) {
        if (*h) UnhookWinEvent(*h);
        *h = nullptr;
    }
    ApplyTaskbar(false);

    g_vsyncStop = true;
    g_vsyncRun = false;
    if (g_vsyncWake) SetEvent(g_vsyncWake);
    if (g_vsyncThread) {
        WaitForSingleObject(g_vsyncThread, 2000);
        CloseHandle(g_vsyncThread);
    }
    if (g_vsyncWake) CloseHandle(g_vsyncWake);
    g_vsyncThread = g_vsyncWake = nullptr;

    g_jobStop = true;
    if (g_jobEvent) SetEvent(g_jobEvent);
    if (g_jobThread) {
        WaitForSingleObject(g_jobThread, 8000);
        CloseHandle(g_jobThread);
    }
    if (g_jobEvent) CloseHandle(g_jobEvent);
    g_jobThread = g_jobEvent = nullptr;
    {
        std::lock_guard<std::mutex> lock(g_jobMx);
        g_jobs.clear();
        g_results.clear();
    }
    g_glassKeyWorker.clear();

    g_snapStop = true;
    if (g_snapEvent) SetEvent(g_snapEvent);
    if (g_snapThread) {
        WaitForSingleObject(g_snapThread, 5000);
        CloseHandle(g_snapThread);
    }
    if (g_snapEvent) CloseHandle(g_snapEvent);
    g_snapThread = g_snapEvent = nullptr;
    {
        std::lock_guard<std::mutex> lock(g_snapMx);
        g_snapJobs.clear();
        g_snapDone.clear();
    }
    g_snaps.clear();

    g_scanStop = true;
    if (g_scanEvent) SetEvent(g_scanEvent);
    if (g_scanThread) {
        WaitForSingleObject(g_scanThread, 5000);
        CloseHandle(g_scanThread);
    }
    if (g_scanEvent) CloseHandle(g_scanEvent);
    g_scanThread = g_scanEvent = nullptr;
    {
        std::lock_guard<std::mutex> lock(g_scanMx);
        g_scanResult.clear();
        g_scanFresh = false;
    }

    g_press = g_drag = g_labelItem = g_insertBefore = g_fileTarget = g_menuItem = nullptr;
    ClosePopup(true);
    RemovePopupHooks();
    if (g_pop.wnd) DestroyWindow(g_pop.wnd);
    g_pop.wnd = nullptr;
    FreePopupBuffers();
    g_prevCand = nullptr;
    HideDragWindow();
    if (g_dragWnd) DestroyWindow(g_dragWnd);
    g_dragWnd = nullptr;
    FreeDragBuffers();
    g_items.clear();
    g_icons.clear();
    g_labelCache.clear();
    g_glass.clear();
    g_font.reset();
    g_fontBoldPtr.reset();
    g_fontSmallPtr.reset();
    g_family.reset();
    FreeBuffers();
    g_envL = g_envR = 0;
    if (g_menuHost) DestroyWindow(g_menuHost);
    g_menuHost = nullptr;
    UnregisterClassW(kDockClass, hInstance);
    UnregisterClassW(kMenuHostClass, hInstance);
    UnregisterClassW(kDragClass, hInstance);
    UnregisterClassW(L"WindhawkAppleDockPopup", hInstance);
    Gdiplus::GdiplusShutdown(gdiplusToken);
    if (SUCCEEDED(oleInit)) OleUninitialize();
    return 0;
}

BOOL Wh_ModInit() {
    Wh_Log(L"Init");
    g_unloading = false;
    g_thread = CreateThread(nullptr, 0, DockThread, nullptr, 0, nullptr);
    return g_thread != nullptr;
}

void Wh_ModUninit() {
    Wh_Log(L"Uninit");
    g_unloading = true;
    HWND hwnd = g_wnd.load();
    if (hwnd) PostMessageW(hwnd, WM_APP_EXIT, 0, 0);
    if (g_thread) {
        WaitForSingleObject(g_thread, 10000);
        CloseHandle(g_thread);
        g_thread = nullptr;
    }
}

void Wh_ModSettingsChanged() {
    Wh_Log(L"SettingsChanged");
    HWND hwnd = g_wnd.load();
    if (hwnd) PostMessageW(hwnd, WM_APP_SETTINGS, 0, 0);
}
