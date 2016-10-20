#include "xbitmap.h"
#include <cstdio>

XBitmap::XBitmap() : isOpen(false)
{

}

XBitmap::XBitmap(const char *filename)
{
    open(filename);
}

bool XBitmap::open(const char *filename)
{
    if (isOpen) {
        delete [] buffer;
    }
    isOpen = false;

    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Failed to open BMP file");
        return false;
    }

    printf("Opened: %s\n", filename);

    printf("\n-- File Header\n");

    fread(&fileHeader, sizeof(XBMPFileHeader), 1, fp);

    printf("file_type:\t%c%c\n", fileHeader.bm[0], fileHeader.bm[1]);
    printf("file_size:\t%d\n", fileHeader.fileSize);
    printf("data_offset:\t%d\n", fileHeader.offset);
    printf("reserved:\t%d\n", fileHeader.reserved);


    printf("\n-- DIB Header\n");

    fread(&DIBHeader, sizeof(XBMPDIBHeader), 1, fp);

    printf("header_size:\t%d\n", DIBHeader.size);
    printf("image_size:\t%d * %d\n", DIBHeader.width, DIBHeader.height);
    printf("plain_id:\t%d\n", DIBHeader.plain);
    printf("bits_count:\t%d\n", DIBHeader.bits);
    printf("compression:\t%d\n", DIBHeader.compression);
    printf("data_size:\t%d\n", DIBHeader.dataSize);
    printf("DPI:\t\t%d, %d\n", DIBHeader.dpiX, DIBHeader.dpiY);
    printf("palette:\t%d\n", DIBHeader.palette);
    printf("primary_color:\t%d\n", DIBHeader.primaryColorCount);

    fseek(fp, fileHeader.offset, SEEK_SET);
    int size = DIBHeader.width * DIBHeader.height;

    buffer = new char[DIBHeader.bits * size];
    fread(buffer, 1, DIBHeader.bits * size, fp);

    isOpen = true;

    fclose(fp);
    return true;
}

Color XBitmap::getPixel(int x, int y) {
    Color cr;
    if (DIBHeader.bits == 32) {
        cr.B = buffer[(y * DIBHeader.width + x) * DIBHeader.bits / 8];
        cr.G = buffer[(y * DIBHeader.width + x) * DIBHeader.bits / 8 + 1];
        cr.R = buffer[(y * DIBHeader.width + x) * DIBHeader.bits / 8 + 2];
        cr.A = buffer[(y * DIBHeader.width + x) * DIBHeader.bits / 8 + 3];
    } else if (DIBHeader.bits == 24) {
        cr.B = buffer[(y * DIBHeader.width + x) * DIBHeader.bits / 8];
        cr.G = buffer[(y * DIBHeader.width + x) * DIBHeader.bits / 8 + 1];
        cr.R = buffer[(y * DIBHeader.width + x) * DIBHeader.bits / 8 + 2];
        cr.A = 255;
    }
    return cr;
}

XBitmap::~XBitmap() {
    delete [] buffer;
}
