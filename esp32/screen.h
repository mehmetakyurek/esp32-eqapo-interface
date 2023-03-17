#include <TFT_eSPI.h>
#include <SPI.h>
#include <TFT_Touch.h>

TFT_eSPI tft = TFT_eSPI();

// These are the pins used to interface between the 2046 touch controller and Arduino Pro
#define DOUT 39 /* Data out pin (T_DO) of touch screen */
#define DIN 32  /* Data in pin (T_DIN) of touch screen */
#define DCS 33  /* Chip select pin (T_CS) of touch screen */
#define DCLK 25 /* Clock pin (T_CLK) of touch screen */

/* Create an instance of the touch screen library */
TFT_Touch touch = TFT_Touch(DCS, DCLK, DIN, DOUT);

void (*event)(uint8_t freq, uint8_t db) = nullptr;
void (*screen_loop)();

uint8_t freqs[100];
float multiplier;
int X_RawData;
int Y_RawData;
int X_Coord;
int Y_Coord;
uint8_t freq_index;

void print_waiting()
{
    tft.fillScreen(TFT_BLACK);
    tft.setTextDatum(4);
    tft.drawCentreString("Waiting for connection...", 160, 100, 4);
}
void line_init()
{
    for (int i = 0; i <= sizeof(freqs) - 2; i++)
    {
        tft.drawLine((i) * (multiplier), freqs[i], (i + 1) * (multiplier), freqs[i + 1], TFT_WHITE);
    }
}
void update_line(uint8_t start, uint8_t newFreq)
{
    if (freqs[start - 1] != newFreq)
    {
        freqs[start - 1] = newFreq;
        tft.fillRect((start - 1) * multiplier, 0, 3 * multiplier, TFT_WIDTH, TFT_BLACK);
        if (start > 1)
            tft.drawLine((start - 1) * (multiplier), freqs[start - 2], (start) * (multiplier), freqs[start - 1], TFT_WHITE);
        if (start < 100)
            tft.drawLine((start) * (multiplier), freqs[start - 1], (start + 1) * (multiplier), freqs[start], TFT_WHITE);
        if (start < 99)
            tft.drawLine((start + 1) * (multiplier), freqs[start], (start + 2) * (multiplier), freqs[start + 1], TFT_WHITE);
    }
}
void setEvent(void (*e)(uint8_t freq, uint8_t db))
{
    event = e;
}

void loop_screen()
{
    if (touch.Pressed())
    {
        X_Coord = touch.X();
        Y_Coord = touch.Y();
        Serial.println(X_Coord);
        freq_index = (X_Coord / multiplier);

        if (freq_index >= 1 && freq_index <= 100)
        {
            update_line(freq_index, Y_Coord);
            event(freq_index, freqs[freq_index - 1]);
        }
    }
}

void wait_for_connection()
{
    if (connected)
    {
        tft.fillScreen(TFT_BLACK);
        line_init();
        screen_loop = &loop_screen;
    }
}
void screen_disconnected()
{
    print_waiting();
    screen_loop = &wait_for_connection;
}
void screen_init()
{
    screen_loop = &wait_for_connection;
    tft.init();
    multiplier = ((float)TFT_HEIGHT / (float)sizeof(freqs));
    // This is the calibration line produced by the TFT_Touch_Calibrate_v2 sketch
    touch.setCal(526, 3443, 750, 3377, 320, 240, 1);

    // Set the TFT and touch screen to landscape orientation
    tft.setRotation(1);
    touch.setRotation(1);

    tft.setTextSize(1);
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE);
    print_waiting();
}
