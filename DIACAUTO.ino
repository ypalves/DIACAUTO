#include <SoftwareSerial.h> //Declara a biblioteca SoftwareSerial

#define SIM800_TX_PIN 10 //TX do SIM800L conectado ao pino 10

#define SIM800_RX_PIN 11 //RX do SIM800L conectado ao pino 11

SoftwareSerial serialSIM800(SIM800_TX_PIN, SIM800_RX_PIN); // Cria um objeto software serial para se comunicar com o SIM800L

int pinBUZ1 = 2; //Definição do pino conectado a buzina
int pinLAM1 = 3; //Definição do pino conectado a lâmpada
int pinPIR1 = 4; //Definição do pino conectado ao sensor PIR 1
int pinPIR2 = 13; //Definição do pino conectado ao sensor PIR 2
int pinPIR3 = 12; //Definição do pino conectado ao sensor PIR 3
int IN1 = 5; //Definição do pino conectado a entrada IN1 da Ponte H
int IN2 = 6; //Definição do pino conectado a entrada IN2 da Ponte H
int cont = 0; //Declaração de variável para o contador de tempo
int buz = 0;  //Declaração de variável para controlar a buzina
int aux = 0;  //Variável auxiliar de controle

void setup()
{
 pinMode(IN1, OUTPUT); //Define o pino como saída
 pinMode(IN2, OUTPUT); //Define o pino como saída
 pinMode(pinPIR1, INPUT); //Define o pino como entrada
 pinMode(pinPIR2, INPUT); //Define o pino como entrada
 pinMode(pinPIR3, INPUT); //Define o pino como entrada
 pinMode(pinBUZ1, OUTPUT); //Define o pino como saída
 digitalWrite(pinBUZ1, HIGH); //Garante que o sistema vai iniciar com a buzina desligada
 pinMode(pinLAM1, OUTPUT); //Define o pino como saída
 digitalWrite(pinLAM1, HIGH); //Garante que o sistema vai iniciar com a lâmpada desligada
 Serial.begin(9600); //Inicia o monitor serial
 while (!Serial); //Espera que a conexão serial seja estabelecida

 //Comunicação com o módulo SIM800L
 serialSIM800.begin(4800);
 delay(1000);
}
 
void loop()
{
  bool valorPir1 = digitalRead(pinPIR1); //Atribui o estado do sensor 1 a uma variável
  bool valorPir2 = digitalRead(pinPIR2); //Atribui o estado do sensor 2 a uma variável
  bool valorPir3 = digitalRead(pinPIR3); //Atribui o estado do sensor 3 a uma variável
  
  if(cont < 15) //Se o tempo for menor que 3 minutos faz (Para a apresentação foi fixado 15 segundos)
  {
    Serial.println(cont); //Mostra a contagem de tempo no monitor serial
      delay(1000); //Espera 1 segundo
      cont ++; //Atribui 1 segundo ao contador
    if(valorPir1) //Se o estado do sensor 1 for verdadeiro faz
      cont = 0; //Zera o contador
  }
  
  if(cont >= 15) //Se o tempo for maior que 3 minutos * faz
  {    
    if(valorPir2 || valorPir3) //Se o estado do sensor 2 ou do sensor 3 for verdadeiro faz
    {
      aux = aux + 1; //atribui + 1 a variável toda vez que um dos sensores for ativado
    }
    
    if(aux == 1) //Se a variável for igual a 1 faz
    {
      //Garante que o vidro está fechado
      //Gira o motor no sentido horário
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      delay(300);
      //Para o motor
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, HIGH);
      delay(1000);
      
      //Abre o vidro
      //Gira o motor no sentido anti-horário
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      delay(300);
      //Para o motor
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, HIGH);
      delay(300);

      //Acende a lâmpada do farol
      digitalWrite(pinLAM1, LOW);
      delay(1000);

      buz = buz + 1; //Atribui 1 a variável para que a buzina seja acionada e as mensagens sejam enviadas
      aux = aux + 1; //Atribui + 1 a variável para que não entre novamente na condição atual
     }

     if(buz > 0) //Se a variável for maior que 0 faz
     {
      //Aciona a buzina
      digitalWrite(pinBUZ1, LOW);
      delay(50);
      digitalWrite(pinBUZ1, HIGH);
      delay(50);
   
      serialSIM800.write("AT+CMGF=1\r\n"); //Seleciona o modo de mensagem de texto SMS
      delay(1000);
 
      serialSIM800.write("AT+CMGS=\"999999999999\"\r\n"); //Define o número a serem enviadas as mensagens
      delay(1000);
   
      serialSIM800.write("Retorne ao seu veiculo!"); //Define o conteúdo da mensagem
      delay(1000);
   
      serialSIM800.write((char)26); //Define o formato da mensagem
      delay(1000);
      
      delay(8000); //Espera 8 segundos até acionar a buzina e enviar a mensagem novamente
     }
   }
}
