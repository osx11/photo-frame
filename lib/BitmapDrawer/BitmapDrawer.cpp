#include "BitmapDrawer.h"

unsigned long long pos = 0;
unsigned long long totalRequested = 0;

size_t in_func(
    JDEC *jd,
    uint8_t *buff,
    size_t nbyte
) {
    IODEV *dev = (IODEV *) jd->device;

    if (buff) {
        memcpy(buff, dev->arr + pos, nbyte);
    } else {
        Serial.println("Delete request");
    }

    Serial.print("Requested ");
    Serial.print(nbyte);
    Serial.println(" Bytes");

    pos += nbyte;
    totalRequested += nbyte;

    return nbyte;
}

int out_func(JDEC *jd, void *bitmap, JRECT *rect) {
    // Serial.println("Output!");
    IODEV *dev = (IODEV *) jd->device;

    const auto x = rect->left;
    const auto y = rect->top;
    const auto w = rect->right - x + 1;
    const auto h = rect->bottom - y + 1;

    dev->tft->drawRGBBitmap(x, y, (uint16_t *) bitmap, w, h);

    return 1;
}


void drawBitmap(Adafruit_ST7735 *tft, char *bitmap) {
    pos = 0;
    totalRequested = 0;

    Serial.print("GO ");
    Serial.println(strlen(bitmap));

    JDEC jdec;
    size_t sz_work = 3500;

    IODEV devid;

    void *work = malloc(sz_work);

    devid.arr = bitmap;
    devid.tft = tft;

    JRESULT res = jd_prepare(&jdec, in_func, work, sz_work, &devid);

    if (res == JDR_OK) {
        // tft->fillScreen(ST77XX_BLACK);
        jd_decomp(&jdec, out_func, 1);
    }

    Serial.print("TOTAL REQUESTED: ");
    Serial.println(totalRequested);

    free(work);
}
