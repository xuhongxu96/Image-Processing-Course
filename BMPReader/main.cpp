#include <iostream>
#include <cstdio>
#include "xbitmap.h"
#pragma pack(1)

char filename[255];
XBitmap bmp;

void inputFileName() {
    printf("Please input BMP file name: \n");
    scanf("%s", filename);
}

void blur(bool avg = true) {
    int l, t;
    printf("Input Length: \n");
    scanf("%d", &l);
    printf("Input Times: \n");
    scanf("%d", &t);
    for (int i = 0; i < t; ++i)
        if (avg)
            bmp.avgBlur(l);
        else
            bmp.midBlur(l);
}

void getPixel() {
    int x, y;
    printf("Input x y: \n");
    scanf("%d %d", &x, &y);
    printf("Color of (%d, %d) is: ", x, y);
    Color cr = bmp.getPixel(x, y);
    std::cout << (int)cr.R << " " << (int)cr.G << " " << (int)cr.B << " " << (int)cr.A << std::endl;
}

void saveFile() {
    char filename[255];
    printf("Input saved BMP file name:\n");
    scanf("%s", filename);
    bmp.save(filename);
}

void chooseOperation() {
    printf("\nPlease input the operation number:\n\t1. Average Filter\n\t2. Median Filter\n\t3. Get Pixel\n\t4. Save BMP\n\t5: Exit\n");
    int o;
    scanf("%d", &o);
    switch (o) {
    case 1:
        blur();
        break;
    case 2:
        blur(false);
        break;
    case 3:
        getPixel();
        break;
    case 4:
        saveFile();
        break;
    case 5:
        exit(0);
        break;
    default:
        printf("Invalid Operation!\n");
        break;
    }
}

void errorPara() {
    printf("Invalid Parameter!\n");
    exit(-1);
}

int main(int argi, char **argv)
{
    if (argi > 1) {
        if (argi == 2) {
            errorPara();
        }
        if (!bmp.open(argv[1])) {
            exit(-1);
        }
        int o = atoi(argv[2]);
        switch (o) {
        case 1:
        {
            if (argi != 6) {
                errorPara();
            }
            int l = atoi(argv[3]);
            int t = atoi(argv[4]);
            for (int i = 0; i < t; ++i)
                bmp.avgBlur(l);
            bmp.save(argv[5]);
        }
            break;
        case 2:
        {
            if (argi != 6) {
                errorPara();
            }
            int l = atoi(argv[3]);
            int t = atoi(argv[4]);
            for (int i = 0; i < t; ++i)
                bmp.midBlur(l);
            bmp.save(argv[5]);
        }
            break;
        case 3:
        {
            if (argi != 5) {
                errorPara();
            }
            int x = atoi(argv[3]), y = atoi(argv[4]);
            Color cr = bmp.getPixel(x, y);
            std::cout << (int)cr.R << " " << (int)cr.G << " " << (int)cr.B << " " << (int)cr.A << std::endl;
        }
            break;
        default:
            errorPara();
            break;
        }
        exit(0);
    }

    bool open = false;
    while (!open) {
        inputFileName();
        open = bmp.open(filename);
    }

    while (1) {
        chooseOperation();
    }
    return 0;
}
