#include <iostream>
#include <cstdio>
#include "xbitmap.h"

int main()
{
    XBitmap bmp("Sketch.bmp");
    bmp.avgBlur(30);
    bmp.save("Blur.bmp");
    while (1) {
        int x, y;
        printf("Input x y: \n");
        scanf("%d %d", &x, &y);
        printf("Color of (%d, %d) is: ", x, y);
        Color cr = bmp.getPixel(x, y);
        std::cout << (int)cr.R << " " << (int)cr.G << " " << (int)cr.B << " " << (int)cr.A << std::endl;
    }
    return 0;
}
