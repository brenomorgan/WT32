#include <ETH.h> //LIB padrão 
#include <Keypad.h>
char Teclas[4][4]={ {'1','2','3','A'}, {'4','5','6','B'}, {'7','8','9','C'}, {'*','0','#','D'} };
byte Pin_Linhas[4]={32,33,5,17};
byte Pin_Colunas[4]={14,12,4,2};
Keypad teclado = Keypad(makeKeymap(Teclas), Pin_Linhas ,Pin_Colunas, 4, 4);

String senha="159#";
String digitada;
void setup() {
  ETH.begin(1, 16,23,18,ETH_PHY_LAN8720,ETH_CLOCK_GPIO0_IN);
  Serial.begin(115200);
}

void loop() {
  char tecla_press= teclado.getKey();
  if(tecla_press !=NULL){
    digitada+=tecla_press;
    Serial.println(digitada);
    if(tecla_press=='#')
    {
      if(senha.compareTo(digitada)==0)
    {
      Serial.println("Senhara Correta");
    }
    else
    {
      Serial.println("Senhara Incorreta");
    }
    digitada="";
    }
    if(tecla_press=='*')
    {
      digitada="";
    }
  }
}
