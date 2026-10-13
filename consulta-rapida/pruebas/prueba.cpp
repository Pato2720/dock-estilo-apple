// Banco de pruebas de "Consulta rápida al seleccionar": carga el código del
// mod tal cual en un proceso que hace de navegador, contra un servidor falso
// compatible con OpenAI (servidor_falso.py), y guarda imágenes de la
// ventanita y del botón en out/*.bgra.
#define NOMINMAX
#include <windows.h>
#include <cstdarg>
#include <cstdio>
#include <map>
#include <string>

// ---- API de Windhawk simulada ----
static std::map<std::wstring, std::wstring> g_testSettings;

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

// ---- El mod, sin cambios ----
#include "../consulta-rapida.wh.cpp"

static int g_pass = 0, g_fail = 0;
static void Check(bool ok, const char* what) {
    printf("  [%s] %s\n", ok ? "OK  " : "FALLA", what);
    (ok ? g_pass : g_fail)++;
}

static void Pump(int ms) {
    ULONGLONG end = GetTickCount64() + ms;
    while (GetTickCount64() < end) {
        MSG m;
        while (PeekMessageW(&m, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&m);
            DispatchMessageW(&m);
        }
        Sleep(5);
    }
}

// Espera a que empiece una consulta nueva (después de 'since') y termine.
static uint64_t g_since = 0;
static void MarkAsk() { g_since = g_askGen.load(); }
static bool WaitAnswer(int ms) {
    ULONGLONG end = GetTickCount64() + ms;
    while (GetTickCount64() < end) {
        AnswerState a = CurrentAnswer();
        if (a.gen > g_since && a.done) return true;
        Pump(20);
    }
    return false;
}

static void SaveBits(const char* path, const uint32_t* bits, int w, int h) {
    FILE* f = fopen(path, "wb");
    if (!f) return;
    int hdr[2] = {w, h};
    fwrite(hdr, sizeof(hdr), 1, f);
    fwrite(bits, 4, (size_t)w * h, f);
    fclose(f);
}

// La ventanita, dibujada con su propio código en un DIB (opaco).
static void DumpPopup(const char* path) {
    LayoutPopup();
    Canvas cv(g_pop.w, g_pop.h);
    PaintPopup(cv.dc);
    // Lo que escribe la persona va en un control de Windows: se imita con su texto.
    wchar_t buf[256] = {};
    GetWindowTextW(g_pop.edit, buf, ARRAYSIZE(buf));
    RECT er;
    GetWindowRect(g_pop.edit, &er);
    MapWindowPoints(nullptr, g_pop.wnd, (POINT*)&er, 2);
    HGDIOBJ of = SelectObject(cv.dc, g_pop.font);
    SetBkMode(cv.dc, TRANSPARENT);
    SetTextColor(cv.dc, buf[0] ? g_pop.c.text : g_pop.c.dim);
    DrawTextW(cv.dc, buf[0] ? buf : L"Pregunta algo sobre el texto…", -1, &er, DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX);
    SelectObject(cv.dc, of);
    GdiFlush();
    for (int i = 0; i < cv.w * cv.h; i++) cv.bits[i] |= 0xFF000000u;
    SaveBits(path, cv.bits, cv.w, cv.h);
}

static void Defaults(const wchar_t* endpoint) {
    g_testSettings.clear();
    g_testSettings[L"provider"] = L"custom";
    g_testSettings[L"endpoint"] = endpoint;
    g_testSettings[L"model"] = L"";
    g_testSettings[L"apiKey"] = L"";
    g_testSettings[L"showButton"] = L"1";
    g_testSettings[L"hotkey"] = L"ctrlShiftSpace";
    g_testSettings[L"defaultAction"] = L"auto";
    g_testSettings[L"answerLanguage"] = L"español";
    g_testSettings[L"translateTo"] = L"inglés";
    g_testSettings[L"maxTokens"] = L"400";
    g_testSettings[L"selectionSource"] = L"clipboard";
    g_testSettings[L"theme"] = L"light";
}

static std::string ReadFile(const char* path) {
    std::string s;
    FILE* f = fopen(path, "rb");
    if (!f) return s;
    char b[4096];
    size_t n;
    while ((n = fread(b, 1, sizeof(b), f)) > 0) s.append(b, n);
    fclose(f);
    return s;
}

int wmain() {
    setvbuf(stdout, nullptr, _IONBF, 0);
    SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
    CreateDirectoryW(L"out", nullptr);
    Defaults(L"http://127.0.0.1:8765/v1");

    printf("== Texto y JSON ==\n");
    Check(JsonString("a\"b\\c\nd\x01") == "\"a\\\"b\\\\c\\nd\\u0001\"", "Escapa comillas, barras y controles");
    {
        std::string j = "{\"x\":1,\"content\" : \"hola \\u00e1rbol \\ud83c\\udf40 \\\"ok\\\"\"}", v;
        Check(JsonFind(j, "content", v) && FromUtf8(v) == L"hola árbol \U0001F340 \"ok\"",
              "Lee cadenas JSON con acentos, emojis y comillas");
        Check(!JsonFind("{\"content\":null}", "content", v), "Un contenido null no es texto");
    }
    Check(JsonIds("{\"data\":[{\"id\":\"a\"},{\"id\":\"b-npu\"}]}") == std::vector<std::wstring>{L"a", L"b-npu"},
          "Lee la lista de modelos");
    Check(PickModel({L"phi-4-mini-cpu", L"Phi-4-mini-instruct-qnn-NPU:2"}) == L"Phi-4-mini-instruct-qnn-NPU:2" &&
              PickModel({L"llama3.2"}) == L"llama3.2" && PickModel({}).empty(),
          "Prefiere los modelos hechos para la NPU");
    Check(FirstLocalUrl("🟢 Model management service is running on http://localhost:5273/openai/status\n") ==
              L"http://localhost:5273",
          "Encuentra la dirección de Foundry Local en «foundry service status»");
    {
        bool thinking = false;
        std::wstring c = CleanAnswer(L"<think>razono</think>## Título\n**Negrita** y `código`\n- uno\n* dos", thinking);
        Check(c == L"Título\nNegrita y código\n• uno\n• dos" && !thinking, "Quita el razonamiento y las marcas de Markdown");
        CleanAnswer(L"<think>todavía pensando", thinking);
        Check(thinking, "Se nota cuando el modelo todavía está pensando");
    }
    Check(CountWords(L"  serendipia ") == 1 && CountWords(L"hola mundo cruel y raro") == 5, "Cuenta palabras");
    {
        SseReader r;
        std::wstring got;
        auto onText = [&](const std::wstring& t) { got += t; };
        auto onErr = [&](const std::wstring&) {};
        std::string a = "data: {\"choices\":[{\"delta\":{\"content\":\"Ho\"}}]}\n\ndata: {\"choices\":[{\"del";
        std::string b = "ta\":{\"content\":\"la\"}}]}\n\ndata: [DONE]\n\n";
        bool more = r.Feed(a.data(), a.size(), onText, onErr) && r.Feed(b.data(), b.size(), onText, onErr);
        Check(got == L"Hola" && !more, "Une las partes de la respuesta aunque una línea llegue cortada");
    }
    {
        Settings st = CurrentSettings();
        std::string body = ChatBody(L"m", SystemPrompt(st), UserPrompt(Action::Define, L"árbol", L"", st), 300);
        Check(body.find("\"stream\":true") != std::string::npos && body.find("\"max_tokens\":300") != std::string::npos &&
                  body.find(ToUtf8(L"«árbol»")) != std::string::npos,
              "Arma el pedido para la API compatible con OpenAI");
    }

    printf("== Servicio ==\n");
    LoadSettings();
    {
        Backend b;
        std::wstring err;
        Check(ResolveBackend(CurrentSettings(), b, err) && b.model == L"Phi-4-mini-instruct-qnn-npu:2",
              "Encuentra el servicio y elige el modelo de NPU");
        printf("    %ls\n", (b.label + L" " + err).c_str());
    }
    Check(Wh_ModInit(), "Wh_ModInit (como en el proceso principal del navegador)");
    for (int i = 0; i < 100 && !(g_host && g_mouseHook && g_keyHook); i++) Pump(20);
    Check(g_host && g_pill && g_mouseHook && g_keyHook, "Se crearon las ventanas y los ganchos");

    printf("== Ventanita ==\n");
    POINT pt{500, 300};
    MarkAsk();
    PostMessageW(g_host, WM_APP_SELECTION, 0, (LPARAM)new SelectionResult{L"serendipia", pt, nullptr});
    Pump(300);
    Check(g_pop.wnd && IsWindowVisible(g_pop.wnd), "Se abre la ventanita con lo seleccionado");
    Check(WaitAnswer(5000), "Llega la respuesta");
    {
        AnswerState a = CurrentAnswer();
        bool err, status;
        std::wstring shown = AnswerText(a, err, status);
        Check(!err && shown.find(L"Serendipia: hallazgo valioso") == 0 && shown.find(L"• Ejemplo") != std::wstring::npos &&
                  shown.find(L"think") == std::wstring::npos,
              "Con una palabra la define, sin el razonamiento");
        printf("    %ls\n", a.label.c_str());
        std::string req = ReadFile("ultimo.json");
        Check(req.find(ToUtf8(L"Define «serendipia»")) != std::string::npos &&
                  req.find("qnn-npu") != std::string::npos,
              "Pide la definición al modelo de NPU");
    }
    Pump(200);
    DumpPopup("out\\1-definir-claro.bgra");
    // Una pregunta propia.
    SetWindowTextW(g_pop.edit, L"¿De qué idioma viene?");
    MarkAsk();
    PostMessageW(g_pop.wnd, WM_APP_SUBMIT, 0, 0);
    Pump(100);
    Check(WaitAnswer(5000), "Responde una pregunta escrita en la caja");
    Check(ReadFile("ultimo.json").find(ToUtf8(L"Pregunta: ¿De qué idioma viene?")) != std::string::npos,
          "La pregunta va con el texto seleccionado");
    // Un botón: Traducir.
    MarkAsk();
    PostMessageW(g_pop.wnd, WM_LBUTTONUP, 0, MAKELPARAM((g_pop.chips[3].left + g_pop.chips[3].right) / 2,
                                                         (g_pop.chips[3].top + g_pop.chips[3].bottom) / 2));
    Pump(100);
    Check(WaitAnswer(5000) && ReadFile("ultimo.json").find(ToUtf8(L"Traduce al inglés")) != std::string::npos,
          "El botón Traducir pide la traducción");
    PostMessageW(g_pop.wnd, WM_KEYDOWN, VK_ESCAPE, 0);
    Pump(200);
    Check(!IsWindowVisible(g_pop.wnd), "Esc la cierra");

    // Texto largo, tema oscuro: explica.
    g_testSettings[L"theme"] = L"dark";
    Wh_ModSettingsChanged();
    MarkAsk();
    PostMessageW(g_host, WM_APP_SELECTION, 0,
                 (LPARAM)new SelectionResult{L"La serendipia es un descubrimiento o un hallazgo afortunado, valioso e "
                                             L"inesperado que se produce de manera accidental o casual.",
                                             pt, nullptr});
    Pump(300);
    Check(WaitAnswer(5000) && ReadFile("ultimo.json").find(ToUtf8(L"Explica con palabras sencillas")) != std::string::npos,
          "Con un texto largo lo explica");
    Pump(200);
    DumpPopup("out\\2-explicar-oscuro.bgra");
    PostMessageW(g_pop.wnd, WM_CLOSE, 0, 0);
    Pump(200);

    // Errores claros.
    g_testSettings[L"endpoint"] = L"http://127.0.0.1:8765/error/v1";
    g_testSettings[L"model"] = L"x";
    g_testSettings[L"theme"] = L"light";
    Wh_ModSettingsChanged();
    MarkAsk();
    PostMessageW(g_host, WM_APP_SELECTION, 0, (LPARAM)new SelectionResult{L"hola", pt, nullptr});
    Pump(300);
    Check(WaitAnswer(5000) && CurrentAnswer().error == L"El modelo no está cargado", "Muestra el error del servicio");
    PostMessageW(g_pop.wnd, WM_CLOSE, 0, 0);
    g_testSettings[L"endpoint"] = L"http://127.0.0.1:1/v1";
    Wh_ModSettingsChanged();
    MarkAsk();
    PostMessageW(g_host, WM_APP_SELECTION, 0, (LPARAM)new SelectionResult{L"hola", pt, nullptr});
    Pump(300);
    Check(WaitAnswer(15000) && CurrentAnswer().error.find(L"No responde el servicio") == 0,
          "Y si el servicio no está en marcha");
    DumpPopup("out\\3-error.bgra");
    PostMessageW(g_pop.wnd, WM_CLOSE, 0, 0);
    Pump(200);

    printf("== Leer la selección copiándola ==\n");
    {
        WNDCLASSW wc{};
        wc.lpfnWndProc = DefWindowProcW;
        wc.hInstance = GetModuleHandleW(nullptr);
        wc.lpszClassName = L"NavegadorFalso";
        RegisterClassW(&wc);
        HWND top = CreateWindowExW(0, L"NavegadorFalso", L"Navegador", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 500,
                                   200, nullptr, nullptr, wc.hInstance, nullptr);
        HWND edit = CreateWindowExW(0, L"EDIT", L"texto elegido", WS_CHILD | WS_VISIBLE, 10, 10, 400, 30, top, nullptr,
                                    wc.hInstance, nullptr);
        SetForegroundWindow(top);
        SetFocus(edit);
        SendMessageW(edit, EM_SETSEL, 0, -1);
        Pump(200);
        // Algo que ya había en el portapapeles.
        if (OpenClipboard(top)) {
            EmptyClipboard();
            const wchar_t prev[] = L"lo de antes";
            HGLOBAL g = GlobalAlloc(GMEM_MOVEABLE, sizeof(prev));
            memcpy(GlobalLock(g), prev, sizeof(prev));
            GlobalUnlock(g);
            SetClipboardData(CF_UNICODETEXT, g);
            CloseClipboard();
        }
        std::wstring got;
        HANDLE t = CreateThread(nullptr, 0, [](LPVOID p) -> DWORD {
            *(std::wstring*)p = SelectionByClipboard();
            return 0;
        }, &got, 0, nullptr);
        while (WaitForSingleObject(t, 0) == WAIT_TIMEOUT) Pump(10);
        CloseHandle(t);
        Check(got == L"texto elegido", "Lee lo seleccionado");
        Check(ClipboardText(top) == L"lo de antes", "Y deja el portapapeles como estaba");
        DestroyWindow(top);
    }

    printf("== Botón \"Preguntar\" ==\n");
    {
        int margin = 0;
        auto light = BuildPill(MakeColors(ThemeMode::Light), 1.25f, margin);
        auto dark = BuildPill(MakeColors(ThemeMode::Dark), 1.25f, margin);
        Check(light && dark && light->bits[(light->h / 2) * light->w + light->w / 2] >> 24 == 255 &&
                  light->bits[0] >> 24 < 40,
              "El botón es opaco al centro y transparente en las esquinas");
        if (light) SaveBits("out\\4-boton-claro.bgra", light->bits, light->w, light->h);
        if (dark) SaveBits("out\\5-boton-oscuro.bgra", dark->bits, dark->w, dark->h);
    }

    Wh_ModUninit();
    Check(g_workers == 0 && !g_uiThread, "Al desactivarlo no queda ningún hilo");
    printf("Resultado: %d bien, %d fallas\n", g_pass, g_fail);
    return g_fail ? 1 : 0;
}
