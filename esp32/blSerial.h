#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;


void print_debug_num(char n)
{
    tft.fillRect(0, 0, 20, 20, TFT_BLACK);
    tft.setCursor(1, 1);
    tft.print(n);
}

void bl_setup()
{
    SerialBT.begin("EqualizerAPOUI"); // Bluetooth device name
    Serial.println("The device started, now you can pair it with bluetooth!");
}
bool connected = false;
void bl_loop()
{
    if (!connected && SerialBT.connected())
    {
        delay(1000);
        print_debug_num('1');
        SerialBT.println("ok");
        connected = true;
    }
    else if (connected && !SerialBT.connected())
    {
        connected = false;
        print_debug_num('0');
    }
    else if (!connected)
    {
        
    }
}

void send_freq(uint8_t freq, int8_t db)
{
    SerialBT.write(freq);
    SerialBT.write((uint8_t)db);
}