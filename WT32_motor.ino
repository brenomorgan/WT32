#include <ETH.h> //LIB padrão 
const int stepPin = 4;
const int dirPin = 2;

void setup () {
pinMode (stepPin, OUTPUT);
pinMode (dirPin, OUTPUT);
ETH.begin(1, 16,23,18,ETH_PHY_LAN8720,ETH_CLOCK_GPIO0_IN);
digitalWrite(dirPin,LOW);
}

void loop() {
//for(int x = 0; x < 200; x++){
digitalWrite(stepPin,HIGH);
delay(5);
digitalWrite(stepPin,LOW);
delay(5);
//}
}
