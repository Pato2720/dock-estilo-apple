// "foundrylocald.exe" falso para el banco de pruebas: el servicio de Foundry
// Local como un proceso de Windows que escucha en un puerto cualquiera. La
// API ofrece todo el catálogo, pero solo responde con el modelo cargado
// (Phi-4-mini de NPU); a los demás, "must be loaded before inference".
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

static const char* kLoaded = "Phi-4-mini-instruct-openvino-npu:2";

static void Reply(SOCKET c, int code, const char* type, const std::string& body) {
    char head[256];
    snprintf(head, sizeof(head), "HTTP/1.1 %d X\r\nContent-Type: %s\r\nContent-Length: %d\r\nConnection: close\r\n\r\n",
             code, type, (int)body.size());
    send(c, head, (int)strlen(head), 0);
    send(c, body.data(), (int)body.size(), 0);
}

int main() {
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);
    SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    sockaddr_in a{};
    a.sin_family = AF_INET;
    a.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    a.sin_port = 0;  // un puerto cualquiera, como Foundry
    bind(s, (sockaddr*)&a, sizeof(a));
    listen(s, 8);
    // Anota su puerto para el banco de pruebas (Wine no dice de qué proceso
    // es cada puerto).
    int alen = sizeof(a);
    getsockname(s, (sockaddr*)&a, &alen);
    if (FILE* f = fopen("pid.txt", "w")) {
        fprintf(f, "%lu", GetCurrentProcessId());
        fclose(f);
    }
    if (FILE* f = fopen("puerto.txt", "w")) {
        fprintf(f, "%d", ntohs(a.sin_port));
        fclose(f);
    }
    for (;;) {
        SOCKET c = accept(s, nullptr, nullptr);
        if (c == INVALID_SOCKET) break;
        std::string req;
        char buf[4096];
        size_t headEnd = std::string::npos;
        int len = 0;
        while (true) {
            int n = recv(c, buf, sizeof(buf), 0);
            if (n <= 0) break;
            req.append(buf, n);
            if (headEnd == std::string::npos && (headEnd = req.find("\r\n\r\n")) != std::string::npos) {
                size_t cl = req.find("Content-Length:");
                if (cl == std::string::npos) cl = req.find("content-length:");
                len = cl != std::string::npos ? atoi(req.c_str() + cl + 15) : 0;
            }
            if (headEnd != std::string::npos && req.size() >= headEnd + 4 + len) break;
        }
        std::string body = headEnd != std::string::npos ? req.substr(headEnd + 4) : "";
        if (req.compare(0, 4, "GET ") == 0 && req.find("/v1/models") != std::string::npos) {
            Reply(c, 200, "application/json",
                  "{\"data\":[{\"id\":\"Phi-3-mini-4k-instruct-openvino-npu:1\"},"
                  "{\"id\":\"DeepSeek-R1-Distill-Qwen-7B-openvino-npu:4\"},{\"id\":\"Phi-4-mini-instruct-openvino-npu:2\"}]}");
        } else if (req.compare(0, 5, "POST ") == 0 && req.find("/v1/chat/completions") != std::string::npos) {
            if (body.find(std::string("\"") + kLoaded + "\"") == std::string::npos) {
                Reply(c, 400, "application/json",
                      "{\"error\":{\"message\":\"Model not loaded: must be loaded before inference\"}}");
            } else if (body.find("\"stream\":true") != std::string::npos) {
                Reply(c, 200, "text/event-stream",
                      "data: {\"choices\":[{\"delta\":{\"content\":\"Hola desde la NPU.\"}}]}\n\ndata: [DONE]\n\n");
            } else {
                Reply(c, 200, "application/json", "{\"choices\":[{\"message\":{\"content\":\"h\"}}]}");
            }
        } else {
            Reply(c, 404, "application/json", "{\"error\":{\"message\":\"no existe\"}}");
        }
        closesocket(c);
    }
    return 0;
}
