#include <ETH.h> //LIB padrão 
void setup()
{
  Serial.begin(115200);
  pinMode(35,INPUT);
  ETH.begin(1, 16,23,18,ETH_PHY_LAN8720,ETH_CLOCK_GPIO0_IN);
}

void loop(){

  if(digitalRead(35)==1){
     Serial.println("Nenhum objeto detectado ");
     delay(500);
  }
  else{
     Serial.println("Objeto detectado ");
     delay(500);
  }
}
