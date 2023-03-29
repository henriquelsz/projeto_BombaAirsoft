#include <LiquidCrystal.h>
#include <Keypad.h> //INCLUSÃO DE BIBLIOTECA
 
const byte qtdLinhas = 4; //QUANTIDADE DE LINHAS DO TECLADO
const byte qtdColunas = 4; //QUANTIDADE DE COLUNAS DO TECLADO
 
//CONSTRUÇÃO DA MATRIZ DE CARACTERES
char matriz_teclas[qtdLinhas][qtdColunas] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
 
 
byte PinosqtdLinhas[qtdLinhas] = {30, 32, 34, 36}; //PINOS UTILIZADOS PELAS LINHAS
byte PinosqtdColunas[qtdColunas] = {31, 33, 35,37}; //PINOS UTILIZADOS PELAS COLUNAS
 
//INICIALIZAÇÃO DO TECLADO
Keypad meuteclado = Keypad( makeKeymap(matriz_teclas), PinosqtdLinhas, PinosqtdColunas, qtdLinhas, qtdColunas); 

// Inicializa o LCD
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);
char senha[5] ={'1','2','3','4'}; 
int conta = 0; //Variável que varre o vetor da senha de índice
char senha_confirma[5] = {}; //Verifica se cada dígito condiz com a senha
void setup()
{
  // Define o LCD com 20 colunas e 4 linhas
  lcd.begin(20, 4);
  
  // Mostra informacoes no display
  lcd.setCursor(3,0);
  lcd.print("MAS E O PT, HEIN?");
  lcd.setCursor(2,1);
  lcd.print("E O LULA");
  lcd.setCursor(1,3);
  lcd.print("KABUUMM");

  Serial.begin(9600); //INICIALIZA A SERIAL
  Serial.println("Aperte uma tecla..."); //IMPRIME O TEXTO NO MONITOR SERIAL
  Serial.println(); //QUEBRA UMA LINHA NO MONITOR SERIAL
  pinMode(10, OUTPUT); //BUZZER
}

void loop()
{
   
    char tecla_pressionada = meuteclado.getKey(); //VERIFICA SE ALGUMA DAS TECLAS FOI PRESSIONADA
    if (tecla_pressionada){ //SE ALGUMA TECLA FOR PRESSIONADA, FAZ
      Serial.print("Tecla pressionada : "); //IMPRIME O TEXTO NO MONITOR SERIAL
      Serial.println(tecla_pressionada); //IMPRIME NO MONITOR SERIAL A TECLA PRESSIONADA
      Serial.println(conta);
      for(int k=0;k < 4;k++){
        senha_confirma[k] = tecla_pressionada; //VERIFICA SE ALGUMA DAS TECLAS FOI PRESSIONADA
        digitalWrite(10, HIGH);
        delay(1000);
        digitalWrite(10, LOW);
        delay(100); 
        Serial.println(senha);
        Serial.println(senha_confirma);
        Serial.println(k);
        delay(1000);
      } 
    
    
    Serial.print(senha_confirma);     
    if(senha_confirma == senha){
      digitalWrite(10, HIGH);

      
      }
}
 

  
