#include "WiFi.h"
#include "AsyncUDP.h"

const char *ssid = "";
const char *password = "";
AsyncUDP udp;
IPAddress local_IP(192, 168, 1, 254);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 0, 0);
void (*client_disconnected)();

void comms_setup()
{
  WiFi.mode(WIFI_STA);
  if (!WiFi.config(local_IP, gateway, subnet))
  {
    Serial.println("STA Failed to configure");
  }
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED)
  {
    Serial.println("WiFi Failed");
    while (1)
    {
      delay(1000);
    }
  }
  if (udp.listen(1234))
  {

    Serial.print("UDP Listening on IP: ");
    Serial.println(WiFi.localIP());
    udp.onPacket(
        [](AsyncUDPPacket packet)
        {
          if (!connected)
          {
            udp.connect(packet.remoteIP(), packet.remotePort());
            connected = true;
          }
          else if (String((char *)packet.data()) == "leaving")
          {
            if(client_disconnected != nullptr) client_disconnected();
            connected = false;
          }
        });
  }
}
void send_freq(uint8_t freq, uint8_t db)
{
  udp.printf("%u,%u", freq, db);
}