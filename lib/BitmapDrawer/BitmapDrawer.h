#pragma once

#include <Arduino.h>
#include <Adafruit_ST7735.h>

extern "C" {
#include "tjpgd.h"
}

typedef struct {
    char *arr; /* Input stream */
    Adafruit_ST7735 *tft;
} IODEV;


#ifndef BITMAPDRAWER_H
#define BITMAPDRAWER_H

void drawBitmap(Adafruit_ST7735 *tft, char *bitmap);

#endif //BITMAPDRAWER_H
