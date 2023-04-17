//Site: https://www.callmebot.com/blog/free-api-whatsapp-messages/

// Ãrea de InclusÃ£o de arquivos.

#include <Arduino.h>                                                    // Para usar a framework do Arduino
#include <WiFi.h>                                                       // Para acesso ao wifi
#include <HTTPClient.h>                                                 // Para criar um cliente HTTP
#include <UrlEncode.h>                                                  // Para enviar a mensagem via post deve ser no formato URL
#include <ETH.h>

//-----------------------------------------------------------------------------------------------------------------------------------------
// DefiniÃ§Ãµes de Hardware

#define Led1    15                                                      // Led1 ligado ao GPIO 15

//-----------------------------------------------------------------------------------------------------------------------------------------
// ProtÃ³tipos de funÃ§Ã£o

void WhatsAppMessages(String message);                                  // FunÃ§Ã£o que envia uma mensagem para o whatsapp

//-----------------------------------------------------------------------------------------------------------------------------------------
// Dados da sua WiFi.

const char* SSID = "ifce-alunos";                           // Nome da rede wifi. Identificador da rede
const char* PASSWORD = "ifce4lun0s";                                // Senha da rede wifi. Sua senha do Wifi

//-----------------------------------------------------------------------------------------------------------------------------------------
// Dados do celular e chave do whatsapp

bool flag = 0;                                                          // Flag para uso na logica do sensor
String phoneNumber = "+558598556306";                                  // Meu nÃºmero de celular com formato internacional
String apiKey = "1435743";                                               // A chave que o bot do whatsapp enviou

//-----------------------------------------------------------------------------------------------------------------------------------------

void setup() 
{
    pinMode(Led1, OUTPUT);                                              // Configura pino do led1 como saida
    Serial.begin(115200);                                               // Inicia a serial com 115200 bps
    WiFi.begin(SSID, PASSWORD);                                         // Inicia o wifi com o nome da rede e a senha
    Serial.println("Conectando");                                       // Envia pela serial que esta conectando

    while(WiFi.status() != WL_CONNECTED)                                // Aguarda a conexÃ£o
    {
        delay(500);                                                     // Espera 500ms
        Serial.print(".");                                              // Vai escrevendo ...
    }
    Serial.println("");                                                 // Pula uma linha
    Serial.print("Conectado no IP: ");                                  // Mostra que esta conectado no IP
    Serial.println(WiFi.localIP());                                     // Mostra o IP no qual foi conectado

    WhatsAppMessages("Teste WhatsApp Bot !");                    // Envia mensagem para o bot do whatsapp
}

//-----------------------------------------------------------------------------------------------------------------------------------------

void loop() 
{
  delay(300000); 
  WhatsAppMessages("Teste WhatsApp Bot !");
}

//-----------------------------------------------------------------------------------------------------------------------------------------
// DefiniÃ§Ã£o das funÃ§Ãµes

void WhatsAppMessages(String message)
{
    // Dados a serem enviados pelo mÃ©todo Post suportado pelo protocolo HTTP
    String url = "https://api.callmebot.com/whatsapp.php?phone=" + phoneNumber + "&apikey=" + apiKey + "&text=" + urlEncode(message);    
    
    HTTPClient http;                                                            // Cria um objeto HTTP
    http.begin(url);                                                            // Inicia o objeto HTTP passando a string acima
    
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");        // Adiciona um cabeÃ§alho
  
    uint16_t httpResponseCode = http.POST(url);                                 // Envia o mÃ©todo POST HTTP de requisiÃ§Ã£o e lÃª o resultado
  
    if (httpResponseCode == 200)                                                // Deu tudo certo?
    {
        digitalWrite(Led1, HIGH);                                               // Liga o led1
        Serial.print("Mensagem enviada com sucesso!");                          // Envia mensagem na serial que deu certo
        delay(1000);                                                            // Espera 1s
        digitalWrite(Led1, LOW);                                                // Desliga o led1
    }
    else                                                                        // Caso contrÃ¡rio, se nÃ£o foi possivel enviar a mensagem
    {
        Serial.println("Erro ao tentar enviar a mensagem!");                    // Envia para a serial a mensagem de erro
        Serial.print("CÃ³digo HTTP: ");                                          // Vai enviar para a serial o cÃ³digo de erro que retornou
        Serial.println(httpResponseCode);                                       // Envia o cÃ³digo de erro
    }
    http.end();                                                                 // Finaliza o objeto http
}
