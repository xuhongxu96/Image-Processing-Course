#include "xbitmap.h"
#include <algorithm>
#include <cstdio>
#include <vector>
#include <cstring>

#pragma pack(1)

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

    FILE *fp = fopen(filename, "rb");
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

    buffer = new unsigned char[DIBHeader.dataSize];
    fread(buffer, 1, DIBHeader.dataSize, fp);

    isOpen = true;

    fclose(fp);
    return true;
}

Color XBitmap::getPixel(unsigned int x, unsigned int y, bool *in) {
    Color cr;
    if (in)
        if (!(*in = (x < DIBHeader.width) && (y < DIBHeader.height)))
            return cr;
    cr.B = buffer[(y * DIBHeader.width + x) * DIBHeader.bits / 8];
    cr.G = buffer[(y * DIBHeader.width + x) * DIBHeader.bits / 8 + 1];
    cr.R = buffer[(y * DIBHeader.width + x) * DIBHeader.bits / 8 + 2];
    if (DIBHeader.bits == 32) {
        cr.A = buffer[(y * DIBHeader.width + x) * DIBHeader.bits / 8 + 3];
    } else if (DIBHeader.bits == 24) {
        cr.A = 255;
    }
    return cr;
}

void XBitmap::avgBlur(int len)
{
    unsigned int width = DIBHeader.width, height = DIBHeader.height;
    unsigned char *newbuf = new unsigned char[DIBHeader.dataSize];

    for (unsigned int i = 0; i < width; ++i) {
        for (unsigned int j = 0; j < height; ++j) {
            int sz = 1;
            Color t = getPixel(i, j);
            for (int p = 1; p <= len; ++p) {
                bool in = false;
                t += getPixel(i + p, j, &in);
                if (in) ++sz;
                in = false;
                t += getPixel(i - p, j, &in);
                if (in) ++sz;
                in = false;
                t += getPixel(i, j + p, &in);
                if (in) ++sz;
                in = false;
                t += getPixel(i, j - p, &in);
                if (in) ++sz;
            }
            t /= sz;
            newbuf[(j * DIBHeader.width + i) * DIBHeader.bits / 8] = t.B;
            newbuf[(j * DIBHeader.width + i) * DIBHeader.bits / 8 + 1] = t.G;
            newbuf[(j * DIBHeader.width + i) * DIBHeader.bits / 8 + 2] = t.R;
            if (DIBHeader.bits == 32) {
                newbuf[(j * DIBHeader.width + i) * DIBHeader.bits / 8 + 3] = t.A;
            }
        }
    }
    memcpy(buffer, newbuf, DIBHeader.dataSize);
}


void XBitmap::midBlur(int len)
{
    unsigned int width = DIBHeader.width, height = DIBHeader.height;
    unsigned char *newbuf = new unsigned char[DIBHeader.dataSize];

    std::vector<Color> colors;

    for (unsigned int i = 0; i < width; ++i) {
        for (unsigned int j = 0; j < height; ++j) {
            Color t = getPixel(i, j);
            colors.clear();
            colors.push_back(t);
            for (int p = 1; p <= len; ++p) {
                bool in = false;
                t = getPixel(i + p, j, &in);
                if (in) {
                    colors.push_back(t);
                }
                in = false;
                t = getPixel(i - p, j, &in);
                if (in) {
                    colors.push_back(t);
                }
                in = false;
                t = getPixel(i, j + p, &in);
                if (in) {
                    colors.push_back(t);
                }
                in = false;
                t = getPixel(i, j - p, &in);
                if (in) {
                    colors.push_back(t);
                }
            }
            std::sort(colors.begin(), colors.end(), Color::CMP());
            t = colors[colors.size() / 2];
            newbuf[(j * DIBHeader.width + i) * DIBHeader.bits / 8] = t.B;
            newbuf[(j * DIBHeader.width + i) * DIBHeader.bits / 8 + 1] = t.G;
            newbuf[(j * DIBHeader.width + i) * DIBHeader.bits / 8 + 2] = t.R;
            if (DIBHeader.bits == 32) {
                newbuf[(j * DIBHeader.width + i) * DIBHeader.bits / 8 + 3] = t.A;
            }
        }
    }
    memcpy(buffer, newbuf, DIBHeader.dataSize);
}

void XBitmap::save(const char *filename)
{
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        perror("Failed to open BMP file");
        return;
    }
    fileHeader.offset = sizeof(fileHeader) + sizeof(DIBHeader);
    fileHeader.fileSize = fileHeader.offset + DIBHeader.dataSize;
    DIBHeader.size = sizeof(DIBHeader);

    fwrite(&fileHeader, sizeof(XBMPFileHeader), 1, fp);
    fwrite(&DIBHeader, sizeof(XBMPDIBHeader), 1, fp);

    fwrite(buffer, 1, DIBHeader.dataSize, fp);
    fclose(fp);
}

XBitmap::~XBitmap() {
    delete [] buffer;
}
