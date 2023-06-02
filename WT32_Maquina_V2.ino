#include <ETH.h> //LIB padrão 
#include <WiFi.h>
#include <HTTPClient.h>
#include "string.h"
#include <Arduino.h>
#include <rdm6300.h>


#define RDM6300_RX_PIN 4 // read the SoftwareSerial doc above! may need to change this pin to 10...
Rdm6300 rdm6300;

static bool eth_connected = false;



//const char* ssid     = "*****"; // nome da rede
//const char* password = "*****"; // senha
String GScriptId = "AKfycbxZreK1sLQB8ocwEE19tMXmrGs3jnx5e84OkiFdA9JSkpsmzB4-1be7mZZYNbCK6vDK"; // id do script
                    
String segredo = "valor1"; // segredo da tabela para retirar item



const int sendInterval = 50;
const int LED_RFID = 12;
const int stepPin = 15;
//const int dirPin = 33; //Pino direcional vai diretamento no VCC ou GND dependendo do sentido de giro
const int TCRT = 36;
//const int TCRT1 = 25;
//const int TCRT2 = 26;
//const int led = 2;
//const int botao1 = 35;
static int pressionado = 0;
static int liberado = 0;
static int libera_2 = 0;

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


/*
WiFiClientSecure client;
void conectar_wifi(void) {
  Serial.print("Conectando ao wifi: ");
  Serial.println(ssid);
  Serial.flush();
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
  
    delay(500);
    Serial.print(WiFi.status());
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.println("IP: ");
  Serial.println(WiFi.localIP());
}
*/

void setup() {

     delay(3000);
  Serial.begin(115200);
  Serial.println("INICIANDO");
  
  WiFi.onEvent(WiFiEvent);

  //ETH.begin(ETH_PHY_ADDR, ETH_PHY_POWER (PIN), ETH_PHY_MDC (PIN), ETH_PHY_MDIO (PIN), ETH_PHY_TYPE, ETH_CLK_MODE);
  ETH.begin(1, 16,23,18,ETH_PHY_LAN8720,ETH_CLOCK_GPIO0_IN);
  Serial.println("CONFIGURADO");
  rdm6300.begin(RDM6300_RX_PIN);
  pinMode (LED_RFID, OUTPUT);
  pinMode (stepPin, OUTPUT);
//pinMode (dirPin, OUTPUT);
pinMode (TCRT, INPUT);
digitalWrite(stepPin,LOW);
  //conectar_wifi();
}

void loop() {
    
    if (rdm6300.get_new_tag_id())
    {
      digitalWrite(LED_RFID,HIGH);
      delay(1000);
      digitalWrite(LED_RFID,LOW);
        //Serial.println(rdm6300.get_tag_id());
        segredo=rdm6300.get_tag_id();
         
        if (eth_connected) {
          Serial.println("ETH Conectado");
        }
        else
        {
          Serial.println("ETH Desconectado");
        }
         Serial.println(segredo);
         libera_2=1;
        pressionado=-1;
        enviar_google_sheet(); 
    } 
}

void enviar_google_sheet(void) {
  
  HTTPClient http;
  String url = "https://script.google.com/macros/s/" + GScriptId + "/exec?id=" + segredo;
  Serial.println(url);
  Serial.println("Atualizando planilha...");
  http.begin(url.c_str());
  http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
  int httpCode = http.GET(); // status code 200 representa sucesso de requisicao
  Serial.print("Status Code: ");
  Serial.println(httpCode);

  String resposta;
   if (httpCode < 0)
   {
      Serial.print("Falha na conecção");
   }
  if (httpCode > 0) {
     resposta = http.getString();

  if(resposta=="sem creditos")
  { 
    Serial.println("Você não possui mais créditos.");
    pressionado = 1;
    liberado = 0; 
    libera_2=0;
    segredo="";
  }
  else if( resposta=="not found")
  {
    Serial.println("Seus cartão não esta cadastrado");
        pressionado = 1;
    liberado = 0; 
    libera_2=0;
    segredo="";
  }
  else
  {
  while(pressionado==-1)
  {        
          if((digitalRead(TCRT)==LOW))
          {
            pressionado = 1;
              liberado = 0;
              libera_2=0;
              segredo="";
              Serial.print("Créditos restantes: ");
              Serial.println(resposta);
          } 
          digitalWrite(stepPin,HIGH);
          delay(15);
          digitalWrite(stepPin,LOW);
          delay(15); 
  }
  
  }  
  }
  
  http.end();

 }
