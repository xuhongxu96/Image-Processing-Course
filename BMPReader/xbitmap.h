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
    int width, height;
    short plain;
    short bits;
    unsigned int compression;
    unsigned int dataSize;
    int dpiX, dpiY;
    unsigned int palette;
    unsigned int primaryColorCount;
};

class XBitmap
{
public:
    XBitmap();
    XBitmap(const char *filename);

    bool open(const char *filename);
private:
    bool isOpen;
    XBMPFileHeader fileHeader;
    XBMPDIBHeader DIBHeader;
};

#endif // XBITMAP_H
