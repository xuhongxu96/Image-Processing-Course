from tkinter import *
from tkinter.simpledialog import askinteger
from tkinter import filedialog
from PIL import ImageTk, Image


class Bitmap:
    def __init__(self, filename):
        self.filename = filename
        self.file_header = {}
        self.dib_header = {}
        self.buffer = []
        with open(filename, "rb") as f:
            f.read(2)
            self.file_header['file_size'] = int.from_bytes(f.read(4), byteorder='little')
            self.file_header['reserved'] = int.from_bytes(f.read(4), byteorder='little')
            self.file_header['offset'] = int.from_bytes(f.read(4), byteorder='little')
            self.dib_header['size'] = int.from_bytes(f.read(4), byteorder='little')
            self.dib_header['width'] = int.from_bytes(f.read(4), byteorder='little')
            self.w = self.dib_header['width']
            self.dib_header['height'] = int.from_bytes(f.read(4), byteorder='little')
            self.h = self.dib_header['height']
            self.dib_header['plain'] = int.from_bytes(f.read(2), byteorder='little')
            self.dib_header['bits'] = int.from_bytes(f.read(2), byteorder='little')
            self.dib_header['compression'] = int.from_bytes(f.read(4), byteorder='little')
            self.dib_header['data_size'] = int.from_bytes(f.read(4), byteorder='little')
            self.dib_header['dpi_x'] = int.from_bytes(f.read(4), byteorder='little', signed=True)
            self.dib_header['dpi_y'] = int.from_bytes(f.read(4), byteorder='little', signed=True)
            self.dib_header['palette'] = int.from_bytes(f.read(4), byteorder='little')
            self.dib_header['primary_color_count'] = int.from_bytes(f.read(4), byteorder='little')
            f.seek(self.file_header['offset'])
            self.file_header['offset'] = 54
            self.buffer = f.read(self.dib_header['data_size'])
        if self.dib_header['bits'] != 24:
            raise Exception('Only 24 bits bitmaps are supported!')

    def get(self, x, y):
        if x < 0 or x >= self.w or y < 0 or y >=self.h:
            return (0, 0, 0), 0
        i = y * self.w + x
        return [self.buffer[i * 3 + 2], self.buffer[i * 3 + 1], self.buffer[i * 3]], True

    def tk_image(self):
        return ImageTk.PhotoImage(Image.frombuffer('RGB', (self.w, self.h), self.buffer, "raw", "BGR", 0, -1))

    def blur(self, lv=10):
        bf = bytearray(self.buffer)
        for x in range(self.w):
            for y in range(self.h):
                sz = 1
                cr, _ = self.get(x, y)
                for p in range(1, lv):
                    cr2, isIn = self.get(x + p, y)
                    cr = [a + b for a, b in zip(cr, cr2)]
                    sz += isIn
                    cr2, isIn = self.get(x - p, y)
                    cr = [a + b for a, b in zip(cr, cr2)]
                    sz += isIn
                    cr2, isIn = self.get(x, y + p)
                    cr = [a + b for a, b in zip(cr, cr2)]
                    sz += isIn
                    cr2, isIn = self.get(x, y - p)
                    cr = [a + b for a, b in zip(cr, cr2)]
                    sz += isIn
                cr = [int(a / sz) for a in cr]
                i = y * self.w + x
                bf[i * 3] = cr[2]
                bf[i * 3 + 1] = cr[1]
                bf[i * 3 + 2] = cr[0]
            print(x)
        self.buffer = bytes(bf)

    def save(self, filename):
        with open(filename, "wb") as f:
            f.write(bytes('BM', encoding='ascii'))
            f.write(self.file_header['file_size'].to_bytes(4, byteorder='little'))
            f.write(self.file_header['reserved'].to_bytes(4, byteorder='little'))
            f.write(self.file_header['offset'].to_bytes(4, byteorder='little'))
            f.write(self.dib_header['size'].to_bytes(4, byteorder='little'))
            f.write(self.dib_header['width'].to_bytes(4, byteorder='little'))
            f.write(self.dib_header['height'].to_bytes(4, byteorder='little'))
            f.write(self.dib_header['plain'].to_bytes(2, byteorder='little'))
            f.write(self.dib_header['bits'].to_bytes(2, byteorder='little'))
            f.write(self.dib_header['compression'].to_bytes(4, byteorder='little'))
            f.write(self.dib_header['data_size'].to_bytes(4, byteorder='little'))
            f.write(self.dib_header['dpi_x'].to_bytes(4, byteorder='little', signed=True))
            f.write(self.dib_header['dpi_y'].to_bytes(4, byteorder='little', signed=True))
            f.write(self.dib_header['palette'].to_bytes(4, byteorder='little'))
            f.write(self.dib_header['primary_color_count'].to_bytes(4, byteorder='little'))
            f.write(self.buffer)


tk = Tk()
file = filedialog.askopenfilename()
bmp = Bitmap(file)
img = bmp.tk_image()
panel = Label(tk, image=img)
panel.pack(side="bottom", fill="both", expand="yes")


def open_bmp():
    global file, bmp, img
    file = filedialog.askopenfilename()
    bmp = Bitmap(file)
    img = bmp.tk_image()
    panel.configure(image=img)


def show_blur():
    global img

    n = askinteger('平滑范围：', '输入参数', initialvalue=3)
    t = askinteger('平滑次数：', '输入参数', initialvalue=3)
    for i in range(t):
        bmp.blur(n)
    img = bmp.tk_image()
    panel.config(image=img)
    bmp.save("blur.bmp")

open_button = Button(tk, text="打开", command=open_bmp)
open_button.pack()
blur_button = Button(tk, text="空间域平滑", command=show_blur)
blur_button.pack()
tk.mainloop()
