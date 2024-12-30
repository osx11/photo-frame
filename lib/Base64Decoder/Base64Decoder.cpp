#include "Base64Decoder.h"


static const char *base64Chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

boolean isBase64(char c) {
    return isalnum(c) || c == '+' || c == '/';
}

int getBase64DecodedSize(char *str) {
    unsigned short equalitySymbolCount = 0;

    for (int i = strlen(str) - 1; i >= 0; i--) {
        if (str[i] == '=') equalitySymbolCount++;
        else break;
    }

    return static_cast<int>((strlen(str) - equalitySymbolCount) * 0.75);
}

void decodeBase64(char *encodedString, char *array) {
    int encStrLen = strlen(encodedString);
    int i = 0;
    int in = 0;

    char charArray4[4], charArray3[3];

    int arrayLastUsedIndex = 0;

    while (encStrLen-- && encodedString[in] != '=' && isBase64(encodedString[in])) {
        charArray4[i++] = encodedString[in];
        in++;

        if (i == 4) {
            for (i = 0; i < 4; i++)
                charArray4[i] = strchr(base64Chars, charArray4[i]) - base64Chars;

            charArray3[0] = (charArray4[0] << 2) + ((charArray4[1] & 0x30) >> 4);
            charArray3[1] = ((charArray4[1] & 0xf) << 4) + ((charArray4[2] & 0x3c) >> 2);
            charArray3[2] = ((charArray4[2] & 0x3) << 6) + charArray4[3];

            for (i = 0; i < 3; i++)
                array[arrayLastUsedIndex++] = charArray3[i];
            i = 0;
        }
    }

    if (i) {
        for (auto j = i; j < 4; j++)
            charArray4[j] = 0;

        for (auto j = 0; j < 4; j++)
            charArray4[j] = strchr(base64Chars, charArray4[i]) - base64Chars;

        charArray3[0] = (charArray4[0] << 2) + ((charArray4[1] & 0x30) >> 4);
        charArray3[1] = ((charArray4[1] & 0xf) << 4) + ((charArray4[2] & 0x3c) >> 2);
        charArray3[2] = ((charArray4[2] & 0x3) << 6) + charArray4[3];

        for (auto j = 0; j < i - 1; j++) array[arrayLastUsedIndex++] = charArray3[j];
    }
}
