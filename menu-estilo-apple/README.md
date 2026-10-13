# Menús estilo Apple para Windhawk

Mod de [Windhawk](https://windhawk.net), independiente del Dock, que cambia el
estilo de los menús del clic derecho y de los menús desplegables (Archivo,
Edición…) en el escritorio, el Explorador y las aplicaciones, para que se vean
como los de macOS.

![Menús claro y oscuro, con resaltado neutro y de acento](captura.png)

> La captura sale del banco de pruebas: los menús se dibujan con el código del
> mod, pero las esquinas, el borde y la sombra (que en Windows pone DWM) están
> simulados en la imagen.

## Qué hace

- Fondo liso claro u oscuro, según el tema de cada menú, sin la franja de los
  iconos a la izquierda.
- La opción bajo el mouse se marca con una pastilla redondeada, gris neutro o
  con el color de acento de Windows y el texto en blanco.
- Palomita, punto de opción y flecha de submenú vectoriales, con antialias.
- Separadores finos, esquinas más redondeadas y un borde fino.
- Las opciones desactivadas se ven tenues y no se resaltan.

## Instalación

1. En Windhawk: *Crear un mod nuevo*, pega el contenido de
   [`menu-estilo-apple.wh.cpp`](menu-estilo-apple.wh.cpp) y compílalo.
2. **Reinicia el Explorador una vez**: Administrador de tareas → Procesos →
   Explorador de Windows → Reiniciar (o cierra sesión y vuelve a entrar).

El paso 2 hace falta para el escritorio y las carpetas: el menú nuevo de
Windows 11 (el de iconos arriba) no es un menú clásico y no se puede restilar.
Con la opción **Menú clásico en el escritorio y el Explorador** (activada de
fábrica) el Explorador usa el menú completo de siempre, que sí toma el estilo,
pero Windows solo lo lee al arrancar el Explorador.

Para volver al menú de Windows 11, apaga esa opción y reinicia el Explorador
antes de quitar el mod.

## Configuración

Resaltado (gris neutro o color de acento), esquinas (redondeadas, poco
redondeadas o rectas), borde fino, resaltar también la barra de menús de las
apps y el menú clásico del Explorador.

## Si un menú no cambia

- Las apps que dibujan sus propios menús no cambian: Chrome, Firefox, Office,
  Visual Studio Code, las apps de la Tienda y el Bloc de notas nuevo.
- En *Registros* del mod, en Windhawk, cada proceso escribe "Menús estilo Apple
  listo" al cargar y "Primer menú con estilo" la primera vez que pinta uno. Si
  en un proceso aparece lo primero pero no lo segundo después de abrir un
  menú, ese menú no pasa por los temas de Windows.

## Cómo está hecho

Se engancha a las funciones de dibujo de temas de Windows (`uxtheme`:
`DrawThemeBackground`, `DrawThemeTextEx` y compañía). Para saber si lo que se
pinta es un menú le pregunta a uxtheme la clase del tema (`GetThemeClass`), así
reconoce también los menús de procesos que ya estaban abiertos. Las formas se
dibujan con su propio rasterizador con antialias y se mezclan sobre el menú;
las esquinas y el borde los pone DWM.

## Pruebas

`pruebas/menus.sh` compila `menu_muestra.cpp` (que incluye el mod tal cual) y
lo corre en Wine: comprueba la lógica (qué temas son de menú, el menú clásico
del Explorador) y dibuja menús de muestra; `pruebas/menus_png.py` los pasa a
PNG. Para compilarlo como Windhawk:
`SRC=../../menu-estilo-apple/menu-estilo-apple.wh.cpp sh herramientas/compilar/compilar.sh`.
