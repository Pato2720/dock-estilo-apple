# Dock estilo Apple para Windhawk

Mod de [Windhawk](https://windhawk.net) que reemplaza la barra de tareas de
Windows 11 por un Dock como el de macOS, abajo de la pantalla principal.

![Ampliación al pasar el mouse](capturas/ampliacion.png)

## Qué hace

- **Ampliación fluida**: cada cuadro se sincroniza con el refresco real del
  monitor (60, 120, 144 Hz). Los iconos se dibujan con precisión de subpíxel.
- **Apps ancladas y abiertas**: toma las apps ancladas a la barra de Windows,
  incluidas las de la Tienda (Configuración, Calculadora…). Las abiertas sin
  anclar aparecen a la derecha, con su indicador.
- **Vista de ventanas** al pasar el mouse sobre una app abierta: miniaturas en
  vivo y, para las ventanas minimizadas, una foto de cómo se veían.
- **Menú propio con clic derecho**: ventanas de la app, Nueva ventana, Ocultar,
  Ocultar las demás, Mantener en el Dock, Abrir al iniciar sesión, Mostrar en
  el Explorador, Ejecutar como administrador, Salir y **Finalizar tarea**.
  Funciona aunque otra app esté en primer plano.
- **Arrastrar** para ordenar, hacia arriba o a la papelera para quitar.
- **Soltar archivos**: en la papelera (la de Windows, con deshacer), en una app
  para abrirlos con ella, o anclar un `.exe`/acceso directo entre los iconos.
- **Inicio** con el logo de Windows del sistema y **Papelera de reciclaje**.
- **Se esconde** con ventanas maximizadas o en pantalla completa; aparece al
  llevar el mouse al borde de abajo, ocupando solo su espacio.
- Rueda del mouse sobre una app para pasar entre ventanas o minimizar.

| Vista de ventanas | Ventana minimizada | Menú |
|---|---|---|
| ![](capturas/vista-de-ventanas.png) | ![](capturas/vista-minimizada.png) | ![](capturas/menu.png) |

> Las capturas salen del banco de pruebas en Wine, donde los iconos son los
> genéricos de Wine y no hay miniaturas en vivo. En Windows se ven los iconos
> y las ventanas reales.

## Instalación

1. Instala [Windhawk](https://windhawk.net).
2. *Crear un mod nuevo*, pega el contenido de
   [`dock-estilo-apple.wh.cpp`](dock-estilo-apple.wh.cpp) y compílalo.
3. Ajusta la configuración del mod a tu gusto.

Hace pareja con el mod **Muesca superior estilo Apple** (barra de menús,
reloj y bandeja arriba).

## Configuración

Tamaño y separación de los iconos, ampliación (tamaño y alcance), estilo del
fondo (cristal líquido, translúcido, oscuro o claro), opacidad, redondeo,
distancia al borde, cuándo esconderse y con qué retardos, ocultar la barra de
tareas, Inicio (con icono propio opcional), papelera (de Windows o
vectorial), indicador de app abierta, nombres, sombras, vista de ventanas
(miniaturas, lista o nada), clic central, rueda del mouse, bloquear el orden,
rebotes y velocidad de las animaciones.

## Cómo está hecho

- Una ventana por capas que nunca toma el foco, del tamaño del Dock.
- El ritmo de cuadros sale de `D3DKMTWaitForVerticalBlankEvent` (con
  `DwmFlush` y un temporizador de alta resolución como respaldo).
- Rasterizador propio en enteros para iconos y fondo; GDI+ solo para texto.
- Hilos aparte para leer las ventanas abiertas, cargar iconos, preparar el
  cristal y tomar las fotos de las ventanas.
- El menú y la vista de ventanas son ventanas propias con ganchos de bajo
  nivel solo mientras están abiertos.

## Herramientas

- `herramientas/compilar/compilar.sh`: compila el mod igual que Windhawk
  (clang + mingw-w64 vía `pip install ziglang`) para revisar errores y
  advertencias fuera de Windhawk.
- `herramientas/pruebas/`: banco de pruebas que carga el mod sin cambios en
  Wine, con una barra de tareas simulada, y recorre los escenarios con el
  mouse simulado (100 comprobaciones: clics, arrastres, menú, vista de
  ventanas, Finalizar tarea, esconderse, 120 fps…).

## Limitaciones

- Las insignias de número y las barras de progreso de la barra de tareas no
  se muestran.
- Solo en el monitor principal.
- Ocultar la barra de tareas la pone en "ocultar automáticamente"; al
  desactivar el mod vuelve como estaba.
