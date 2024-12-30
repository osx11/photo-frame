#include "ButtonController.h"

ButtonController::ButtonController(uint8_t gpio) {
    _gpio = gpio;
};

void ButtonController::onSingleClick(void (*callback)()) {
    _onSingleClickCallback = callback;
}

void ButtonController::onDoubleClick(void (*callback)()) {
    _onDoubleClickCallback = callback;
}

void ButtonController::onHold(void (*callback)()) {
    _onHoldCallback = callback;
}

constexpr unsigned long holdTimeout = 500;
constexpr unsigned long doubleClickTimeout = 250;
constexpr unsigned long debounceTimeout = 5;

uint8_t getDebouncedButtonState(const uint8_t gpio) {
    static unsigned long debounceTime = 0;
    static uint8_t finalButtonState = HIGH;
    static uint8_t prevButtonState = HIGH;
    const uint8_t currentButtonState = digitalRead(gpio);

    if (currentButtonState != prevButtonState) {
        debounceTime = millis();
    }

    if (millis() - debounceTime > debounceTimeout) {
        finalButtonState = currentButtonState;
    }

    prevButtonState = currentButtonState;

    return finalButtonState;
}

void ButtonController::doTick() {
    const uint8_t realButtonPressed = !getDebouncedButtonState(_gpio);

    if (realButtonPressed && !_justPressed) {
        _justPressed = true;
        _pressedAt = millis();
        _justDbClick = false;
    }

    if (!realButtonPressed && _justPressed) {
        _justPressed = false;
        _justReleased = true;
        _lastReleasedAt = millis();
    }

    const auto pressedWithinDoubleClickWindow = _pressedAt - _lastReleasedAt <= doubleClickTimeout;
    const auto releasedAfterDoubleClick = _justReleased && millis() - _lastReleasedAt > 300;
    const auto holdingNow = millis() - _pressedAt >= holdTimeout;

    // Pressed again within bounds for double click
    if (_justPressed && pressedWithinDoubleClickWindow && !_dbClickAvailable && !_justDbClick) {
        _dbClickAvailable = true;
    }

    if (releasedAfterDoubleClick) {
        if (!_justHold && !_justDbClick) {
            // single click
            if (!_dbClickAvailable) {
                if (_onSingleClickCallback) _onSingleClickCallback();
            }

            // double click
            if (_dbClickAvailable) {
                if (_onDoubleClickCallback) _onDoubleClickCallback();
                _justDbClick = true;
            }
        } else if (_justDbClick) _justDbClick = false;

        _dbClickAvailable = false;
        _justReleased = false;
        _justHold = false;
    }

    // Set result. Pressed and fit into hold bounds
    if (holdingNow && _justPressed && !_justHold) {
        _justHold = true;
        if (!_justDbClick && _onHoldCallback) _onHoldCallback();
    }
};
