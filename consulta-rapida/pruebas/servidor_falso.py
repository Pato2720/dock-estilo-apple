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

# Con "foundry" como segundo argumento imita a Foundry Local actual: la API
# ofrece todo el catálogo (también modelos sin descargar), solo responde con
# los cargados (los que anotó «foundry model load» en cargado.txt) y no tiene
# los endpoints viejos /openai/loadedmodels ni /openai/load.
FOUNDRY = len(sys.argv) > 2 and sys.argv[2] == "foundry"
MODELOS = ["Phi-3-mini-4k-instruct-openvino-npu:1", "DeepSeek-R1-Distill-Qwen-7B-openvino-npu:4",
           "Phi-4-mini-instruct-openvino-npu:2", "phi-4-mini-cpu"] if FOUNDRY \
    else ["phi-4-mini-cpu", "Phi-4-mini-instruct-qnn-npu:2"]

def cargados():
    try:
        return {open("cargado.txt").read().strip()}
    except OSError:
        return set()

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
        from urllib.parse import unquote
        if self.path.endswith("/v1/models"):
            self._json(200, {"object": "list", "data": [{"id": m} for m in MODELOS]})

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
        if FOUNDRY and req.get("model") not in cargados():
            m = req.get("model")
            self._json(400, {"error": {"message": f"Model not loaded: Model '{m}' must be loaded before inference"}})
            return
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
