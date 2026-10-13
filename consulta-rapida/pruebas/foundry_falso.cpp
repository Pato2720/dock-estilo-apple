// "foundry.exe" falso para el banco de pruebas: imita la versión nueva
// ("foundry server status") o la vieja ("foundry service status") según la
// variable FAKE_FOUNDRY (new | old | load | alias | silent | off), apuntando
// al servidor falso. "alias" escribe las listas solo con alias (phi-4-mini) y
// "silent" no escribe nada en ellas.
// En "load" imita a Foundry actual con modelos: «model list --loaded/--cached»
// y «model load» (los cargados se anotan en cargado.txt, que también lee el
// servidor falso). Cargar un modelo sin descargar se queda esperando una
// confirmación por la entrada, como al descargar.
#include <windows.h>
#include <cstdio>
#include <cstring>
#include <cwchar>

static const char* kCached[] = {"DeepSeek-R1-Distill-Qwen-7B-openvino-npu:4", "Phi-4-mini-instruct-openvino-npu:2"};
static const char* kAlias[] = {"deepseek-r1-7b", "phi-4-mini"};

int wmain(int argc, wchar_t** argv) {
    wchar_t mode[16] = L"new";
    GetEnvironmentVariableW(L"FAKE_FOUNDRY", mode, 16);
    if (!wcscmp(mode, L"hang")) {
        Sleep(60000);  // como desde el navegador: el comando no termina nunca
        return 1;
    }
    if (argc < 3) return 1;
    bool server = !wcscmp(argv[1], L"server"), service = !wcscmp(argv[1], L"service");
    bool status = !wcscmp(argv[2], L"status");
    bool alias = !wcscmp(mode, L"alias"), silent = !wcscmp(mode, L"silent");
    if (!wcscmp(mode, L"load") || alias || silent) {
        if (server && status) {
            printf("Foundry Local daemon: running\n  Local service: http://127.0.0.1:8766/v1\n");
            return 0;
        }
        if (!wcscmp(argv[1], L"model") && !wcscmp(argv[2], L"list") && argc > 3) {
            if (silent) return 0;
            if (alias) {
                // Tabla con colores y solo alias.
                printf("\x1b[1mAlias\x1b[0m        Device   Size\n");
                if (!wcscmp(argv[3], L"--loaded")) {
                    char buf[256] = {};
                    FILE* f = fopen("cargado.txt", "rb");
                    if (f) {
                        fread(buf, 1, sizeof(buf) - 1, f);
                        fclose(f);
                    }
                    for (int i = 0; i < 2; i++) {
                        if (buf[0] && !strcmp(buf, kCached[i])) printf("\x1b[32m%s\x1b[0m   NPU   3.9 GB\n", kAlias[i]);
                    }
                } else {
                    for (const char* a : kAlias) printf("\x1b[32m%s\x1b[0m   NPU   3.9 GB\n", a);
                }
                return 0;
            }
            printf("Alias            Model ID\n");
            if (!wcscmp(argv[3], L"--loaded")) {
                char buf[256] = {};
                FILE* f = fopen("cargado.txt", "rb");
                if (f) {
                    fread(buf, 1, sizeof(buf) - 1, f);
                    fclose(f);
                    printf("loaded           %s\n", buf);
                }
            } else {
                for (const char* m : kCached) printf("model            %s\n", m);
            }
            return 0;
        }
        if (!wcscmp(argv[1], L"model") && !wcscmp(argv[2], L"load") && argc > 3) {
            char name[256] = {};
            WideCharToMultiByte(CP_UTF8, 0, argv[3], -1, name, sizeof(name), nullptr, nullptr);
            bool cached = false;
            for (const char* m : kCached) cached = cached || !strcmp(m, name);
            if (!cached) {
                printf("Model not found locally. Download it? [y/N] ");
                fflush(stdout);
                return getchar() == 'y' ? 0 : 1;
            }
            FILE* f = fopen("cargado.txt", "wb");
            if (f) {
                fputs(name, f);
                fclose(f);
            }
            printf("Model %s loaded\n", name);
            return 0;
        }
        return 1;
    }
    if (!wcscmp(mode, L"new") && server && status) {
        printf("Foundry Local daemon: running (PID 4242)\n  Local service: http://127.0.0.1:8765/v1\n  Logs: C:\\logs\n");
        return 0;
    }
    if (!wcscmp(mode, L"old") && service && status) {
        printf("\xF0\x9F\x9F\xA2 Model management service is running on http://127.0.0.1:8765/openai/status\n");
        return 0;
    }
    if (!wcscmp(mode, L"old") && server) {
        printf("Unknown command 'server'\n");
        return 1;
    }
    printf("Service is not running.\n");
    return 1;
}
