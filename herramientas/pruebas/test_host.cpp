// Banco de pruebas del Dock: carga el código del mod tal cual dentro de un
// proceso que imita a explorer.exe (con su propia ventana Shell_TrayWnd) y
// recorre escenarios con el mouse real (SendInput), comprobando el estado
// interno en el hilo del Dock.
#define NOMINMAX
#include <windows.h>
#include <cstdio>
#include <cstdarg>
#include <functional>
#include <map>
#include <string>
#include <vector>
#include <mutex>

// ---- API de Windhawk simulada ----
static std::map<std::wstring, std::wstring> g_testSettings;
static std::map<std::wstring, std::wstring> g_testStore;
static std::mutex g_testMx;
static int g_logCount = 0;

static void TestLog(const wchar_t* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    wchar_t buf[2048];
    _vsnwprintf(buf, 2047, fmt, ap);
    buf[2047] = 0;
    va_end(ap);
    fwprintf(stdout, L"[mod] %ls\n", buf);
    fflush(stdout);
    g_logCount++;
}
#define Wh_Log(message, ...) TestLog(L"[%d:%hs]: " message, __LINE__, __FUNCTION__, ##__VA_ARGS__)

static std::wstring SettingRaw(PCWSTR name) {
    std::lock_guard<std::mutex> l(g_testMx);
    auto it = g_testSettings.find(name);
    return it == g_testSettings.end() ? L"" : it->second;
}
int Wh_GetIntSetting(PCWSTR name, ...) { return _wtoi(SettingRaw(name).c_str()); }
PCWSTR Wh_GetStringSetting(PCWSTR name, ...) { return _wcsdup(SettingRaw(name).c_str()); }
void Wh_FreeStringSetting(PCWSTR s) { free((void*)s); }
int Wh_GetIntValue(PCWSTR name, int def) {
    std::lock_guard<std::mutex> l(g_testMx);
    auto it = g_testStore.find(name);
    return it == g_testStore.end() ? def : _wtoi(it->second.c_str());
}
BOOL Wh_SetIntValue(PCWSTR name, int v) {
    std::lock_guard<std::mutex> l(g_testMx);
    g_testStore[name] = std::to_wstring(v);
    return TRUE;
}
size_t Wh_GetStringValue(PCWSTR name, PWSTR buf, size_t n) {
    std::lock_guard<std::mutex> l(g_testMx);
    auto it = g_testStore.find(name);
    if (!n) return 0;
    if (it == g_testStore.end()) {
        buf[0] = 0;
        return 0;
    }
    size_t c = std::min(n - 1, it->second.size());
    wmemcpy(buf, it->second.c_str(), c);
    buf[c] = 0;
    return c;
}
BOOL Wh_SetStringValue(PCWSTR name, PCWSTR v) {
    std::lock_guard<std::mutex> l(g_testMx);
    g_testStore[name] = v;
    return TRUE;
}

// ---- El mod, sin cambios ----
#include "../../dock-estilo-apple.wh.cpp"

// ---------------------------------------------------------------------------
// Utilidades de prueba
// ---------------------------------------------------------------------------

static int g_pass = 0, g_fail = 0;
static void Check(bool ok, const char* what) {
    printf("  [%s] %s\n", ok ? "OK  " : "FALLA", what);
    fflush(stdout);
    (ok ? g_pass : g_fail)++;
}

// Ejecutar código en el hilo del Dock (gancho WH_GETMESSAGE).
static UINT g_runMsg = 0;
static std::function<void()> g_runFn;
static HANDLE g_runDone = nullptr;
static HHOOK g_runHook = nullptr;
static std::atomic<int> g_frames{0};
static std::vector<LONGLONG> g_frameStamps;  // QPC de cada vblank recibido
static std::atomic<bool> g_recordFrames{false};
static std::vector<LONGLONG> g_frameLat;  // vblank -> el hilo lo atiende

static LRESULT CALLBACK RunHook(int code, WPARAM wParam, LPARAM lParam) {
    if (code >= 0 && wParam == PM_REMOVE) {
        MSG* m = (MSG*)lParam;
        if (m->message == WM_APP_FRAME) {
            g_frames++;
            if (g_recordFrames) {
                g_frameStamps.push_back((LONGLONG)m->lParam);
                LARGE_INTEGER q; QueryPerformanceCounter(&q);
                g_frameLat.push_back(q.QuadPart - (LONGLONG)m->lParam);
            }
        }
        if (m->message == g_runMsg) {
            if (g_runFn) g_runFn();
            g_runFn = nullptr;
            m->message = WM_NULL;
            SetEvent(g_runDone);
        }
    }
    return CallNextHookEx(nullptr, code, wParam, lParam);
}

static void OnUi(std::function<void()> fn) {
    HWND h = g_wnd.load();
    if (!h) return;
    g_runFn = std::move(fn);
    ResetEvent(g_runDone);
    PostMessageW(h, g_runMsg, 0, 0);
    if (WaitForSingleObject(g_runDone, 5000) != WAIT_OBJECT_0) {
        printf("  [FALLA] el hilo del Dock no respondió en 5 s\n");
        g_fail++;
    }
}

static void Pump(int ms) {
    DWORD end = GetTickCount() + ms;
    while ((int)(end - GetTickCount()) > 0) {
        MSG m;
        while (PeekMessageW(&m, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&m);
            DispatchMessageW(&m);
        }
        Sleep(5);
    }
}

static void MouseMove(int x, int y) {
    INPUT in{};
    in.type = INPUT_MOUSE;
    in.mi.dx = (LONG)((x * 65535.0) / (GetSystemMetrics(SM_CXSCREEN) - 1));
    in.mi.dy = (LONG)((y * 65535.0) / (GetSystemMetrics(SM_CYSCREEN) - 1));
    in.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;
    SendInput(1, &in, sizeof(in));
}

static void MouseGlide(int x0, int y0, int x1, int y1, int ms) {
    int steps = std::max(1, ms / 16);
    for (int i = 1; i <= steps; i++) {
        MouseMove(x0 + (x1 - x0) * i / steps, y0 + (y1 - y0) * i / steps);
        Pump(16);
    }
}

static void MouseButton(DWORD flag) {
    INPUT in{};
    in.type = INPUT_MOUSE;
    in.mi.dwFlags = flag;
    SendInput(1, &in, sizeof(in));
}

struct ItemSnap {
    Kind kind;
    bool pinned;
    std::wstring name, exe;
    size_t windows;
    double cx, y, size, presence, dot;
    bool removing, launching, bouncing;
};

static std::vector<ItemSnap> SnapItems() {
    std::vector<ItemSnap> out;
    OnUi([&] {
        Layout L = ComputeLayout(Now());
        for (size_t i = 0; i < g_items.size(); i++) {
            auto& it = *g_items[i];
            out.push_back({it.kind, it.pinned, it.name, it.exe, it.windows.size(),
                           g_winX + L.g[i].cx, g_winY + L.g[i].y, L.g[i].size, it.presence.x,
                           it.dot.x, it.removing, it.launching, it.bounceStart >= 0});
        }
    });
    return out;
}

static void PrintItems(const std::vector<ItemSnap>& v) {
    const char* kinds[] = {"Inicio", "App", "Sep", "Papelera", "Fin"};
    for (auto& s : v) {
        printf("    %-8s %-22ls %s ventanas=%zu cx=%.0f tam=%.0f presencia=%.2f punto=%.2f%s%s\n",
               kinds[(int)s.kind], s.name.c_str(), s.pinned ? "anclada " : "        ", s.windows,
               s.cx, s.size, s.presence, s.dot, s.removing ? " saliendo" : "",
               s.bouncing ? " rebotando" : "");
    }
    fflush(stdout);
}

static int FindItem(const std::vector<ItemSnap>& v, const wchar_t* exeTail) {
    for (size_t i = 0; i < v.size(); i++) {
        if (v[i].kind == Kind::App && !v[i].removing && EndsWithI(v[i].exe, exeTail)) return (int)i;
    }
    return -1;
}

// Guarda lo que dibuja el Dock (BGRA premultiplicado) para revisarlo.
static void Dump(const char* name) {
    OnUi([&] {
        Render(Now());
        char path[MAX_PATH];
        snprintf(path, sizeof(path), "out\\%s.bgra", name);
        FILE* f = fopen(path, "wb");
        if (f) {
            int hdr[2] = {g_winW, g_winH};
            fwrite(hdr, sizeof(hdr), 1, f);
            fwrite(g_bits, 4, (size_t)g_winW * g_winH, f);
            fclose(f);
        }
    });
}

static void SetSetting(const wchar_t* k, const wchar_t* v) {
    std::lock_guard<std::mutex> l(g_testMx);
    g_testSettings[k] = v;
}

static void DefaultSettings() {
    SetSetting(L"iconSize", L"48");
    SetSetting(L"magnification", L"1");
    SetSetting(L"magnifiedSize", L"80");
    SetSetting(L"style", L"glass");
    SetSetting(L"autoHide", L"maximized");
    SetSetting(L"hideTaskbar", L"1");
    SetSetting(L"startIcon", L"");
    SetSetting(L"iconShadows", L"1");
    SetSetting(L"showStart", L"1");
    SetSetting(L"showTrash", L"1");
    SetSetting(L"showIndicators", L"1");
    SetSetting(L"showLabels", L"1");
    SetSetting(L"clickMinimizes", L"1");
    SetSetting(L"bounceOnLaunch", L"1");
    SetSetting(L"bounceOnAttention", L"1");
    SetSetting(L"animationBounce", L"0");
    SetSetting(L"animationSpeed", L"normal");
    SetSetting(L"font", L"Tahoma");
    SetSetting(L"spacing", L"5");
    SetSetting(L"magnificationRange", L"3");
    SetSetting(L"backgroundOpacity", L"100");
    SetSetting(L"cornerRadius", L"100");
    SetSetting(L"bottomMargin", L"7");
    SetSetting(L"revealDelay", L"80");
    SetSetting(L"hideDelay", L"700");
    SetSetting(L"trashIcon", L"windows");
    SetSetting(L"showUnpinned", L"1");
    SetSetting(L"indicator", L"dots");
    SetSetting(L"windowPreviews", L"off");  // se enciende solo en su prueba (ver 9)
    SetSetting(L"previewDelay", L"450");
    SetSetting(L"middleClick", L"newWindow");
    SetSetting(L"scrollAction", L"cycle");
    SetSetting(L"lockItems", L"0");
    SetSetting(L"startAction", L"startMenu");
    SetSetting(L"frameRate", L"monitor");
}

static void MakeLnk(const std::wstring& lnk, const std::wstring& target) {
    IShellLinkW* sl = nullptr;
    if (FAILED(CoCreateInstance(CLSID_ShellLink, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&sl))))
        return;
    sl->SetPath(target.c_str());
    IPersistFile* pf = nullptr;
    if (SUCCEEDED(sl->QueryInterface(IID_PPV_ARGS(&pf)))) {
        pf->Save(lnk.c_str(), TRUE);
        pf->Release();
    }
    sl->Release();
}

static HANDLE Spawn(const std::wstring& cmd) {
    STARTUPINFOW si{sizeof(si)};
    PROCESS_INFORMATION pi{};
    std::wstring c = cmd;
    if (!CreateProcessW(nullptr, c.data(), nullptr, nullptr, FALSE, 0, nullptr, nullptr, &si, &pi))
        return nullptr;
    CloseHandle(pi.hThread);
    return pi.hProcess;
}

static LRESULT CALLBACK FakeTrayProc(HWND h, UINT m, WPARAM w, LPARAM l) {
    return DefWindowProcW(h, m, w, l);
}

// Un IDataObject mínimo con CF_HDROP para probar soltar archivos.
class FakeData : public IDataObject {
   public:
    explicit FakeData(std::vector<std::wstring> files) : m_files(std::move(files)) {}
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID r, void** p) override {
        if (r == IID_IUnknown || r == IID_IDataObject) {
            *p = this;
            AddRef();
            return S_OK;
        }
        *p = nullptr;
        return E_NOINTERFACE;
    }
    ULONG STDMETHODCALLTYPE AddRef() override { return ++m_ref; }
    ULONG STDMETHODCALLTYPE Release() override { return --m_ref; }
    HRESULT STDMETHODCALLTYPE GetData(FORMATETC* f, STGMEDIUM* m) override {
        if (f->cfFormat != CF_HDROP) return DV_E_FORMATETC;
        size_t chars = 1;
        for (auto& s : m_files) chars += s.size() + 1;
        HGLOBAL g = GlobalAlloc(GHND, sizeof(DROPFILES) + chars * sizeof(wchar_t));
        auto* df = (DROPFILES*)GlobalLock(g);
        df->pFiles = sizeof(DROPFILES);
        df->fWide = TRUE;
        wchar_t* p = (wchar_t*)(df + 1);
        for (auto& s : m_files) {
            wcscpy(p, s.c_str());
            p += s.size() + 1;
        }
        *p = 0;
        GlobalUnlock(g);
        m->tymed = TYMED_HGLOBAL;
        m->hGlobal = g;
        m->pUnkForRelease = nullptr;
        return S_OK;
    }
    HRESULT STDMETHODCALLTYPE GetDataHere(FORMATETC*, STGMEDIUM*) override { return E_NOTIMPL; }
    HRESULT STDMETHODCALLTYPE QueryGetData(FORMATETC* f) override {
        return f->cfFormat == CF_HDROP ? S_OK : DV_E_FORMATETC;
    }
    HRESULT STDMETHODCALLTYPE GetCanonicalFormatEtc(FORMATETC*, FORMATETC*) override { return E_NOTIMPL; }
    HRESULT STDMETHODCALLTYPE SetData(FORMATETC*, STGMEDIUM*, BOOL) override { return E_NOTIMPL; }
    HRESULT STDMETHODCALLTYPE EnumFormatEtc(DWORD, IEnumFORMATETC**) override { return E_NOTIMPL; }
    HRESULT STDMETHODCALLTYPE DAdvise(FORMATETC*, DWORD, IAdviseSink*, DWORD*) override { return E_NOTIMPL; }
    HRESULT STDMETHODCALLTYPE DUnadvise(DWORD) override { return E_NOTIMPL; }
    HRESULT STDMETHODCALLTYPE EnumDAdvise(IEnumSTATDATA**) override { return E_NOTIMPL; }

   private:
    std::vector<std::wstring> m_files;
    ULONG m_ref = 1;
};

// ---------------------------------------------------------------------------

int wmain() {
    setvbuf(stdout, nullptr, _IONBF, 0);
    SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
    CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
    CreateDirectoryW(L"out", nullptr);
    DefaultSettings();
    g_runMsg = RegisterWindowMessageW(L"DockTestRun");
    g_runDone = CreateEventW(nullptr, TRUE, FALSE, nullptr);

    int sw = GetSystemMetrics(SM_CXSCREEN), sh = GetSystemMetrics(SM_CYSCREEN);
    printf("Pantalla %dx%d\n", sw, sh);

    // Apps ancladas de la barra de tareas (accesos directos de verdad).
    wchar_t appdata[MAX_PATH];
    ExpandEnvironmentStringsW(L"%APPDATA%", appdata, MAX_PATH);
    std::wstring pinDir = std::wstring(appdata) + L"\\Microsoft\\Internet Explorer\\Quick Launch\\User Pinned\\TaskBar";
    SHCreateDirectoryExW(nullptr, pinDir.c_str(), nullptr);
    MakeLnk(pinDir + L"\\Bloc de notas.lnk", L"C:\\windows\\notepad.exe");
    MakeLnk(pinDir + L"\\WordPad.lnk", L"C:\\Program Files\\Windows NT\\Accessories\\wordpad.exe");
    MakeLnk(pinDir + L"\\Explorador.lnk", L"C:\\windows\\explorer.exe");

    // Barra de tareas falsa en este proceso (como en explorer.exe).
    WNDCLASSW wc{};
    wc.lpfnWndProc = FakeTrayProc;
    wc.hInstance = GetModuleHandleW(nullptr);
    wc.lpszClassName = L"Shell_TrayWnd";
    wc.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
    RegisterClassW(&wc);
    HWND tray = CreateWindowExW(WS_EX_TOOLWINDOW | WS_EX_TOPMOST, L"Shell_TrayWnd", L"", WS_POPUP,
                                0, sh - 48, sw, 48, nullptr, nullptr, wc.hInstance, nullptr);
    ShowWindow(tray, SW_SHOWNA);
    HWND found = FindWindowW(L"Shell_TrayWnd", nullptr);
    DWORD fpid = 0;
    GetWindowThreadProcessId(found, &fpid);
    Check(found == tray && fpid == GetCurrentProcessId(), "La barra falsa es la que encuentra el mod");

    printf("\n== 1. Arranque ==\n");
    double t0 = Now();
    (void)t0;
    g_forcedPeriodMs = 1000.0 / 120.0;  // como un monitor de 120 Hz
    Check(Wh_ModInit(), "Wh_ModInit");
    for (int i = 0; i < 100 && !g_wnd.load(); i++) Pump(50);
    HWND dock = g_wnd.load();
    Check(dock != nullptr, "Se creó la ventana del Dock");
    if (!dock) return 1;
    DWORD dockTid = GetWindowThreadProcessId(dock, nullptr);
    g_runHook = SetWindowsHookExW(WH_GETMESSAGE, RunHook, nullptr, dockTid);
    Pump(1500);
    LONG_PTR ex = GetWindowLongPtrW(dock, GWL_EXSTYLE);
    Check((ex & WS_EX_LAYERED) && (ex & WS_EX_NOACTIVATE) && (ex & WS_EX_TOPMOST),
          "Ventana por capas, sin foco y siempre encima");
    Check(IsWindowVisible(dock), "El Dock está visible");
    Check(!IsWindowVisible(tray), "La barra de tareas quedó escondida");
    auto items = SnapItems();
    PrintItems(items);
    Check(items.size() >= 6 && items.front().kind == Kind::Start && items.back().kind == Kind::End,
          "Orden: Inicio … apps … separador, papelera, fin");
    Check(FindItem(items, L"\\notepad.exe") >= 0 && FindItem(items, L"\\wordpad.exe") >= 0,
          "Se tomaron las apps ancladas de la barra de tareas");
    Check(items.size() > 1 && items[1].kind == Kind::App && EndsWithI(items[1].exe, L"\\explorer.exe"),
          "El Explorador va primero");
    {
        std::wstring pins;
        std::lock_guard<std::mutex> l(g_testMx);
        Check(g_testStore.count(L"tbHidden") && g_testStore[L"tbHidden"] == L"1",
              "Se guardó que la barra está escondida (para restaurarla)");
    }
    // Sin animaciones, el Dock no debe dibujar.
    g_frames = 0;
    Pump(1000);
    Check(g_frames.load() == 0, "En reposo no se dibujan cuadros");
    {
        double hide = 0; bool fs = false, mx = false; std::wstring fgc; int states[4] = {0,0,0,0};
        int blackIcons = 0;
        OnUi([&] {
            hide = g_hide.x; fs = g_fgFullscreen; mx = g_fgMaximized;
            HWND fg = GetForegroundWindow(); fgc = fg ? std::wstring(ClassName(fg).s) : L"(ninguna)";
            for (auto& kv : g_icons) {
                if (kv.first.rfind(L"::ph", 0) == 0) continue;
                states[std::clamp(kv.second.state, 0, 3)]++;
                if (kv.second.set && !kv.second.set->mips.empty()) {
                    const Img& m = kv.second.set->mips[0];
                    uint32_t c = m.px[(size_t)(m.h / 2) * m.w + m.w / 2];
                    if (c == 0xFF000000u) blackIcons++;
                }
            }
        });
        printf("    escondido=%.2f pantallaCompleta=%d maximizada=%d primerPlano=%ls\n", hide, fs, mx, fgc.c_str());
        printf("    iconos: sinPedir=%d pedidos=%d listos=%d fallaron=%d negros=%d\n", states[0], states[1], states[2], states[3], blackIcons);
        Check(blackIcons == 0, "Ningún icono salió negro");
        int startState = 0, trashState = 0;
        OnUi([&] {
            startState = g_icons.count(L"::start") ? g_icons[L"::start"].state : 0;
            trashState = g_icons.count(L"::trash0") ? g_icons[L"::trash0"].state
                       : g_icons.count(L"::trash1") ? g_icons[L"::trash1"].state : 0;
        });
        Check(startState == 2, "Icono de Inicio cargado (del sistema)");
        Check(trashState == 2, "Icono de la papelera cargado (el de Windows)");
        RECT wr{};
        GetWindowRect(dock, &wr);
        double bgL = 0, bgR = 0;
        OnUi([&] { bgL = g_lastLayout.bgL + g_winX; bgR = g_lastLayout.bgR + g_winX; });
        printf("    ventana del Dock: %ld..%ld (ancho %ld de %d); fondo %.0f..%.0f\n", wr.left, wr.right, wr.right - wr.left, sw, bgL, bgR);
        Check(wr.right - wr.left < sw * 2 / 3 && wr.left <= bgL && wr.right >= bgR,
              "La ventana ocupa solo el espacio del Dock, no los lados");
        Check(hide < 0.01, "El Dock está a la vista al arrancar");
    }
    Dump("01-inicio");

    printf("\n== 2. Ampliación al pasar el mouse ==\n");
    int ni = FindItem(items, L"\\notepad.exe");
    double nx = items[ni].cx, ny = items[ni].y + items[ni].size / 2;
    MouseGlide(sw / 2, sh / 2, (int)nx, sh / 2, 200);
    MouseGlide((int)nx, sh / 2, (int)nx, (int)ny, 250);
    g_frames = 0;
    Pump(700);
    int frames = g_frames.load();
    items = SnapItems();
    PrintItems(items);
    double mag = 0, label = 0;
    std::wstring labelName;
    OnUi([&] {
        mag = g_mag.x;
        label = g_labelA.x;
        labelName = g_labelItem ? ItemLabel(*g_labelItem) : L"";
    });
    Check(mag > 0.99, "La ampliación llegó a 1");
    Check(std::fabs(items[ni].size - S(80)) < 2.5, "El icono bajo el mouse tiene el tamaño ampliado");
    Check(std::fabs(items[ni].cx - nx) < items[ni].size * 0.5,
          "El icono sigue bajo el mouse tras ampliarse");
    Check(items[ni - 1].size > S(48) && items[ni - 1].size < items[ni].size,
          "El vecino crece menos (caída suave)");
    Check(label > 0.99 && labelName == L"Bloc de notas", "Aparece el nombre de la app");
    printf("    cuadros en 0.7 s mientras se animaba: %d\n", frames);
    Check(frames > 5, "Se dibujó en cada refresco mientras había animación");
    Dump("02-ampliado");
    // Recorrer el Dock de lado a lado
    {
        int x0 = (int)nx, x1 = (int)items[items.size() - 2].cx;
        int steps = 25;
        for (int i = 1; i <= steps; i++) {
            int x = x0 + (x1 - x0) * i / steps;
            MouseMove(x, (int)ny);
            Pump(16);
            double m = 0, l = 0, r = 0, top = 0; bool hv = false;
            OnUi([&] { m = g_mag.x; hv = g_hover; l = g_lastLayout.bgL + g_winX; r = g_lastLayout.bgR + g_winX; top = g_lastLayout.topMost + g_winY; });
            if (i % 5 == 0 || !hv) printf("      x=%d y=%d hover=%d mag=%.2f fondo=[%.0f,%.0f] arriba=%.0f\n", x, (int)ny, hv, m, l, r, top);
        }
    }
    Pump(300);
    Dump("03-ampliado-papelera");
    OnUi([&] { labelName = g_labelItem ? ItemLabel(*g_labelItem) : L""; });
    Check(labelName == L"Papelera de reciclaje", "Nombre de la papelera al pasar sobre ella");
    OnUi([&] { mag = g_mag.x; });
    Check(mag > 0.99, "Sigue ampliado al llegar a la papelera");

    printf("\n== 3. Clic: abrir una app (rebote) ==\n");
    items = SnapItems();
    ni = FindItem(items, L"\\notepad.exe");
    MouseGlide((int)items[items.size() - 2].cx, (int)ny, (int)items[ni].cx, (int)ny, 250);
    Pump(300);
    items = SnapItems();
    MouseMove((int)items[ni].cx, (int)(items[ni].y + items[ni].size / 2));
    Pump(50);
    MouseButton(MOUSEEVENTF_LEFTDOWN);
    Pump(80);
    MouseButton(MOUSEEVENTF_LEFTUP);
    Pump(250);
    items = SnapItems();
    Check(items[ni].bouncing && items[ni].launching, "Rebota mientras se abre");
    Dump("04-rebote");
    bool opened = false;
    for (int i = 0; i < 60 && !opened; i++) {
        Pump(100);
        items = SnapItems();
        ni = FindItem(items, L"\\notepad.exe");
        opened = ni >= 0 && items[ni].windows > 0;
    }
    Check(opened, "La ventana del Bloc de notas apareció en el Dock");
    Pump(1200);
    items = SnapItems();
    ni = FindItem(items, L"\\notepad.exe");
    Check(!items[ni].bouncing && !items[ni].launching, "Dejó de rebotar al abrirse");
    Check(items[ni].dot > 0.99, "Tiene el punto de app abierta");

    printf("\n== 4. App abierta sin anclar ==\n");
    MouseGlide((int)items[ni].cx, (int)ny, sw / 2, sh / 3, 200);
    Pump(900);
    HANDLE reg = Spawn(L"C:\\windows\\regedit.exe");
    Check(reg != nullptr, "Se abrió el Editor del Registro");
    int ri = -1;
    for (int i = 0; i < 80 && ri < 0; i++) {
        Pump(100);
        items = SnapItems();
        ri = FindItem(items, L"\\regedit.exe");
    }
    Check(ri >= 0, "Apareció en el Dock");
    if (ri >= 0) {
        Check(!items[ri].pinned, "No está anclada");
        int sep = -1;
        for (size_t i = 0; i < items.size(); i++)
            if (items[i].kind == Kind::Sep) sep = (int)i;
        Check(ri == sep - 1, "Va al final de las apps, antes del separador");
    }
    Pump(800);
    items = SnapItems();
    PrintItems(items);
    Dump("05-regedit");

    printf("\n== 5. Clic en la app activa: minimizar ==\n");
    ni = FindItem(items, L"\\notepad.exe");
    HWND npWnd = nullptr;
    OnUi([&] {
        for (auto& it : g_items)
            if (EndsWithI(it->exe, L"\\notepad.exe") && !it->windows.empty()) npWnd = it->windows[0];
    });
    if (npWnd) {
        SetForegroundWindow(npWnd);
        Pump(300);
        double y = items[ni].y + items[ni].size / 2;
        MouseGlide(sw / 2, sh / 3, (int)items[ni].cx, (int)y, 250);
        Pump(400);
        items = SnapItems();
        MouseMove((int)items[ni].cx, (int)(items[ni].y + items[ni].size / 2));
        Pump(50);
        printf("    primer plano antes del clic: %s\n", GetForegroundWindow() == npWnd ? "Bloc de notas" : "otra");
        MouseButton(MOUSEEVENTF_LEFTDOWN);
        Pump(60);
        MouseButton(MOUSEEVENTF_LEFTUP);
        Pump(600);
        Check(IsIconic(npWnd), "Se minimizó");
        // Otro clic la restaura
        MouseButton(MOUSEEVENTF_LEFTDOWN);
        Pump(60);
        MouseButton(MOUSEEVENTF_LEFTUP);
        Pump(600);
        Check(!IsIconic(npWnd), "Otro clic la restaura");
    } else {
        Check(false, "Ventana del Bloc de notas encontrada");
    }

    printf("\n== 6. Arrastrar para anclar y reordenar ==\n");
    items = SnapItems();
    ri = FindItem(items, L"\\regedit.exe");
    if (ri >= 0) {
        double y = items[ri].y + items[ri].size / 2;
        MouseGlide(sw / 2, sh / 3, (int)items[ri].cx, (int)y, 200);
        Pump(400);
        items = SnapItems();
        ri = FindItem(items, L"\\regedit.exe");
        int x = (int)items[ri].cx;
        y = items[ri].y + items[ri].size / 2;
        MouseMove(x, (int)y);
        Pump(60);
        MouseButton(MOUSEEVENTF_LEFTDOWN);
        Pump(60);
        {
            bool pr = false; int pk = 0; SHORT ks = GetAsyncKeyState(VK_LBUTTON);
            int dd = 0, pd = 0, di = 0, pr2 = 0, up = 0;
            OnUi([&] { printf("    [diag] ULW fallidos=%d ventana x=%ld envL=%d\n", g_dbgUlwFail, [] { RECT r; GetWindowRect(g_wnd.load(), &r); return r.left; }(), g_envL); pr = g_press != nullptr; pk = (int)g_pop.kind; dd = g_dbgDockDown; pd = g_dbgPopDown; di = g_dbgDownIndex; pr2 = g_dbgPollRelease; up = g_dbgUp; });
            OnUi([&] {
                printf("    [diag] clic en lienzo (%ld,%ld) -> pantalla (%ld,%ld); prueba apuntó a (%d,%d)\n", g_dbgDownPt.x, g_dbgDownPt.y,
                       g_dbgDownPt.x + g_winX, g_dbgDownPt.y + g_winY, x, (int)y);
                for (size_t k = 0; k < g_items.size(); k++) {
                    if (!EndsWithI(g_items[k]->exe, L"\\regedit.exe")) continue;
                    const Geo& ge = g_lastLayout.g[k];
                    printf("    [diag] regedit en g_lastLayout: slot %.0f..%.0f y=%.0f tam=%.0f shown=%d (pantalla %.0f..%.0f)\n", ge.slotL, ge.slotR, ge.y, ge.size, ge.shown, ge.slotL + g_winX, ge.slotR + g_winX);
                }
            });
            printf("    [diag] tras presionar: press=%d popup=%d boton=%x clicsDock=%d clicsPopup=%d indice=%d soltadosPorRespaldo=%d ups=%d\n", pr, pk, (unsigned)(USHORT)ks, dd, pd, di, pr2, up);
        }
        // Mover a la izquierda hasta que el hueco quede antes de WordPad.
        std::wstring before;
        bool dragging = false;
        for (int k = 0; k < 260; k++) {
            x -= 3;
            MouseMove(x, (int)y - 4);
            Pump(25);
            OnUi([&] {
                dragging = g_drag != nullptr;
                before = g_insertBefore ? g_insertBefore->name : L"";
            });
            if (before == L"WordPad") break;
        }
        Check(dragging, "Arrastre en curso");
        Check(before == L"WordPad", "El hueco se abrió antes de WordPad");
        // Quieto: el hueco no debe brincar de lugar.
        bool stable = true;
        for (int k = 0; k < 15; k++) {
            Pump(30);
            std::wstring b2;
            OnUi([&] { b2 = g_insertBefore ? g_insertBefore->name : L""; });
            if (b2 != before) stable = false;
        }
        Check(stable, "El hueco se queda quieto con el mouse quieto");
        Dump("06-arrastre");
        MouseButton(MOUSEEVENTF_LEFTUP);
        Pump(900);
        items = SnapItems();
        PrintItems(items);
        ri = FindItem(items, L"\\regedit.exe");
        int wi = FindItem(items, L"\\wordpad.exe");
        Check(ri >= 0 && items[ri].pinned, "Quedó anclada al soltarla entre las ancladas");
        Check(ri >= 0 && wi >= 0 && ri == wi - 1, "Quedó justo antes de WordPad");
        double fly = 0;
        OnUi([&] {
            for (auto& it : g_items)
                if (EndsWithI(it->exe, L"\\regedit.exe")) fly = std::fabs(it->flyX.x) + std::fabs(it->flyY.x);
        });
        Check(fly < 0.5, "Terminó de volar a su lugar");
        std::wstring pins;
        {
            std::lock_guard<std::mutex> l(g_testMx);
            pins = g_testStore[L"pins"];
        }
        Check(pins.find(L"regedit.exe") != std::wstring::npos &&
                  pins.find(L"regedit.exe") < pins.find(L"WordPad.lnk"),
              "Se guardó el nuevo orden");
    }

    printf("\n== 7. Arrastrar hacia arriba para quitar ==\n");
    items = SnapItems();
    int wi = FindItem(items, L"\\wordpad.exe");
    if (wi >= 0) {
        double y = items[wi].y + items[wi].size / 2;
        MouseGlide(sw / 2, (int)y, (int)items[wi].cx, (int)y, 200);
        Pump(400);
        items = SnapItems();
        wi = FindItem(items, L"\\wordpad.exe");
        MouseMove((int)items[wi].cx, (int)(items[wi].y + items[wi].size / 2));
        Pump(60);
        MouseButton(MOUSEEVENTF_LEFTDOWN);
        Pump(60);
        MouseGlide((int)items[wi].cx, (int)y, (int)items[wi].cx, (int)y - S(170), 400);
        Pump(300);
        bool remove = false;
        RECT dr{};
        bool dv = false;
        OnUi([&] {
            remove = g_dragRemove && !g_dragOverTrash;
            dv = g_dragWnd && IsWindowVisible(g_dragWnd);
            if (g_dragWnd) GetWindowRect(g_dragWnd, &dr);
        });
        Check(remove, "Marca \"Quitar\" al subirla");
        POINT cp;
        GetCursorPos(&cp);
        Check(dv && PtInRect(&dr, cp), "El icono arrastrado sigue al cursor fuera del Dock");
        OnUi([&] {
            if (!g_dragBits) return;
            FILE* f = fopen("out\\07b-ventanita.bgra", "wb");
            int hdr[2] = {g_dragW, g_dragH};
            fwrite(hdr, sizeof(hdr), 1, f);
            fwrite(g_dragBits, 4, (size_t)g_dragW * g_dragH, f);
            fclose(f);
        });
        Dump("07-quitar");
        {
            bool d = false; HWND cap = nullptr;
            OnUi([&] { d = g_drag != nullptr; cap = GetCapture(); });
            printf("    antes de soltar: arrastre=%d captura=%s\n", d, cap == g_wnd.load() ? "dock" : (cap ? "otra" : "nadie"));
        }
        MouseButton(MOUSEEVENTF_LEFTUP);
        Pump(120);
        {
            bool d = false; HWND cap = nullptr;
            OnUi([&] { d = g_drag != nullptr; cap = GetCapture(); });
            printf("    después de soltar: arrastre=%d captura=%s\n", d, cap == g_wnd.load() ? "dock" : (cap ? "otra" : "nadie"));
        }
        bool puff = false, dragVisible = false;
        OnUi([&] { puff = g_dragPuffOn; dragVisible = g_dragWnd && IsWindowVisible(g_dragWnd); });
        Check(puff && dragVisible, "Nubecita al soltarla (en la ventanita que sigue al cursor)");
        Dump("08-nube");
        Pump(800);
        items = SnapItems();
        Check(FindItem(items, L"\\wordpad.exe") < 0, "WordPad ya no está en el Dock");
        std::wstring pins;
        {
            std::lock_guard<std::mutex> l(g_testMx);
            pins = g_testStore[L"pins"];
        }
        Check(pins.find(L"WordPad") == std::wstring::npos, "Ni en los anclados guardados");
    }

    printf("\n== 8. Soltar archivos ==\n");
    {
        // Una app soltada entre los iconos se ancla.
        FakeData data({L"C:\\windows\\system32\\winemine.exe"});
        items = SnapItems();
        int ei = 1;
        POINTL pt{(LONG)(items[ei].cx + items[ei].size * 0.55), (LONG)(items[ei].y + items[ei].size / 2)};
        const DWORD kAllowed = DROPEFFECT_COPY | DROPEFFECT_MOVE | DROPEFFECT_LINK;
        DWORD eff = kAllowed;
        OnUi([&] {
            g_dropTarget.DragEnter(&data, 0, pt, &eff);
            eff = kAllowed;
            g_dropTarget.DragOver(0, pt, &eff);
        });
        Check(eff == DROPEFFECT_LINK, "Un .exe muestra el efecto de anclar");
        Pump(400);
        OnUi([&] { eff = kAllowed; g_dropTarget.Drop(&data, 0, pt, &eff); });
        Pump(800);
        items = SnapItems();
        int mi = FindItem(items, L"\\winemine.exe");
        Check(mi == 2 && items[mi].pinned, "Se ancló después del Explorador");
        // Un archivo sobre la papelera
        wchar_t tmp[MAX_PATH];
        GetTempPathW(MAX_PATH, tmp);
        std::wstring junk = std::wstring(tmp) + L"dock-test-basura.txt";
        HANDLE fh = CreateFileW(junk.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, 0, nullptr);
        CloseHandle(fh);
        FakeData trash({junk});
        int ti = (int)items.size() - 2;
        POINTL tp{(LONG)items[ti].cx, (LONG)(items[ti].y + items[ti].size / 2)};
        OnUi([&] {
            eff = kAllowed;
            g_dropTarget.DragEnter(&trash, 0, tp, &eff);
            eff = kAllowed;
            g_dropTarget.DragOver(0, tp, &eff);
        });
        Check(eff == DROPEFFECT_MOVE, "Un archivo sobre la papelera: mover");
        OnUi([&] { eff = kAllowed; g_dropTarget.Drop(&trash, 0, tp, &eff); });
        bool gone = false;
        for (int i = 0; i < 30 && !gone; i++) {
            Pump(100);
            gone = GetFileAttributesW(junk.c_str()) == INVALID_FILE_ATTRIBUTES;
        }
        Check(gone, "El archivo se fue a la papelera");
        // Un acceso directo sobre la papelera se borra (no se ancla).
        std::wstring lnk = std::wstring(tmp) + L"dock-test-acceso.lnk";
        MakeLnk(lnk, L"C:\\windows\\notepad.exe");
        size_t before = SnapItems().size();
        FakeData lnkData({lnk});
        items = SnapItems();
        ti = (int)items.size() - 2;
        tp = POINTL{(LONG)items[ti].cx, (LONG)(items[ti].y + items[ti].size / 2)};
        OnUi([&] {
            DWORD allowed = DROPEFFECT_COPY | DROPEFFECT_MOVE | DROPEFFECT_LINK;
            eff = allowed;
            g_dropTarget.DragEnter(&lnkData, 0, tp, &eff);
            eff = allowed;
            g_dropTarget.DragOver(0, tp, &eff);
        });
        Check(eff == DROPEFFECT_MOVE, "Un acceso directo sobre la papelera también es mover");
        OnUi([&] {
            DWORD allowed = DROPEFFECT_COPY | DROPEFFECT_MOVE | DROPEFFECT_LINK;
            eff = allowed;
            g_dropTarget.Drop(&lnkData, 0, tp, &eff);
        });
        gone = false;
        for (int i = 0; i < 30 && !gone; i++) {
            Pump(100);
            gone = GetFileAttributesW(lnk.c_str()) == INVALID_FILE_ATTRIBUTES;
        }
        Pump(400);
        Check(gone && SnapItems().size() == before, "Se borró y no se ancló");
    }

    printf("\n== 8b. Arrastrar un icono a la papelera ==\n");
    {
        items = SnapItems();
        int mi = FindItem(items, L"\\winemine.exe");
        if (mi >= 0) {
            double y = items[mi].y + items[mi].size / 2;
            MouseGlide(sw / 2, sh / 3, (int)items[mi].cx, (int)y, 200);
            Pump(400);
            items = SnapItems();
            mi = FindItem(items, L"\\winemine.exe");
            int x = (int)items[mi].cx;
            y = items[mi].y + items[mi].size / 2;
            MouseMove(x, (int)y);
            Pump(60);
            MouseButton(MOUSEEVENTF_LEFTDOWN);
            Pump(60);
            // Hacia la papelera, siguiéndola mientras el Dock se acomoda.
            bool over = false;
            for (int k = 0; k < 200 && !over; k++) {
                items = SnapItems();
                int ti = (int)items.size() - 2;
                int tx = (int)items[ti].cx;
                x += std::clamp(tx - x, -6, 6);
                MouseMove(x, (int)y);
                Pump(20);
                OnUi([&] { over = g_dragOverTrash; });
            }
            double hl = 0;
            Pump(250);
            OnUi([&] { if (auto* t = FindKind(Kind::Trash)) hl = t->hilite.x; });
            Check(over, "El icono está sobre la papelera");
            Check(hl > 0.9, "La papelera se resalta");
            Dump("08b-a-la-papelera");
            MouseButton(MOUSEEVENTF_LEFTUP);
            Pump(700);
            items = SnapItems();
            std::wstring pins;
            {
                std::lock_guard<std::mutex> l(g_testMx);
                pins = g_testStore[L"pins"];
            }
            Check(FindItem(items, L"\\winemine.exe") < 0 && pins.find(L"winemine") == std::wstring::npos,
                  "Soltarlo en la papelera lo quitó del Dock");
            MouseGlide(x, (int)y, sw / 2, sh / 3, 200);
            Pump(600);
        } else {
            Check(false, "Buscaminas anclado para la prueba");
        }
    }

    printf("\n== 9. Vista de ventanas, activación y menú propio ==\n");
    {
        auto popKind = [&] { PopKind k = PopKind::None; OnUi([&] { k = PopupOpen() ? g_pop.kind : PopKind::None; }); return k; };
        auto clickAt = [&](int x, int y, DWORD down, DWORD up) {
            MouseMove(x, y);
            Pump(60);
            MouseButton(down);
            Pump(50);
            MouseButton(up);
        };
        OnUi([&] {
            for (auto& it : g_items) {
                if (!EndsWithI(it->exe, L"\\explorer.exe")) continue;
                printf("    [diag] Explorador: %zu ventanas\n", it->windows.size());
                for (HWND w : it->windows) {
                    wchar_t t[128] = {}, c[64] = {};
                    InternalGetWindowText(w, t, 128);
                    GetClassNameW(w, c, 64);
                    RECT r; GetWindowRect(w, &r);
                    printf("      [%ls] clase=%ls %ldx%ld\n", t, c, r.right - r.left, r.bottom - r.top);
                }
            }
        });
        // Una segunda ventana del Bloc de notas
        HANDLE np2 = Spawn(L"C:\\windows\\notepad.exe");
        size_t nw = 0;
        for (int k = 0; k < 80 && nw < 2; k++) {
            Pump(100);
            items = SnapItems();
            ni = FindItem(items, L"\\notepad.exe");
            nw = ni >= 0 ? items[ni].windows : 0;
        }
        Check(nw == 2, "El Bloc de notas tiene 2 ventanas");
        Pump(600);
        // --- Vista de ventanas al pasar el mouse ---
        SetSetting(L"windowPreviews", L"thumbnails");
        Wh_ModSettingsChanged();
        Pump(800);
        items = SnapItems();
        ni = FindItem(items, L"\\notepad.exe");
        double y = items[ni].y + items[ni].size / 2;
        MouseGlide(sw / 2, sh / 3, (int)items[ni].cx, (int)y, 250);
        Pump(250);
        items = SnapItems();
        MouseMove((int)items[ni].cx, (int)(items[ni].y + items[ni].size / 2));
        Pump(900);
        size_t n = 0;
        bool vis = false, list = false;
        OnUi([&] { n = g_pop.prev.size(); vis = g_pop.wnd && IsWindowVisible(g_pop.wnd); list = g_pop.list; });
        Check(popKind() == PopKind::Preview && n == 2 && vis, "Al pasar el mouse se ven sus 2 ventanas");
        printf("    modo: %s\n", list ? "lista" : "miniaturas");
        OnUi([&] {
            FILE* f = fopen("out\\09-vista.bgra", "wb");
            int hdr[2] = {g_pop.w, g_pop.h};
            fwrite(hdr, sizeof(hdr), 1, f);
            fwrite(g_pop.bits, 4, (size_t)g_pop.w * g_pop.h, f);
            fclose(f);
        });
        // Subir hasta la segunda tarjeta y elegirla
        RECT card{};
        int px = 0, py = 0;
        HWND target = nullptr;
        OnUi([&] { card = g_pop.prev[1].card; px = g_pop.x; py = g_pop.y; target = g_pop.prev[1].hwnd; });
        int cx = px + (card.left + card.right) / 2, cy = py + (card.top + card.bottom) / 2;
        POINT cur;
        GetCursorPos(&cur);
        MouseGlide(cur.x, cur.y, cx, cy, 350);
        Pump(300);
        Check(popKind() == PopKind::Preview, "Sigue abierta al subir del icono a la vista");
        int hot = -1;
        OnUi([&] { hot = g_pop.hot; });
        Check(hot == 1, "La tarjeta bajo el mouse se resalta");
        clickAt(cx, cy, MOUSEEVENTF_LEFTDOWN, MOUSEEVENTF_LEFTUP);
        Pump(120);
        bool closedNow = popKind() == PopKind::None;
        Pump(480);
        {
            int pk = 0; bool cl = false, pr = false; uint64_t sup = 0, uid = 0;
            OnUi([&] { pk = (int)g_pop.kind; cl = g_pop.closing; pr = g_press != nullptr; sup = g_prevSuppress; uid = g_pop.itemUid; });
            printf("    [diag] tras elegir tarjeta: popup=%d cerrando=%d press=%d suprimida=%llu uid=%llu\n", pk, cl, pr, (unsigned long long)sup, (unsigned long long)uid);
        }
        Check(GetForegroundWindow() == target, "Clic en una ventana de la vista la trae al frente");
        Check(closedNow, "Y la vista se cierra");
        SetSetting(L"windowPreviews", L"off");
        Wh_ModSettingsChanged();
        Pump(800);
        // Que nada esté maximizado (Wine recuerda el tamaño del Bloc de notas)
        OnUi([&] { for (auto& it : g_items) for (HWND w : it->windows) if (IsZoomed(w)) ShowWindow(w, SW_RESTORE); });
        if (target) ActivateWindow(target);
        Pump(800);
        // --- Activar otra app con una app distinta al frente ---
        items = SnapItems();
        int ri = FindItem(items, L"\\regedit.exe");
        HWND regWnd = nullptr;
        OnUi([&] { for (auto& it : g_items) if (EndsWithI(it->exe, L"\\regedit.exe") && !it->windows.empty()) regWnd = it->windows[0]; });
        Check(GetForegroundWindow() != regWnd, "El Bloc de notas está al frente");
        GetCursorPos(&cur);
        y = items[ri].y + items[ri].size / 2;
        MouseGlide(cur.x, cur.y, (int)items[ri].cx, (int)y, 300);
        Pump(150);
        items = SnapItems();
        clickAt((int)items[ri].cx, (int)(items[ri].y + items[ri].size / 2), MOUSEEVENTF_LEFTDOWN, MOUSEEVENTF_LEFTUP);
        Pump(700);
        {
            bool pr = false; int pk = 0, dd = 0, up = 0, di = 0, rel = 0, pd = 0;
            OnUi([&] { pr = g_press != nullptr; pk = (int)g_pop.kind; dd = g_dbgDockDown; up = g_dbgUp; di = g_dbgDownIndex; rel = g_dbgPollRelease; pd = g_dbgPopDown; });
            wchar_t cls[64] = {}; GetClassNameW(GetForegroundWindow(), cls, 64);
            printf("    [diag] activar: press=%d popup=%d downs=%d ups=%d indice=%d respaldo=%d clicsPopup=%d primerPlano=%ls\n", pr, pk, dd, up, di, rel, pd, cls);
        }
        Check(regWnd && GetForegroundWindow() == regWnd, "Con otra app al frente, el clic trae el Editor del Registro");
        // --- Menú propio ---
        items = SnapItems();
        ni = FindItem(items, L"\\notepad.exe");
        GetCursorPos(&cur);
        MouseGlide(cur.x, cur.y, (int)items[ni].cx, (int)(items[ni].y + items[ni].size / 2), 250);
        Pump(150);
        items = SnapItems();
        clickAt((int)items[ni].cx, (int)(items[ni].y + items[ni].size / 2), MOUSEEVENTF_RIGHTDOWN, MOUSEEVENTF_RIGHTUP);
        Pump(500);
        std::vector<std::wstring> texts;
        OnUi([&] { for (auto& e : g_pop.menu) if (!e.sep) texts.push_back(e.text); });
        auto has = [&](const wchar_t* t) { return std::find(texts.begin(), texts.end(), t) != texts.end(); };
        Check(popKind() == PopKind::Menu, "Clic derecho abre el menú propio");
        Check(has(L"Nueva ventana") && has(L"Mantener en el Dock") && has(L"Cerrar todas las ventanas") &&
                  has(L"Finalizar tarea") && has(L"Ocultar las demás") && has(L"Abrir al iniciar sesión"),
              "El menú trae las opciones nuevas");
        printf("    opciones:");
        for (auto& t : texts) printf(" [%ls]", t.c_str());
        printf("\n");
        OnUi([&] {
            FILE* f = fopen("out\\09b-menu.bgra", "wb");
            int hdr[2] = {g_pop.w, g_pop.h};
            fwrite(hdr, sizeof(hdr), 1, f);
            fwrite(g_pop.bits, 4, (size_t)g_pop.w * g_pop.h, f);
            fclose(f);
        });
        {
            std::vector<int> widths;
            OnUi([&] {
                widths.push_back(g_pop.w);
                for (auto& it : g_items) {
                    if (it->kind == Kind::Sep || it->kind == Kind::End) continue;
                    OpenMenu(it.get(), g_winX + 400);
                    widths.push_back(g_pop.w);
                }
                OpenMenu(nullptr, g_winX + 400);
                widths.push_back(g_pop.w);
                ClosePopup(true);
            });
            bool same = std::all_of(widths.begin(), widths.end(), [&](int w) { return w == widths[0]; });
            printf("    anchos de menú:");
            for (int w : widths) printf(" %d", w);
            printf("\n");
            Check(same, "Todos los menús tienen el mismo ancho");
            clickAt((int)items[ni].cx, (int)(items[ni].y + items[ni].size / 2), MOUSEEVENTF_RIGHTDOWN, MOUSEEVENTF_RIGHTUP);
            Pump(400);
        }
        keybd_event(VK_DOWN, 0, 0, 0);
        keybd_event(VK_DOWN, 0, KEYEVENTF_KEYUP, 0);
        Pump(200);
        OnUi([&] { hot = g_pop.hot; });
        // Resaltar "Finalizar tarea" para la captura
        OnUi([&] {
            for (size_t k = 0; k < g_pop.menu.size(); k++) if (g_pop.menu[k].id == M_KILL) g_pop.hot = (int)k;
            RenderPopup();
            FILE* f = fopen("out\\09c-menu-resaltado.bgra", "wb");
            int hdr[2] = {g_pop.w, g_pop.h};
            fwrite(hdr, sizeof(hdr), 1, f);
            fwrite(g_pop.bits, 4, (size_t)g_pop.w * g_pop.h, f);
            fclose(f);
            g_pop.hot = hot;
        });
        Check(hot >= 0, "Las flechas mueven la selección");
        keybd_event(VK_ESCAPE, 0, 0, 0);
        keybd_event(VK_ESCAPE, 0, KEYEVENTF_KEYUP, 0);
        Pump(500);
        {
            int pk = 0; bool cl = false; bool hk = false;
            int on = 0, off = 0, keys = 0;
            OnUi([&] { pk = (int)g_pop.kind; cl = g_pop.closing; hk = g_llKey != nullptr; on = g_dbgHookOn; off = g_dbgHookOff; keys = g_dbgKeys; });
            printf("    [diag] tras Esc: popup=%d cerrando=%d gancho=%d puestos=%d quitados=%d teclas=%d\n", pk, cl, hk, on, off, keys);
        }
        Check(popKind() == PopKind::None, "Esc lo cierra");
        clickAt((int)items[ni].cx, (int)(items[ni].y + items[ni].size / 2), MOUSEEVENTF_RIGHTDOWN, MOUSEEVENTF_RIGHTUP);
        Pump(400);
        Check(popKind() == PopKind::Menu, "Se vuelve a abrir");
        clickAt(sw / 2, sh / 4, MOUSEEVENTF_LEFTDOWN, MOUSEEVENTF_LEFTUP);
        Pump(500);
        Check(popKind() == PopKind::None, "Un clic fuera lo cierra");
        // --- Finalizar tarea ---
        items = SnapItems();
        ri = FindItem(items, L"\\regedit.exe");
        GetCursorPos(&cur);
        MouseGlide(cur.x, cur.y, (int)items[ri].cx, (int)(items[ri].y + items[ri].size / 2), 300);
        Pump(150);
        items = SnapItems();
        clickAt((int)items[ri].cx, (int)(items[ri].y + items[ri].size / 2), MOUSEEVENTF_RIGHTDOWN, MOUSEEVENTF_RIGHTUP);
        Pump(500);
        RECT kr{};
        bool found = false;
        OnUi([&] {
            for (auto& e : g_pop.menu) if (e.id == M_KILL && !e.disabled) { kr = e.rc; found = true; }
            px = g_pop.x; py = g_pop.y;
        });
        Check(found, "Finalizar tarea disponible para el Editor del Registro");
        if (found) {
            int kx = px + (kr.left + kr.right) / 2, ky = py + (kr.top + kr.bottom) / 2;
            GetCursorPos(&cur);
            MouseGlide(cur.x, cur.y, kx, ky, 250);
            Pump(150);
            clickAt(kx, ky, MOUSEEVENTF_LEFTDOWN, MOUSEEVENTF_LEFTUP);
            bool ended = reg && WaitForSingleObject(reg, 5000) == WAIT_OBJECT_0;
            Check(ended, "Finalizar tarea cerró el proceso");
            Pump(1800);
            items = SnapItems();
            ri = FindItem(items, L"\\regedit.exe");
            Check(ri >= 0 && items[ri].windows == 0 && items[ri].pinned, "Queda anclada, ya sin ventanas");
        }
        // --- Rueda del mouse ---
        HWND np = nullptr;
        OnUi([&] { for (auto& it : g_items) if (EndsWithI(it->exe, L"\\notepad.exe") && !it->windows.empty()) np = it->windows[0]; });
        if (np) {
            ActivateWindow(np);
            Pump(400);
            items = SnapItems();
            ni = FindItem(items, L"\\notepad.exe");
            GetCursorPos(&cur);
            MouseGlide(cur.x, cur.y, (int)items[ni].cx, (int)(items[ni].y + items[ni].size / 2), 250);
            Pump(200);
            HWND fgBefore = GetForegroundWindow();
            INPUT in{};
            in.type = INPUT_MOUSE;
            in.mi.dwFlags = MOUSEEVENTF_WHEEL;
            in.mi.mouseData = (DWORD)-WHEEL_DELTA;
            SendInput(1, &in, sizeof(in));
            Pump(700);
            Check(fgBefore && IsIconic(fgBefore), "Rueda abajo minimiza la ventana de la app");
            in.mi.mouseData = WHEEL_DELTA;
            SendInput(1, &in, sizeof(in));
            Pump(700);
            Check(fgBefore && !IsIconic(fgBefore), "Rueda arriba la trae de vuelta");
        }
        // --- Vista con una ventana minimizada ---
        {
            SetSetting(L"windowPreviews", L"thumbnails");
            Wh_ModSettingsChanged();
            Pump(800);
            HWND w0 = nullptr, w1 = nullptr;
            OnUi([&] {
                for (auto& it : g_items)
                    if (EndsWithI(it->exe, L"\\notepad.exe") && it->windows.size() >= 2) { w0 = it->windows[0]; w1 = it->windows[1]; }
            });
            if (w0 && w1) {
                for (HWND w : {w0, w1}) if (IsIconic(w)) ShowWindow(w, SW_RESTORE);
                ActivateWindow(w0);
                Pump(500);
                {
                    std::shared_ptr<Snapshot> t;
                    bool hung = IsHungAppWindow(w0), vis = IsWindowVisible(w0), ic = IsIconic(w0);
                    OnUi([&] { t = CaptureWindow(w0); });
                    RECT r; GetWindowRect(w0, &r);
                    for (UINT fl : {2u, 0u}) {
                        RECT r; GetWindowRect(w0, &r);
                        int ww = r.right - r.left, wh = r.bottom - r.top;
                        BITMAPINFO bi{}; bi.bmiHeader.biSize = sizeof(bi.bmiHeader); bi.bmiHeader.biWidth = ww; bi.bmiHeader.biHeight = -wh;
                        bi.bmiHeader.biPlanes = 1; bi.bmiHeader.biBitCount = 32; bi.bmiHeader.biCompression = BI_RGB;
                        void* bits = nullptr; HDC dc = CreateCompatibleDC(nullptr);
                        HBITMAP dib = CreateDIBSection(dc, &bi, DIB_RGB_COLORS, &bits, nullptr, 0);
                        HGDIOBJ o = SelectObject(dc, dib);
                        memset(bits, 0, (size_t)ww * wh * 4);
                        BOOL ok = 0;
                        OnUi([&] { ok = PrintWindow(w0, dc, fl); });
                        GdiFlush();
                        size_t black = 0, n = (size_t)ww * wh;
                        for (size_t k = 0; k < n; k++) black += (((uint32_t*)bits)[k] & 0xFFFFFF) == 0;
                        printf("    [diag] PrintWindow(%u): ok=%d negro=%.1f%%\n", fl, ok, 100.0 * black / n);
                        SelectObject(dc, o); DeleteObject(dib); DeleteDC(dc);
                    }
                    printf("    [diag] captura directa: %s (visible=%d minimizada=%d colgada=%d %ldx%ld)\n", t ? "ok" : "nada", vis, ic, hung, r.right - r.left, r.bottom - r.top);
                }
                // Como lo hace el Dock: foto y luego minimizar.
                OnUi([&] { MinimizeWindow(w0); });
                Pump(800);
                bool hasSnap = false;
                OnUi([&] { hasSnap = SnapshotOf(w0) != nullptr; });
                printf("    [diag] tras minimizar: minimizada=%d foto=%d\n", (int)IsIconic(w0), hasSnap);
                Check(IsIconic(w0) && hasSnap, "Al minimizar se guardó una foto de la ventana");
                items = SnapItems();
                ni = FindItem(items, L"\\notepad.exe");
                GetCursorPos(&cur);
                MouseGlide(cur.x, cur.y, sw / 2, sh / 3, 150);
                Pump(300);
                MouseGlide(sw / 2, sh / 3, (int)items[ni].cx, (int)(items[ni].y + items[ni].size / 2), 250);
                Pump(200);
                items = SnapItems();
                MouseMove((int)items[ni].cx, (int)(items[ni].y + items[ni].size / 2));
                Pump(900);
                bool shown = false, fit = false;
                OnUi([&] {
                    for (auto& e : g_pop.prev) if (e.hwnd == w0) { shown = e.iconic; fit = !e.snapFit.empty(); }
                });
                {
                    int pk = (int)popKind(); size_t n = 0, nw = 0; int iconicCount = 0;
                    OnUi([&] { n = g_pop.prev.size(); for (auto& e : g_pop.prev) iconicCount += e.iconic;
                               for (auto& it : g_items) if (EndsWithI(it->exe, L"\\notepad.exe")) nw = it->windows.size(); });
                    POINT c; GetCursorPos(&c);
                    printf("    [diag] w0 minimizada ahora=%d\n", (int)IsIconic(w0)); printf("    [diag] vista: tipo=%d entradas=%zu minimizadas=%d ventanasBloc=%zu cursor=(%ld,%ld) icono=(%.0f,%.0f)\n", pk, n, iconicCount, nw, c.x, c.y, items[ni].cx, items[ni].y + items[ni].size / 2);
                }
                Check(popKind() == PopKind::Preview && shown, "La vista incluye la ventana minimizada");
                Check(fit, "Y la muestra con su foto (no solo el icono)");
                OnUi([&] {
                    FILE* f = fopen("out\\09d-vista-minimizada.bgra", "wb");
                    int hdr[2] = {g_pop.w, g_pop.h};
                    fwrite(hdr, sizeof(hdr), 1, f);
                    fwrite(g_pop.bits, 4, (size_t)g_pop.w * g_pop.h, f);
                    fclose(f);
                });
                GetCursorPos(&cur);
                MouseGlide(cur.x, cur.y, sw / 2, sh / 3, 200);
                Pump(800);
                ShowWindow(w0, SW_RESTORE);
            } else {
                Check(false, "Dos ventanas del Bloc de notas para la prueba");
            }
            SetSetting(L"windowPreviews", L"off");
            Wh_ModSettingsChanged();
            Pump(600);
        }
        if (np2) TerminateProcess(np2, 0);
        Pump(1500);
    }

    printf("\n== 10. Rebote de atención ==\n");
    {
        OnUi([&] {
            for (auto& it : g_items)
                if (EndsWithI(it->exe, L"\\notepad.exe") && !it->windows.empty())
                    SendMessageW(g_wnd.load(), g_msgShellHook, HSHELL_FLASH, (LPARAM)it->windows[0]);
        });
        Pump(150);
        items = SnapItems();
        ni = FindItem(items, L"\\notepad.exe");
        Check(items[ni].bouncing, "Rebota cuando la ventana pide atención");
        Pump(2400);
        items = SnapItems();
        Check(!items[ni].bouncing, "Termina tras tres saltos");
    }

    printf("\n== 11. Configuración: ocultar siempre, estilo claro ==\n");
    MouseGlide(sw / 2, sh - 60, sw / 2, sh / 3, 200);
    Pump(1000);
    SetSetting(L"autoHide", L"always");
    SetSetting(L"style", L"light");
    SetSetting(L"magnifiedSize", L"96");
    Wh_ModSettingsChanged();
    Pump(1200);
    double hide = 0;
    OnUi([&] { hide = g_hide.x; });
    Check(hide > 0.99, "El Dock se escondió");
    Dump("09-escondido");
    MouseGlide(sw / 2, sh / 3, sw / 2, sh - 1, 300);
    Pump(700);
    OnUi([&] { hide = g_hide.x; });
    Check(hide < 0.01, "Aparece al llevar el mouse al borde de abajo");
    Dump("10-claro");
    MouseGlide(sw / 2, sh - 1, sw / 2, sh / 3, 200);
    Pump(1500);
    OnUi([&] { hide = g_hide.x; });
    {
        int pk = 0; bool rv = false, hv = false, mo = false;
        OnUi([&] { pk = (int)g_pop.kind; rv = g_revealed; hv = g_hover; mo = g_menuOpen; });
        printf("    [diag] al alejarse: escondido=%.2f popup=%d revelado=%d hover=%d menu=%d\n", hide, pk, rv, hv, mo);
    }
    Check(hide > 0.99, "Se vuelve a esconder al alejarse");

    printf("\n== 11b. Esconderse con una ventana maximizada ==\n");
    {
        SetSetting(L"autoHide", L"maximized");
        SetSetting(L"style", L"glass");
        SetSetting(L"magnifiedSize", L"80");
        Wh_ModSettingsChanged();
        Pump(900);
        HWND np = nullptr;
        OnUi([&] {
            for (auto& it : g_items)
                if (EndsWithI(it->exe, L"\\notepad.exe") && !it->windows.empty()) np = it->windows[0];
        });
        double hide = 1;
        OnUi([&] { hide = g_hide.x; });
        Check(hide < 0.01, "Sin ventana maximizada, a la vista");
        if (np) {
            ShowWindow(np, SW_MAXIMIZE);
            SetForegroundWindow(np);
            Pump(1200);
            OnUi([&] { hide = g_hide.x; });
            Check(hide > 0.99, "Con el Bloc de notas maximizado, el Dock se esconde");
            MouseGlide(sw / 2, sh / 2, sw / 2, sh - 1, 300);
            Pump(800);
            RECT wr{};
            GetWindowRect(dock, &wr);
            OnUi([&] { hide = g_hide.x; });
            Check(hide < 0.01, "Aparece al llevar el mouse al borde de abajo");
            Check(wr.right - wr.left < sw * 2 / 3, "Al aparecer ocupa solo su espacio (no los lados)");
            Dump("10b-sobre-maximizada");
            MouseGlide(sw / 2, sh - 1, sw / 2, sh / 2, 200);
            Pump(1500);
            OnUi([&] { hide = g_hide.x; });
            {
                int pk = 0; bool rv = false, hv = false, cl = false, fm = false, ff = false;
                bool pr = false, dg = false;
                OnUi([&] { pk = (int)g_pop.kind; cl = g_pop.closing; rv = g_revealed; hv = g_hover; fm = g_fgMaximized; ff = g_fgFullscreen; pr = g_press != nullptr; dg = g_drag != nullptr; });
                printf("    [diag] 11b press=%d drag=%d\n", pr, dg);
                printf("    [diag] 11b al alejarse: escondido=%.2f popup=%d cerrando=%d revelado=%d hover=%d max=%d full=%d\n", hide, pk, cl, rv, hv, fm, ff);
            }
            Check(hide > 0.99, "Se vuelve a esconder al alejarse");
            ShowWindow(np, SW_RESTORE);
            Pump(1200);
            OnUi([&] { hide = g_hide.x; });
            Check(hide < 0.01, "Al restaurar la ventana vuelve a estar a la vista");
        } else {
            Check(false, "Ventana del Bloc de notas para la prueba");
        }
    }

    printf("\n== 11c. Separación entre apps y bloquear el orden ==\n");
    {
        MouseGlide(sw / 2, sh - 60, sw / 2, sh / 3, 200);
        Pump(1200);
        double w0 = 0, w1 = 0;
        int slots = 0;
        OnUi([&] { w0 = g_lastLayout.baseR - g_lastLayout.baseL; for (auto& it : g_items) slots += it->kind != Kind::End; });
        SetSetting(L"spacing", L"20");
        Wh_ModSettingsChanged();
        Pump(900);
        OnUi([&] { w1 = g_lastLayout.baseR - g_lastLayout.baseL; });
        int expect = slots * (S(20) - S(5));
        printf("    ancho: %.0f -> %.0f (esperado +%d)\n", w0, w1, expect);
        Check(std::fabs((w1 - w0) - expect) < 3, "La separación cambia el ancho del Dock");
        Dump("11c-separacion");
        SetSetting(L"spacing", L"5");
        SetSetting(L"lockItems", L"1");
        Wh_ModSettingsChanged();
        Pump(900);
        items = SnapItems();
        ni = FindItem(items, L"\\notepad.exe");
        double y = items[ni].y + items[ni].size / 2;
        MouseGlide(sw / 2, sh / 3, (int)items[ni].cx, (int)y, 200);
        Pump(300);
        items = SnapItems();
        MouseMove((int)items[ni].cx, (int)(items[ni].y + items[ni].size / 2));
        Pump(50);
        MouseButton(MOUSEEVENTF_LEFTDOWN);
        Pump(50);
        MouseGlide((int)items[ni].cx, (int)y, (int)items[ni].cx - 120, (int)y - 4, 300);
        bool dragging = true;
        OnUi([&] { dragging = g_drag != nullptr; });
        MouseButton(MOUSEEVENTF_LEFTUP);
        Pump(300);
        Check(!dragging, "Con el orden bloqueado no se puede arrastrar");
        SetSetting(L"lockItems", L"0");
        Wh_ModSettingsChanged();
        MouseGlide((int)items[ni].cx - 120, (int)y, sw / 2, sh / 3, 200);
        Pump(900);
    }

    printf("\n== 12. Rendimiento ==\n");
    SetSetting(L"autoHide", L"maximized");
    SetSetting(L"style", L"glass");
    Wh_ModSettingsChanged();
    Pump(800);
    {
        items = SnapItems();
        double y = items[1].y + items[1].size / 2;
        // Medir cuánto tarda cada cuadro con la ampliación activa.
        MouseGlide(sw / 2, sh / 3, (int)items[1].cx, (int)y, 200);
        Pump(500);
        double total = 0, worst = 0;
        int n = 0;
        for (int k = 0; k < 60; k++) {
            OnUi([&] {
                g_anchor.Snap(items[1].cx - g_winX + k * 6);
                g_prevBounds = RECT{};
                double a = Now();
                Render(Now());
                double d = Now() - a;
                total += d;
                worst = std::max(worst, d);
                n++;
            });
        }
        printf("    Cuadro completo (dibujo + ventana) con ampliación: promedio %.2f ms, peor %.2f ms\n",
               total * 1000 / n, worst * 1000);
        Check(total * 1000 / n < 6.0, "Promedio por cuadro bien por debajo de 8.3 ms (120 Hz)");
        OnUi([&] {
            Layout L = ComputeLayout(Now());
            Surface sf{g_bits, g_winW, g_winH};
            RECT all{g_envL, 0, g_envR, g_winH};
            auto timeit = [&](const char* name, std::function<void()> fn) {
                double t0 = Now();
                for (int r = 0; r < 20; r++) fn();
                printf("      %-10s %.2f ms\n", name, (Now() - t0) * 50);
            };
            timeit("fondo", [&] { DrawBackground(L, all); });
            timeit("iconos", [&] {
                for (size_t i = 0; i < g_items.size(); i++) {
                    const IconSet* st = L.g[i].shown ? IconFor(*g_items[i]) : nullptr;
                    if (st) BlitIcon(sf, all, *st, L.g[i].x, L.g[i].y, L.g[i].size, 1, 1, true);
                }
            });
            timeit("limpiar", [&] { for (int y = 0; y < g_winH; y++) memset(g_bits + (size_t)y * g_winW + g_envL, 0, (size_t)(g_envR - g_envL) * 4); });
            timeit("ventana", [&] {
                POINT dst{g_winX + g_envL, g_winY}; SIZE sz{g_envR - g_envL, g_winH}; POINT src{g_envL, 0};
                BLENDFUNCTION bf{AC_SRC_OVER, 0, 255, AC_SRC_ALPHA};
                UpdateLayeredWindow(g_wnd.load(), nullptr, &dst, &sz, g_memDC, &src, 0, &bf, ULW_ALPHA);
            });
        });
        // Ritmo real: mover el mouse de lado a lado y medir los intervalos.
        g_frameStamps.clear();
        g_frameLat.clear();
        OnUi([&] { g_profileMax.clear(); g_profileOn = true; });
        g_recordFrames = true;
        LONGLONG qf = 0;
        { LARGE_INTEGER f; QueryPerformanceFrequency(&f); qf = f.QuadPart; }
        for (int k = 0; k < 3; k++) {
            MouseGlide((int)items[1].cx, (int)y, (int)items[items.size() - 2].cx, (int)y, 500);
            MouseGlide((int)items[items.size() - 2].cx, (int)y, (int)items[1].cx, (int)y, 500);
        }
        g_recordFrames = false;
        OnUi([&] {
            g_profileOn = false;
            for (auto& kv : g_profileMax)
                if (kv.second.first > 2) printf("      mensaje 0x%X: máx %.2f ms, %d veces > 4 ms\n", kv.first, kv.second.first, kv.second.second);
        });
        std::vector<double> iv;
        for (size_t k = 1; k < g_frameStamps.size(); k++) iv.push_back((g_frameStamps[k] - g_frameStamps[k - 1]) * 1000.0 / qf);
        std::sort(iv.begin(), iv.end());
        if (!iv.empty()) {
            double mean = 0;
            for (double v : iv) mean += v;
            mean /= iv.size();
            double p95 = iv[iv.size() * 95 / 100], p99 = iv[iv.size() * 99 / 100];
            int over = 0;
            for (double v : iv) over += v > 12.5;  // un cuadro perdido a 120 Hz
            printf("    %zu cuadros en 3 s: intervalo medio %.2f ms (%.0f fps), p95 %.2f ms, p99 %.2f ms, cuadros perdidos %d\n",
                   iv.size() + 1, mean, 1000 / mean, p95, p99, over);
            std::vector<double> lat;
            for (auto l : g_frameLat) lat.push_back(l * 1000.0 / qf);
            std::sort(lat.begin(), lat.end());
            printf("    espera del hilo del Dock: mediana %.2f ms, p95 %.2f ms, máx %.2f ms\n",
                   lat[lat.size() / 2], lat[lat.size() * 95 / 100], lat.back());
            // ¿Dónde están los saltos? (intervalos > 12.5 ms, en orden)
            for (size_t k = 1; k < g_frameStamps.size(); k++) {
                double d = (g_frameStamps[k] - g_frameStamps[k - 1]) * 1000.0 / qf;
                if (d > 12.5) printf("      salto de %.1f ms en el cuadro %zu (espera previa %.2f ms)\n", d, k, g_frameLat[k - 1] * 1000.0 / qf);
            }
            Check(mean < 8.8 && mean > 7.8, "Corre a ~120 cuadros por segundo");
            Check(over <= (int)iv.size() / 100, "Menos de 1 % de cuadros perdidos");
        }
        DWORD gdi0 = GetGuiResources(GetCurrentProcess(), GR_GDIOBJECTS);
        for (int k = 0; k < 6; k++) {
            MouseGlide((int)items[1].cx, (int)y, (int)items[items.size() - 2].cx, (int)y, 300);
            MouseGlide((int)items[items.size() - 2].cx, (int)y, (int)items[1].cx, (int)y, 300);
        }
        Pump(500);
        DWORD gdi1 = GetGuiResources(GetCurrentProcess(), GR_GDIOBJECTS);
        printf("    objetos GDI antes/después: %lu / %lu\n", gdi0, gdi1);
        Check(gdi1 <= gdi0 + 4, "Sin fugas de objetos GDI");
        // Con tope de 60 cuadros por segundo (en el monitor de 120 Hz).
        SetSetting(L"frameRate", L"max60");
        Wh_ModSettingsChanged();
        Pump(500);
        g_frameStamps.clear();
        g_frameLat.clear();
        g_recordFrames = true;
        MouseGlide((int)items[1].cx, (int)y, (int)items[items.size() - 2].cx, (int)y, 500);
        MouseGlide((int)items[items.size() - 2].cx, (int)y, (int)items[1].cx, (int)y, 500);
        g_recordFrames = false;
        if (g_frameStamps.size() > 10) {
            double span = (g_frameStamps.back() - g_frameStamps.front()) * 1000.0 / qf;
            double mean = span / (g_frameStamps.size() - 1);
            printf("    con tope de 60: %zu cuadros, intervalo medio %.2f ms (%.0f fps)\n", g_frameStamps.size(), mean,
                   1000 / mean);
            Check(mean > 15.5 && mean < 18.5, "Con el tope de 60 corre a ~60 cuadros por segundo");
        } else {
            Check(false, "Con el tope de 60 corre a ~60 cuadros por segundo");
        }
        SetSetting(L"frameRate", L"monitor");
        Wh_ModSettingsChanged();
        Pump(500);
    }

    printf("\n== 13. Desactivar el mod ==\n");
    if (g_runHook) UnhookWindowsHookEx(g_runHook);
    g_runHook = nullptr;
    DWORD a = GetTickCount();
    Wh_ModUninit();
    printf("    Wh_ModUninit tardó %lu ms\n", GetTickCount() - a);
    Check(g_wnd.load() == nullptr && !IsWindow(dock), "La ventana del Dock se destruyó");
    Pump(300);
    Check(IsWindowVisible(tray), "La barra de tareas volvió a mostrarse");
    {
        std::lock_guard<std::mutex> l(g_testMx);
        Check(g_testStore[L"tbHidden"] == L"0", "Quedó guardado que ya no está escondida");
    }
    // Volver a activarlo (como al cambiar de versión)
    Check(Wh_ModInit(), "Se puede volver a activar");
    for (int i = 0; i < 100 && !g_wnd.load(); i++) Pump(50);
    if (g_wnd.load()) g_runHook = SetWindowsHookExW(WH_GETMESSAGE, RunHook, nullptr,
                                                    GetWindowThreadProcessId(g_wnd.load(), nullptr));
    Pump(800);
    items = SnapItems();
    {
        int ri2 = FindItem(items, L"\\regedit.exe");
        Check(ri2 >= 0 && items[ri2].pinned && FindItem(items, L"\\winemine.exe") < 0 &&
                  FindItem(items, L"\\wordpad.exe") < 0,
              "Recuerda los anclados (y los que se quitaron siguen fuera)");
    }
    if (g_runHook) UnhookWindowsHookEx(g_runHook);
    Wh_ModUninit();

    if (reg) TerminateProcess(reg, 0);
    printf("\nResultado: %d bien, %d fallas\n", g_pass, g_fail);
    return g_fail ? 1 : 0;
}
