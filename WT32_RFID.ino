#include <ETH.h> //LIB padrão 
#include <Arduino.h>
#include <rdm6300.h>

#define RDM6300_RX_PIN 4 // read the SoftwareSerial doc above! may need to change this pin to 10...
#define READ_LED_PIN 13

Rdm6300 rdm6300;

void setup()
{
  Serial.begin(115200);

  pinMode(READ_LED_PIN, OUTPUT);
  digitalWrite(READ_LED_PIN, LOW);

  rdm6300.begin(RDM6300_RX_PIN);
  ETH.begin(1, 16,23,18,ETH_PHY_LAN8720,ETH_CLOCK_GPIO0_IN);

  Serial.println("\nPlace RFID tag near the rdm6300...");
}

void loop()
{
  int cartao=digitalRead(READ_LED_PIN);
  /* get_new_tag_id returns the tag_id of a "new" near tag,
  following calls will return 0 as long as the same tag is kept near. */
  if (rdm6300.get_new_tag_id())
  {
    Serial.println(rdm6300.get_tag_id(), HEX);
    Serial.println("VALOR : ");
    Serial.println(rdm6300.get_tag_id());
    Serial.println(" , ");
    Serial.println(cartao);
    Serial.println(" , dentro ");
    Serial.println(rdm6300.get_new_tag_id());
  digitalWrite(READ_LED_PIN, rdm6300.get_tag_id());
  }
  delay(10);
}
