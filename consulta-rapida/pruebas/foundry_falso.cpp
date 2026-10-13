// "foundry.exe" falso para el banco de pruebas: imita la versión nueva
// ("foundry server status") o la vieja ("foundry service status") según la
// variable FAKE_FOUNDRY (new | old | off), apuntando al servidor falso.
#include <windows.h>
#include <cstdio>
#include <cwchar>

int wmain(int argc, wchar_t** argv) {
    wchar_t mode[16] = L"new";
    GetEnvironmentVariableW(L"FAKE_FOUNDRY", mode, 16);
    if (argc < 3) return 1;
    bool server = !wcscmp(argv[1], L"server"), service = !wcscmp(argv[1], L"service");
    bool status = !wcscmp(argv[2], L"status");
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
