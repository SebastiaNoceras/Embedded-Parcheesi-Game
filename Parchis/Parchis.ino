
/**
  @file Parchis
  @author Sebastià A. Noceras Anderson
  @date 27/04/2020
  @brief Proyecto final, versión simple del juego de Parchis.
 */
 
#include "libreriaParchis.h";
#include "LCD.h"
#include "Keypad.h"
#include <Ethernet2.h>
#include <PubSubClient.h>
#include <stdio.h>
 
/**************************************/
/* MACROS                             */
/**************************************/
#define KEYPAD_COL0_PIN 25
#define KEYPAD_COL1_PIN 24
#define KEYPAD_COL2_PIN 23
#define KEYPAD_COL3_PIN 22

#define KEYPAD_ROW0_PIN 29
#define KEYPAD_ROW1_PIN 28
#define KEYPAD_ROW2_PIN 27
#define KEYPAD_ROW3_PIN 26

#define KEYPAD_LED0_PIN 37
#define KEYPAD_LED1_PIN 36
#define KEYPAD_LED2_PIN 35
#define KEYPAD_LED3_PIN 34
#define KEYPAD_LED4_PIN 33
#define KEYPAD_LED5_PIN 32
#define KEYPAD_LED6_PIN 31
#define KEYPAD_LED7_PIN 30

#define PIN_BUZZER A1

#define PIN_RS A3
#define PIN_RW A4
#define PIN_EN A5

#define PIN_DB4 A8
#define PIN_DB5 A9
#define PIN_DB6 A10
#define PIN_DB7 A11

#define RESET 0
#define BEGIN 5
#define NO_KEY 255


/**************************************/
/* Object declaration                 */
/**************************************/

Parchis p;     // Parchis object
Keypad keypad; // KEYPAD OBJECT
buzzer b(PIN_BUZZER);// Buzzer object
LCD lcd(PIN_RS, PIN_RW, PIN_EN, PIN_DB4, PIN_DB5, PIN_DB6, PIN_DB7);//LCD object


/**************************************/
/* General variables                  */
/**************************************/

volatile uint8_t segundos = 15;
volatile uint8_t play = false;
volatile uint8_t key = 0;
volatile uint8_t numFicha;
volatile uint8_t numDado;
volatile uint8_t turnoAzul = true;
volatile uint8_t num = NO_KEY;
uint8_t frase;
uint8_t ficha;
char* topic;
uint8_t pressKey = 1;


/**************************************/
/* ISR                                */
/**************************************/

ISR(TIMER1_COMPA_vect)
{
  if (play){
    segundos--;
    if (turnoAzul) Serial.write(segundos + 20);
    else Serial.write(segundos + 40);
  }
}



/**************************************/
/* MQTT CONFIGURATION                 */
/**************************************/

byte mac[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x11 };

// Update these with values suitable for your network
IPAddress ip    (192, 168, 1, 53);
IPAddress gw    (192, 168, 1, 1);

// DNS information
IPAddress mydns (8,8,8,8);

// Broker information
const char         broker_name[] = "tom.uib.es";
const unsigned int broker_port = 1883;
const char client_id[] = "sna"; // Update

//Global variables for MQTT
byte msg[4] = {};

uint16_t VRx = 0;
uint16_t VRy = 0;

bool on = true;
bool value = true;
bool oldValue = true;

volatile bool send_msg = false;

EthernetClient ethClient;
PubSubClient mqttClient(ethClient);

void callback(char* topic, byte* payload, unsigned int length);
void reconnect();

/**************************************/
/* TRANSMITTING FUNCTIONS             */
/**************************************/

void enviarNumeroDado(){
  numDado = random(1,7);      
  switch (numDado){       
     case 1:                
       mqttClient.publish("sna/numDado", "1");
       break;
     case 2:
       mqttClient.publish("sna/numDado", "2");
       break;
     case 3:
       mqttClient.publish("sna/numDado", "3");
       break;
     case 4:
       mqttClient.publish("sna/numDado", "4");
       break;
     case 5:
       mqttClient.publish("sna/numDado", "5");
       break;
     case 6:
       mqttClient.publish("sna/numDado", "6");
       break;           
  }                      
}
/******************************************************************************/

void enviarNumeroFicha(){
  ficha = keypad.readKeypad(NO_BLOCK);
  if (ficha < 4 && ficha >= 0){
    if (turnoAzul){
      topic = "sna/j1/numFicha";
    }else{
      topic = "sna/j2/numFicha";
    }
    while (pressKey != KEYPAD_NO_KEY) pressKey = keypad.readKeypad(NO_BLOCK); //UNTIL YOU DONT UNPRESS THE BUTTON IT WONT COME OUT
    pressKey = 1;        
    switch (ficha){
      case 0:
        mqttClient.publish(topic, "1");
        break;
      case 1:
        mqttClient.publish(topic, "2");
        break;
      case 2:
        mqttClient.publish(topic, "3");
        break;
      case 3:
        mqttClient.publish(topic, "4");
        break;
    }
    ficha = NO_KEY;
  }
}
/******************************************************************************/

void enviarFrase(){
  frase = keypad.readKeypad(NO_BLOCK);
  if (frase > 3 && frase < 8){
    if (turnoAzul){
      topic = "sna/j1/lcd";
    }else{
      topic = "sna/j2/lcd";
    }
    while (pressKey != KEYPAD_NO_KEY) pressKey = keypad.readKeypad(NO_BLOCK); //UNTIL YOU DONT UNPRESS THE BUTTON IT WONT COME OUT
    pressKey = 1;
    switch (frase){
      case 4:
        mqttClient.publish(topic, "4");
        break;
      case 5:
        mqttClient.publish(topic, "5");
        break;
      case 6:
        mqttClient.publish(topic, "6");
        break;
      case 7:
        mqttClient.publish(topic, "7");
        break;
    }
    frase = NO_KEY;
  }
}
/******************************************************************************/
/** SETUP *********************************************************************/
/******************************************************************************/

void setup()
{
  p.parchisInit();
  keypad.initKeypad(KEYPAD_COL0_PIN, KEYPAD_COL1_PIN, KEYPAD_COL2_PIN, KEYPAD_COL3_PIN,
                    KEYPAD_ROW0_PIN, KEYPAD_ROW1_PIN, KEYPAD_ROW2_PIN, KEYPAD_ROW3_PIN);
  keypad.initLeds(KEYPAD_LED0_PIN, KEYPAD_LED1_PIN, KEYPAD_LED2_PIN, KEYPAD_LED3_PIN,
                  KEYPAD_LED4_PIN, KEYPAD_LED5_PIN, KEYPAD_LED6_PIN, KEYPAD_LED7_PIN);
  lcd.init();
  
  Serial.begin(115200);

  // Configure timer
  noInterrupts();
  TCCR1A = 0;
  TCCR1B = 0;

  TCCR1B |= (1 << WGM12); // CTC => WGMn3:0 = 0100
  OCR1A = 49911; //One second
  TIMSK1 |= (1 << OCIE1A);
  TCCR1B |= (1 << CS12);
  interrupts();

  Ethernet.begin(mac, ip, mydns, gw);
  mqttClient.setServer(broker_name, broker_port);
  mqttClient.setCallback(callback);
  delay(1500);

  randomSeed(analogRead(0));
}

/******************************************************************************/
/** LOOP **********************************************************************/
/******************************************************************************/

void loop()
{
  uint8_t frase;
  uint8_t ficha;
  char* topic;
  uint8_t pressKey = 1;
  
  while(1)
  {
    if (!mqttClient.connected()) { reconnect(); }
    
    if (play)
    {  
      if((num < 4 && num >= 0) || segundos < 1){

        if (segundos < 1) num = random(0, 3);

        if (turnoAzul) numFicha = num;
        else numFicha = num + 4;
        
        if (p.puedoMover(numFicha, numDado)){//Comprueva que puede mover la ficha seleccionada.
          if (p.vaAdesbloquear(numFicha)) p.desbloquear(numFicha);
          p.enviarFicha(numFicha, numDado);//Envía la ficha a mover al Precessing.
          if (p.haBloqueado(numFicha)) p.bloquear(numFicha);
        }
        //AQUI ENVIAMOS LA FREQUENCIA DE SONIDO AL ZUMBADOR
          if (p.haMatado(numFicha)){//Comprueba si ha matado y envía msg a Processing si lo ha hecho.
            b.buzzer_freq(700, 100000);
            b.buzzer_freq(900, 100000);
          }
          else if (p.haMetido(numFicha)){
            b.buzzer_freq(750, 250000);
            b.buzzer_freq(850, 250000);
            b.buzzer_freq(1000, 250000);
          }
          else b.buzzer_freq(440, 250000);
       
        num = NO_KEY;
        segundos = 15;
        if (turnoAzul) turnoAzul = false;
        else turnoAzul = true;
//-------------------------------TRANSMISION DEL NUMERO DEL DADO-----------------------------
                           // El nodo intermedio envía el numero de dado a los dos nodos
        enviarNumeroDado();// No distinguimos a que jugador lo enviamos, ya que los dos
                           // comparten los mismos leds y pueden ver el numero del dado
                           // del oponente. El turno de cada oponente se ve en la IG del Processing.                
      }
//-------------------TRANSMISION DEL NUMERO DE FICHA A MOVER (J1 Y J2)-------------------------
                           // Los nodos correspondientes a los jugadores 1 y 2 transmiten
      enviarNumeroFicha(); // el numero de ficha a mover. Si es el turno del J1 transmite                       
    }                      // con un topic y, en  caso contrario con otro topic
                           // El numero de ficha se transmite con los 4 botones de la
                           // primera fila del teclado matricial.
                           
//-------------------------------TRANSMISION MENSAJES DE CHAT (J1 Y J2)----------------------------------
                           // Los nodos correspondientes a los jugadores 1 y 2 transmiten
    enviarFrase();         // las frases que quieran enviar a cada jugador. Solo pueden transmitir
                           // en su turno. Estos mensajes se transmiten con los 4 botones de la segunda 
                           // fila del teclado matricial. 
                           
//----------------------------------RECEPCIÓN MENSAJES DEL PROCESSING--------------------------------------
    
    if (Serial.available() > 0) //Aqui se recibel los mensajes vía serial desde el Processing
    {                           //solamente recibimos dos tipos de mensaje (PLAY Y RESET)
      key = Serial.read();
      
      if (key == BEGIN){
        play = true;
        segundos = 15;
        enviarNumeroDado();
      }
      else if (key == RESET){
        turnoAzul = true;
        play = false;
        mqttClient.publish("sna/numDado", "0");//para resetear leds
        mqttClient.publish("sna/j1/lcd", "8");//para resetear lcd
        num = NO_KEY;
        p.parchisInit();
      }
    }
//---------------------------------------------------------------------------------------------------------
    // RECEPCIÓN
    mqttClient.loop(); // Esto se pone en el loop principal para que se ejecute el maximo de veces
                       // para asegurarse de que se leen todos los mensajes y evitar acumulamiento. 
  }
}

/******************************************************************************/
/** AUXILIARY FUNCTIONS *******************************************************/
/******************************************************************************/

void callback(char* topic, byte* payload, unsigned int length)//RECEPCIÓN DE MSG DE DIFERENTES NODOS
{
  if(topic[7] == 'l'){//RECEPCIÓN DE MENSAJES DEL CHAT (J1 Y J2). LOS NODOS CORRESPONDIENTES A LOS JUGADORES 1 O 2
    lcd.clear();      // RECIBEN EL MENSAJE DEL OTRO JUGADOR PARA IMPRIMIRLO POR EL LCD Y SE PUEDAN COMUNICAR.
    if(topic[5] == '1') {//J1 IMPRIME SOBRE LA PRIMERA LINEA DEL LCD
      lcd.setCursor(1, 1);
      lcd.print("J1: ");
    }
    else {//J2 IMPRIME SOBRE LA SEGUNDA LINEA DEL LCD
      lcd.setCursor(2, 1);
      lcd.print("J2: ");
    }
    if (payload[0] == '4') lcd.print("Hola");
    else if (payload[0] == '5') lcd.print("Buena suerte");
    else if (payload[0] == '6') lcd.print("Buena jugada");
    else if (payload[0] == '7') lcd.print("Te comere");
    else lcd.clear();
  }
  else if (topic[4] == 'n'){//RECEPCIÓN DE NUMERO DE DADO (J1 Y J2). LOS DOS JUGADORES RECIBEN EL NUMERO DE DADO
    for (int i = 0; i < 6; i++){// PARA INDICAR EL NUMERO DEL DADO AL JUGADOR MEDIANTE EL ENCENDIDO DE EL LED
      keypad.offLed(i);    // CORRESPONDIENTE DEL TECLADO MATRICIAL.
    }
    if (payload[0] == '1') keypad.onLed(0);
    else if (payload[0] == '2') keypad.onLed(1);
    else if (payload[0] == '3') keypad.onLed(2);
    else if (payload[0] == '4') keypad.onLed(3);
    else if (payload[0] == '5') keypad.onLed(4);
    else if (payload[0] == '6') keypad.onLed(5);
    else ; //reset. Apaga los leds.
  }
  else{//RECEPCIÓN DE NUMERO DE DADO (NODO INTERMEDIARIO). RECIBE NUMERO DE DADOS DE DOS NODOS (J1 Y J2)
    if (payload[0] == '1') num = 0;
    else if (payload[0] == '2') num = 1;
    else if (payload[0] == '3') num = 2;
    else num = 3;
  }
}

//----------------------------------------------------------------------------------

void reconnect()
{
  // Loop until we're reconnected
  while (!mqttClient.connected())
  {
    lcd.print("Att. MQTT...");
    
    // Attempt to connect
    if (mqttClient.connect(client_id)) {
      lcd.print("connected");//suscribirse a topics
      mqttClient.subscribe("sna/j1/numFicha");
      mqttClient.subscribe("sna/j2/numFicha");
      mqttClient.subscribe("sna/j1/lcd");
      mqttClient.subscribe("sna/j2/lcd");
      mqttClient.subscribe("sna/numDado");
      lcd.clear();

    } else {
      lcd.print("failed, rc=");
      lcd.print(mqttClient.state());
      lcd.print(" try again in 2 seconds");
      delay(2000);
    }
  }
}
