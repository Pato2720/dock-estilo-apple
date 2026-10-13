// ==WindhawkMod==
// @id              consulta-rapida
// @name            Consulta rápida al seleccionar
// @description     Selecciona texto en el navegador y pregunta, define, explica o traduce ahí mismo en una ventanita, con un modelo local en la NPU (Foundry Local) u otro servicio
// @version         1.1
// @author          local
// @include         chrome.exe
// @include         msedge.exe
// @include         firefox.exe
// @include         brave.exe
// @include         opera.exe
// @include         vivaldi.exe
// @compilerOptions -lwinhttp -lole32 -loleaut32 -luuid -ldwmapi -lgdi32 -luser32 -ladvapi32
// ==/WindhawkMod==

// ==WindhawkModReadme==
/*
# Consulta rápida al seleccionar

Como Click to Do, pero dentro del navegador: seleccionas texto y aparece un
botoncito **Preguntar**. Al tocarlo se abre una ventanita junto al texto que
lo **define**, **explica**, **resume** o **traduce**, o responde lo que le
preguntes sobre él. Sin abrir Copilot ni cambiar de pestaña.

## Con qué responde

* **Foundry Local** (de fábrica): modelos de Microsoft que corren en tu
  equipo, en la **NPU** si tienes un PC Copilot+ (o en la GPU o la CPU si
  no). El texto no sale de tu equipo. Para instalarlo:

      winget install Microsoft.FoundryLocal
      foundry run phi-4-mini

  El mod encuentra solo la dirección del servicio y prefiere los modelos
  hechos para la NPU.
* **Ollama**: otro servidor local (`ollama run llama3.2`).
* **Otro servicio** compatible con la API de OpenAI (local o en la nube),
  con su dirección, modelo y clave. En la nube, el texto seleccionado se
  envía a ese servicio.

El modelo de Click to Do (Phi Silica) no se puede usar desde un mod: Windows
solo se lo deja a apps empaquetadas con un permiso de Microsoft.

## Cómo se usa

* Selecciona texto y toca **Preguntar** (aparece junto al mouse unos
  segundos), o selecciona y pulsa el atajo (Ctrl+Mayús+Espacio de fábrica).
* Con una a tres palabras empieza definiendo; con más, explicando. Los
  botones cambian la acción y abajo puedes escribir tu propia pregunta.
* Esc o un clic fuera la cierra. **Copiar** copia la respuesta.

## Notas

* Para leer la selección el mod la copia (como Ctrl+C) y después devuelve el
  portapapeles como estaba. Con "Accesibilidad de Windows" la lee sin
  copiar, pero hace que Chrome y Edge activen su modo de accesibilidad, que
  gasta algo más.
* Funciona en Chrome, Edge, Firefox, Brave, Opera y Vivaldi.
*/
// ==/WindhawkModReadme==

// ==WindhawkModSettings==
/*
- provider: foundry
  $name: Con qué responde
  $options:
  - foundry: Foundry Local (en tu equipo, usa la NPU)
  - ollama: Ollama (en tu equipo)
  - custom: Otro servicio compatible con OpenAI
- endpoint: ""
  $name: Dirección del servicio
  $description: Vacío = la de Foundry Local u Ollama. Ejemplo http://localhost:1234/v1
- model: ""
  $name: Modelo
  $description: Vacío = el primero que ofrezca el servicio (los de NPU primero)
- apiKey: ""
  $name: Clave de la API
  $description: Solo para servicios que la piden
- showButton: true
  $name: Botón "Preguntar" al seleccionar texto
- hotkey: ctrlShiftSpace
  $name: Atajo de teclado
  $options:
  - ctrlShiftSpace: Ctrl+Mayús+Espacio
  - ctrlAltSpace: Ctrl+Alt+Espacio
  - altQ: Alt+Q
  - none: Ninguno
- defaultAction: auto
  $name: Al abrir
  $options:
  - auto: Definir una palabra, explicar un texto más largo
  - define: Siempre definir
  - explain: Siempre explicar
  - none: Esperar a que elija
- answerLanguage: español
  $name: Idioma de las respuestas
- translateTo: inglés
  $name: Traducir al
  $description: Si el texto ya está en ese idioma, se traduce al de las respuestas
- maxTokens: 400
  $name: Largo máximo de la respuesta (tokens)
- selectionSource: clipboard
  $name: Cómo leer la selección
  $options:
  - clipboard: Copiarla y devolver el portapapeles como estaba
  - uia: Accesibilidad de Windows (sin copiar; si falla, copiarla)
- theme: auto
  $name: Tema de la ventanita
  $options:
  - auto: El de Windows
  - light: Claro
  - dark: Oscuro
*/
// ==/WindhawkModSettings==

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <windowsx.h>
#include <dwmapi.h>
#include <uiautomationclient.h>
#include <winhttp.h>

#include <algorithm>
#include <atomic>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <functional>
#include <memory>
#include <string>
#include <vector>

// ---------------------------------------------------------------------------
// Constantes
// ---------------------------------------------------------------------------

static const wchar_t kHostClass[] = L"WindhawkConsultaRapidaHost";
static const wchar_t kPillClass[] = L"WindhawkConsultaRapidaBoton";
static const wchar_t kPopupClass[] = L"WindhawkConsultaRapida";
static const wchar_t kClipClass[] = L"WindhawkConsultaRapidaPortapapeles";

enum : UINT {
    WM_APP_SELECTION = WM_APP + 1,  // lParam = SelectionResult* (lo libera quien lo recibe)
    WM_APP_ANSWER,                  // llegó texto nuevo de la respuesta
    WM_APP_MOUSEUP,                 // wParam = x, lParam = y (pantalla)
    WM_APP_HIDEPILL,
    WM_APP_HOTKEY,
    WM_APP_SUBMIT,                  // Enter en la caja de la pregunta
};

enum : UINT_PTR { TIMER_PILL_ASK = 1, TIMER_PILL_HIDE, TIMER_DOTS };

// Atributos de DWM de Windows 11 (no están en todos los encabezados).
constexpr DWORD kDwmDarkMode = 20, kDwmCornerPreference = 33, kDwmBorderColor = 34;
constexpr DWORD kCornerRound = 2;

// UI Automation (se declaran aquí para no depender de la biblioteca uuid).
static const CLSID kCLSID_CUIAutomation = {0xff48dba4, 0x60ef, 0x4201, {0xaa, 0x87, 0x54, 0x10, 0x3e, 0xef, 0x59, 0x4e}};
static const IID kIID_IUIAutomation = {0x30cbe57d, 0xd9d0, 0x452a, {0xab, 0x13, 0x7a, 0xc5, 0xac, 0x48, 0x25, 0xee}};
static const IID kIID_TextPattern = {0x32eba289, 0x3583, 0x42c9, {0x9c, 0x59, 0x3b, 0x6d, 0x9a, 0x1e, 0x9b, 0x6a}};
constexpr PATTERNID kTextPatternId = 10014;

// ---------------------------------------------------------------------------
// Configuración (se lee en el hilo de Windhawk y se usa en otros: se copia
// entera bajo un candado)
// ---------------------------------------------------------------------------

enum class Provider { Foundry, Ollama, Custom };
enum class Hotkey { CtrlShiftSpace, CtrlAltSpace, AltQ, None };
enum class DefaultAction { Auto, Define, Explain, None };
enum class ThemeMode { Auto, Light, Dark };

struct Settings {
    Provider provider = Provider::Foundry;
    std::wstring endpoint, model, apiKey;
    bool showButton = true;
    Hotkey hotkey = Hotkey::CtrlShiftSpace;
    DefaultAction defaultAction = DefaultAction::Auto;
    std::wstring language = L"español", translateTo = L"inglés";
    int maxTokens = 400;
    bool uia = false;
    ThemeMode theme = ThemeMode::Auto;
};

static SRWLOCK g_setLock = SRWLOCK_INIT;
static Settings g_set;

static Settings CurrentSettings() {
    AcquireSRWLockShared(&g_setLock);
    Settings s = g_set;
    ReleaseSRWLockShared(&g_setLock);
    return s;
}

static std::wstring StringSetting(PCWSTR name) {
    PCWSTR s = Wh_GetStringSetting(name);
    std::wstring r = s ? s : L"";
    if (s) Wh_FreeStringSetting(s);
    return r;
}

static std::wstring Trim(const std::wstring& s) {
    size_t a = s.find_first_not_of(L" \t\r\n"), b = s.find_last_not_of(L" \t\r\n");
    return a == std::wstring::npos ? L"" : s.substr(a, b - a + 1);
}

static void LoadSettings() {
    Settings s;
    std::wstring p = StringSetting(L"provider");
    s.provider = p == L"ollama" ? Provider::Ollama : p == L"custom" ? Provider::Custom : Provider::Foundry;
    s.endpoint = Trim(StringSetting(L"endpoint"));
    while (!s.endpoint.empty() && s.endpoint.back() == L'/') s.endpoint.pop_back();
    s.model = Trim(StringSetting(L"model"));
    s.apiKey = Trim(StringSetting(L"apiKey"));
    s.showButton = Wh_GetIntSetting(L"showButton") != 0;
    std::wstring h = StringSetting(L"hotkey");
    s.hotkey = h == L"ctrlAltSpace" ? Hotkey::CtrlAltSpace : h == L"altQ" ? Hotkey::AltQ
             : h == L"none" ? Hotkey::None : Hotkey::CtrlShiftSpace;
    std::wstring a = StringSetting(L"defaultAction");
    s.defaultAction = a == L"define" ? DefaultAction::Define : a == L"explain" ? DefaultAction::Explain
                    : a == L"none" ? DefaultAction::None : DefaultAction::Auto;
    s.language = Trim(StringSetting(L"answerLanguage"));
    if (s.language.empty()) s.language = L"español";
    s.translateTo = Trim(StringSetting(L"translateTo"));
    if (s.translateTo.empty()) s.translateTo = L"inglés";
    s.maxTokens = std::clamp(Wh_GetIntSetting(L"maxTokens"), 50, 4000);
    s.uia = StringSetting(L"selectionSource") == L"uia";
    std::wstring t = StringSetting(L"theme");
    s.theme = t == L"light" ? ThemeMode::Light : t == L"dark" ? ThemeMode::Dark : ThemeMode::Auto;
    AcquireSRWLockExclusive(&g_setLock);
    g_set = s;
    ReleaseSRWLockExclusive(&g_setLock);
}

// ---------------------------------------------------------------------------
// Texto: UTF-8, JSON y limpieza de la respuesta
// ---------------------------------------------------------------------------

static std::string ToUtf8(const std::wstring& w) {
    if (w.empty()) return {};
    int n = WideCharToMultiByte(CP_UTF8, 0, w.data(), (int)w.size(), nullptr, 0, nullptr, nullptr);
    std::string s(n, '\0');
    WideCharToMultiByte(CP_UTF8, 0, w.data(), (int)w.size(), s.data(), n, nullptr, nullptr);
    return s;
}

static std::wstring FromUtf8(const std::string& s) {
    if (s.empty()) return {};
    int n = MultiByteToWideChar(CP_UTF8, 0, s.data(), (int)s.size(), nullptr, 0);
    std::wstring w(n, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, s.data(), (int)s.size(), w.data(), n);
    return w;
}

// Cadena JSON (con comillas) a partir de texto UTF-8.
static std::string JsonString(const std::string& s) {
    std::string o = "\"";
    for (unsigned char c : s) {
        switch (c) {
            case '"': o += "\\\""; break;
            case '\\': o += "\\\\"; break;
            case '\n': o += "\\n"; break;
            case '\r': o += "\\r"; break;
            case '\t': o += "\\t"; break;
            default:
                if (c < 0x20) {
                    char b[8];
                    snprintf(b, sizeof(b), "\\u%04x", c);
                    o += b;
                } else {
                    o += (char)c;
                }
        }
    }
    return o + "\"";
}

static void AppendUtf8(std::string& o, uint32_t cp) {
    if (cp < 0x80) {
        o += (char)cp;
    } else if (cp < 0x800) {
        o += (char)(0xC0 | (cp >> 6));
        o += (char)(0x80 | (cp & 0x3F));
    } else if (cp < 0x10000) {
        o += (char)(0xE0 | (cp >> 12));
        o += (char)(0x80 | ((cp >> 6) & 0x3F));
        o += (char)(0x80 | (cp & 0x3F));
    } else {
        o += (char)(0xF0 | (cp >> 18));
        o += (char)(0x80 | ((cp >> 12) & 0x3F));
        o += (char)(0x80 | ((cp >> 6) & 0x3F));
        o += (char)(0x80 | (cp & 0x3F));
    }
}

// Lee una cadena JSON que empieza en j[i] == '"'. Deja i después de ella.
static bool ParseJsonString(const std::string& j, size_t& i, std::string& out) {
    if (i >= j.size() || j[i] != '"') return false;
    out.clear();
    for (i++; i < j.size(); i++) {
        char c = j[i];
        if (c == '"') {
            i++;
            return true;
        }
        if (c != '\\') {
            out += c;
            continue;
        }
        if (++i >= j.size()) return false;
        switch (j[i]) {
            case 'n': out += '\n'; break;
            case 'r': out += '\r'; break;
            case 't': out += '\t'; break;
            case 'b': out += '\b'; break;
            case 'f': out += '\f'; break;
            case 'u': {
                if (i + 4 >= j.size()) return false;
                uint32_t cp = (uint32_t)strtoul(j.substr(i + 1, 4).c_str(), nullptr, 16);
                i += 4;
                // Par sustituto (emojis y demás fuera del plano básico).
                if (cp >= 0xD800 && cp < 0xDC00 && i + 6 < j.size() && j[i + 1] == '\\' && j[i + 2] == 'u') {
                    uint32_t lo = (uint32_t)strtoul(j.substr(i + 3, 4).c_str(), nullptr, 16);
                    if (lo >= 0xDC00 && lo < 0xE000) {
                        cp = 0x10000 + ((cp - 0xD800) << 10) + (lo - 0xDC00);
                        i += 6;
                    }
                }
                AppendUtf8(out, cp);
                break;
            }
            default: out += j[i]; break;  // \" \\ \/
        }
    }
    return false;
}

// Valor de texto de la primera clave "key" (desde 'from'). false si no hay o
// no es una cadena (por ejemplo null).
static bool JsonFind(const std::string& j, const char* key, std::string& out, size_t from = 0) {
    std::string k = std::string("\"") + key + "\"";
    for (size_t p = j.find(k, from); p != std::string::npos; p = j.find(k, p + 1)) {
        size_t i = p + k.size();
        while (i < j.size() && (j[i] == ' ' || j[i] == '\t' || j[i] == '\r' || j[i] == '\n')) i++;
        if (i >= j.size() || j[i] != ':') continue;
        i++;
        while (i < j.size() && (j[i] == ' ' || j[i] == '\t' || j[i] == '\r' || j[i] == '\n')) i++;
        return ParseJsonString(j, i, out);
    }
    return false;
}

// Todos los "id" de una lista de modelos (/v1/models).
static std::vector<std::wstring> JsonIds(const std::string& j) {
    std::vector<std::wstring> ids;
    size_t p = 0;
    while ((p = j.find("\"id\"", p)) != std::string::npos) {
        std::string v;
        if (JsonFind(j, "id", v, p) && !v.empty()) ids.push_back(FromUtf8(v));
        p += 4;
    }
    return ids;
}

// Lo que se muestra de la respuesta: sin el razonamiento (<think>…</think>)
// y sin las marcas de Markdown más comunes.
static std::wstring CleanAnswer(const std::wstring& raw, bool& thinking) {
    std::wstring s = raw;
    thinking = false;
    for (size_t a; (a = s.find(L"<think>")) != std::wstring::npos;) {
        size_t b = s.find(L"</think>", a);
        if (b == std::wstring::npos) {
            s.erase(a);
            thinking = true;
            break;
        }
        s.erase(a, b + 8 - a);
    }
    std::wstring out;
    size_t pos = 0;
    while (pos <= s.size()) {
        size_t end = s.find(L'\n', pos);
        if (end == std::wstring::npos) end = s.size();
        std::wstring line = s.substr(pos, end - pos);
        pos = end + 1;
        size_t k = line.find_first_not_of(L' ');
        if (k != std::wstring::npos && line[k] == L'#') {
            size_t h = line.find_first_not_of(L'#', k);
            line = line.substr(h == std::wstring::npos ? line.size() : h);
            line = Trim(line);
        } else if (k != std::wstring::npos && k + 1 < line.size() && (line[k] == L'-' || line[k] == L'*') &&
                   line[k + 1] == L' ') {
            line = line.substr(0, k) + L"• " + line.substr(k + 2);
        }
        for (const wchar_t* m : {L"**", L"__", L"`"}) {
            for (size_t f; (f = line.find(m)) != std::wstring::npos;) line.erase(f, wcslen(m));
        }
        out += line;
        if (end < s.size()) out += L'\n';
        if (end >= s.size()) break;
    }
    return Trim(out);
}

static int CountWords(const std::wstring& s) {
    int n = 0;
    bool in = false;
    for (wchar_t c : s) {
        bool space = iswspace(c) != 0;
        if (!space && !in) n++;
        in = !space;
    }
    return n;
}

// ---------------------------------------------------------------------------
// Acciones y lo que se le pide al modelo
// ---------------------------------------------------------------------------

enum class Action { Define, Explain, Summarize, Translate, Question };

static const wchar_t* const kActionNames[] = {L"Definir", L"Explicar", L"Resumir", L"Traducir"};

static std::wstring SystemPrompt(const Settings& st) {
    return L"Respondes dentro de una ventanita junto a un texto que la persona seleccionó en el navegador. "
           L"Responde siempre en " + st.language +
           L", directo y breve (como mucho unas 120 palabras), sin saludos ni preámbulos y sin tablas.";
}

static std::wstring UserPrompt(Action a, const std::wstring& text, const std::wstring& question,
                               const Settings& st) {
    std::wstring quoted = L"«" + text + L"»";
    switch (a) {
        case Action::Define:
            return L"Define " + quoted +
                   L". Si tiene varios significados, da los dos más comunes. Agrega un ejemplo de uso corto.";
        case Action::Explain:
            return L"Explica con palabras sencillas qué quiere decir este texto:\n\n" + quoted;
        case Action::Summarize:
            return L"Resume en dos o tres frases:\n\n" + quoted;
        case Action::Translate:
            return L"Traduce al " + st.translateTo + L" (si ya está en " + st.translateTo + L", tradúcelo al " +
                   st.language + L"). Da solo la traducción:\n\n" + quoted;
        default:
            return L"Texto seleccionado:\n" + quoted + L"\n\nPregunta: " + question;
    }
}

static std::string ChatBody(const std::wstring& model, const std::wstring& system, const std::wstring& user,
                            int maxTokens) {
    std::string b = "{\"model\":" + JsonString(ToUtf8(model));
    b += ",\"messages\":[{\"role\":\"system\",\"content\":" + JsonString(ToUtf8(system)) + "},";
    b += "{\"role\":\"user\",\"content\":" + JsonString(ToUtf8(user)) + "}]";
    b += ",\"stream\":true,\"temperature\":0.3,\"max_tokens\":" + std::to_string(maxTokens) + "}";
    return b;
}

// ---------------------------------------------------------------------------
// HTTP (WinHTTP), con respuestas por partes
// ---------------------------------------------------------------------------

// Hilos de trabajo vivos: al descargar el mod se espera a que terminen.
static std::atomic<int> g_workers{0};
static std::atomic<bool> g_unloading{false};

struct WorkerScope {
    WorkerScope() { g_workers++; }
    ~WorkerScope() { g_workers--; }
};

// La petición en curso (para cortarla al cerrar la ventanita o pedir otra).
static SRWLOCK g_reqLock = SRWLOCK_INIT;
static HINTERNET g_reqHandle = nullptr;
static std::atomic<uint64_t> g_askGen{0};

static void AbortRequest() {
    AcquireSRWLockExclusive(&g_reqLock);
    if (g_reqHandle) WinHttpCloseHandle(g_reqHandle);  // la llamada que espera vuelve con error
    g_reqHandle = nullptr;
    ReleaseSRWLockExclusive(&g_reqLock);
}

struct HttpResult {
    DWORD status = 0;
    std::string body;     // lo que no se fue entregando por partes
    std::wstring error;   // fallo de red, en palabras
};

// GET o POST. Con onData, cada parte se entrega al llegar (y si devuelve
// false, se corta).
static HttpResult HttpRequest(const std::wstring& url, const char* method, const std::string& body,
                              const std::wstring& apiKey, const std::function<bool(const char*, size_t)>& onData,
                              DWORD receiveTimeoutMs) {
    HttpResult r;
    URL_COMPONENTS uc{sizeof(uc)};
    wchar_t host[256] = {}, path[2048] = {};
    uc.lpszHostName = host;
    uc.dwHostNameLength = ARRAYSIZE(host);
    uc.lpszUrlPath = path;
    uc.dwUrlPathLength = ARRAYSIZE(path);
    if (!WinHttpCrackUrl(url.c_str(), 0, 0, &uc)) {
        r.error = L"La dirección del servicio no es válida: " + url;
        return r;
    }
    bool https = uc.nScheme == INTERNET_SCHEME_HTTPS;
    HINTERNET session = WinHttpOpen(L"ConsultaRapida/1.0", WINHTTP_ACCESS_TYPE_AUTOMATIC_PROXY, WINHTTP_NO_PROXY_NAME,
                                    WINHTTP_NO_PROXY_BYPASS, 0);
    if (!session) {
        session = WinHttpOpen(L"ConsultaRapida/1.0", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME,
                              WINHTTP_NO_PROXY_BYPASS, 0);
    }
    if (!session) {
        r.error = L"No se pudo usar la red";
        return r;
    }
    WinHttpSetTimeouts(session, 5000, 5000, 15000, (int)receiveTimeoutMs);
    HINTERNET conn = WinHttpConnect(session, host, uc.nPort, 0);
    HINTERNET req = conn ? WinHttpOpenRequest(conn, std::wstring(method, method + strlen(method)).c_str(), path,
                                              nullptr, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES,
                                              https ? WINHTTP_FLAG_SECURE : 0)
                         : nullptr;
    if (req) {
        AcquireSRWLockExclusive(&g_reqLock);
        g_reqHandle = req;
        ReleaseSRWLockExclusive(&g_reqLock);
        std::wstring headers = L"Content-Type: application/json\r\nAccept: text/event-stream, application/json\r\n";
        if (!apiKey.empty()) headers += L"Authorization: Bearer " + apiKey + L"\r\n";
        BOOL ok = WinHttpSendRequest(req, headers.c_str(), (DWORD)-1L, (LPVOID)(body.empty() ? nullptr : body.data()),
                                     (DWORD)body.size(), (DWORD)body.size(), 0) &&
                  WinHttpReceiveResponse(req, nullptr);
        if (ok) {
            DWORD size = sizeof(r.status);
            WinHttpQueryHeaders(req, WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER, WINHTTP_HEADER_NAME_BY_INDEX,
                                &r.status, &size, WINHTTP_NO_HEADER_INDEX);
            char buf[4096];
            for (;;) {
                DWORD got = 0;
                if (!WinHttpReadData(req, buf, sizeof(buf), &got) || got == 0) break;
                if (onData && r.status == 200) {
                    if (!onData(buf, got)) break;
                } else if (r.body.size() < 1 << 20) {
                    r.body.append(buf, got);
                }
            }
        } else {
            DWORD e = GetLastError();
            r.error = e == ERROR_WINHTTP_CANNOT_CONNECT  ? L"No responde el servicio en " + url
                    : e == ERROR_WINHTTP_TIMEOUT         ? L"El servicio tardó demasiado en responder"
                    : e == ERROR_WINHTTP_OPERATION_CANCELLED || e == ERROR_INVALID_HANDLE ? L""
                                                                                          : L"Falló la conexión con " + url;
        }
        AcquireSRWLockExclusive(&g_reqLock);
        if (g_reqHandle == req) {
            WinHttpCloseHandle(req);
            g_reqHandle = nullptr;
        }
        ReleaseSRWLockExclusive(&g_reqLock);
    } else {
        r.error = L"No se pudo preparar la conexión con " + url;
    }
    if (conn) WinHttpCloseHandle(conn);
    WinHttpCloseHandle(session);
    return r;
}

// Mensaje de error que manda el servicio ({"error":{"message":"…"}}).
static std::wstring ServiceError(const HttpResult& r) {
    std::string msg;
    if (JsonFind(r.body, "message", msg) || JsonFind(r.body, "error", msg)) return FromUtf8(msg);
    return L"El servicio respondió con el código " + std::to_wstring(r.status);
}

// ---------------------------------------------------------------------------
// Dónde está el servicio y con qué modelo
// ---------------------------------------------------------------------------

// Corre un programa de consola sin ventana y devuelve lo que escribe.
static bool RunCapture(const std::wstring& cmdLine, DWORD timeoutMs, std::string& out) {
    SECURITY_ATTRIBUTES sa{sizeof(sa), nullptr, TRUE};
    HANDLE rd = nullptr, wr = nullptr;
    if (!CreatePipe(&rd, &wr, &sa, 0)) return false;
    SetHandleInformation(rd, HANDLE_FLAG_INHERIT, 0);
    STARTUPINFOW si{sizeof(si)};
    si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;
    si.hStdOutput = si.hStdError = wr;
    PROCESS_INFORMATION pi{};
    std::wstring cmd = cmdLine;
    BOOL ok = CreateProcessW(nullptr, cmd.data(), nullptr, nullptr, TRUE, CREATE_NO_WINDOW, nullptr, nullptr, &si, &pi);
    CloseHandle(wr);
    if (!ok) {
        CloseHandle(rd);
        return false;
    }
    ULONGLONG until = GetTickCount64() + timeoutMs;
    char buf[1024];
    for (;;) {
        DWORD avail = 0;
        if (PeekNamedPipe(rd, nullptr, 0, nullptr, &avail, nullptr) && avail) {
            DWORD got = 0;
            if (ReadFile(rd, buf, std::min<DWORD>(avail, sizeof(buf)), &got, nullptr) && got) out.append(buf, got);
            continue;
        }
        if (WaitForSingleObject(pi.hProcess, 50) == WAIT_OBJECT_0) {
            DWORD got = 0;
            while (PeekNamedPipe(rd, nullptr, 0, nullptr, &avail, nullptr) && avail &&
                   ReadFile(rd, buf, std::min<DWORD>(avail, sizeof(buf)), &got, nullptr) && got) {
                out.append(buf, got);
            }
            break;
        }
        if (GetTickCount64() > until || g_unloading) {
            TerminateProcess(pi.hProcess, 1);
            break;
        }
    }
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
    CloseHandle(rd);
    return true;
}

// La primera dirección http://host:puerto que aparezca en el texto.
static std::wstring FirstLocalUrl(const std::string& text) {
    size_t p = text.find("http://");
    if (p == std::string::npos) return L"";
    size_t hostStart = p + 7;
    size_t end = hostStart;
    while (end < text.size() && (isalnum((unsigned char)text[end]) || text[end] == '.' || text[end] == ':' ||
                                 text[end] == '-' || text[end] == '[' || text[end] == ']')) {
        end++;
    }
    std::string base = text.substr(p, end - p);
    while (!base.empty() && (base.back() == '.' || base.back() == ':')) base.pop_back();
    return FromUtf8(base);
}

struct Backend {
    std::wstring base;   // .../v1
    std::wstring model;
    std::wstring label;  // lo que se muestra abajo de la ventanita
};

static SRWLOCK g_backendLock = SRWLOCK_INIT;
static Backend g_backend;  // el último que funcionó (se vuelve a buscar si falla)
static std::wstring g_backendKey;

static std::wstring ProviderName(Provider p) {
    return p == Provider::Foundry ? L"Foundry Local" : p == Provider::Ollama ? L"Ollama" : L"Servicio propio";
}

// Elige el modelo entre los que ofrece el servicio: los hechos para la NPU
// primero y, entre ellos, los que responden rápido antes que los de
// razonamiento (DeepSeek-R1 y compañía piensan mucho antes de contestar).
static std::wstring PickModel(const std::vector<std::wstring>& ids) {
    std::wstring best;
    int bestScore = -1000;
    for (auto& id : ids) {
        std::wstring low = id;
        CharLowerBuffW(low.data(), (DWORD)low.size());
        auto has = [&](const wchar_t* t) { return low.find(t) != std::wstring::npos; };
        int score = 0;
        if (has(L"npu")) score += 100;
        if (has(L"r1") || has(L"reason") || has(L"qwq") || has(L"think")) score -= 30;
        if (has(L"phi") || has(L"mini")) score += 10;
        if (score > bestScore) {
            bestScore = score;
            best = id;
        }
    }
    return best;
}

// Todas las cadenas de un arreglo JSON (["a","b"]).
static std::vector<std::wstring> JsonStrings(const std::string& j) {
    std::vector<std::wstring> out;
    for (size_t i = 0; i < j.size(); i++) {
        if (j[i] != '"') continue;
        std::string v;
        if (!ParseJsonString(j, i, v)) break;
        if (!v.empty()) out.push_back(FromUtf8(v));
        i--;
    }
    return out;
}

// Raíz del servicio (sin el /v1 del final).
static std::wstring ServiceRoot(const std::wstring& base) {
    size_t p = base.rfind(L"/v1");
    return p != std::wstring::npos && p + 3 == base.size() ? base.substr(0, p) : base;
}

static std::wstring UrlPart(const std::wstring& s) {
    std::string u = ToUtf8(s), o;
    for (unsigned char c : u) {
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            o += (char)c;
        } else {
            char b[4];
            snprintf(b, sizeof(b), "%%%02X", c);
            o += b;
        }
    }
    return FromUtf8(o);
}

// Los modelos que Foundry Local ya tiene cargados (vacío si no lo dice).
static std::vector<std::wstring> FoundryLoadedModels(const std::wstring& base, const std::wstring& key) {
    HttpResult r = HttpRequest(ServiceRoot(base) + L"/openai/loadedmodels", "GET", "", key, nullptr, 10000);
    if (!r.error.empty() || r.status != 200) return {};
    size_t first = r.body.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) return {};
    return r.body[first] == '[' ? JsonStrings(r.body) : JsonIds(r.body);
}

static bool IsNotLoadedError(std::wstring msg) {
    CharLowerBuffW(msg.data(), (DWORD)msg.size());
    return msg.find(L"not loaded") != std::wstring::npos || msg.find(L"must be loaded") != std::wstring::npos;
}

static bool RunFoundry(const std::wstring& args, DWORD timeoutMs, std::string& out);

// Carga un modelo en Foundry Local: por su API y, si no, con el comando.
static void LoadFoundryModel(const std::wstring& base, const std::wstring& model, const std::wstring& key) {
    HttpResult r = HttpRequest(ServiceRoot(base) + L"/openai/load/" + UrlPart(model), "GET", "", key, nullptr, 600000);
    if (r.error.empty() && r.status == 200) return;
    std::string out;
    RunFoundry(L"model load \"" + model + L"\"", 600000, out);
    if (FoundryLoadedModels(base, key).empty()) {
        // Algunas versiones lo quieren sin el número de versión (":4").
        size_t colon = model.rfind(L':');
        if (colon != std::wstring::npos) RunFoundry(L"model load \"" + model.substr(0, colon) + L"\"", 600000, out);
    }
}

// Corre un comando de Foundry Local. Prueba "foundry" (en el PATH) y el
// acceso de la Tienda en WindowsApps: si el navegador se abrió antes de
// instalarlo, su PATH todavía no lo tiene.
static bool RunFoundry(const std::wstring& args, DWORD timeoutMs, std::string& out) {
    if (RunCapture(L"foundry " + args, timeoutMs, out)) return true;
    wchar_t local[MAX_PATH] = {};
    if (!ExpandEnvironmentStringsW(L"%LOCALAPPDATA%\\Microsoft\\WindowsApps\\foundry.exe", local, MAX_PATH)) return false;
    return GetFileAttributesW(local) != INVALID_FILE_ATTRIBUTES &&
           RunCapture(L"\"" + std::wstring(local) + L"\" " + args, timeoutMs, out);
}

// Dirección del servicio de Foundry Local (http://127.0.0.1:PUERTO). Las
// versiones nuevas usan "foundry server …"; las de antes, "foundry service …".
// Si no está en marcha, lo arranca.
static bool FindFoundry(std::wstring& url, std::wstring& error) {
    std::string out;
    if (!RunFoundry(L"server status", 15000, out)) {
        error = L"No se encontró Foundry Local. Instálalo con «winget install Microsoft.FoundryLocal», "
                L"descarga un modelo con «foundry run phi-4-mini» y vuelve a abrir el navegador.";
        return false;
    }
    url = FirstLocalUrl(out);
    for (const wchar_t* cmd : {L"service status", L"server start", L"service start"}) {
        if (!url.empty()) break;
        bool start = wcsstr(cmd, L"start") != nullptr;
        std::string more;
        RunFoundry(cmd, start ? 60000 : 15000, more);
        if (start) {
            more.clear();
            RunFoundry(L"server status", 15000, more);
            if (FirstLocalUrl(more).empty()) RunFoundry(L"service status", 15000, more);
        }
        url = FirstLocalUrl(more);
        out += more;
    }
    if (url.empty()) {
        std::wstring said = FromUtf8(out.substr(0, 300));
        Wh_Log(L"Foundry Local no dio su dirección. Respondió: %s", said.c_str());
        error = L"Foundry Local no está en marcha y no se pudo arrancar. En una terminal: «foundry server start» "
                L"y «foundry run phi-4-mini». Si sigue, pon la dirección de «foundry server status» en la "
                L"configuración del mod.";
        return false;
    }
    return true;
}

static bool ResolveBackend(const Settings& st, Backend& out, std::wstring& error) {
    std::wstring key = std::to_wstring((int)st.provider) + L"|" + st.endpoint + L"|" + st.model;
    AcquireSRWLockShared(&g_backendLock);
    bool cached = !g_backend.base.empty() && key == g_backendKey;
    if (cached) out = g_backend;
    ReleaseSRWLockShared(&g_backendLock);
    if (cached) return true;

    Backend b;
    b.base = st.endpoint;
    if (b.base.empty() && st.provider == Provider::Ollama) b.base = L"http://localhost:11434/v1";
    if (b.base.empty() && st.provider == Provider::Custom) {
        error = L"Falta la dirección del servicio en la configuración del mod.";
        return false;
    }
    if (b.base.empty()) {
        // Foundry Local cambia de puerto en cada arranque: se le pregunta.
        std::wstring url;
        if (!FindFoundry(url, error)) return false;
        b.base = url + L"/v1";
    } else if (b.base.size() < 3 || b.base.compare(b.base.size() - 3, 3, L"/v1") != 0) {
        if (b.base.find(L"/v1/") == std::wstring::npos && b.base.find(L"/openai") == std::wstring::npos) {
            b.base += L"/v1";
        }
    }
    b.model = st.model;
    if (b.model.empty()) {
        HttpResult r = HttpRequest(b.base + L"/models", "GET", "", st.apiKey, nullptr, 15000);
        if (!r.error.empty()) {
            error = r.error;
            return false;
        }
        if (r.status != 200) {
            error = ServiceError(r);
            return false;
        }
        std::vector<std::wstring> loaded;
        if (st.provider == Provider::Foundry) loaded = FoundryLoadedModels(b.base, st.apiKey);
        b.model = PickModel(loaded.empty() ? JsonIds(r.body) : loaded);
        if (b.model.empty()) {
            error = st.provider == Provider::Ollama
                        ? L"Ollama no tiene modelos. Descarga uno con «ollama pull llama3.2»."
                        : L"El servicio no tiene modelos cargados. Con Foundry Local: «foundry run phi-4-mini».";
            return false;
        }
    }
    b.label = ProviderName(st.provider) + L" · " + b.model;
    AcquireSRWLockExclusive(&g_backendLock);
    g_backend = b;
    g_backendKey = key;
    ReleaseSRWLockExclusive(&g_backendLock);
    out = b;
    return true;
}

static void ForgetBackend() {
    AcquireSRWLockExclusive(&g_backendLock);
    g_backend = Backend{};
    ReleaseSRWLockExclusive(&g_backendLock);
}

// ---------------------------------------------------------------------------
// La respuesta en curso (la escribe el hilo de la consulta, la lee la
// ventanita)
// ---------------------------------------------------------------------------

struct AnswerState {
    std::wstring raw;     // lo que va llegando, tal cual
    std::wstring error;   // si falló
    std::wstring label;   // servicio · modelo
    bool done = false;
    bool waiting = false;  // todavía sin la primera palabra
    std::wstring status;   // qué está haciendo (si tarda), por ejemplo cargar el modelo
    uint64_t gen = 0;      // de qué consulta es
};

static SRWLOCK g_ansLock = SRWLOCK_INIT;
static AnswerState g_answer;
static std::atomic<HWND> g_popup{nullptr};

static void UpdateAnswer(const std::function<void(AnswerState&)>& fn) {
    AcquireSRWLockExclusive(&g_ansLock);
    fn(g_answer);
    ReleaseSRWLockExclusive(&g_ansLock);
    HWND p = g_popup.load();
    if (p) PostMessageW(p, WM_APP_ANSWER, 0, 0);
}

static AnswerState CurrentAnswer() {
    AcquireSRWLockShared(&g_ansLock);
    AnswerState a = g_answer;
    ReleaseSRWLockShared(&g_ansLock);
    return a;
}

// Lee las líneas "data: {...}" de una respuesta por partes y agrega el
// texto nuevo. Devuelve false cuando llega el final.
struct SseReader {
    std::string pending;
    bool sawData = false;

    bool Feed(const char* p, size_t n, const std::function<void(const std::wstring&)>& onText,
              const std::function<void(const std::wstring&)>& onError) {
        pending.append(p, n);
        size_t nl;
        while ((nl = pending.find('\n')) != std::string::npos) {
            std::string line = pending.substr(0, nl);
            pending.erase(0, nl + 1);
            if (!line.empty() && line.back() == '\r') line.pop_back();
            if (line.compare(0, 5, "data:") != 0) continue;
            std::string data = line.substr(5);
            size_t a = data.find_first_not_of(' ');
            data = a == std::string::npos ? "" : data.substr(a);
            sawData = true;
            if (data == "[DONE]") return false;
            std::string piece, msg;
            if (data.find("\"error\"") != std::string::npos && JsonFind(data, "message", msg)) {
                onError(FromUtf8(msg));
                return false;
            }
            size_t delta = data.find("\"delta\"");
            if (JsonFind(data, "content", piece, delta == std::string::npos ? 0 : delta) && !piece.empty()) {
                onText(FromUtf8(piece));
            }
        }
        return true;
    }
};

struct AskJob {
    uint64_t gen = 0;
    Action action = Action::Explain;
    std::wstring text, question;
};

static DWORD WINAPI AskThread(LPVOID param) {
    WorkerScope scope;
    std::unique_ptr<AskJob> job((AskJob*)param);
    Settings st = CurrentSettings();
    auto current = [&] { return g_askGen.load() == job->gen && !g_unloading; };
    Backend b;
    std::wstring error;
    if (!ResolveBackend(st, b, error)) {
        if (current()) UpdateAnswer([&](AnswerState& a) { a.error = error; a.done = true; a.waiting = false; });
        return 0;
    }
    if (current()) UpdateAnswer([&](AnswerState& a) { a.label = b.label; });
    SseReader sse;
    std::string whole;  // por si el servicio no responde por partes
    auto onText = [&](const std::wstring& t) {
        UpdateAnswer([&](AnswerState& a) { a.raw += t; a.waiting = false; a.status.clear(); });
    };
    auto onError = [&](const std::wstring& e) {
        UpdateAnswer([&](AnswerState& a) { a.error = e; });
    };
    auto send = [&] {
        std::string body = ChatBody(b.model, SystemPrompt(st), UserPrompt(job->action, job->text, job->question, st),
                                    st.maxTokens);
        whole.clear();
        sse = SseReader{};
        return HttpRequest(b.base + L"/chat/completions", "POST", body, st.apiKey,
                           [&](const char* p, size_t n) {
                               if (!current()) return false;
                               if (whole.size() < 1 << 20) whole.append(p, n);
                               return sse.Feed(p, n, onText, onError);
                           },
                           120000);
    };
    HttpResult r = send();
    if (!current()) return 0;
    bool foundry = st.provider == Provider::Foundry;
    // Foundry Local se apaga solo tras un rato sin uso (y vuelve en otro
    // puerto): se busca de nuevo y se reintenta una vez.
    if (!r.error.empty() && r.status == 0 && foundry && st.endpoint.empty()) {
        ForgetBackend();
        if (ResolveBackend(st, b, error) && current()) {
            UpdateAnswer([&](AnswerState& a) { a.label = b.label; });
            r = send();
            if (!current()) return 0;
        }
    }
    // Foundry ofrece todos los modelos descargados, pero solo responde con los
    // cargados: se carga (la primera vez tarda) y se reintenta.
    if (r.error.empty() && r.status != 200 && foundry && IsNotLoadedError(ServiceError(r))) {
        UpdateAnswer([&](AnswerState& a) { a.status = L"Cargando el modelo (la primera vez tarda un poco)"; });
        LoadFoundryModel(b.base, b.model, st.apiKey);
        if (!current()) return 0;
        UpdateAnswer([&](AnswerState& a) { a.status.clear(); });
        r = send();
        if (!current()) return 0;
    }
    if (!r.error.empty() || r.status != 200) {
        ForgetBackend();  // quizá cambió de puerto o se cerró: buscarlo de nuevo la próxima vez
        std::wstring e = !r.error.empty() ? r.error : ServiceError(r);
        UpdateAnswer([&](AnswerState& a) { a.error = e; a.done = true; a.waiting = false; });
        return 0;
    }
    if (!sse.sawData) {
        // Respuesta entera de una vez ({"choices":[{"message":{"content":…}}]}).
        std::string content;
        size_t msg = whole.find("\"message\"");
        if (JsonFind(whole, "content", content, msg == std::string::npos ? 0 : msg)) {
            std::wstring t = FromUtf8(content);
            UpdateAnswer([&](AnswerState& a) { a.raw = t; });
        }
    }
    UpdateAnswer([&](AnswerState& a) {
        a.done = true;
        a.waiting = false;
        if (a.raw.empty() && a.error.empty()) a.error = L"El modelo no devolvió texto.";
    });
    return 0;
}

static void StartAsk(Action action, const std::wstring& text, const std::wstring& question) {
    AbortRequest();
    auto* job = new AskJob;
    job->gen = ++g_askGen;
    job->action = action;
    job->text = text;
    job->question = question;
    uint64_t gen = job->gen;
    UpdateAnswer([gen](AnswerState& a) {
        std::wstring label = a.label;
        a = AnswerState{};
        a.label = label;
        a.waiting = true;
        a.gen = gen;
    });
    HANDLE t = CreateThread(nullptr, 0, AskThread, job, 0, nullptr);
    if (t) CloseHandle(t);
    else delete job;
}

// ---------------------------------------------------------------------------
// Leer la selección: con UI Automation o copiándola (y devolviendo el
// portapapeles como estaba)
// ---------------------------------------------------------------------------

struct ClipFormat {
    UINT format;
    std::vector<BYTE> data;
};

// Copia de todo lo que hay en el portapapeles que se pueda guardar en
// memoria (texto, HTML, imágenes en DIB, archivos…).
static std::vector<ClipFormat> SaveClipboard(HWND owner) {
    std::vector<ClipFormat> saved;
    if (!OpenClipboard(owner)) return saved;
    size_t total = 0;
    for (UINT f = EnumClipboardFormats(0); f; f = EnumClipboardFormats(f)) {
        // Los de objetos GDI se regeneran solos a partir de CF_DIB / CF_UNICODETEXT.
        if (f == CF_BITMAP || f == CF_ENHMETAFILE || f == CF_METAFILEPICT || f == CF_PALETTE || f == CF_OWNERDISPLAY ||
            f == CF_DSPBITMAP || f == CF_DSPENHMETAFILE || f == CF_DSPMETAFILEPICT || f == CF_TEXT ||
            f == CF_OEMTEXT || f == CF_LOCALE) {
            continue;
        }
        HANDLE h = GetClipboardData(f);
        if (!h) continue;
        SIZE_T n = GlobalSize(h);
        if (!n || total + n > (64u << 20)) continue;
        const BYTE* p = (const BYTE*)GlobalLock(h);
        if (!p) continue;
        saved.push_back({f, std::vector<BYTE>(p, p + n)});
        total += n;
        GlobalUnlock(h);
    }
    CloseClipboard();
    return saved;
}

static void RestoreClipboard(HWND owner, const std::vector<ClipFormat>& saved) {
    for (int i = 0; i < 20 && !OpenClipboard(owner); i++) Sleep(10);
    EmptyClipboard();
    for (auto& f : saved) {
        HGLOBAL g = GlobalAlloc(GMEM_MOVEABLE, f.data.size());
        if (!g) continue;
        if (void* p = GlobalLock(g)) {
            memcpy(p, f.data.data(), f.data.size());
            GlobalUnlock(g);
            if (!SetClipboardData(f.format, g)) GlobalFree(g);
        } else {
            GlobalFree(g);
        }
    }
    CloseClipboard();
}

static std::wstring ClipboardText(HWND owner) {
    std::wstring t;
    for (int i = 0; i < 20 && !OpenClipboard(owner); i++) Sleep(10);
    if (HANDLE h = GetClipboardData(CF_UNICODETEXT)) {
        if (const wchar_t* p = (const wchar_t*)GlobalLock(h)) {
            t = p;
            GlobalUnlock(h);
        }
    }
    CloseClipboard();
    return t;
}

static void SendCtrlC() {
    INPUT in[4] = {};
    for (auto& i : in) i.type = INPUT_KEYBOARD;
    in[0].ki.wVk = VK_CONTROL;
    in[1].ki.wVk = 'C';
    in[2].ki.wVk = 'C';
    in[2].ki.dwFlags = KEYEVENTF_KEYUP;
    in[3].ki.wVk = VK_CONTROL;
    in[3].ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(4, in, sizeof(INPUT));
}

static bool AnyModifierDown() {
    for (int vk : {VK_SHIFT, VK_CONTROL, VK_MENU, VK_LWIN, VK_RWIN}) {
        if (GetAsyncKeyState(vk) & 0x8000) return true;
    }
    return false;
}

static LRESULT CALLBACK ClipWndProc(HWND h, UINT m, WPARAM w, LPARAM l) { return DefWindowProcW(h, m, w, l); }

static std::wstring SelectionByClipboard() {
    // Ventanita propia para ser dueña del portapapeles al devolverlo.
    HWND owner = CreateWindowExW(0, kClipClass, L"", 0, 0, 0, 0, 0, HWND_MESSAGE, nullptr, GetModuleHandleW(nullptr),
                                 nullptr);
    // Con el atajo, las teclas pueden seguir apretadas: Ctrl+Mayús+C abriría
    // las herramientas de desarrollo.
    for (int i = 0; i < 100 && AnyModifierDown(); i++) Sleep(10);
    std::vector<ClipFormat> saved = SaveClipboard(owner);
    DWORD seq = GetClipboardSequenceNumber();
    SendCtrlC();
    std::wstring text;
    for (int i = 0; i < 60; i++) {
        Sleep(10);
        if (GetClipboardSequenceNumber() != seq) {
            Sleep(20);  // que termine de escribir todos los formatos
            text = ClipboardText(owner);
            break;
        }
    }
    if (GetClipboardSequenceNumber() != seq) RestoreClipboard(owner, saved);
    if (owner) DestroyWindow(owner);
    return text;
}

template <typename T>
static void SafeRelease(T*& p) {
    if (p) p->Release();
    p = nullptr;
}

static std::wstring SelectionByUia() {
    std::wstring text;
    IUIAutomation* uia = nullptr;
    if (FAILED(CoCreateInstance(kCLSID_CUIAutomation, nullptr, CLSCTX_INPROC_SERVER, kIID_IUIAutomation,
                                (void**)&uia)) || !uia) {
        return text;
    }
    IUIAutomationElement* el = nullptr;
    IUIAutomationTreeWalker* walker = nullptr;
    uia->get_ControlViewWalker(&walker);
    uia->GetFocusedElement(&el);
    // El elemento con el foco o alguno de sus padres (el documento) lee la selección.
    for (int depth = 0; el && depth < 12 && text.empty(); depth++) {
        IUIAutomationTextPattern* tp = nullptr;
        if (SUCCEEDED(el->GetCurrentPatternAs(kTextPatternId, kIID_TextPattern, (void**)&tp)) && tp) {
            IUIAutomationTextRangeArray* ranges = nullptr;
            if (SUCCEEDED(tp->GetSelection(&ranges)) && ranges) {
                int n = 0;
                ranges->get_Length(&n);
                for (int i = 0; i < n; i++) {
                    IUIAutomationTextRange* range = nullptr;
                    BSTR s = nullptr;
                    if (SUCCEEDED(ranges->GetElement(i, &range)) && range && SUCCEEDED(range->GetText(4000, &s)) && s) {
                        if (!text.empty()) text += L"\n";
                        text += s;
                        SysFreeString(s);
                    }
                    SafeRelease(range);
                }
                SafeRelease(ranges);
            }
            SafeRelease(tp);
        }
        if (!text.empty() || !walker) break;
        IUIAutomationElement* parent = nullptr;
        walker->GetParentElement(el, &parent);
        SafeRelease(el);
        el = parent;
    }
    SafeRelease(el);
    SafeRelease(walker);
    SafeRelease(uia);
    return text;
}

struct SelectionResult {
    std::wstring text;
    POINT pt{};
    HWND owner = nullptr;
};

struct SelectionJob {
    POINT pt{};
    HWND owner = nullptr;
    HWND reply = nullptr;
};

// Hilo aparte: UI Automation llama al propio navegador y copiar espera al
// portapapeles; ninguna de las dos cosas puede frenar los ganchos del mouse.
static DWORD WINAPI SelectionThread(LPVOID param) {
    WorkerScope scope;
    std::unique_ptr<SelectionJob> job((SelectionJob*)param);
    CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    Settings st = CurrentSettings();
    std::wstring text = st.uia ? SelectionByUia() : L"";
    if (Trim(text).empty() && !g_unloading) text = SelectionByClipboard();
    CoUninitialize();
    if (g_unloading) return 0;
    auto* r = new SelectionResult{Trim(text), job->pt, job->owner};
    if (!PostMessageW(job->reply, WM_APP_SELECTION, 0, (LPARAM)r)) delete r;
    return 0;
}

// ---------------------------------------------------------------------------
// Dibujo: colores y formas con antialias (como en los menús)
// ---------------------------------------------------------------------------

struct Colors {
    bool dark;
    COLORREF bg, text, dim, chip, chipHot, chipOn, input, inputBorder, error;
};

static COLORREF Mix(COLORREF a, COLORREF b, float t) {
    auto ch = [&](int x, int y) { return (BYTE)std::lround(x + (y - x) * t); };
    return RGB(ch(GetRValue(a), GetRValue(b)), ch(GetGValue(a), GetGValue(b)), ch(GetBValue(a), GetBValue(b)));
}

static bool AppsUseLightTheme() {
    DWORD v = 1, size = sizeof(v);
    RegGetValueW(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize",
                 L"AppsUseLightTheme", RRF_RT_REG_DWORD, nullptr, &v, &size);
    return v != 0;
}

static Colors MakeColors(ThemeMode mode) {
    bool dark = mode == ThemeMode::Dark || (mode == ThemeMode::Auto && !AppsUseLightTheme());
    Colors c;
    c.dark = dark;
    // Los mismos tonos que el menú del Dock; los botones en gris claro.
    c.bg = dark ? RGB(36, 36, 40) : RGB(246, 246, 248);
    c.text = dark ? RGB(242, 242, 245) : RGB(28, 28, 30);
    c.dim = dark ? RGB(150, 150, 156) : RGB(120, 120, 128);
    COLORREF ink = dark ? RGB(255, 255, 255) : RGB(0, 0, 0);
    c.chip = Mix(c.bg, ink, dark ? 0.09f : 0.055f);
    c.chipHot = Mix(c.bg, ink, dark ? 0.15f : 0.095f);
    c.chipOn = Mix(c.bg, ink, dark ? 0.22f : 0.14f);
    c.input = dark ? RGB(28, 28, 31) : RGB(255, 255, 255);
    c.inputBorder = Mix(c.bg, ink, dark ? 0.2f : 0.16f);
    c.error = dark ? RGB(255, 120, 110) : RGB(200, 40, 30);
    return c;
}

// Lienzo ARGB premultiplicado.
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
        dib = dc && w > 0 && h > 0 ? CreateDIBSection(dc, &bi, DIB_RGB_COLORS, &p, nullptr, 0) : nullptr;
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
};

static inline float RoundRectSdf(float px, float py, float cx, float cy, float hw, float hh, float r) {
    float qx = std::fabs(px - cx) - (hw - r);
    float qy = std::fabs(py - cy) - (hh - r);
    float ox = std::max(qx, 0.0f), oy = std::max(qy, 0.0f);
    return std::sqrt(ox * ox + oy * oy) + std::min(std::max(qx, qy), 0.0f) - r;
}

// Rectángulo redondeado opaco con antialias sobre un fondo liso conocido
// (la ventanita es opaca: se mezcla el color directo).
static void FillRoundSolid(HDC dc, const RECT& rc, float r, COLORREF fill, COLORREF bg, COLORREF border = CLR_INVALID) {
    int w = rc.right - rc.left, h = rc.bottom - rc.top;
    if (w <= 0 || h <= 0) return;
    Canvas cv(w, h);
    if (!cv.bits) return;
    float cx = w / 2.0f, cy = h / 2.0f;
    r = std::min(r, std::min(cx, cy));
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            float d = RoundRectSdf(x + 0.5f, y + 0.5f, cx, cy, cx, cy, r);
            float inside = std::clamp(0.5f - d, 0.0f, 1.0f);
            COLORREF c = Mix(bg, fill, inside);
            if (border != CLR_INVALID) {
                float edge = std::clamp(1.0f - std::fabs(d + 0.5f), 0.0f, 1.0f);
                c = Mix(c, border, edge);
            }
            cv.bits[(size_t)y * w + x] = 0xFF000000u | (GetRValue(c) << 16) | (GetGValue(c) << 8) | GetBValue(c);
        }
    }
    GdiFlush();
    BitBlt(dc, rc.left, rc.top, w, h, cv.dc, 0, 0, SRCCOPY);
}

static HFONT MakeFont(float px, int weight) {
    for (const wchar_t* face : {L"Segoe UI Variable Text", L"Segoe UI"}) {
        HFONT f = CreateFontW(-(int)std::lround(px), 0, 0, 0, weight, 0, 0, 0, DEFAULT_CHARSET, 0, 0, CLEARTYPE_QUALITY,
                              0, face);
        if (f) return f;
    }
    return (HFONT)GetStockObject(DEFAULT_GUI_FONT);
}

// ---------------------------------------------------------------------------
// Botón "Preguntar" que aparece junto a la selección
// ---------------------------------------------------------------------------

static HWND g_host = nullptr;  // ventana de mensajes del hilo de la interfaz
static HWND g_pill = nullptr;
static POINT g_pillPt{};
static HWND g_pillOwner = nullptr;

static float ScaleAt(POINT pt) {
    UINT dpi = 96;
    HMONITOR mon = MonitorFromPoint(pt, MONITOR_DEFAULTTONEAREST);
    using GetDpiForMonitorFn = HRESULT(WINAPI*)(HMONITOR, int, UINT*, UINT*);
    static auto fn = (GetDpiForMonitorFn)(void*)GetProcAddress(LoadLibraryW(L"shcore.dll"), "GetDpiForMonitor");
    UINT dx = 96, dy = 96;
    if (fn && SUCCEEDED(fn(mon, 0, &dx, &dy)) && dx) dpi = dx;
    return dpi / 96.0f;
}

static void HidePill() {
    if (g_pill && IsWindowVisible(g_pill)) ShowWindow(g_pill, SW_HIDE);
    if (g_host) KillTimer(g_host, TIMER_PILL_HIDE);
}

// Pastilla "Preguntar" con bordes suaves, sombra y transparencia por
// píxel (ARGB premultiplicado). margin = lo que ocupa la sombra alrededor.
static std::unique_ptr<Canvas> BuildPill(const Colors& c, float s, int& margin) {
    const wchar_t* label = L"Preguntar";
    HFONT font = MakeFont(13 * s, FW_SEMIBOLD);
    int pad = (int)std::lround(12 * s), icon = (int)std::lround(14 * s), gap = (int)std::lround(6 * s);
    SIZE ts{};
    {
        HDC m = CreateCompatibleDC(nullptr);
        HGDIOBJ o = SelectObject(m, font);
        GetTextExtentPoint32W(m, label, (int)wcslen(label), &ts);
        SelectObject(m, o);
        DeleteDC(m);
    }
    margin = (int)std::lround(6 * s);
    int pw = pad * 2 + icon + gap + ts.cx, ph = std::max<int>((int)ts.cy, icon) + (int)std::lround(12 * s);
    int W = pw + 2 * margin, H = ph + 2 * margin;
    auto cv = std::make_unique<Canvas>(W, H);
    if (!cv->bits) {
        DeleteObject(font);
        return nullptr;
    }
    // 1) Fondo opaco y texto con GDI, sobre el color de la pastilla.
    RECT all{0, 0, W, H};
    HBRUSH bg = CreateSolidBrush(c.bg);
    FillRect(cv->dc, &all, bg);
    DeleteObject(bg);
    HGDIOBJ of = SelectObject(cv->dc, font);
    SetBkMode(cv->dc, TRANSPARENT);
    SetTextColor(cv->dc, c.text);
    RECT tr{margin + pad + icon + gap, margin, margin + pw - pad, margin + ph};
    DrawTextW(cv->dc, label, -1, &tr, DT_SINGLELINE | DT_VCENTER | DT_LEFT | DT_NOPREFIX);
    SelectObject(cv->dc, of);
    DeleteObject(font);
    GdiFlush();
    // 2) Forma: pastilla con borde fino, una chispa a la izquierda y sombra.
    float cx = W / 2.0f, cy = H / 2.0f, hw = pw / 2.0f, hh = ph / 2.0f, r = hh;
    float sx = margin + pad + icon / 2.0f, sy = cy, sr = icon / 2.0f;
    COLORREF spark = c.dark ? RGB(255, 214, 102) : RGB(255, 159, 10);
    for (int y = 0; y < H; y++) {
        for (int x = 0; x < W; x++) {
            float px = x + 0.5f, py = y + 0.5f;
            float d = RoundRectSdf(px, py, cx, cy, hw, hh, r);
            float cov = std::clamp(0.5f - d, 0.0f, 1.0f);
            uint32_t& p = cv->bits[(size_t)y * W + x];
            COLORREF col = RGB((p >> 16) & 0xFF, (p >> 8) & 0xFF, p & 0xFF);
            // Chispa de cuatro puntas (rombo de lados cóncavos).
            float ax = std::fabs(px - sx) / sr, ay = std::fabs(py - sy) / sr;
            float star = std::sqrt(ax) + std::sqrt(ay);
            float sc = std::clamp((1.0f - star) * sr * 0.9f + 0.5f, 0.0f, 1.0f);
            col = Mix(col, spark, sc);
            float edge = std::clamp(1.0f - std::fabs(d + 0.5f), 0.0f, 1.0f);
            col = Mix(col, Mix(c.bg, c.text, c.dark ? 0.22f : 0.16f), edge * cov);
            float a = cov;
            if (cov < 1.0f) {
                // Sombra suave alrededor.
                float ds = RoundRectSdf(px, py - s, cx, cy, hw, hh, r);
                float sh = std::clamp(1.0f - ds / margin, 0.0f, 1.0f);
                float sa = 0.22f * sh * sh * (1.0f - cov);
                a = cov + sa;
                float k = a > 0 ? cov / a : 0;
                col = RGB((BYTE)(GetRValue(col) * k), (BYTE)(GetGValue(col) * k), (BYTE)(GetBValue(col) * k));
            }
            uint32_t A = (uint32_t)std::lround(a * 255);
            p = (A << 24) | ((uint32_t)std::lround(GetRValue(col) * a) << 16) |
                ((uint32_t)std::lround(GetGValue(col) * a) << 8) | (uint32_t)std::lround(GetBValue(col) * a);
        }
    }
    GdiFlush();
    return cv;
}

static void ShowPill(POINT pt, HWND owner) {
    float s = ScaleAt(pt);
    int margin = 0;
    auto cv = BuildPill(MakeColors(CurrentSettings().theme), s, margin);
    if (!cv) return;
    int W = cv->w, H = cv->h;
    // Junto al mouse, abajo a la derecha (o donde quepa).
    MONITORINFO mi{sizeof(mi)};
    GetMonitorInfoW(MonitorFromPoint(pt, MONITOR_DEFAULTTONEAREST), &mi);
    int x = pt.x + (int)std::lround(8 * s) - margin, y = pt.y + (int)std::lround(14 * s) - margin;
    if (x + W > mi.rcWork.right) x = mi.rcWork.right - W;
    if (y + H > mi.rcWork.bottom) y = pt.y - (int)std::lround(14 * s) - H + margin;
    POINT dst{x, y}, src{0, 0};
    SIZE size{W, H};
    BLENDFUNCTION bf{AC_SRC_OVER, 0, 255, AC_SRC_ALPHA};
    UpdateLayeredWindow(g_pill, nullptr, &dst, &size, cv->dc, &src, 0, &bf, ULW_ALPHA);
    SetWindowPos(g_pill, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_SHOWWINDOW);
    g_pillPt = pt;
    g_pillOwner = owner;
    SetTimer(g_host, TIMER_PILL_HIDE, 4000, nullptr);
}

static void ReadSelection(POINT pt, HWND owner) {
    auto* job = new SelectionJob{pt, owner, g_host};
    HANDLE t = CreateThread(nullptr, 0, SelectionThread, job, 0, nullptr);
    if (t) CloseHandle(t);
    else delete job;
}

static LRESULT CALLBACK PillWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_MOUSEACTIVATE:
            return MA_NOACTIVATE;  // el navegador sigue con el foco (y su selección)
        case WM_LBUTTONUP:
            HidePill();
            ReadSelection(g_pillPt, g_pillOwner);
            return 0;
        case WM_SETCURSOR:
            SetCursor(LoadCursorW(nullptr, IDC_HAND));
            return TRUE;
    }
    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

// ---------------------------------------------------------------------------
// La ventanita
// ---------------------------------------------------------------------------

struct Popup {
    HWND wnd = nullptr, edit = nullptr, owner = nullptr;
    std::wstring text;  // lo seleccionado
    Action action = Action::Explain;
    bool hasAction = false;
    float s = 1;
    Colors c{};
    HFONT font = nullptr, fontSmall = nullptr, fontBold = nullptr;
    HBRUSH inputBrush = nullptr;
    int w = 0, h = 0;
    RECT quote{}, chips[4]{}, answer{}, input{}, footer{}, copy{}, close{};
    int hotChip = -1;  // 0..3 botones, 4 copiar, 5 cerrar
    int scroll = 0, contentH = 0;
    int dots = 0;
    bool copied = false;
};

static Popup g_pop;

static void FreePopupGdi() {
    for (HFONT* f : {&g_pop.font, &g_pop.fontSmall, &g_pop.fontBold}) {
        if (*f) DeleteObject(*f);
        *f = nullptr;
    }
    if (g_pop.inputBrush) DeleteObject(g_pop.inputBrush);
    g_pop.inputBrush = nullptr;
}

static int TextHeight(HFONT font, const std::wstring& t, int width) {
    if (t.empty() || width <= 0) return 0;
    HDC dc = CreateCompatibleDC(nullptr);
    HGDIOBJ o = SelectObject(dc, font);
    RECT r{0, 0, width, 0};
    DrawTextW(dc, t.c_str(), (int)t.size(), &r, DT_WORDBREAK | DT_EDITCONTROL | DT_NOPREFIX | DT_CALCRECT);
    SelectObject(dc, o);
    DeleteDC(dc);
    return r.bottom - r.top;
}

// Lo que se ve en la zona de la respuesta (y si es un error).
static std::wstring AnswerText(const AnswerState& a, bool& isError, bool& isStatus) {
    isError = !a.error.empty();
    isStatus = false;
    bool thinking = false;
    std::wstring t = CleanAnswer(a.raw, thinking);
    if (isError) return t.empty() ? a.error : t + L"\n\n" + a.error;
    if (t.empty() && !a.done) {
        isStatus = true;
        static const wchar_t* kDots[] = {L"", L".", L"..", L"..."};
        std::wstring what = thinking ? L"Pensando" : !a.status.empty() ? a.status : a.waiting ? L"Preguntando al modelo" : L"";
        return what + kDots[g_pop.dots % 4];
    }
    if (t.empty()) {
        isStatus = true;
        return L"Elige qué hacer con el texto o escribe una pregunta.";
    }
    return t;
}

// Acomoda todo según el largo de la respuesta (la ventanita crece hasta un tope).
static void LayoutPopup() {
    float s = g_pop.s;
    auto S = [&](double v) { return (int)std::lround(v * s); };
    int W = S(400), pad = S(16);
    int y = pad;
    g_pop.close = RECT{W - pad - S(22), y - S(2), W - pad + S(2), y + S(22)};
    g_pop.quote = RECT{pad, y, W - pad - S(28), y + S(20)};
    y += S(20) + S(12);
    // Botones de las acciones, del ancho de su texto.
    HDC dc = CreateCompatibleDC(nullptr);
    HGDIOBJ o = SelectObject(dc, g_pop.font);
    int x = pad, chipH = S(28);
    for (int i = 0; i < 4; i++) {
        SIZE ts{};
        GetTextExtentPoint32W(dc, kActionNames[i], (int)wcslen(kActionNames[i]), &ts);
        int cw = ts.cx + S(24);
        g_pop.chips[i] = RECT{x, y, x + cw, y + chipH};
        x += cw + S(6);
    }
    SelectObject(dc, o);
    DeleteDC(dc);
    y += chipH + S(14);
    bool err, status;
    std::wstring t = AnswerText(CurrentAnswer(), err, status);
    int textW = W - 2 * pad;
    g_pop.contentH = std::max(S(22), TextHeight(g_pop.font, t, textW));
    int answerH = std::min(g_pop.contentH, S(300));
    g_pop.scroll = std::clamp(g_pop.scroll, 0, std::max(0, g_pop.contentH - answerH));
    g_pop.answer = RECT{pad, y, W - pad, y + answerH};
    y += answerH + S(14);
    g_pop.input = RECT{pad, y, W - pad, y + S(34)};
    y += S(34) + S(10);
    g_pop.footer = RECT{pad, y, W - pad - S(70), y + S(18)};
    g_pop.copy = RECT{W - pad - S(64), y - S(3), W - pad, y + S(21)};
    y += S(18) + S(12);
    g_pop.w = W;
    g_pop.h = y;
    if (g_pop.edit) {
        int ih = S(20);
        MoveWindow(g_pop.edit, g_pop.input.left + S(12), (g_pop.input.top + g_pop.input.bottom - ih) / 2,
                   g_pop.input.right - g_pop.input.left - S(24), ih, TRUE);
    }
}

static void PaintPopup(HDC dc) {
    const Colors& c = g_pop.c;
    float s = g_pop.s;
    RECT all{0, 0, g_pop.w, g_pop.h};
    HBRUSH bg = CreateSolidBrush(c.bg);
    FillRect(dc, &all, bg);
    DeleteObject(bg);
    SetBkMode(dc, TRANSPARENT);
    HGDIOBJ of = SelectObject(dc, g_pop.fontSmall);
    // Lo seleccionado, entre comillas y en una línea.
    std::wstring q = g_pop.text;
    std::replace(q.begin(), q.end(), L'\n', L' ');
    std::replace(q.begin(), q.end(), L'\r', L' ');
    q = L"«" + q + L"»";
    SetTextColor(dc, c.dim);
    RECT qr = g_pop.quote;
    DrawTextW(dc, q.c_str(), (int)q.size(), &qr, DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS | DT_NOPREFIX);
    // Cerrar (×).
    {
        const RECT& r = g_pop.close;
        if (g_pop.hotChip == 5) FillRoundSolid(dc, r, (r.right - r.left) / 2.0f, c.chipHot, c.bg);
        HPEN pen = CreatePen(PS_SOLID, std::max(1, (int)std::lround(1.4 * s)), c.dim);
        HGDIOBJ op = SelectObject(dc, pen);
        int cx = (r.left + r.right) / 2, cy = (r.top + r.bottom) / 2, k = (int)std::lround(4.5 * s);
        MoveToEx(dc, cx - k, cy - k, nullptr);
        LineTo(dc, cx + k + 1, cy + k + 1);
        MoveToEx(dc, cx - k, cy + k, nullptr);
        LineTo(dc, cx + k + 1, cy - k - 1);
        SelectObject(dc, op);
        DeleteObject(pen);
    }
    // Botones de las acciones (gris claro; el elegido, un poco más oscuro).
    SelectObject(dc, g_pop.font);
    for (int i = 0; i < 4; i++) {
        bool on = g_pop.hasAction && (int)g_pop.action == i;
        COLORREF fill = on ? c.chipOn : g_pop.hotChip == i ? c.chipHot : c.chip;
        const RECT& r = g_pop.chips[i];
        FillRoundSolid(dc, r, (r.bottom - r.top) / 2.0f, fill, c.bg);
        SetTextColor(dc, c.text);
        RECT tr = r;
        DrawTextW(dc, kActionNames[i], -1, &tr, DT_SINGLELINE | DT_VCENTER | DT_CENTER | DT_NOPREFIX);
    }
    // La respuesta.
    bool err, status;
    AnswerState a = CurrentAnswer();
    std::wstring t = AnswerText(a, err, status);
    SetTextColor(dc, err ? c.error : status ? c.dim : c.text);
    {
        int saved = SaveDC(dc);
        IntersectClipRect(dc, g_pop.answer.left, g_pop.answer.top, g_pop.answer.right, g_pop.answer.bottom);
        RECT tr = g_pop.answer;
        tr.top -= g_pop.scroll;
        tr.bottom = tr.top + g_pop.contentH + 4;
        DrawTextW(dc, t.c_str(), (int)t.size(), &tr, DT_WORDBREAK | DT_EDITCONTROL | DT_NOPREFIX);
        RestoreDC(dc, saved);
        // Se nota que hay más abajo: un desvanecido en el borde.
        if (g_pop.contentH > g_pop.answer.bottom - g_pop.answer.top + g_pop.scroll) {
            int fh = (int)std::lround(14 * s);
            // Cada fila del borde se acerca al color del fondo (el DC es de
            // memoria: leer y escribir píxeles es barato).
            for (int k = 0; k < fh; k++) {
                int yy = g_pop.answer.bottom - fh + k;
                for (int x = g_pop.answer.left; x < g_pop.answer.right; x++) {
                    COLORREF p = GetPixel(dc, x, yy);
                    if (p != CLR_INVALID) SetPixel(dc, x, yy, Mix(p, c.bg, (float)(k + 1) / fh));
                }
            }
        }
    }
    // Caja de la pregunta.
    FillRoundSolid(dc, g_pop.input, 10 * s, c.input, c.bg, c.inputBorder);
    // Pie: servicio y modelo; Copiar.
    SelectObject(dc, g_pop.fontSmall);
    SetTextColor(dc, c.dim);
    RECT fr = g_pop.footer;
    std::wstring label = a.label.empty() ? L"" : a.label;
    DrawTextW(dc, label.c_str(), (int)label.size(), &fr, DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS | DT_NOPREFIX);
    if (g_pop.hotChip == 4) FillRoundSolid(dc, g_pop.copy, 8 * s, c.chipHot, c.bg);
    SetTextColor(dc, (a.raw.empty() || status) ? c.dim : c.text);
    RECT cr = g_pop.copy;
    DrawTextW(dc, g_pop.copied ? L"Copiado" : L"Copiar", -1, &cr, DT_SINGLELINE | DT_VCENTER | DT_CENTER | DT_NOPREFIX);
    SelectObject(dc, of);
}

static void ResizePopup() {
    LayoutPopup();
    RECT r;
    GetWindowRect(g_pop.wnd, &r);
    if (r.right - r.left != g_pop.w || r.bottom - r.top != g_pop.h) {
        // Si crece y ya no cabe abajo, sube.
        MONITORINFO mi{sizeof(mi)};
        GetMonitorInfoW(MonitorFromWindow(g_pop.wnd, MONITOR_DEFAULTTONEAREST), &mi);
        int y = std::min<int>(r.top, mi.rcWork.bottom - g_pop.h);
        SetWindowPos(g_pop.wnd, nullptr, r.left, std::max<int>(y, mi.rcWork.top), g_pop.w, g_pop.h,
                     SWP_NOZORDER | SWP_NOACTIVATE);
    }
    InvalidateRect(g_pop.wnd, nullptr, FALSE);
}

static void RunAction(Action a, const std::wstring& question = L"") {
    g_pop.action = a;
    g_pop.hasAction = a != Action::Question;
    g_pop.scroll = 0;
    g_pop.copied = false;
    g_pop.dots = 0;
    StartAsk(a, g_pop.text, question);
    SetTimer(g_pop.wnd, TIMER_DOTS, 350, nullptr);
    ResizePopup();
}

static void ClosePopup() {
    if (!g_pop.wnd || !IsWindowVisible(g_pop.wnd)) return;
    ++g_askGen;
    AbortRequest();
    KillTimer(g_pop.wnd, TIMER_DOTS);
    ShowWindow(g_pop.wnd, SW_HIDE);
    if (g_pop.owner && IsWindow(g_pop.owner)) SetForegroundWindow(g_pop.owner);
}

static WNDPROC g_editProc = nullptr;

// Enter manda la pregunta; Esc cierra.
static LRESULT CALLBACK EditProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (msg == WM_KEYDOWN && wParam == VK_RETURN) {
        PostMessageW(GetParent(hwnd), WM_APP_SUBMIT, 0, 0);
        return 0;
    }
    if (msg == WM_KEYDOWN && wParam == VK_ESCAPE) {
        PostMessageW(GetParent(hwnd), WM_CLOSE, 0, 0);
        return 0;
    }
    if (msg == WM_CHAR && (wParam == L'\r' || wParam == 27)) return 0;  // sin pitido
    return CallWindowProcW(g_editProc, hwnd, msg, wParam, lParam);
}

static void CopyAnswer() {
    bool err, status;
    std::wstring t = AnswerText(CurrentAnswer(), err, status);
    if (t.empty() || status || err) return;
    if (!OpenClipboard(g_pop.wnd)) return;
    EmptyClipboard();
    HGLOBAL g = GlobalAlloc(GMEM_MOVEABLE, (t.size() + 1) * sizeof(wchar_t));
    if (g) {
        memcpy(GlobalLock(g), t.c_str(), (t.size() + 1) * sizeof(wchar_t));
        GlobalUnlock(g);
        if (!SetClipboardData(CF_UNICODETEXT, g)) GlobalFree(g);
    }
    CloseClipboard();
    g_pop.copied = true;
    InvalidateRect(g_pop.wnd, nullptr, FALSE);
}

static int PopupHit(POINT pt) {
    for (int i = 0; i < 4; i++) {
        if (PtInRect(&g_pop.chips[i], pt)) return i;
    }
    if (PtInRect(&g_pop.copy, pt)) return 4;
    if (PtInRect(&g_pop.close, pt)) return 5;
    return -1;
}

static LRESULT CALLBACK PopupWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC dc = BeginPaint(hwnd, &ps);
            HDC mem = CreateCompatibleDC(dc);
            HBITMAP bmp = CreateCompatibleBitmap(dc, g_pop.w, g_pop.h);
            HGDIOBJ old = SelectObject(mem, bmp);
            PaintPopup(mem);
            BitBlt(dc, 0, 0, g_pop.w, g_pop.h, mem, 0, 0, SRCCOPY);
            SelectObject(mem, old);
            DeleteObject(bmp);
            DeleteDC(mem);
            EndPaint(hwnd, &ps);
            return 0;
        }
        case WM_ERASEBKGND:
            return 1;
        case WM_CTLCOLOREDIT: {
            HDC dc = (HDC)wParam;
            SetTextColor(dc, g_pop.c.text);
            SetBkColor(dc, g_pop.c.input);
            return (LRESULT)g_pop.inputBrush;
        }
        case WM_MOUSEMOVE: {
            POINT pt{GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
            int hot = PopupHit(pt);
            if (hot != g_pop.hotChip) {
                g_pop.hotChip = hot;
                TRACKMOUSEEVENT tme{sizeof(tme), TME_LEAVE, hwnd, 0};
                TrackMouseEvent(&tme);
                InvalidateRect(hwnd, nullptr, FALSE);
            }
            return 0;
        }
        case WM_MOUSELEAVE:
            g_pop.hotChip = -1;
            InvalidateRect(hwnd, nullptr, FALSE);
            return 0;
        case WM_SETCURSOR:
            if (LOWORD(lParam) == HTCLIENT && g_pop.hotChip >= 0) {
                SetCursor(LoadCursorW(nullptr, IDC_HAND));
                return TRUE;
            }
            break;
        case WM_LBUTTONUP: {
            int hit = PopupHit(POINT{GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)});
            if (hit >= 0 && hit < 4) RunAction((Action)hit);
            else if (hit == 4) CopyAnswer();
            else if (hit == 5) ClosePopup();
            return 0;
        }
        case WM_MOUSEWHEEL: {
            int step = (int)std::lround(40 * g_pop.s);
            g_pop.scroll -= GET_WHEEL_DELTA_WPARAM(wParam) > 0 ? step : -step;
            LayoutPopup();
            InvalidateRect(hwnd, nullptr, FALSE);
            return 0;
        }
        case WM_APP_ANSWER: {
            AnswerState a = CurrentAnswer();
            if (a.done) KillTimer(hwnd, TIMER_DOTS);
            ResizePopup();
            return 0;
        }
        case WM_APP_SUBMIT: {
            wchar_t buf[1024] = {};
            GetWindowTextW(g_pop.edit, buf, ARRAYSIZE(buf));
            std::wstring q = Trim(buf);
            if (!q.empty()) {
                SetWindowTextW(g_pop.edit, L"");
                RunAction(Action::Question, q);
            }
            return 0;
        }
        case WM_TIMER:
            if (wParam == TIMER_DOTS) {
                g_pop.dots++;
                InvalidateRect(hwnd, nullptr, FALSE);
            }
            return 0;
        case WM_KEYDOWN:
            if (wParam == VK_ESCAPE) ClosePopup();
            return 0;
        case WM_ACTIVATE:
            if (LOWORD(wParam) == WA_INACTIVE) {
                // Se cierra al hacer clic fuera (salvo hacia su propia caja).
                HWND to = (HWND)lParam;
                if (to != g_pop.edit) ClosePopup();
            } else if (g_pop.edit) {
                SetFocus(g_pop.edit);
            }
            return 0;
        case WM_CLOSE:
            ClosePopup();
            return 0;
    }
    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

static void OpenPopup(const std::wstring& text, POINT pt, HWND owner) {
    Settings st = CurrentSettings();
    std::wstring sel = text.size() > 6000 ? text.substr(0, 6000) : text;
    FreePopupGdi();
    g_pop.s = ScaleAt(pt);
    g_pop.c = MakeColors(st.theme);
    g_pop.font = MakeFont(14 * g_pop.s, FW_NORMAL);
    g_pop.fontSmall = MakeFont(12.5f * g_pop.s, FW_NORMAL);
    g_pop.fontBold = MakeFont(14 * g_pop.s, FW_SEMIBOLD);
    g_pop.inputBrush = CreateSolidBrush(g_pop.c.input);
    g_pop.text = sel;
    g_pop.owner = owner;
    g_pop.hasAction = false;
    g_pop.hotChip = -1;
    g_pop.scroll = 0;
    g_pop.copied = false;
    ++g_askGen;
    AbortRequest();
    UpdateAnswer([](AnswerState& a) {
        std::wstring label = a.label;
        a = AnswerState{};
        a.label = label;
    });
    if (!g_pop.wnd) {
        g_pop.wnd = CreateWindowExW(WS_EX_TOOLWINDOW | WS_EX_TOPMOST, kPopupClass, L"Consulta rápida", WS_POPUP, 0, 0, 1,
                                    1, nullptr, nullptr, GetModuleHandleW(nullptr), nullptr);
        if (!g_pop.wnd) return;
        g_popup = g_pop.wnd;
        g_pop.edit = CreateWindowExW(0, L"EDIT", L"", WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 0, 0, 10, 10, g_pop.wnd,
                                     nullptr, GetModuleHandleW(nullptr), nullptr);
        g_editProc = (WNDPROC)SetWindowLongPtrW(g_pop.edit, GWLP_WNDPROC, (LONG_PTR)EditProc);
        DWORD corner = kCornerRound;
        DwmSetWindowAttribute(g_pop.wnd, kDwmCornerPreference, &corner, sizeof(corner));
    }
    BOOL dark = g_pop.c.dark;
    DwmSetWindowAttribute(g_pop.wnd, kDwmDarkMode, &dark, sizeof(dark));
    COLORREF border = Mix(g_pop.c.bg, g_pop.c.text, g_pop.c.dark ? 0.2f : 0.18f);
    DwmSetWindowAttribute(g_pop.wnd, kDwmBorderColor, &border, sizeof(border));
    SendMessageW(g_pop.edit, WM_SETFONT, (WPARAM)g_pop.font, TRUE);
    SendMessageW(g_pop.edit, EM_SETCUEBANNER, TRUE, (LPARAM)L"Pregunta algo sobre el texto…");
    SetWindowTextW(g_pop.edit, L"");
    // Al lado de la selección (donde estaba el mouse), sin salirse.
    LayoutPopup();
    MONITORINFO mi{sizeof(mi)};
    GetMonitorInfoW(MonitorFromPoint(pt, MONITOR_DEFAULTTONEAREST), &mi);
    int x = std::clamp<int>(pt.x - g_pop.w / 4, mi.rcWork.left + 8, mi.rcWork.right - g_pop.w - 8);
    int y = pt.y + (int)std::lround(18 * g_pop.s);
    if (y + g_pop.h > mi.rcWork.bottom - 8) y = std::max<int>(mi.rcWork.top + 8, pt.y - g_pop.h - (int)std::lround(18 * g_pop.s));
    SetWindowPos(g_pop.wnd, HWND_TOPMOST, x, y, g_pop.w, g_pop.h, SWP_SHOWWINDOW);
    SetForegroundWindow(g_pop.wnd);
    SetFocus(g_pop.edit);
    DefaultAction da = st.defaultAction;
    if (da == DefaultAction::Auto) da = CountWords(sel) <= 3 ? DefaultAction::Define : DefaultAction::Explain;
    if (da == DefaultAction::Define) RunAction(Action::Define);
    else if (da == DefaultAction::Explain) RunAction(Action::Explain);
    InvalidateRect(g_pop.wnd, nullptr, FALSE);
}

// ---------------------------------------------------------------------------
// Ganchos del mouse y del teclado (solo cuando el navegador está al frente)
// ---------------------------------------------------------------------------

static HHOOK g_mouseHook = nullptr, g_keyHook = nullptr;
static POINT g_downPt{};
static DWORD g_lastUpTime = 0;
static POINT g_lastUpPt{};

static bool OurBrowserInFront() {
    HWND fg = GetForegroundWindow();
    DWORD pid = 0;
    if (fg) GetWindowThreadProcessId(fg, &pid);
    return pid == GetCurrentProcessId() && fg != g_pop.wnd;
}

// ¿El punto cae en el contenido de una página (no en pestañas ni barras)?
static bool IsPageContent(POINT pt) {
    HWND h = WindowFromPoint(pt);
    wchar_t cls[64] = {};
    if (h) GetClassNameW(h, cls, ARRAYSIZE(cls));
    return !wcscmp(cls, L"Chrome_RenderWidgetHostHWND") || !wcscmp(cls, L"MozillaWindowClass");
}

static LRESULT CALLBACK MouseHook(int code, WPARAM w, LPARAM l) {
    if (code == HC_ACTION) {
        auto* m = (MSLLHOOKSTRUCT*)l;
        switch (w) {
            case WM_LBUTTONDOWN:
                g_downPt = m->pt;
                PostMessageW(g_host, WM_APP_HIDEPILL, 0, MAKELPARAM(m->pt.x, m->pt.y));
                break;
            case WM_LBUTTONUP: {
                // Al soltar después de arrastrar, o en un doble clic (palabra).
                int dx = m->pt.x - g_downPt.x, dy = m->pt.y - g_downPt.y;
                bool drag = dx * dx + dy * dy > 64;
                bool dbl = m->time - g_lastUpTime < GetDoubleClickTime() && std::abs(m->pt.x - g_lastUpPt.x) < 6 &&
                           std::abs(m->pt.y - g_lastUpPt.y) < 6;
                g_lastUpTime = m->time;
                g_lastUpPt = m->pt;
                if ((drag || dbl) && OurBrowserInFront()) {
                    PostMessageW(g_host, WM_APP_MOUSEUP, (WPARAM)m->pt.x, (LPARAM)m->pt.y);
                }
                break;
            }
            case WM_RBUTTONDOWN:
            case WM_MOUSEWHEEL:
                PostMessageW(g_host, WM_APP_HIDEPILL, 0, 0);
                break;
        }
    }
    return CallNextHookEx(nullptr, code, w, l);
}

static bool HotkeyMatches(Hotkey hk, DWORD vk) {
    bool ctrl = GetAsyncKeyState(VK_CONTROL) & 0x8000, shift = GetAsyncKeyState(VK_SHIFT) & 0x8000,
         alt = GetAsyncKeyState(VK_MENU) & 0x8000;
    switch (hk) {
        case Hotkey::CtrlShiftSpace:
            return vk == VK_SPACE && ctrl && shift && !alt;
        case Hotkey::CtrlAltSpace:
            return vk == VK_SPACE && ctrl && alt && !shift;
        case Hotkey::AltQ:
            return vk == 'Q' && alt && !ctrl && !shift;
        default:
            return false;
    }
}

static std::atomic<int> g_hotkey{(int)Hotkey::CtrlShiftSpace};

static LRESULT CALLBACK KeyHook(int code, WPARAM w, LPARAM l) {
    if (code == HC_ACTION && (w == WM_KEYDOWN || w == WM_SYSKEYDOWN)) {
        auto* k = (KBDLLHOOKSTRUCT*)l;
        if (k->vkCode != VK_SHIFT && k->vkCode != VK_LSHIFT && k->vkCode != VK_RSHIFT && k->vkCode != VK_CONTROL &&
            k->vkCode != VK_LCONTROL && k->vkCode != VK_RCONTROL && k->vkCode != VK_MENU && k->vkCode != VK_LMENU &&
            k->vkCode != VK_RMENU) {
            if (HotkeyMatches((Hotkey)g_hotkey.load(), k->vkCode) && OurBrowserInFront()) {
                PostMessageW(g_host, WM_APP_HOTKEY, 0, 0);
                return 1;  // el atajo es del mod
            }
            PostMessageW(g_host, WM_APP_HIDEPILL, 0, 0);
        }
    }
    return CallNextHookEx(nullptr, code, w, l);
}

static POINT g_pendingPt{};
static HWND g_pendingOwner = nullptr;

static LRESULT CALLBACK HostWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_APP_MOUSEUP: {
            if (!CurrentSettings().showButton) return 0;
            POINT pt{(LONG)wParam, (LONG)lParam};
            if (!IsPageContent(pt)) return 0;
            g_pendingPt = pt;
            g_pendingOwner = GetAncestor(GetForegroundWindow(), GA_ROOT);
            // Un momento, por si fue el primero de un doble clic o sigue
            // ajustando la selección.
            SetTimer(hwnd, TIMER_PILL_ASK, 220, nullptr);
            return 0;
        }
        case WM_APP_HIDEPILL: {
            POINT pt{GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
            RECT pr{};
            if (g_pill && IsWindowVisible(g_pill)) GetWindowRect(g_pill, &pr);
            if (lParam && PtInRect(&pr, pt)) return 0;  // el clic es en el propio botón
            KillTimer(hwnd, TIMER_PILL_ASK);
            HidePill();
            return 0;
        }
        case WM_APP_HOTKEY: {
            HidePill();
            POINT pt;
            GetCursorPos(&pt);
            ReadSelection(pt, GetAncestor(GetForegroundWindow(), GA_ROOT));
            return 0;
        }
        case WM_APP_SELECTION: {
            std::unique_ptr<SelectionResult> r((SelectionResult*)lParam);
            if (r->text.empty()) {
                MessageBeep(MB_OK);
                return 0;
            }
            OpenPopup(r->text, r->pt, r->owner);
            return 0;
        }
        case WM_TIMER:
            if (wParam == TIMER_PILL_ASK) {
                KillTimer(hwnd, TIMER_PILL_ASK);
                if (OurBrowserInFront()) ShowPill(g_pendingPt, g_pendingOwner);
            } else if (wParam == TIMER_PILL_HIDE) {
                HidePill();
            }
            return 0;
    }
    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

// ---------------------------------------------------------------------------
// Hilo de la interfaz
// ---------------------------------------------------------------------------

static HANDLE g_uiThread = nullptr;
static DWORD g_uiThreadId = 0;

static HMODULE ThisModule() {
    HMODULE m = nullptr;
    GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                       (LPCWSTR)&ThisModule, &m);
    return m;
}

static DWORD WINAPI UiThread(LPVOID) {
    SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
    HINSTANCE inst = GetModuleHandleW(nullptr);
    WNDCLASSEXW wc{sizeof(wc)};
    wc.hInstance = inst;
    wc.lpfnWndProc = HostWndProc;
    wc.lpszClassName = kHostClass;
    RegisterClassExW(&wc);
    wc.lpfnWndProc = ClipWndProc;
    wc.lpszClassName = kClipClass;
    RegisterClassExW(&wc);
    wc.lpfnWndProc = PillWndProc;
    wc.lpszClassName = kPillClass;
    wc.hCursor = LoadCursorW(nullptr, IDC_HAND);
    RegisterClassExW(&wc);
    wc.lpfnWndProc = PopupWndProc;
    wc.lpszClassName = kPopupClass;
    wc.hCursor = LoadCursorW(nullptr, IDC_ARROW);
    wc.style = CS_DROPSHADOW;
    RegisterClassExW(&wc);

    g_host = CreateWindowExW(0, kHostClass, L"", 0, 0, 0, 0, 0, HWND_MESSAGE, nullptr, inst, nullptr);
    g_pill = CreateWindowExW(WS_EX_LAYERED | WS_EX_TOOLWINDOW | WS_EX_TOPMOST | WS_EX_NOACTIVATE, kPillClass, L"",
                             WS_POPUP, 0, 0, 1, 1, nullptr, nullptr, inst, nullptr);
    HMODULE mod = ThisModule();
    g_mouseHook = SetWindowsHookExW(WH_MOUSE_LL, MouseHook, mod, 0);
    g_keyHook = SetWindowsHookExW(WH_KEYBOARD_LL, KeyHook, mod, 0);
    Wh_Log(L"Consulta rápida lista (ganchos: %s)", g_mouseHook && g_keyHook ? L"sí" : L"no");

    MSG m;
    while (GetMessageW(&m, nullptr, 0, 0) > 0) {
        TranslateMessage(&m);
        DispatchMessageW(&m);
    }

    if (g_mouseHook) UnhookWindowsHookEx(g_mouseHook);
    if (g_keyHook) UnhookWindowsHookEx(g_keyHook);
    g_mouseHook = g_keyHook = nullptr;
    g_popup = nullptr;
    if (g_pop.wnd) DestroyWindow(g_pop.wnd);
    g_pop.wnd = g_pop.edit = nullptr;
    FreePopupGdi();
    if (g_pill) DestroyWindow(g_pill);
    if (g_host) DestroyWindow(g_host);
    g_pill = g_host = nullptr;
    for (const wchar_t* c : {kHostClass, kClipClass, kPillClass, kPopupClass}) UnregisterClassW(c, inst);
    return 0;
}

// Solo en el proceso principal del navegador (no en los de las páginas ni
// en los auxiliares, que no tienen ventanas ni red).
static bool IsBrowserMainProcess() {
    std::wstring cmd = GetCommandLineW();
    return cmd.find(L"--type=") == std::wstring::npos && cmd.find(L"-contentproc") == std::wstring::npos;
}

// ---------------------------------------------------------------------------
// Windhawk
// ---------------------------------------------------------------------------

BOOL Wh_ModInit() {
    if (!IsBrowserMainProcess()) return TRUE;
    LoadSettings();
    g_hotkey = (int)CurrentSettings().hotkey;
    g_unloading = false;
    g_uiThread = CreateThread(nullptr, 0, UiThread, nullptr, 0, &g_uiThreadId);
    return g_uiThread != nullptr;
}

void Wh_ModUninit() {
    if (!g_uiThread) return;
    g_unloading = true;
    ++g_askGen;
    AbortRequest();
    PostThreadMessageW(g_uiThreadId, WM_QUIT, 0, 0);
    WaitForSingleObject(g_uiThread, 5000);
    CloseHandle(g_uiThread);
    g_uiThread = nullptr;
    // Que no quede ningún hilo del mod corriendo cuando se descargue.
    for (int i = 0; i < 100 && g_workers > 0; i++) {
        AbortRequest();
        Sleep(50);
    }
}

void Wh_ModSettingsChanged() {
    LoadSettings();
    g_hotkey = (int)CurrentSettings().hotkey;
    ForgetBackend();
}
