#include <ETH.h> //LIB padrão 
#include <WiFi.h>
#include <HTTPClient.h>
#include "string.h"
#include <Keypad.h>

static bool eth_connected = false;

char Teclas[4][4]={ {'1','2','3','A'}, {'4','5','6','B'}, {'7','8','9','C'}, {'*','0','#','D'} };
byte Pin_Linhas[4]={32,33,5,17};
byte Pin_Colunas[4]={14,12,4,2};
Keypad teclado = Keypad(makeKeymap(Teclas), Pin_Linhas ,Pin_Colunas, 4, 4);
char tecla_press= teclado.getKey();
String digitada;

//const char* ssid     = "ifce-alunos"; // nome da rede
//const char* password = "ifce4lun0s"; // senha
String GScriptId = "AKfycbxZreK1sLQB8ocwEE19tMXmrGs3jnx5e84OkiFdA9JSkpsmzB4-1be7mZZYNbCK6vDK"; // id do script
                    
String segredo = "valor1"; // segredo da tabela para retirar item

const int sendInterval = 50;

const int stepPin = 15;
//const int dirPin = 33; //Pino direcional vai diretamento no VCC ou GND dependendo do sentido de giro
const int TCRT = 36;
//const int TCRT1 = 25;
//const int TCRT2 = 26;
//const int led = 2;
const int botao1 = 35;
static int pressionado = 0;
static int liberado = 0;
static int libera_2 = 0;
int Code=0;
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
  //pinMode(led, OUTPUT);
  pinMode(botao1, INPUT);
  pinMode (stepPin, OUTPUT);
//pinMode (dirPin, OUTPUT);
pinMode (TCRT, INPUT);
  //conectar_wifi();
  //digitalWrite(dirPin,HIGH);
}

void loop() {
  Teclado();
  if(libera_2 > 0)
  {
    lerBotao();
  }
  if (liberado > 0) {
      if (eth_connected) {
    Serial.println("ETH Conectado");
  }
  else
  {
    Serial.println("ETH Desconectado");
  }
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
  Code=httpCode;
  String resposta;
  if (httpCode > 0) {
     resposta = http.getString();
     Serial.print("Resposta: ");
     Serial.println(resposta);
  }
  // AQUI
  if( (resposta=="sem creditos") || Code!=200)
  {
    pressionado = 1;
    liberado = 0; 
    libera_2=0;
    digitada="";
  }
  else if(Code==200)
  {
    Serial.println("Motor gira");
  while(pressionado==-1)
  
  { 
      //digitalWrite(led, HIGH);
      
          //if((digitalRead(TCRT1)==LOW)||(digitalRead(TCRT2)==LOW))
          if((digitalRead(TCRT)==LOW))
          {
            Serial.println("Absorvente Caiu");
            pressionado = 1;
              liberado = 0;
              libera_2=0;
              digitada="";
              
          } 
          digitalWrite(stepPin,HIGH);
          delay(15);
          digitalWrite(stepPin,LOW);
          delay(15);  
  } 
  }
  http.end();

 }

void lerBotao(void) {
  int estadoBotao1 = digitalRead(botao1);  
  if ( estadoBotao1 == HIGH )
  {
    pressionado=-1;
  }
  if ( estadoBotao1 == LOW )
  {
      if (pressionado < 0) 
      {
        liberado = 1;        
      }
      //digitalWrite(led, LOW); 
  }
   delay(250);
}



void Teclado(void) {
   char tecla_press= teclado.getKey();
  if(tecla_press !=NULL){
    if(tecla_press=='#')
    {
     /* if(senha.compareTo(digitada)==0)
    {
      Serial.println("Senhara Correta");
    }
    else
    {
      Serial.println("Senhara Incorreta");
    }
    */
    Serial.println(digitada);
    Serial.println(libera_2);
    segredo=digitada;
    Serial.println(segredo);
    libera_2=1;
    Serial.println(libera_2);
    }
    digitada+=tecla_press;
    Serial.println(digitada);
    if(tecla_press=='*')
    {
      digitada="";
    }
  }
}
