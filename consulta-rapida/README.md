# Consulta rápida al seleccionar

Mod de [Windhawk](https://windhawk.net), independiente de los demás, que hace
en el navegador algo parecido a Click to Do de Copilot: seleccionas texto,
aparece un botón **Preguntar** y la respuesta sale ahí mismo, en una
ventanita, sin abrir Copilot ni cambiar de pestaña.

![Ventanita definiendo una palabra (claro) y explicando un texto (oscuro), y el botón Preguntar](captura.png)

> La captura sale del banco de pruebas, con un servidor falso; las esquinas,
> el borde y la sombra (que en Windows pone DWM) están simulados.

## Qué hace

- Al seleccionar texto en una página (arrastrando o con doble clic) aparece
  **Preguntar** junto al mouse unos segundos. También sirve un atajo
  (Ctrl+Mayús+Espacio de fábrica).
- La ventanita **define** si seleccionaste de una a tres palabras y
  **explica** si es un texto más largo. Los botones **Definir**,
  **Explicar**, **Resumir** y **Traducir** cambian la acción, y abajo puedes
  escribir tu propia pregunta sobre el texto (Enter).
- La respuesta llega palabra por palabra. **Copiar** la copia; Esc o un clic
  fuera cierra la ventanita.
- Chrome, Edge, Firefox, Brave, Opera y Vivaldi.

## Con qué responde

| Opción | Dónde corre | Cómo prepararlo |
|---|---|---|
| **Foundry Local** (de fábrica) | En tu equipo, en la **NPU** de un PC Copilot+ (o en la GPU/CPU) | `winget install Microsoft.FoundryLocal` y `foundry run phi-4-mini` |
| Ollama | En tu equipo (GPU/CPU) | `ollama run llama3.2` |
| Otro servicio | Cualquiera compatible con la API de OpenAI, local o en la nube | Dirección, modelo y clave en la configuración |

Con Foundry Local el mod encuentra solo la dirección del servicio (cambia en
cada arranque), lo arranca si está apagado y elige un modelo hecho para la
NPU si hay uno. Funciona con la versión actual de Foundry Local (`foundry
server …`) y con la anterior (`foundry service …`).

### Si dice que Foundry Local no está en marcha

En una terminal (PowerShell):

```
foundry server status        # ¿está corriendo? muestra su dirección
foundry server start         # si no, arráncalo
foundry run phi-4-mini       # descarga y carga un modelo (la primera vez tarda)
foundry model list           # qué modelos hay (los "npu" usan la NPU)
```

En la versión anterior los comandos eran `foundry service status` y
`foundry service start`. Si `foundry` no se reconoce, no está instalado
(`winget install Microsoft.FoundryLocal`) o hay que abrir una terminal nueva.
Si se instaló con el navegador abierto, ciérralo y ábrelo otra vez.

Foundry ofrece todos los modelos descargados, pero solo responde con los
**cargados**. Si el elegido no lo está, el mod lo carga solo ("Cargando el
modelo…", la primera vez tarda) y vuelve a preguntar. Elige primero los que
ya están cargados; si no, uno de NPU, y entre ellos los rápidos (Phi) antes
que los de razonamiento (DeepSeek-R1, que piensa mucho antes de contestar).
Para usar uno en particular, pon su nombre en **Modelo** (los nombres salen
en `foundry model list`) y cárgalo con `foundry model load <nombre>`.

Si aun así falla, copia la dirección que muestra `foundry server status`
(por ejemplo `http://127.0.0.1:5273`) en **Dirección del servicio**, en la
configuración del mod. Para que no cambie de puerto:
`foundry server restart --port 5273 --idle-timeout 0`. En *Registros* del
mod, en Windhawk, aparece lo que respondió Foundry. Con los
locales, el texto no sale de tu equipo; con un servicio en la nube, el texto
seleccionado se envía a ese servicio.

**¿Por qué no el mismo modelo de Click to Do?** Click to Do usa Phi Silica,
que Windows solo deja usar a apps empaquetadas con un permiso especial de
Microsoft. Un mod no puede pedirlo; Foundry Local es la forma abierta de usar
la NPU.

## Configuración

Servicio, dirección, modelo y clave; botón al seleccionar (sí/no); atajo
(Ctrl+Mayús+Espacio, Ctrl+Alt+Espacio, Alt+Q o ninguno); qué hacer al abrir
(automático, definir, explicar o esperar); idioma de las respuestas; idioma al
que traducir; largo máximo de la respuesta; cómo leer la selección; tema de
la ventanita.

## Cómo lee la selección

De fábrica la **copia** (como Ctrl+C) solo cuando tocas Preguntar o el atajo,
y después devuelve el portapapeles como estaba (texto, imágenes, archivos;
los formatos raros de algunas apps pueden perderse). La alternativa es
**Accesibilidad de Windows** (UI Automation), que no toca el portapapeles
pero hace que Chrome y Edge activen su modo de accesibilidad, que gasta algo
más; si no consigue el texto, lo copia.

## Pruebas

`pruebas/correr.sh` levanta un servidor falso compatible con OpenAI
(`servidor_falso.py`, con respuestas por partes, razonamiento `<think>`,
acentos y emojis), compila `prueba.cpp` (que incluye el mod tal cual) y lo
corre en Wine, con un `foundry.exe` falso que imita la versión nueva y la
vieja, y un servidor que solo responde con modelos cargados: 35 comprobaciones (JSON, respuestas por partes, elección del
modelo de NPU, la dirección de Foundry Local en sus dos versiones, la ventanita y sus botones, la
pregunta propia, errores claros, leer la selección y devolver el
portapapeles, que no queden hilos al desactivarlo). `pruebas/a_png.py` pasa
las imágenes a PNG. Para compilarlo como Windhawk:
`SRC=../../consulta-rapida/consulta-rapida.wh.cpp sh herramientas/compilar/compilar.sh`.

No se pudo probar con un navegador ni con Foundry Local de verdad (el banco
corre en Linux con Wine).
