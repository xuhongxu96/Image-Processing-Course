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
    isOpen = false;

    std::FILE *fp = std::fopen(filename, "r");
    if (!fp) {
        std::perror("Failed to open BMP file");
        return false;
    }

    std::printf("Opened: %s\n", filename);

    std::printf("\n-- File Header\n");

    std::fread(&fileHeader, sizeof(XBMPFileHeader), 1, fp);

    std::printf("file_type:\t%c%c\n", fileHeader.bm[0], fileHeader.bm[1]);
    std::printf("file_size:\t%d\n", fileHeader.fileSize);
    std::printf("data_offset:\t%d\n", fileHeader.offset);
    std::printf("reserved:\t%d\n", fileHeader.reserved);


    std::printf("\n-- DIB Header\n");

    std::fread(&DIBHeader, sizeof(XBMPDIBHeader), 1, fp);

    std::printf("header_size:\t%d\n", DIBHeader.size);
    std::printf("image_size:\t%d * %d\n", DIBHeader.width, DIBHeader.height);
    std::printf("plain_id:\t%d\n", DIBHeader.plain);
    std::printf("bits_count:\t%d\n", DIBHeader.bits);
    std::printf("compression:\t%d\n", DIBHeader.compression);
    std::printf("data_size:\t%d\n", DIBHeader.dataSize);
    std::printf("DPI:\t\t%d, %d\n", DIBHeader.dpiX, DIBHeader.dpiY);
    std::printf("palette:\t%d\n", DIBHeader.palette);
    std::printf("primary_color:\t%d\n", DIBHeader.primaryColorCount);

    isOpen = true;

    fclose(fp);
}
