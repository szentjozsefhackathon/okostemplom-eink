/**
 * @file ostyamerleg_client.ino
 * @author Pályi Lőrinc (palyilorinc33@gmail.com)
 * @brief MQTT kliens az ostyamérleghez, a darabszám időszakos frissítésére
 * @date 2024-05-25
 *
 */

#define LILYGO_T5_V213

#include <GxEPD.h>
// GxEPD.h must be included first
#include <GxDEPG0213BN/GxDEPG0213BN.h>  // 2.13" b/w  form DKE GROUP
#include <boards.h>

#include GxEPD_BitmapExamples
#include <Adafruit_GFX.h>
#include <GxIO/GxIO.h>
#include <GxIO/GxIO_SPI/GxIO_SPI.h>
#include <U8g2_for_Adafruit_GFX.h>

const int32_t uS_TO_S_FACTOR = 1000000;
uint64_t timeToSleep = 10;  // in seconds

GxIO_Class io(SPI, EPD_CS, EPD_DC, EPD_RSET);
GxEPD_Class display(io, EPD_RSET, EPD_BUSY);
U8G2_FOR_ADAFRUIT_GFX u8Display;

RTC_DATA_ATTR int16_t displayWidth = 0;
RTC_DATA_ATTR int16_t displayHeight = 0;
RTC_DATA_ATTR int8_t lineHeight = 0;

RTC_DATA_ATTR bool afterReset = true;
RTC_DATA_ATTR uint8_t counter = 0;

void displayHackathonLogo();

void setupFont(U8G2_FOR_ADAFRUIT_GFX& u8Display, const uint8_t* font);


void onWakeUp();
void sendToDeepSleep();

void setup() {
    if (afterReset) {
        afterReset = false;
        // enable diagnostic output on Serial
        Serial.begin(115200);
        Serial.println();

        // display setup
        SPI.begin(EPD_SCLK, EPD_MISO, EPD_MOSI);
        display.init();
        u8Display.begin(display);

        displayHackathonLogo();

        display.setRotation(1);
        displayWidth = display.width();
        displayHeight = display.height();
        setupFont(u8Display, u8g2_font_helvB10_te);

        // rounded rectangle border for screen with wider stroke
        display.fillRoundRect(3, 3, displayWidth - 6, displayHeight - 6, 10, GxEPD_BLACK);
        display.fillRoundRect(6, 6, displayWidth - 12, displayHeight - 12, 10, GxEPD_WHITE);
        u8Display.setCursor(10, display.height()/2);
        u8Display.println("Szinkronizálás folyamatban...");
        display.update();
        display.powerDown();

        // writeTimeOnScreen();

        delay(2000);

        sendToDeepSleep();
    } else {
        onWakeUp();
        sendToDeepSleep();
    }
}

void loop() {
    Serial.print("Counter: ");
    Serial.println(counter);
    delay(2000);
}

void setupFont(U8G2_FOR_ADAFRUIT_GFX& u8Display, const uint8_t* font) {
    u8Display.setFontMode(1);       // use u8g2 transparent mode (this is default)
    u8Display.setFontDirection(0);  // left to right (this is default)
    u8Display.setForegroundColor(GxEPD_BLACK);
    u8Display.setBackgroundColor(GxEPD_WHITE);
    u8Display.setFont(font);
}

/**
 * @brief Called in setup() when ESP32 is woken up by RTC timer
 *
 */
void onWakeUp() {
    counter++;
    Serial.begin(115200);
    Serial.println();
    Serial.print("Counter: ");
    Serial.println(counter);

    // display setup
    SPI.begin(EPD_SCLK, EPD_MISO, EPD_MOSI);
    display.init();
    u8Display.begin(display);
    display.setRotation(1);
    writeTimeOnScreen();
}

void sendToDeepSleep() {
    esp_sleep_enable_timer_wakeup(timeToSleep * uS_TO_S_FACTOR);
    esp_deep_sleep_start();
}

void writeTimeOnScreen() {
    setupFont(u8Display, u8g2_font_helvB10_te);
    display.fillRoundRect(6, 6, displayWidth - 12, displayHeight - 12, 10, GxEPD_WHITE);
    u8Display.setCursor(10, display.height() / 2);
    u8Display.print("Counter: ");
    u8Display.println(counter);
    display.update();
    display.powerDown();
}

void displayHackathonLogo() {
    display.eraseDisplay();
    setupFont(u8Display, u8g2_font_t0_11b_mn);
    display.setRotation(1);
    auto displayWidth = display.width();
    auto displayHeight = display.height();
    auto lineWidth = u8Display.getUTF8Width("00011000");
    auto lineHeight = u8Display.getFontAscent();
    auto textHeight = 8 * lineHeight;
    int16_t cursorY = (displayHeight - textHeight) / 2;
    u8Display.setCursor((displayWidth - lineWidth) / 2, cursorY);
    u8Display.println("00011000");
    cursorY += lineHeight;
    u8Display.setCursor((displayWidth - lineWidth) / 2, cursorY);
    u8Display.println("00011000");
    cursorY += lineHeight;
    u8Display.setCursor((displayWidth - lineWidth) / 2, cursorY);
    u8Display.println("11111111");
    cursorY += lineHeight;
    u8Display.setCursor((displayWidth - lineWidth) / 2, cursorY);
    u8Display.println("00011000");
    cursorY += lineHeight;
    u8Display.setCursor((displayWidth - lineWidth) / 2, cursorY);
    u8Display.println("00011000");
    cursorY += lineHeight;
    u8Display.setCursor((displayWidth - lineWidth) / 2, cursorY);
    u8Display.println("00011000");
    display.update();
}