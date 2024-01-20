
/*
 * touch-CustomSPI.ino
 *
 *  Created on: 20.02.2016
 *  Updated 2024-01-20 by marcelrv
 *
 * required librarys:
 *  - SPI (arduino core)
 *  - XPT2046 (https://github.com/Links2004/XPT2046)
 */

#include <Arduino.h>
#include <SPI.h>
#include <XPT2046.h>

#define XPT2046_IRQ 36
#define XPT2046_MOSI 32
#define XPT2046_MISO 39
#define XPT2046_CLK 25
#define XPT2046_CS 33

// Note: the ESP32 has 2 SPI ports, to have ESP32-2432S028R work with the TFT and Touch on different SPI ports each needs to be defined and passed to the library
SPIClass vspi = SPIClass(VSPI);

XPT2046 touch = XPT2046(XPT2046_CS, XPT2046_IRQ, vspi);

void touchEvent(bool press, uint16_t x, uint16_t y, uint16_t z)
{
    Serial.print("[Touch] press: ");
    Serial.print(press);
    Serial.print(" X: ");
    Serial.print(x);
    Serial.print(" Y: ");
    Serial.print(y);
    Serial.print(" Z: ");
    Serial.println(z);
}

void setup()
{

    Serial.begin(115200);
    Serial.println("[setup] Start...");

    // Setup the custom SPI port pins, or just use the default pins with vspi.begin();
    vspi.begin(XPT2046_CLK, XPT2046_MISO, XPT2046_MOSI, XPT2046_CS);

    touch.begin(320, 240);                      // screen resolution
    touch.setRotation(1);                       // match the cords to screen
    touch.setCalibration(350, 550, 3550, 3600); // Calibrat screen

    // call back for change
    // parameter:
    // 1 - minimum pixel move for new event
    // 2 - minimum z for a event
    // 3 - callback
    touch.onChange(3, 200, touchEvent);

    Serial.println("[setup] Done.");
}

long ts = 0;
void loop()
{
    touch.loop();

    if (ts + 5000 < millis()) // Show I'm still alive and kicking
    {
        printf(" ts: %d\n", millis());
        ts = millis();
    }
}