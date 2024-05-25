/*
    LilyGo Ink Screen Series u8g2Fonts Test
        - Created by Lewis he
        - Modified by Pályi Lőrinc
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

GxIO_Class io(SPI, EPD_CS, EPD_DC, EPD_RSET);
GxEPD_Class display(io, EPD_RSET, EPD_BUSY);
U8G2_FOR_ADAFRUIT_GFX u8Display;

int16_t displayWidth = 0;
int16_t displayHeight = 0;
int8_t lineHeight = 0;

void displayHackathonLogo();

void setupFont(U8G2_FOR_ADAFRUIT_GFX& u8Display, const uint8_t* font) {
    u8Display.setFontMode(1);       // use u8g2 transparent mode (this is default)
    u8Display.setFontDirection(0);  // left to right (this is default)
    u8Display.setForegroundColor(GxEPD_BLACK);
    u8Display.setBackgroundColor(GxEPD_WHITE);
    u8Display.setFont(font);
}

void setup(void) {
    Serial.begin(115200);  // enable diagnostic output on Serial
    Serial.println();
    Serial.println("setup");

    SPI.begin(EPD_SCLK, EPD_MISO, EPD_MOSI);
    display.init();
    u8Display.begin(display);

    Serial.println("setup done");
    displayHackathonLogo();
    display.setRotation(1);
    displayWidth = display.width();
    displayHeight = display.height();
    setupFont(u8Display, u8g2_font_helvB10_te);
    lineHeight = u8Display.getFontAscent() + abs(u8Display.getFontDescent());
    Serial.print("Ascent: ");
    Serial.print(u8Display.getFontAscent());
    Serial.print("Descent: ");
    Serial.print(u8Display.getFontDescent());
    Serial.print("Line: ");
    Serial.println(lineHeight);

    display.fillRoundRect(5, 5, displayWidth - 10, displayHeight - 10, 10, GxEPD_BLACK);
    display.fillRoundRect(8, 8, displayWidth - 16, displayHeight - 16, 10, GxEPD_WHITE);
    u8Display.setCursor(displayWidth / 4, displayHeight / 2);
    u8Display.println("Helló, Szent József");
    u8Display.setCursor(displayWidth / 4, displayHeight / 2 + lineHeight);
    u8Display.println(" Hackathon v2.0");
    display.update();
}

void loop() {
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
