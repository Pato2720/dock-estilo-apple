import struct, glob, os, sys
from PIL import Image
wp=Image.open("fondo.png").convert("RGBA")
os.makedirs("png",exist_ok=True)
outs=[]
for f in sorted(glob.glob("out/*.bgra")):
    d=open(f,"rb").read()
    w,h=struct.unpack("<ii",d[:8])
    img=Image.frombuffer("RGBA",(w,h),d[8:8+w*h*4],"raw","BGRA",0,1)
    # premultiplicado -> recto
    r,g,b,a=img.split()
    import numpy as np
    arr=np.array(img).astype(np.float32)
    al=arr[...,3:4]
    rgb=np.where(al>0, np.clip(arr[...,:3]*255.0/np.maximum(al,1),0,255), 0)
    out=Image.fromarray(np.concatenate([rgb,al],axis=2).astype(np.uint8),"RGBA")
    name=os.path.basename(f)[:-5]
    if w==1600:
        bg=wp.crop((0,900-h,w,900)).copy(); bg.alpha_composite(out); bg=bg.crop((420,0,1180,h))
    else:
        bg=wp.crop((700,300,700+w,300+h)).copy(); bg.alpha_composite(out)
    bg.save(f"png/{name}.png"); outs.append(bg)
W=max(i.width for i in outs); H=sum(i.height+6 for i in outs)
sheet=Image.new("RGB",(W,H),(20,20,20)); y=0
for i in outs: sheet.paste(i,(0,y)); y+=i.height+6
sheet.save("png/sheet.png"); print(len(outs), sheet.size)
