# Imágenes del banco de pruebas (out/*.bgra) a PNG, sobre el fondo de prueba.
import glob, os, struct
from PIL import Image, ImageDraw, ImageFilter

def load(f):
    d = open(f, "rb").read()
    w, h = struct.unpack("<ii", d[:8])
    img = Image.frombuffer("RGBA", (w, h), d[8:8 + w * h * 4], "raw", "BGRA", 0, 1)
    r, g, b, a = img.split()
    # premultiplicado -> recto
    px = img.load()
    out = Image.new("RGBA", (w, h))
    op = out.load()
    for y in range(h):
        for x in range(w):
            B, G, R, A = d[8 + (y * w + x) * 4: 12 + (y * w + x) * 4]
            op[x, y] = (min(255, R * 255 // A), min(255, G * 255 // A), min(255, B * 255 // A), A) if A else (0, 0, 0, 0)
    return out

def framed(img, dark, behind=None, radius=8):
    # Esquinas, borde y sombra: en Windows los pone DWM. Con 'behind', el
    # cristal: lo de atrás desenfocado (también lo hace DWM) bajo la ventanita.
    w, h = img.size
    if behind is not None:
        glass = behind.resize((w, h)).filter(ImageFilter.GaussianBlur(14)).convert("RGBA")
        glass.alpha_composite(img)
        img = glass
    m = 24
    out = Image.new("RGBA", (w + 2 * m, h + 2 * m), (0, 0, 0, 0))
    shadow = Image.new("L", out.size, 0)
    ImageDraw.Draw(shadow).rounded_rectangle((m, m + 6, m + w - 1, m + h + 5), radius, fill=130 if dark else 80)
    out.paste(Image.new("RGBA", out.size, (0, 0, 0, 255)), (0, 0), shadow.filter(ImageFilter.GaussianBlur(12)))
    mask = Image.new("L", (w * 4, h * 4), 0)
    ImageDraw.Draw(mask).rounded_rectangle((0, 0, w * 4 - 1, h * 4 - 1), radius * 4, fill=255)
    out.paste(img, (m, m), mask.resize((w, h), Image.LANCZOS))
    border = Image.new("L", (w * 4, h * 4), 0)
    ImageDraw.Draw(border).rounded_rectangle((0, 0, w * 4 - 1, h * 4 - 1), radius * 4, outline=255, width=4)
    col = (82, 82, 88, 255) if dark else (205, 205, 210, 255)
    out.paste(Image.new("RGBA", (w, h), col), (m, m), border.resize((w, h), Image.LANCZOS))
    return out

os.makedirs("png", exist_ok=True)
wp = Image.open("../../herramientas/pruebas/fondo.png").convert("RGBA")
p1, p2 = load("out/1-definir-claro.bgra"), load("out/2-explicar-oscuro.bgra")
pops = [framed(p1, False, wp.crop((34, 34, 34 + p1.width, 34 + p1.height))),
        framed(p2, True, wp.crop((p1.width + 92, 34, p1.width + 92 + p2.width, 34 + p2.height)))]
pills = [load("out/4-boton-claro.bgra"), load("out/5-boton-oscuro.bgra")]
W = sum(p.width for p in pops) + 30
H = max(p.height for p in pops) + max(p.height for p in pills) + 40
sheet = wp.resize((max(W, wp.width), max(H, wp.height))).crop((0, 0, W, H))
x = 10
for p in pops:
    sheet.alpha_composite(p, (x, 10))
    x += p.width + 10
y = max(p.height for p in pops) + 20
x = 40
for p in pills:
    sheet.alpha_composite(p, (x, y))
    x += p.width + 30
sheet.convert("RGB").save("png/consulta-rapida.png")
for f in glob.glob("out/*.bgra"):
    load(f).save("png/" + os.path.basename(f)[:-5] + ".png")
print(sheet.size)
