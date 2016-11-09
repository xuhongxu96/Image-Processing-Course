#ifndef XBITMAP_H
#define XBITMAP_H

#pragma pack(1)

struct XBMPFileHeader {
    char bm[2];
    unsigned int fileSize;
    unsigned int reserved;
    unsigned int offset;
};

struct XBMPDIBHeader {
    unsigned int size;
    unsigned int width, height;
    short plain;
    short bits;
    unsigned int compression;
    unsigned int dataSize;
    int dpiX, dpiY;
    unsigned int palette;
    unsigned int primaryColorCount;
};

class Color {
public:
    int R, G, B, A;
    Color() {
        R = G = B = A = 0;
    }
    Color(const Color &c) {
        *this = c;
    }
    Color(int r, int g, int b, int a) {
        R = r;
        G = g;
        B = b;
        A = a;
    }
    Color &operator =(const Color &c) {
        R = c.R;
        G = c.G;
        B = c.B;
        A = c.A;
        return *this;
    }

    Color operator +(const Color &t) const {
        return Color(R + t.R, G + t.G, B + t.B, A + t.A);
    }

    Color operator +=(const Color &t) {
        return *this = Color(R + t.R, G + t.G, B + t.B, A + t.A);
    }

    Color operator /=(const int &n) {
        return *this = Color(R / n, G / n, B / n, A / n);
    }

    class CMP {
    public:
        bool operator ()(Color &a, Color &b) {
            return a.R + a.G + a.B < b.R + b.G + b.B;
        }
    };
};

class XBitmap
{
public:
    XBitmap();
    XBitmap(const char *filename);
    ~XBitmap();

    bool open(const char *filename);
    Color getPixel(unsigned int x, unsigned int y, bool *in = nullptr);
    void avgBlur(int len = 1);
    void midBlur(int len = 1);
    void save(const char *filename);
private:
    bool isOpen;
    XBMPFileHeader fileHeader;
    XBMPDIBHeader DIBHeader;
    unsigned char *buffer;
};

#endif // XBITMAP_H
