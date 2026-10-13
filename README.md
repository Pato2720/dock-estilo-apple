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
- **Tope de cuadros por segundo** opcional (60, 30 o 60 solo con batería)
  para ahorrar batería en monitores de alta frecuencia.

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
reloj y bandeja arriba) y con **Menús estilo Apple**, un mod aparte que está en
[`menu-estilo-apple/`](menu-estilo-apple/).

## Configuración

Tamaño y separación de los iconos, ampliación (tamaño y alcance), estilo del
fondo (cristal líquido, translúcido, oscuro o claro), opacidad, redondeo,
distancia al borde, cuándo esconderse y con qué retardos, ocultar la barra de
tareas, Inicio (con icono propio opcional), papelera (de Windows o
vectorial), indicador de app abierta, nombres, sombras, vista de ventanas
(miniaturas, lista o nada), clic central (otra ventana, minimizar, cerrar),
clic en Inicio (menú Inicio, todas las aplicaciones o búsqueda), rueda del
mouse, bloquear el orden, rebotes, velocidad de las animaciones y cuadros por
segundo.

## Cómo está hecho

- Una ventana por capas que nunca toma el foco, del tamaño del Dock.
- El ritmo de cuadros sale de `D3DKMTWaitForVerticalBlankEvent` (con
  `DwmFlush` y un temporizador de alta resolución como respaldo).
- Rasterizador propio en enteros para iconos y fondo; GDI+ solo para texto.
- Hilos aparte para leer las ventanas abiertas, cargar iconos, preparar el
  cristal y tomar las fotos de las ventanas.
- El menú y la vista de ventanas son ventanas propias con ganchos de bajo
  nivel solo mientras están abiertos.
- Poco trabajo en segundo plano: el listado de ventanas valida cada proceso
  una vez por recorrido; la barra de tareas se busca por su clase; solo se
  escuchan los eventos del sistema que se usan; las fotos de ventanas se
  toman solo con la vista en miniaturas, nunca con un juego en pantalla
  completa, y se reducen con un promedio de área separable.

## Menús estilo Apple

Mod aparte, en su propia carpeta: [`menu-estilo-apple/`](menu-estilo-apple/).
Cambia el estilo de los menús del clic derecho y de los menús desplegables en
el escritorio, el Explorador y las aplicaciones: paletas (una con los colores
del menú del Dock), resaltado en gris claro u otros colores y formas,
separadores, marcas, animaciones, retraso de los submenús y apps excluidas.
No necesita el Dock.

## Herramientas

- `herramientas/compilar/compilar.sh`: compila el mod igual que Windhawk
  (clang + mingw-w64 vía `pip install ziglang`) para revisar errores y
  advertencias fuera de Windhawk.
- `herramientas/pruebas/`: banco de pruebas que carga el mod sin cambios en
  Wine, con una barra de tareas simulada, y recorre los escenarios con el
  mouse simulado (101 comprobaciones: clics, arrastres, menú, vista de
  ventanas, Finalizar tarea, esconderse, 120 fps, tope de 60…).

## Limitaciones

- Las insignias de número y las barras de progreso de la barra de tareas no
  se muestran.
- Solo en el monitor principal.
- Ocultar la barra de tareas la pone en "ocultar automáticamente"; al
  desactivar el mod vuelve como estaba.
