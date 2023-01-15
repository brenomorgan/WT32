#include <ETH.h> //LIB padrão 

static bool eth_connected = false;

void WiFiEvent(WiFiEvent_t event)
{
  Serial.println("WiFiEvent");
  switch (event) {
    case ARDUINO_EVENT_ETH_START:
      Serial.println("ETH Started");
      //set eth hostname here
      ETH.setHostname("esp32-ethernet");
      break;
    case ARDUINO_EVENT_ETH_CONNECTED:
      Serial.println("ETH Connected");
      break;
    case ARDUINO_EVENT_ETH_GOT_IP:
      Serial.print("ETH MAC: ");
      Serial.print(ETH.macAddress());
      Serial.print(", IPv4: ");
      Serial.print(ETH.localIP());
      if (ETH.fullDuplex()) {
        Serial.print(", FULL_DUPLEX");
      }
      Serial.print(", ");
      Serial.print(ETH.linkSpeed());
      Serial.println("Mbps");
      eth_connected = true;
      break;
    case ARDUINO_EVENT_ETH_DISCONNECTED:
      Serial.println("ETH Disconnected");
      eth_connected = false;
      break;
    case ARDUINO_EVENT_ETH_STOP:
      Serial.println("ETH Stopped");
      eth_connected = false;
      break;
    default:
      break;
  }
}

void testClient(const char * host, uint16_t port)
{
  Serial.print("\nconnecting to ");
  Serial.println(host);

  WiFiClient client;
  if (!client.connect(host, port)) {
    Serial.println("connection failed");
    return;
  }
  client.printf("GET / HTTP/1.1\r\nHost: %s\r\n\r\n", host);
  while (client.connected() && !client.available());
  while (client.available()) {
    Serial.write(client.read());
  }

  Serial.println("closing connection\n");
  client.stop();
}

void setup()
{
  delay(3000);
  Serial.begin(115200);
  Serial.println("INICIANDO");
  pinMode(17,OUTPUT);
  WiFi.onEvent(WiFiEvent);

  //ETH.begin(ETH_PHY_ADDR, ETH_PHY_POWER (PIN), ETH_PHY_MDC (PIN), ETH_PHY_MDIO (PIN), ETH_PHY_TYPE, ETH_CLK_MODE);
  ETH.begin(1, 16,23,18,ETH_PHY_LAN8720,ETH_CLOCK_GPIO0_IN);
  Serial.println("CONFIGURADO");
}

void loop()
{
  if (eth_connected) {
    testClient("google.com", 80);
    Serial.println("ETH Conectado");
    digitalWrite(17,HIGH);
    delay(1000);
    digitalWrite(17,LOW);
    
  }
  else
  {
    Serial.println("LOOP");
  }

  delay(4000);
}
