#ifndef XBITMAP_H
#define XBITMAP_H

#pragma pack(2)

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

struct Color {
    unsigned char B, G, R, A;
};

class XBitmap
{
public:
    XBitmap();
    XBitmap(const char *filename);
    ~XBitmap();

    bool open(const char *filename);
    Color getPixel(int x, int y);
private:
    bool isOpen;
    XBMPFileHeader fileHeader;
    XBMPDIBHeader DIBHeader;
    Color *buffer;
};

#endif // XBITMAP_H
