#pragma once
#include <Arduino.h>

#ifndef BUTTONCONTROLLER_H
#define BUTTONCONTROLLER_H

class ButtonController {
public:
    explicit ButtonController(uint8_t gpio);

    void onSingleClick(void (*callback)());

    void onDoubleClick(void (*callback)());

    void onHold(void (*callback)());

    void doTick();

private:
    uint8_t _gpio = 0;

    boolean _justPressed = false;
    // pure press flag
    boolean _justReleased = false;
    // pure release flag
    boolean _justHold = false;
    // used internally to prevent single/double click event at the same time with hold
    boolean _justDbClick = false;
    // double click was trigerred and event sent. prevents extra emit of same event, and prevents holding.
    boolean _dbClickAvailable = false;
    // indicates that time between last release and next press less than double click available window

    unsigned long _pressedAt = 0; // pure press millis
    unsigned long _lastReleasedAt = 0; // pure release millis

    void (*_onSingleClickCallback)() = nullptr;

    void (*_onDoubleClickCallback)() = nullptr;

    void (*_onHoldCallback)() = nullptr;
};

#endif //BUTTONCONTROLLER_H
