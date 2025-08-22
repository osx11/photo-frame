#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7796S.h>
#include <EEPROM.h>
#include <ESP8266WiFi.h>;
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>

#include <ButtonController.h>
#include <Base64Decoder.h>
#include <BitmapDrawer.h>
#include <ESPAsyncWebServer.h>
#include <WiFiManager.h>
#include <HttpClient.h>
#include <FixedSizeList.h>
#include <FusionBrainApi.h>

extern "C" {
#include "user_interface.h"
}

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 160

#define WIFI_CONNECTION_TIMEOUT 60000

char *ENCODED =
        "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\0";

#define BTN_PIN D1
#define LED_PIN D2

#define TFT_RST -1
#define TFT_CS 15
#define TFT_DC 2

ButtonController button = ButtonController(BTN_PIN);
Adafruit_ST7796S tft = Adafruit_ST7796S(TFT_CS, TFT_DC, TFT_RST);
WiFiManager wifiManager = WiFiManager(WIFI_SSID, WIFI_PASSWORD);
WiFiClientSecure wifiClient = WiFiClientSecure();
HttpClient httpClient = HttpClient(&wifiClient);
FusionBrainApi fbApi = FusionBrainApi(&httpClient, FB_API, FB_API_KEY, FB_SECRET);

boolean initSuccess = false;

#define ENC_BUFF_SIZE 684
#define DEC_BUFF_SIZE 513 // = ENC_BUFF_SIZE * 0.75

char *decodedCurr;
constexpr auto terminator = '\0';

size_t posCurr = 0;
size_t lastByte = 0;

void readBytes(size_t bytes, char *dest) {
    for (auto i = 0; i < bytes; i++) {
        if (ENCODED[i + lastByte] == terminator) return;
        dest[i] = ENCODED[i + lastByte];
    }

    lastByte += bytes;
}

void fillDecodedFull() {
    char *tmp = new char[ENC_BUFF_SIZE + 1];

    readBytes(684, tmp);
    tmp[ENC_BUFF_SIZE] = '\0'; // otherwise strlen won't work as expected

    decodeBase64(tmp, decodedCurr);

    delete tmp;
}

size_t in_func(
    JDEC *jd,
    uint8_t *buff,
    size_t nbyte
) {
    if (!buff) {
        // delete nByte from the decoded data.

        Serial.println("Delete requested");
        memset(decodedCurr + posCurr, NULL, nbyte);
        posCurr += nbyte;

        return nbyte;
    }

    // means no data in decodedCurr
    if (posCurr == 0 && decodedCurr[0] == NULL) {
        fillDecodedFull();
    }

    // it asked to give everything we have or less (normal scenario)
    if (posCurr + nbyte < DEC_BUFF_SIZE) {
        memcpy(buff, decodedCurr + posCurr, nbyte);
        memset(decodedCurr + posCurr, NULL, nbyte);

        // if we gave all the available data, set posCurr to 0, otherwise operate as normal
        posCurr = posCurr + nbyte < DEC_BUFF_SIZE - 1 ? posCurr + nbyte : 0;

        return nbyte;
    }

    // it asked to give more than we have (overflow scenario)
    if (posCurr + nbyte >= DEC_BUFF_SIZE) {
        const auto numOfLastTakenBytes = DEC_BUFF_SIZE - posCurr;
        memcpy(buff, decodedCurr + posCurr, numOfLastTakenBytes);

        // re-fill current array
        fillDecodedFull();

        // give remaining bytes
        memcpy(buff + numOfLastTakenBytes, decodedCurr, nbyte - numOfLastTakenBytes);
        posCurr = nbyte - numOfLastTakenBytes;

        return nbyte;
    }

    return 0;
}

void decodeAndDrawJpg() {
    decodedCurr = new char[DEC_BUFF_SIZE];


    posCurr = 0;
    lastByte = 0;
    drawBitmap(&tft, in_func);
    Serial.println("DONE!");
    delete decodedCurr;
}

struct Settings {
    char wifiUser[20];
    char wifiPassword[20];
};

void resetScreen() {
    tft.fillScreen(ST77XX_BLACK);
    tft.setTextColor(ST77XX_WHITE);
    tft.setCursor(0, 0);
    tft.setTextSize(1);
}

void readEEPROMAndPrint() {
    Serial.println("Reading EEPROM...");

    Settings readSettings{};
    EEPROM.get(0, readSettings);

    Serial.print("Username: ");
    Serial.print(readSettings.wifiUser);
    Serial.print(" Password: ");
    Serial.println(readSettings.wifiPassword);
}

void writeEEPROM() {
    Serial.println("Writing EEPROM...");

    Settings settings{};

    strcpy(settings.wifiUser, "osx11");
    strcpy(settings.wifiPassword, "superpass");

    EEPROM.put(0, settings);
    EEPROM.commit();

    Serial.println("EEPROM written");
}

void setup() {
    pinMode(BTN_PIN, INPUT_PULLUP);
    pinMode(LED_PIN, OUTPUT);

    Serial.begin(9600);
    EEPROM.begin(100);

    tft.init(320, 480, 0, 0, ST7796S_BGR);
    tft.setRotation(2);
    tft.invertDisplay(true); // hack for ST7796S (probably bug in a library?)

    resetScreen();

    tft.write("Connecting to WiFi");

    wifiManager.setOnAttempCallback([] {
        tft.write(".");
        delay(500);
    });

    const auto status = wifiManager.tryAutoConnectAsStation(WIFI_CONNECTION_TIMEOUT);

    if (status != WL_CONNECTED) {
        resetScreen();

        tft.write("Connection failed \n");
        tft.write("Code: ");
        tft.write(WiFiManager::convertWifiStatus(status));

        return;
    }

    resetScreen();
    tft.setTextSize(2);
    tft.print("Ready.");

    initSuccess = true;
    wifiClient.setInsecure();
}

void loop() {
    if (!initSuccess) return;

    wifiManager.checkRealWifiStatus();

    if (!wifiManager.isConnected()) {
        resetScreen();

        tft.write("WiFi connection lost.\n");
        tft.write("Reboot to reconnect.");

        initSuccess = false;

        return;
    }

    button.doTick();

    button.onSingleClick([] {
        digitalWrite(LED_PIN, HIGH);
        delay(500);
        digitalWrite(LED_PIN, LOW);
        decodeAndDrawJpg();
    });

    // button.onSingleClick([] {
    //     digitalWrite(LED_PIN, HIGH);
    //
    //     const auto models = fbApi.getModels();
    //
    //     if (models == nullptr) Serial.println("An error has occurred during model fetch");
    //     else {
    //         for (auto i = 0; i < models->size(); i++) {
    //             const auto model = models->get(i);
    //             if (model.type != "TEXT2IMAGE") continue;
    //             Serial.println("Available T2I Model name: " + String(model.name) + " (id=" + String(model.id) + ")");
    //         }
    //     }
    //
    //     delete models;
    //
    //     digitalWrite(LED_PIN, LOW);
    //
    //     resetScreen();
    //
    //     Serial.println("Free RAM before: " + String(system_get_free_heap_size()));
    //     delay(1000);
    //
    //     decodeAndDrawJpg();
    //
    //     Serial.println("Free RAM after: " + String(system_get_free_heap_size()));
    //
    //     const auto jsonString = getResponse();
    //
    //     if (jsonString == "") {
    //         return;
    //     }
    //
    //     JsonDocument modelsDoc;
    //     deserializeJson(modelsDoc, jsonString);
    //
    //     const auto model = modelsDoc[0];
    //     const char* name = model["name"];
    //
    //     Serial.println(name);
    // });
}
