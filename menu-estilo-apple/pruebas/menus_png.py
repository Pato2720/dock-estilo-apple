# Muestras de "Menús estilo Apple" (out/menu-*.bgra) a PNG, sobre el fondo de
# prueba. Las esquinas redondeadas, el borde y la sombra los pone DWM en
# Windows; aquí se simulan para ver el conjunto.
import struct, os
from PIL import Image, ImageDraw, ImageFilter

def load(name):
    d = open(f"out/{name}.bgra", "rb").read()
    w, h = struct.unpack("<ii", d[:8])
    img = Image.frombuffer("RGBA", (w, h), d[8:8 + w * h * 4], "raw", "BGRA", 0, 1)
    return img.convert("RGB")

def framed(img, dark, radius=8):
    w, h = img.size
    m = 24
    out = Image.new("RGBA", (w + 2 * m, h + 2 * m), (0, 0, 0, 0))
    shadow = Image.new("L", out.size, 0)
    ImageDraw.Draw(shadow).rounded_rectangle((m, m + 4, m + w - 1, m + h + 3), radius, fill=120 if dark else 70)
    shadow = shadow.filter(ImageFilter.GaussianBlur(10))
    out.paste(Image.new("RGBA", out.size, (0, 0, 0, 255)), (0, 0), shadow)
    mask = Image.new("L", (w * 4, h * 4), 0)
    ImageDraw.Draw(mask).rounded_rectangle((0, 0, w * 4 - 1, h * 4 - 1), radius * 4, fill=255)
    mask = mask.resize((w, h), Image.LANCZOS)
    out.paste(img, (m, m), mask)
    border = Image.new("L", (w * 4, h * 4), 0)
    ImageDraw.Draw(border).rounded_rectangle((0, 0, w * 4 - 1, h * 4 - 1), radius * 4, outline=255, width=4)
    border = border.resize((w, h), Image.LANCZOS)
    out.paste(Image.new("RGBA", (w, h), (72, 72, 78, 255) if dark else (200, 200, 206, 255)), (m, m), border)
    return out

import glob
files = sorted(glob.glob("out/[0-9]-*.bgra"))
dark_of = lambda f: "oscuro" in f or "hielo" in f or "contraste" in f
cards = [framed(load(os.path.basename(f)[:-5]), dark_of(f)) for f in files]
cols = 3
rows = (len(cards) + cols - 1) // cols
cw, ch = cards[0].width, cards[0].height
W, H = cols * cw + 20, rows * ch + 20
wp = Image.open("../../herramientas/pruebas/fondo.png").convert("RGBA")
sheet = wp.resize((max(W, wp.width), max(H, wp.height))).crop((0, 0, W, H))
for k, c in enumerate(cards):
    sheet.alpha_composite(c, (10 + (k % cols) * cw, 10 + (k // cols) * ch))
os.makedirs("png", exist_ok=True)
sheet.convert("RGB").save("png/menus.png")
print(sheet.size)
