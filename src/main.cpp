#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <EEPROM.h>
#include <ESP8266WiFi.h>;
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>

#include <ButtonController.h>
#include <Base64Decoder.h>
#include <BitmapDrawer.h>
#include <WiFiManager.h>

extern "C" {
#include "user_interface.h"
}

#define N_BPP 2

#define WIFI_CONNECTION_TIMEOUT 60000

char *ENCODED =
        "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\0";

#define BTN_PIN D1
#define LED_PIN D2

#define TFT_RST 2
#define TFT_CS 0
#define TFT_DC 15

ButtonController button = ButtonController(BTN_PIN);
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
WiFiManager wifiManager = WiFiManager(WIFI_SSID, WIFI_PASSWORD);
WiFiClientSecure wifiClient = WiFiClientSecure();

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
    char *tmp = new char[ENC_BUFF_SIZE+1];

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

void setup() {
    pinMode(BTN_PIN, INPUT_PULLUP);
    pinMode(LED_PIN, OUTPUT);

    Serial.begin(9600);
    EEPROM.begin(100);

    tft.initR(INITR_BLACKTAB);
    tft.setRotation(2);

    resetScreen();

    tft.write("Connecting to WiFi");

    // wifiManager.setOnAttempCallback([] {
    //     tft.write(".");
    //     delay(500);
    // });
    //
    // const auto status = wifiManager.tryAutoConnectAsStation(WIFI_CONNECTION_TIMEOUT);
    //
    // if (status != WL_CONNECTED) {
    //     resetScreen();
    //
    //     tft.write("Connection failed \n");
    //     tft.write("Code: ");
    //     tft.write(WiFiManager::convertWifiStatus(status));
    //
    //     return;
    // }

    resetScreen();
    tft.setTextSize(2);
    tft.print("Ready.");

    initSuccess = true;
    wifiClient.setInsecure();
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

String getResponse() {
    Serial.println("Connecting");

    const auto connected = wifiClient.connect(FB_API, 443);

    if (!connected) {
        Serial.println("Connection failed");
        return "";
    }

    String request = "GET " + String("/key/api/v1/models") + " HTTP/1.1\r\n";

    request += "Host: " + String(FB_API) + "\r\n";
    request += "Connection: close\r\n";
    request += "X-Key: Key " + String(FB_API_KEY) + "\r\n";
    request += "X-Secret: Secret " + String(FB_SECRET) + "\r\n";
    request += "\r\n";

    wifiClient.print(request);

    String jsonResponse = "";
    const auto buff = new char[1];

    // Read and print the response
    Serial.println("---------------------------- Response ------------------------");
    while (wifiClient.connected() || wifiClient.available()) {
        if (wifiClient.available()) {
            wifiClient.readBytes(buff, 1);

            if (buff[0] == '[' || buff[0] == ']') {
                while (buff[0] != '\n') {
                    Serial.print(buff[0]);
                    wifiClient.readBytes(buff, 1);
                }

                Serial.println();
            }
            // String line = wifiClient.readStringUntil('\n');
            // if (line.startsWith("[")) jsonResponse = line;
        }
    }

    Serial.println("-------------------------End Response--------------------------------");
    wifiClient.stop();

    free(buff);

    return jsonResponse;
}

void loop() {
    // if (!initSuccess) return;
    //
    // wifiManager.checkRealWifiStatus();
    //
    // if (!wifiManager.isConnected()) {
    //     resetScreen();
    //
    //     tft.write("WiFi connection lost.\n");
    //     tft.write("Reboot to reconnect.");
    //
    //     initSuccess = false;
    //
    //     return;
    // }

    button.doTick();

    button.onSingleClick([] {
        resetScreen();

        Serial.println("Free RAM before: " + String(system_get_free_heap_size()));
        delay(1000);

        decodeAndDrawJpg();

        Serial.println("Free RAM after: " + String(system_get_free_heap_size()));

        // const auto jsonString = getResponse();
        //
        // if (jsonString == "") {
        //     return;
        // }
        //
        // JsonDocument modelsDoc;
        // deserializeJson(modelsDoc, jsonString);
        //
        // const auto model = modelsDoc[0];
        // const char* name = model["name"];
        //
        // Serial.println(name);
    });
}
