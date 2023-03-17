#if 0
#include "ble.h"

#endif
bool connected = false;

#include "screen.h"

#if 1
#include "commswifi.h"
#endif

#if 0
#include "blSerial.h"
#endif

void on_client_disconnected()
{
  screen_disconnected();
}

void on_change(uint8_t freq, uint8_t db)
{
  send_freq(freq, db);
}
void setup()
{
  Serial.begin(115200);
  comms_setup();
  readFrequencies();
  screen_init();
  setEvent(on_change);
  client_disconnected = &on_client_disconnected;
}
void loop()
{
  screen_loop();
}

void readFrequencies()
{
  for (int i = 0; i <= sizeof(freqs); i++)
  {
    freqs[i] = 120;
  }
}
