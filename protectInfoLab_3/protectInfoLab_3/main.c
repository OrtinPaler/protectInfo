/*
 * Based on Wikipedia article:
 * https://ru.wikipedia.org/wiki/BMP
 */

#if defined(_WIN32) && defined(_WIN64)
#include <io.h>
#elif defined(__APPLE__) || defined(__linux__)
#include <unistd.h>
#endif

#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

char *pathTask = "/Users/ortin/Universe/6_semester/protectinfo/protectInfoLab_3/task/1.bmp";
char *pathSend = "/Users/ortin/Universe/6_semester/protectinfo/protectInfoLab_3/protectInfoLab_3/send.txt";
char *pathRecv = "/Users/ortin/Universe/6_semester/protectinfo/protectInfoLab_3/protectInfoLab_3/recv.txt";
char *pathPict = "/Users/ortin/Universe/6_semester/protectinfo/protectInfoLab_3/protectInfoLab_3/update.bmp";

typedef unsigned char BYTE, *PBYTE, *LPBYTE;
typedef unsigned long DWORD, *PDWORD, *LPDWORD;
typedef unsigned short WORD, *PWORD, *LPWORD;
typedef long LONG, *PLONG, *LPLONG;

typedef struct tagRGBQUAD {
  BYTE rgbBlue;
  BYTE rgbGreen;
  BYTE rgbRed;
  BYTE rgbReserved;
} RGBQUAD;

typedef struct tagBITMAPFILEHEADER {
  WORD  bfType;
  DWORD bfSize;
  WORD  bfReserved1;
  WORD  bfReserved2;
  DWORD bfOffBits;
} BITMAPFILEHEADER, *LPBITMAPFILEHEADER, *PBITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER {
  DWORD biSize;
  LONG  biWidth;
  LONG  biHeight;
  WORD  biPlanes;
  WORD  biBitCount;
  DWORD biCompression;
  DWORD biSizeImage;
  LONG  biXPelsPerMeter;
  LONG  biYPelsPerMeter;
  DWORD biClrUsed;
  DWORD biClrImportant;
} BITMAPINFOHEADER, *LPBITMAPINFOHEADER, *PBITMAPINFOHEADER;

void hideByteIntoPixel(RGBQUAD *pixel, uint8_t hide_byte) {
    pixel->rgbBlue &= (0xFC);
    pixel->rgbBlue |= (hide_byte >> 6) & 0x3;
    pixel->rgbGreen &= (0xFC);
    pixel->rgbGreen |= (hide_byte >> 4) & 0x3;
    pixel->rgbRed &= (0xFC);
    pixel->rgbRed |= (hide_byte >> 2) & 0x3;
    pixel->rgbReserved &= (0xFC);
    pixel->rgbReserved |= (hide_byte) & 0x3;
}

uint8_t getByteFromPixel(RGBQUAD *pixel) {
    uint8_t hide_byte;
    
    hide_byte = pixel->rgbBlue & 0x3;
    hide_byte = hide_byte << 2;
    hide_byte |= pixel->rgbGreen & 0x3;
    hide_byte = hide_byte << 2;
    hide_byte |= pixel->rgbRed & 0x3;
    hide_byte = hide_byte << 2;
    hide_byte |= pixel->rgbReserved & 0x3;
    
    return hide_byte;
}

void hiding(void) {
    int fdTask, fdSend, fdPict;
    LONG width, height, size;
    BITMAPFILEHEADER bmp;
    BITMAPINFOHEADER inf;
    RGBQUAD rgb;
    uint8_t byteToHide;
    
    if ((fdTask = open(pathTask, O_RDONLY)) == -1) {
        printf("Error open fdTask\n");
        exit(1);
    }
    
    read(fdTask, &bmp, sizeof(BITMAPFILEHEADER));
    read(fdTask, &inf, sizeof(BITMAPINFOHEADER));
    
    width = inf.biWidth;
    height = inf.biHeight;
    
    if (!inf.biSizeImage)
        size = (width * 3 + width % 4) * height;
    else
        size = inf.biSizeImage;
    
    if ((fdSend = open(pathSend, O_RDONLY)) == -1) {
        printf("Error open fdSend\n");
        exit(1);
    }

    if ((fdPict = open(pathPict, O_WRONLY)) == -1) {
        printf("Error open fdPict\n");
        exit(1);
    }

    write(fdPict, &bmp, sizeof(BITMAPFILEHEADER));
    write(fdPict, &inf, sizeof(BITMAPINFOHEADER));
    
    for (int i = 0; i < 1000000; i++) {
        if (!read(fdSend, &byteToHide, 1))
            byteToHide = 0xFF;
        read(fdTask, &rgb, sizeof(rgb));
        hideByteIntoPixel(&rgb, byteToHide);
        write(fdPict, &rgb, sizeof(rgb));
    }
    
    if (close(fdSend)) {
        printf("Error close fdSend\n");
        exit(1);
    }

    if (close(fdPict)) {
        printf("Error close fdPict\n");
        exit(1);
    }
    
    if (close(fdTask)) {
        printf("Error close fdTask\n");
        exit(1);
    }
    
    puts("Information hiding completed");
}

void extraction(void) {
    int fdRecv, fdPict;
    BITMAPFILEHEADER bmp;
    BITMAPINFOHEADER inf;
    RGBQUAD rgb;
    uint8_t byteToHide = 0x00;
    
    if ((fdPict = open(pathPict, O_RDONLY)) == -1) {
        printf("Error open fdPict\n");
        exit(1);
    }
    
    read(fdPict, &bmp, sizeof(BITMAPFILEHEADER));
    read(fdPict, &inf, sizeof(BITMAPINFOHEADER));
    
    if ((fdRecv = open(pathRecv, O_WRONLY)) == -1) {
        printf("Error open fdSend\n");
        exit(1);
    }
    
    while (byteToHide != 0xFF) {
        read(fdPict, &rgb, sizeof(rgb));
        byteToHide = getByteFromPixel(&rgb);

        if (byteToHide != 0xFF)
            write(fdRecv, &byteToHide, sizeof(byteToHide));
    }
    
    if (close(fdRecv)) {
        printf("Error close fdRecv\n");
        exit(1);
    }
    
    if (close(fdPict)) {
        printf("Error close fdPict\n");
        exit(1);
    }

    puts("Information extraction completed");
}

int main() {
    puts("Program start");
    hiding();
    extraction();
    return 0;
}
