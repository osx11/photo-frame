#pragma once

#include <Arduino.h>
#include <Adafruit_ST7735.h>

extern "C" {
#include "tjpgd.h"
}

typedef struct {
    // char *arr;
    Adafruit_ST7735 *tft;
} IODEV;


#ifndef BITMAPDRAWER_H
#define BITMAPDRAWER_H

void drawBitmap(Adafruit_ST7735 *tft, size_t (*inFunc)(JDEC *jd, uint8_t *buff, size_t nbyte));

#endif //BITMAPDRAWER_H
