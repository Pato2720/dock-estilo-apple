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
    Check(PickModel({L"DeepSeek-R1-Distill-Qwen-7B-openvino-npu:4", L"Phi-4-mini-instruct-openvino-npu:2"}) ==
              L"Phi-4-mini-instruct-openvino-npu:2" &&
              PickModel({L"phi-4-mini-cpu", L"Phi-4-mini-instruct-qnn-NPU:2"}) == L"Phi-4-mini-instruct-qnn-NPU:2" &&
              PickModel({L"llama3.2"}) == L"llama3.2" && PickModel({}).empty(),
          "Prefiere los modelos de NPU, y los rápidos antes que los de razonamiento");
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
    // Foundry Local: la dirección sale de su propio comando (versión nueva y vieja).
    g_testSettings[L"provider"] = L"foundry";
    g_testSettings[L"endpoint"] = L"";
    LoadSettings();
    for (const wchar_t* mode : {L"new", L"old"}) {
        SetEnvironmentVariableW(L"FAKE_FOUNDRY", mode);
        ForgetBackend();
        Backend b;
        std::wstring err;
        bool ok = ResolveBackend(CurrentSettings(), b, err);
        printf("    %ls: %ls%ls\n", mode, b.base.c_str(), err.c_str());
        Check(ok && b.base == L"http://127.0.0.1:8765/v1" && b.model == L"Phi-4-mini-instruct-qnn-npu:2",
              wcscmp(mode, L"new") ? "Encuentra Foundry Local de antes («foundry service status»)"
                                   : "Encuentra Foundry Local nuevo («foundry server status»)");
    }
    SetEnvironmentVariableW(L"FAKE_FOUNDRY", L"off");
    ForgetBackend();
    {
        Backend b;
        std::wstring err;
        Check(!ResolveBackend(CurrentSettings(), b, err) && err.find(L"foundry server start") != std::wstring::npos,
              "Si Foundry Local no arranca, dice cómo arrancarlo");
    }
    // Foundry ofrece un modelo descargado pero no cargado: se carga solo.
    SetEnvironmentVariableW(L"FAKE_FOUNDRY", L"load");
    ForgetBackend();
    DeleteFileW(L"cargado.txt");
    {
        // Un comando que pregunta algo (¿descargar?) no deja al mod esperando.
        std::string out;
        ULONGLONG t0 = GetTickCount64();
        RunFoundry(L"model load \"Phi-3-mini-4k-instruct-openvino-npu:1\"", 30000, out, nullptr);
        Check(GetTickCount64() - t0 < 5000, "Si Foundry pregunta algo, no se queda esperando la respuesta");
    }
    MarkAsk();
    StartAsk(Action::Define, L"hola", L"");
    Check(WaitAnswer(20000) && CurrentAnswer().error.empty() && !CurrentAnswer().raw.empty(),
          "Si el modelo no está cargado, lo carga y responde");
    std::string loadedName = ReadFile("cargado.txt");
    printf("    cargado: %s · %ls\n", loadedName.c_str(), CurrentAnswer().label.c_str());
    Check(loadedName == "Phi-4-mini-instruct-openvino-npu:2",
          "Elige uno descargado de NPU y rápido (no el del catálogo sin descargar ni el de razonamiento)");
    ForgetBackend();
    MarkAsk();
    StartAsk(Action::Define, L"hola", L"");
    Check(WaitAnswer(20000) && CurrentAnswer().error.empty() &&
              CurrentAnswer().label.find(L"Phi-4-mini-instruct-openvino-npu:2") != std::wstring::npos,
          "La siguiente vez usa el que ya está cargado");
    // Si la lista no nombra los modelos (solo alias o nada), se le pregunta a la API.
    struct Case {
        const wchar_t* mode;
        const char* loaded;
        const char* what;
    };
    for (const Case& c : {Case{L"silent", "Phi-4-mini-instruct-openvino-npu:2", "Si la lista no dice nada, encuentra el cargado preguntando a la API"},
                          Case{L"alias", "DeepSeek-R1-Distill-Qwen-7B-openvino-npu:4", "Con una lista de alias (y colores), encuentra el cargado"}}) {
        SetEnvironmentVariableW(L"FAKE_FOUNDRY", c.mode);
        FILE* f = fopen("cargado.txt", "wb");
        if (f) {
            fputs(c.loaded, f);
            fclose(f);
        }
        ForgetBackend();
        MarkAsk();
        StartAsk(Action::Define, L"hola", L"");
        bool ok = WaitAnswer(20000) && CurrentAnswer().error.empty();
        printf("    %ls: %ls %ls\n", c.mode, CurrentAnswer().label.c_str(), CurrentAnswer().error.c_str());
        Check(ok && CurrentAnswer().label.find(FromUtf8(c.loaded)) != std::wstring::npos, c.what);
    }
    {
        // Como en tu equipo: los comandos de Foundry se cuelgan desde el
        // navegador. Se encuentra por el puerto de su proceso y el modelo
        // cargado, preguntando a la API, en pocos segundos.
        SetEnvironmentVariableW(L"FAKE_FOUNDRY", L"hang");
        STARTUPINFOW si{sizeof(si)};
        PROCESS_INFORMATION pi{};
        wchar_t cmd[] = L"foundrylocald.exe";
        bool started = CreateProcessW(nullptr, cmd, nullptr, nullptr, FALSE, CREATE_NO_WINDOW, nullptr, nullptr, &si, &pi);
        Sleep(700);
        g_foundryPorts = [] {
            std::vector<USHORT> v;
            std::string p = ReadFile("puerto.txt");
            if (!p.empty()) v.push_back((USHORT)atoi(p.c_str()));
            return v;
        };
        ForgetBackend();
        ULONGLONG t0 = GetTickCount64();
        MarkAsk();
        StartAsk(Action::Define, L"hola", L"");
        bool ok = WaitAnswer(30000) && CurrentAnswer().error.empty();
        ULONGLONG ms = GetTickCount64() - t0;
        printf("    %llu ms: %ls · %ls %ls\n", ms, CurrentAnswer().label.c_str(), CurrentAnswer().raw.c_str(),
               CurrentAnswer().error.c_str());
        Check(started && ok && CurrentAnswer().label.find(L"Phi-4-mini-instruct-openvino-npu:2") != std::wstring::npos &&
                  CurrentAnswer().raw == L"Hola desde la NPU.",
              "Con los comandos de Foundry colgados, lo encuentra por su puerto y usa el modelo cargado");
        Check(ms < 6000, "Y en pocos segundos (sin esperar a los comandos)");
        g_foundryPorts = FoundryListenPorts;
        DeleteFileW(L"puerto.txt");
        if (started) {
            TerminateProcess(pi.hProcess, 0);
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
        }
    }
    ForgetBackend();
    Defaults(L"http://127.0.0.1:8765/v1");
    LoadSettings();
    Check(Wh_ModInit(), "Wh_ModInit (como en el proceso principal del navegador)");
    for (int i = 0; i < 100 && !(g_host && g_mouseHook && g_keyHook); i++) Pump(20);
    Check(g_host && g_pill && g_mouseHook && g_keyHook, "Se crearon las ventanas y los ganchos");

    printf("== Ventanita ==\n");
    // Un "navegador" en primer plano, con el foco en su texto.
    WNDCLASSW bwc{};
    bwc.lpfnWndProc = DefWindowProcW;
    bwc.hInstance = GetModuleHandleW(nullptr);
    bwc.lpszClassName = L"NavegadorEnUso";
    RegisterClassW(&bwc);
    HWND browser = CreateWindowExW(0, L"NavegadorEnUso", L"Navegador", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 40, 40, 700,
                                   500, nullptr, nullptr, bwc.hInstance, nullptr);
    HWND page = CreateWindowExW(0, L"EDIT", L"texto de la página", WS_CHILD | WS_VISIBLE, 10, 10, 400, 30, browser,
                                nullptr, bwc.hInstance, nullptr);
    SetForegroundWindow(browser);
    SetFocus(page);
    Pump(300);
    auto pageText = [&] {
        wchar_t b[128] = {};
        GetWindowTextW(page, b, ARRAYSIZE(b));
        return std::wstring(b);
    };
    auto sendKeys = [](std::initializer_list<WORD> vks) {
        for (WORD vk : vks) {
            INPUT in[2] = {};
            in[0].type = in[1].type = INPUT_KEYBOARD;
            in[0].ki.wVk = in[1].ki.wVk = vk;
            in[0].ki.wScan = in[1].ki.wScan = (WORD)MapVirtualKeyW(vk, MAPVK_VK_TO_VSC);
            in[1].ki.dwFlags = KEYEVENTF_KEYUP;
            SendInput(2, in, sizeof(INPUT));
        }
    };
    auto sendText = [](const std::wstring& t) {
        for (wchar_t ch : t) {
            INPUT in[2] = {};
            in[0].type = in[1].type = INPUT_KEYBOARD;
            in[0].ki.wScan = in[1].ki.wScan = ch;
            in[0].ki.dwFlags = KEYEVENTF_UNICODE;
            in[1].ki.dwFlags = KEYEVENTF_UNICODE | KEYEVENTF_KEYUP;
            SendInput(2, in, sizeof(INPUT));
        }
    };
    POINT pt{500, 300};
    MarkAsk();
    PostMessageW(g_host, WM_APP_SELECTION, 0, (LPARAM)new SelectionResult{L"serendipia", pt, nullptr});
    Pump(300);
    Check(g_pop.wnd && IsWindowVisible(g_pop.wnd), "Se abre la ventanita con lo seleccionado");
    Check(GetForegroundWindow() == browser && GetFocus() == page &&
              (GetWindowLongPtrW(g_pop.wnd, GWL_EXSTYLE) & (WS_EX_NOACTIVATE | WS_EX_TOPMOST)) ==
                  (WS_EX_NOACTIVATE | WS_EX_TOPMOST),
          "No le quita el primer plano ni el foco a la app en uso");
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
    // Una pregunta propia, escrita con el teclado sin que la ventanita
    // tome el foco.
    PostMessageW(g_pop.wnd, WM_LBUTTONUP, 0, MAKELPARAM((g_pop.input.left + g_pop.input.right) / 2,
                                                         (g_pop.input.top + g_pop.input.bottom) / 2));
    Pump(100);
    Check(g_typing, "Al tocar la caja se puede escribir");
    sendKeys({'X', VK_BACK});
    sendText(L"¿De qué idioma viene");
    sendKeys({VK_OEM_2});  // una tecla normal: pasa por la distribución de teclado (aquí, /)
    Pump(300);
    printf("    escrito: «%ls»\n", g_pop.query.c_str());
    Check(g_pop.query.rfind(L"¿De qué idioma viene", 0) == 0, "Lo que se teclea va a la caja (con acentos)");
    Check(pageText() == L"texto de la página" && GetForegroundWindow() == browser && GetFocus() == page,
          "La página no recibe las teclas y sigue en primer plano");
    g_pop.query = L"¿De qué idioma viene?";
    MarkAsk();
    sendKeys({VK_RETURN});
    Pump(200);
    Check(WaitAnswer(5000), "Enter manda la pregunta y responde");
    Check(ReadFile("ultimo.json").find(ToUtf8(L"Pregunta: ¿De qué idioma viene?")) != std::string::npos,
          "La pregunta va con el texto seleccionado");
    Check(!g_typing && g_pop.query.empty(), "Después de mandarla, el teclado vuelve a la app");
    // Un botón: Traducir.
    MarkAsk();
    PostMessageW(g_pop.wnd, WM_LBUTTONUP, 0, MAKELPARAM((g_pop.chips[3].left + g_pop.chips[3].right) / 2,
                                                         (g_pop.chips[3].top + g_pop.chips[3].bottom) / 2));
    Pump(100);
    Check(WaitAnswer(5000) && ReadFile("ultimo.json").find(ToUtf8(L"Traduce al inglés")) != std::string::npos,
          "El botón Traducir pide la traducción");
    sendKeys({VK_ESCAPE});
    Pump(300);
    Check(!IsWindowVisible(g_pop.wnd) && GetForegroundWindow() == browser && GetFocus() == page,
          "Esc la cierra y la app sigue en primer plano");

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
    {
        // Otra ventana "siempre visible" encima: la ventanita vuelve a quedar arriba.
        RECT pr;
        GetWindowRect(g_pop.wnd, &pr);
        HWND other = CreateWindowExW(WS_EX_TOPMOST | WS_EX_TOOLWINDOW, L"NavegadorEnUso", L"Otra", WS_POPUP | WS_VISIBLE,
                                     pr.left, pr.top, 300, 200, nullptr, nullptr, GetModuleHandleW(nullptr), nullptr);
        SetWindowPos(other, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
        Pump(600);
        bool above = false;
        for (HWND h = GetWindow(g_pop.wnd, GW_HWNDNEXT); h; h = GetWindow(h, GW_HWNDNEXT)) {
            if (h == other) above = true;
        }
        Check(above, "Si otra ventana siempre visible la tapa, vuelve a quedar encima");
        DestroyWindow(other);
    }
    // Así se ve mientras se escribe.
    PostMessageW(g_pop.wnd, WM_LBUTTONUP, 0, MAKELPARAM((g_pop.input.left + g_pop.input.right) / 2,
                                                         (g_pop.input.top + g_pop.input.bottom) / 2));
    Pump(100);
    g_pop.query = L"¿Y cómo se dice en inglés?";
    g_pop.caretOn = true;
    Pump(200);
    DumpPopup("out\\2-explicar-oscuro.bgra");
    PostMessageW(g_pop.wnd, WM_CLOSE, 0, 0);
    Pump(200);

    // Una pregunta seleccionada: la responde.
    g_testSettings[L"theme"] = L"light";
    Wh_ModSettingsChanged();
    Check(LooksLikeQuestion(L"How does it work?") && LooksLikeQuestion(L"«¿Por qué el cielo es azul»") &&
              LooksLikeQuestion(L"(¿cuándo?)") && !LooksLikeQuestion(L"serendipia") && !LooksLikeQuestion(L""),
          "Reconoce cuándo lo seleccionado es una pregunta");
    auto clickChip = [](int i) {
        PostMessageW(g_pop.wnd, WM_LBUTTONUP, 0, MAKELPARAM((g_pop.chips[i].left + g_pop.chips[i].right) / 2,
                                                             (g_pop.chips[i].top + g_pop.chips[i].bottom) / 2));
    };
    MarkAsk();
    PostMessageW(g_host, WM_APP_SELECTION, 0,
                 (LPARAM)new SelectionResult{L"¿Por qué el cielo es azul?", pt, nullptr});
    Pump(300);
    Check(WaitAnswer(5000) && g_pop.action == Action::Answer &&
              ReadFile("ultimo.json").find(ToUtf8(L"Responde esta pregunta")) != std::string::npos,
          "Si lo seleccionado es una pregunta, la responde (botón Pregunta)");
    Pump(100);
    DumpPopup("out\\6-pregunta.bgra");
    {
        RECT last = g_pop.chips[kChips - 1];
        Check(last.right <= g_pop.close.right && last.top == g_pop.chips[0].top, "Los cinco botones caben en una fila");
    }
    // Cambiar de opción mientras responde: el modelo sigue ocupado con la
    // anterior un rato; el mod espera y pide la nueva, sin mostrar error.
    MarkAsk();
    clickChip(2);  // Resumir (el servidor falso lo hace lento)
    Pump(700);
    Check(!CurrentAnswer().done, "El resumen todavía no termina");
    MarkAsk();
    clickChip(1);  // Explicar, antes de que termine el resumen
    bool waited = false, done = false;
    for (ULONGLONG end = GetTickCount64() + 15000; GetTickCount64() < end;) {
        AnswerState a = CurrentAnswer();
        if (a.gen > g_since && a.status.find(L"Esperando") == 0) waited = true;
        if (a.gen > g_since && a.done) { done = true; break; }
        Pump(20);
    }
    printf("    error: «%ls»\n", CurrentAnswer().error.c_str());
    Check(done && CurrentAnswer().error.empty() && !CurrentAnswer().raw.empty() && g_pop.action == Action::Explain &&
              ReadFile("ultimo.json").find(ToUtf8(L"Explica con palabras sencillas")) != std::string::npos,
          "Al cambiar de opción mientras responde, no queda en «ocupado»: responde la nueva");
    Check(waited, "Y mientras tanto avisa que espera al modelo");
    // Volver a una opción ya respondida: al instante, sin preguntar otra vez.
    DeleteFileA("ultimo.json");
    MarkAsk();
    clickChip(4);
    Pump(100);
    {
        AnswerState a = CurrentAnswer();
        Check(a.done && a.error.empty() && !a.raw.empty() && g_pop.action == Action::Answer &&
                  GetFileAttributesA("ultimo.json") == INVALID_FILE_ATTRIBUTES,
              "Volver a una opción ya respondida la muestra al instante, sin preguntar de nuevo");
    }
    MarkAsk();
    clickChip(4);
    Pump(100);
    Check(g_askGen.load() == g_since, "Tocar la opción que ya se ve no la vuelve a pedir");
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
