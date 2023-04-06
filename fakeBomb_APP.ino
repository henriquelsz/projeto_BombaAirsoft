#include <LiquidCrystal.h>
#include <Keypad.h> //INCLUSÃO DE BIBLIOTECA
#include <Timer.h> //BIBLIOTECA PARA CONTROLE DE TEMPO
Timer counter;
Timer timerBeep;
const byte qtdLinhas = 4; //QUANTIDADE DE LINHAS DO TECLADO
const byte qtdColunas = 4; //QUANTIDADE DE COLUNAS DO TECLADO

//CONSTRUÇÃO DA MATRIZ DE CARACTERES
char matriz_teclas[qtdLinhas][qtdColunas] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
//Pinos do teclado
byte PinosqtdLinhas[qtdLinhas] = {30, 32, 34, 36}; //PINOS UTILIZADOS PELAS LINHAS
byte PinosqtdColunas[qtdColunas] = {31, 33, 35,37}; //PINOS UTILIZADOS PELAS COLUNAS

//INICIALIZAÇÃO DO TECLADO
Keypad teclado = Keypad( makeKeymap(matriz_teclas), PinosqtdLinhas, PinosqtdColunas, qtdLinhas, qtdColunas);

// Inicializa o LCD
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);


//Variaveis do sistema
bool Ativado;
bool SenhaConfigurada;
bool TempoConfigurado;
bool setHora;
bool setMinutos;
bool setSegundos;
bool detonada;
String Senha;
String SenhaConfirmacao;
String TempoTexto;
String TempoTextoFormatado;
String SenhaTexto;
String Password;
int Horas = 0;
int Minutos = 0;
int Segundos = 0;
int QuantidadeSenhaInvalida = 5;
double intelavoAntigo = 1000;

void setup()
{
  Serial.begin(9600); //INICIALIZA A SERIAL
  SplachScreen();
  
  lcd.begin(20, 4); // Define o LCD com 20 colunas e 4 linhas
  Animacao();
  pinMode(10, OUTPUT); //BUZZER
  pinMode(12, OUTPUT); //LED VERMELHO
  pinMode(13, OUTPUT); //LED VERDE
  Reset();
}

void loop()
{
  //Monitora as teclas pressionadas
  KeyboardPress();
  //Caso a bomba estaja ativada ou detonada ela vai para o metodo boom
  if (Ativado || detonada)
    Contador();
  //Usado para gerenciar o beep
  if (Ativado && !detonada)
    timerBeep.decrement(millis());

}

void Boom () {
  if (detonada) {
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("      GAME OVER!      ");
    delay(10);
    detonada = false;
  }

  digitalWrite(10, HIGH);
  digitalWrite(12, HIGH);

}
void GerenciarBeep(double secMillisAntigo) {
  //Verifica o tempo decorrido padrão e muda a velocidade do tempo do beep de acordo com o tempo
  int tempototal = (counter.hours * 60) + (counter.minutes * 60) + counter.seconds;

  if (tempototal > 30) // Se for maior que 30 segundos ele faz o beep de 1 e 1 segundo
    timerBeep.interval = 1000;
  else if (tempototal > 10)// Se for maior que 10 segundos e menor que 30 ele faz o beep de 1/2 em 1/2 segundo
    timerBeep.interval  = 500;
  else
    timerBeep.interval  = 300;// Se for menor que 10 segundos ele faz o beep de a cada 300 miles segundos

  //Usei o While para calcular o tempo sem usar um delay() pois o mesmo trava o processo do arduino, com isso eu simulo uma thread separadade processo nao travando o sistema
  while ((millis() - timerBeep.secMillis) > timerBeep.interval) {
    timerBeep.secMillis = millis();
    digitalWrite(10, HIGH);
    digitalWrite(12, HIGH);
    delay(50);
    digitalWrite(10, LOW);
    digitalWrite(12, LOW);
  }


}
void Contador() {
  //Se a bomba não tiver sido detonada ele executa o decremento do tempo e exibe no display
  if (!detonada) {
    if (counter.decrement(millis()))
    {
      DisplayCabecalho();
      detonada = true;
      Ativado = false;

    }
    else
    {
      //Caso a senha digitada seja igual a senha de confirmação ele dearma a bomba e reseta o sistema
      if (SenhaConfirmacao == Senha) {
        Reset();
      } else {
        lcd.setCursor(0, 1);
        lcd.print("TEMPO: ");
        if (counter.hours < 10)
          lcd.print("0");
        lcd.print(counter.hours);
        lcd.print(":");
        if (counter.minutes < 10)
          lcd.print("0");
        lcd.print(counter.minutes);
        lcd.print(":");
        if (counter.seconds < 10)
          lcd.print("0");
        lcd.print(counter.seconds);
        GerenciarBeep(timerBeep.secMillis);
      }
    }
  }
  else
  {
    Boom();
  }
}
void DisplayCabecalho() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("  BOMBA AIRSOFT");
  if (TempoConfigurado) {
    lcd.setCursor(0, 1);
    lcd.print("TEMPO: ");
    lcd.print(TempoTextoFormatado);
    lcd.setCursor(0, 2);
    lcd.print("DIGITE A SENHA! ");
  }
  if (SenhaConfigurada) {
    lcd.setCursor(0, 1);
    lcd.print("TEMPO: ");
    lcd.print(TempoTextoFormatado);
    lcd.setCursor(0, 2);
    lcd.print("     ARMADO     ");
  }

}
void DefinirTempo(char tecla) {
  if (tecla != '*') {
    FormatHora(tecla);
    Serial.print(tecla);
  }
}
void DefinirSenha(char tecla) {
  if (tecla == '#') {
    Senha = SenhaTexto;
    SenhaConfigurada = true;
    DisplayCabecalho();
    Ativado = true;
    digitalWrite(10, HIGH);
    delay(200);
    digitalWrite(10, LOW);
    delay(200);
    digitalWrite(10, HIGH);
    delay(200);
    digitalWrite(10, LOW);
    TempoTextoFormatado = "";
  }
  else if (tecla != '*') {
    SenhaTexto += tecla;
    Serial.print(tecla);
    lcd.setCursor(0, 3);
    Password += '*';
    lcd.print(Password);

  }
}
void Reset() {
  Senha = "";
  SenhaTexto = "";
  Password = "";
  QuantidadeSenhaInvalida = 5;
  counter.interval = 1000;
  timerBeep.interval = 1000;
  TempoTextoFormatado = "";
  SenhaConfirmacao = "";
  detonada = false;
  digitalWrite(10, HIGH);
  digitalWrite(12, HIGH);
  digitalWrite(13, HIGH);
  delay(200);
  digitalWrite(10, LOW);
  digitalWrite(12, LOW);
  digitalWrite(13, LOW);
  pinMode(10, OUTPUT); //BUZZER
  pinMode(12, OUTPUT); //LED VERMELHO
  pinMode(13, OUTPUT); //LED VERDE
  Ativado = false;
  SenhaConfigurada = false;
  TempoConfigurado = false;
  setHora = false;
  setMinutos = false;
  setSegundos = false;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("BOMBA AIRSOFT");
  lcd.setCursor(0, 1);
  lcd.print("DIGITE O TEMPO! ");
  lcd.setCursor(0, 2);
  lcd.print("00:00:00");
}
void FormatHora(char tecla) {
  //Se a hora ainda nao foi definida ele entra para definir a hora, depois os minutos e por fim os segundos validando se os mesmos estão corretos
  if (!setHora) {
    if (TempoTexto.length() < 2) {
      TempoTextoFormatado += tecla;
      TempoTexto += tecla;
      lcd.setCursor(0, 3);
      lcd.print("                ");
      lcd.setCursor(0, 3);
      lcd.print("HORAS");
    }
    if (TempoTexto.length() == 2) {
      TempoTextoFormatado += ":";
      Horas = TempoTexto.toInt();
      TempoTexto = "";
      if (Horas <= 24) {
        setHora = true;
        lcd.setCursor(0, 3);
        lcd.print("MINUTOS");
      }
      else {
        lcd.setCursor(0, 2);
        lcd.print(TempoTextoFormatado);
        lcd.setCursor(0, 3);
        lcd.print("HORA INVALIDA! ");
        TempoTextoFormatado = "";
      }
    }
  } else if (!setMinutos) {
    if (TempoTexto.length() < 2) {
      TempoTextoFormatado += tecla;
      TempoTexto += tecla;
      lcd.setCursor(0, 3);
      lcd.print("                ");
      lcd.setCursor(0, 3);
      lcd.print("MINUTOS");
    }
    if (TempoTexto.length() == 2) {
      TempoTextoFormatado += ":";
      Minutos = TempoTexto.toInt();
      TempoTexto = "";
      if (Minutos <= 59) {
        setMinutos = true;
        lcd.setCursor(0, 3);
        lcd.print("SEGUNDOS");
      }
      else {
        lcd.setCursor(0, 2);
        lcd.print(TempoTextoFormatado);
        lcd.setCursor(0, 3);
        lcd.print("MIN. INVALIDO! ");
        if (Horas < 10)
          TempoTextoFormatado = "0";
        TempoTextoFormatado += Horas;
        TempoTextoFormatado += ":";
      }
    }
  } else if (!setSegundos) {
    if (TempoTexto.length() < 2) {
      TempoTextoFormatado += tecla;
      TempoTexto += tecla;
      lcd.setCursor(0, 3);
      lcd.print("                ");
      lcd.setCursor(0, 3);
      lcd.print("SEGUNDOS");
    }
    if (TempoTexto.length() == 2) {
      lcd.setCursor(0, 2);
      lcd.print(TempoTextoFormatado);
      Segundos = TempoTexto.toInt();
      TempoTexto = "";
      if (Segundos <= 59)
      {
        setSegundos = true;

        counter.setTime(Horas, Minutos, Segundos);
        timerBeep.setTime(Horas, Minutos, Segundos);
        TempoConfigurado = true;
        digitalWrite(10, HIGH);
        digitalWrite(12, HIGH);
        delay(200);
        digitalWrite(10, LOW);
        digitalWrite(12, LOW);
        delay(200);
        digitalWrite(10, HIGH);
        digitalWrite(12, HIGH);
        delay(200);
        digitalWrite(10, LOW);
        digitalWrite(12, HIGH);
        DisplayCabecalho();
      }
      else {
        lcd.setCursor(0, 2);
        lcd.print(TempoTextoFormatado);
        lcd.setCursor(0, 3);
        lcd.print("SEG. INVALIDO! ");
        TempoTextoFormatado = "";
        if (Horas < 10)
          TempoTextoFormatado = "0";
        TempoTextoFormatado += Horas;
        TempoTextoFormatado += ":";
        if (Minutos < 10)
          TempoTextoFormatado += "0";
        TempoTextoFormatado += Minutos;
        TempoTextoFormatado += ":";
      }


    }
  }
  if (!TempoConfigurado) {
    lcd.setCursor(0, 2);
    lcd.print(TempoTextoFormatado);
  }
}
void KeyboardPress() {
  char tecla_pressionada = teclado.getKey(); //VERIFICA SE ALGUMA DAS TECLAS FOI PRESSIONADA

  if (tecla_pressionada) { //SE ALGUMA TECLA FOR PRESSIONADA, FAZ
    digitalWrite(10, HIGH);
    delay(200);
    digitalWrite(10, LOW);
    if (!Ativado && tecla_pressionada == '*')
    {
      Reset();
      return;
    } else if (Ativado) {
      SenhaConfirmacao += tecla_pressionada;
      lcd.setCursor(0, 3);
      lcd.print("                ");
      lcd.setCursor(0, 3);
      lcd.print(SenhaConfirmacao);
      if (SenhaConfirmacao == Senha) {
        lcd.clear();
        lcd.setCursor(0, 1);
          lcd.print("  BOMBA DESATIVADA");
        digitalWrite(13, HIGH);
        delay(3000);
        Reset();
      } else if (SenhaConfirmacao.length() > Senha.length()) {
        SenhaConfirmacao = "";
        QuantidadeSenhaInvalida--;
        lcd.setCursor(0, 3);
        lcd.print(QuantidadeSenhaInvalida);
        lcd.print(" SENHA INVALIDA");
        counter.interval  -= 150;
        if (QuantidadeSenhaInvalida <= 0)
        {
          detonada = true;
          Ativado = false;
        }
      }
    } else if (!TempoConfigurado) {
      DefinirTempo(tecla_pressionada);
    } else if (!SenhaConfigurada) {
      DefinirSenha(tecla_pressionada);
    }
  }
}
void Animacao(){
  lcd.setCursor(0, 0);
  lcd.print("!!AIRSOFT BOMB!!");
  for(int i=0; i<3; i++){
    lcd.setCursor(8, 2); //aponta para coluna 0, linha 0
    lcd.write(i); //escreve lcd.write(1); lcd.write(2); lcd.write(3);
    delay(800);
}
}
void SplachScreen() {
  byte bomb01[8] = {
    B00010,
    B00100,
    B00100,
    B01110,
    B11111,
    B11111,
    B11111,
    B01110
  };
  lcd.createChar(0, bomb01);
  byte bomb02[8] = {
    B00000,
    B00100,
    B00100,
    B01110,
    B11111,
    B11111,
    B11111,
    B01110
  };
  lcd.createChar(1, bomb02);
  byte bomb03[8] = {
    B00000,
    B00000,
    B00100,
    B01110,
    B11111,
    B11111,
    B11111,
    B01110
  };
  lcd.createChar(2, bomb03);
  byte bomb04[8] = {
    B00000,
    B00000,
    B00000,
    B01110,
    B11111,
    B11111,
    B11111,
    B01110
  };
  lcd.createChar(3, bomb04);
}
