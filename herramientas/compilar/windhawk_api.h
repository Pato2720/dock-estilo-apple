// Réplica mínima de la API de Windhawk para compilar fuera de Windhawk.
#pragma once
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#ifdef __cplusplus
extern "C" {
#endif
__declspec(dllimport) void Wh_Log_External(PCWSTR format, ...);
__declspec(dllimport) int Wh_GetIntValue(PCWSTR valueName, int defaultValue);
__declspec(dllimport) BOOL Wh_SetIntValue(PCWSTR valueName, int value);
__declspec(dllimport) size_t Wh_GetStringValue(PCWSTR valueName, PWSTR stringBuffer, size_t bufferChars);
__declspec(dllimport) BOOL Wh_SetStringValue(PCWSTR valueName, PCWSTR value);
__declspec(dllimport) int Wh_GetIntSetting(PCWSTR valueName, ...);
__declspec(dllimport) PCWSTR Wh_GetStringSetting(PCWSTR valueName, ...);
__declspec(dllimport) void Wh_FreeStringSetting(PCWSTR string);
#ifdef __cplusplus
}
#endif
#define Wh_Log(message, ...) Wh_Log_External(L"[%d:%S]: " message, __LINE__, __FUNCTION__, ##__VA_ARGS__)
// Windhawk exporta estas funciones del mod; aquí se declaran para comprobarlo.
BOOL Wh_ModInit();
void Wh_ModUninit();
void Wh_ModSettingsChanged();
