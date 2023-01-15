
#include <ETH.h> //LIB padrão 

void setup()
{
  
  pinMode(0,OUTPUT);
  pinMode(1,OUTPUT);
  pinMode(3,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(15,OUTPUT);
  pinMode(32,OUTPUT);
  pinMode(33,OUTPUT);
  ETH.begin(1, 16,23,18,ETH_PHY_LAN8720,ETH_CLOCK_GPIO0_IN);
}

void loop()
{
  digitalWrite(0,HIGH);
  digitalWrite(1,HIGH);
  digitalWrite(3,HIGH);
  digitalWrite(5,HIGH);
  digitalWrite(17,HIGH);
  digitalWrite(32,HIGH);
  digitalWrite(33,HIGH);
  delay(1000);

  
  digitalWrite(0,LOW);
  digitalWrite(1,LOW);
  digitalWrite(3,LOW);
  digitalWrite(5,LOW);
  digitalWrite(17,LOW);
  digitalWrite(32,LOW);
  digitalWrite(33,LOW);
  delay(1000);
}
