# Servidor falso compatible con la API de OpenAI para el banco de pruebas:
# /v1/models y /v1/chat/completions (por partes, como Foundry Local u Ollama).
# /error/v1/... responde con un error. Guarda el último pedido en ultimo.json.
import json, sys, time
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer

RESPUESTA = [
    "<think>La persona quiere ",
    "una definición breve.</think>",
    "**Serendipia**: hallazgo valioso que se ",
    "produce por casualidad, cuando se buscaba otra cosa. 🍀\n",
    "- Ejemplo: descubrir la penicilina ",
    "fue una serendipia.\n- Sinónimo: chiripa.",
]

class H(BaseHTTPRequestHandler):
    def log_message(self, *a):
        pass

    def _json(self, code, obj):
        b = json.dumps(obj).encode()
        self.send_response(code)
        self.send_header("Content-Type", "application/json")
        self.send_header("Content-Length", str(len(b)))
        self.end_headers()
        self.wfile.write(b)

    def do_GET(self):
        if self.path.endswith("/v1/models"):
            self._json(200, {"object": "list", "data": [{"id": "phi-4-mini-cpu"}, {"id": "Phi-4-mini-instruct-qnn-npu:2"}]})
        else:
            self._json(404, {"error": {"message": "no existe"}})

    def do_POST(self):
        n = int(self.headers.get("Content-Length", 0))
        body = self.rfile.read(n)
        open("ultimo.json", "wb").write(body)
        if self.path.startswith("/error/"):
            self._json(500, {"error": {"message": "El modelo no está cargado"}})
            return
        req = json.loads(body)
        self.send_response(200)
        self.send_header("Content-Type", "text/event-stream")
        self.end_headers()
        for i, part in enumerate(RESPUESTA):
            line = "data: " + json.dumps({"choices": [{"delta": {"content": part}}]}) + "\n\n"
            raw = line.encode()
            # Partido a la mitad, para probar líneas que llegan en dos trozos.
            half = len(raw) // 2 if i == 2 else len(raw)
            self.wfile.write(raw[:half]); self.wfile.flush(); time.sleep(0.03)
            self.wfile.write(raw[half:]); self.wfile.flush(); time.sleep(0.03)
        self.wfile.write(b"data: [DONE]\n\n")
        self.wfile.flush()

ThreadingHTTPServer(("127.0.0.1", int(sys.argv[1]) if len(sys.argv) > 1 else 8765), H).serve_forever()
