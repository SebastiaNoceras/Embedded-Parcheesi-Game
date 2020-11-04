/**
  @file libreriaParchis.cpp
  @author Sebastià A. Noceras Anderson
  @date 09/05/2020
  @brief Keypad library.
 */
 
#include "libreriaParchis.h"
#include "Arduino.h"


Parchis::Parchis(){}

/**************************************/
/* Functions                          */
/**************************************/

void Parchis::parchisInit(){
  for(int i = 0; i < 8; i++){
    fichas[i] = 0;
  }
  for(int i = 0; i < 47; i++){
    bloqueo[i] = false;
  }
}

void Parchis::enviarFicha(byte ficha, byte numero){
  for (int i = 0; i < numero; ++i){
    Serial.write(ficha);
    fichas[ficha] = fichas[ficha] + 1;
    delay(200);
  }
}

//----------------------------------------------------------------------------------

bool Parchis::haBloqueado(byte ficha){
  if (ficha < 4){
    for (int i = 0; i < 4; i++){
      if ((fichas[ficha] == fichas[i]) && (ficha != i) && (ficha != 41)){
        return true;
      }
    }
  }else{
     for (int i = 4; i < 8; i++){
       if ((fichas[ficha] == fichas[i]) && (ficha != i) && (ficha != 41)){
         return true;
      }
    }
  }
  return false;
}

//----------------------------------------------------------------------------------

bool Parchis::vaAdesbloquear(byte ficha){
  if (ficha < 4){
    if (bloqueo[fichas[ficha]] == true) return true;
  }else{
    if (fichas[ficha] < 23){
      if (bloqueo[fichas[ficha] + 22] == true) return true;
    }else{
      if (bloqueo[fichas[ficha] - 22] == true) return true;
    }
  }
  return false;
}

//----------------------------------------------------------------------------------

void Parchis::bloquear(byte ficha){
  if (ficha < 4 && fichas[ficha] < 40){
    bloqueo[fichas[ficha]] = true;
  }else{
    if (fichas[ficha] < 23){
      bloqueo[fichas[ficha] + 22] = true;
    }else if(fichas[ficha] > 22 && fichas[ficha] < 40){
      bloqueo[fichas[ficha] - 22] = true;
    }
  }
}

//----------------------------------------------------------------------------------

void Parchis::desbloquear(byte ficha){
   if (ficha < 4 && fichas[ficha] < 40){
    bloqueo[fichas[ficha]] = false;
  }else{
    if (fichas[ficha] < 23){
      bloqueo[fichas[ficha] + 22] = false;
    }else if(fichas[ficha] > 22 && fichas[ficha] < 40){
      bloqueo[fichas[ficha] - 22] = false;
    }
  }
}

//----------------------------------------------------------------------------------

bool Parchis::hayBloqueo(byte ficha, byte numero){
  if (ficha < 4){
    for (int i = (fichas[ficha] + 1); i <= (fichas[ficha] + numero); i++){
      if (bloqueo[i] == true && i < 40){
        return true;
      }
    }
  }else{
     for (int i = (fichas[ficha] + 1); i <= (fichas[ficha] + numero); i++){
      if (i < 23){
        if (bloqueo[i + 22]){
          return true;
        }
      }else{
        if (bloqueo[i - 22] == true && i < 40){
          return true;
        }
      }
    }
  }
  return false;
}

//----------------------------------------------------------------------------------

bool Parchis::puedoMover(byte ficha, byte numero){
  if (fichas[ficha] == 0 && numero != 5 && numero != 1){
    delay(1000);
    return false;
  }
  else if ((fichas[ficha] + numero) > 41){
    delay(1000);
    return false;
  }else if (hayBloqueo(ficha, numero)){
    delay(1000);
    return false;
  }else{
    return true;
  } 
}

//----------------------------------------------------------------------------------

bool Parchis::haMatado(byte ficha){
  if (ficha < 4){
    for (int i = 4; i < 8; i++){
      if (fichas[i] < 18 && (fichas[ficha] - 22) == fichas[i] && fichas[i] > 0){
        Serial.write(i + 10);
        fichas[i] = 0;
        return true;
      }else if (fichas[i] > 22 && fichas[ficha] == (fichas[i] - 22) && fichas[ficha] < 18){
        Serial.write(i + 10);
        fichas[i] = 0;
        return true;
      }
    }
  }else{
    for (int i = 0; i < 4; i++){
      if (fichas[ficha] < 18 && (fichas[i] - 22) == fichas[ficha] && fichas[ficha] > 0){
        Serial.write(i + 10);
        fichas[i] = 0;
        return true;
      }else if (fichas[ficha] > 22 && fichas[i] == (fichas[ficha] - 22) && i < 18 && fichas[i] > 0 && fichas[ficha] < 40){
        Serial.write(i + 10);
        fichas[i] = 0;
        return true;
      }
    }
  }
  return false;
}

bool Parchis::haMetido(byte ficha){
  if (fichas[ficha] == 41){
    fichas[ficha] = 42;
    return true;
  }
  else return false;
}

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------

buzzer::buzzer(uint8_t buzzer_pin)
{
  this->buzzer_pin = buzzer_pin;

  pinMode(buzzer_pin, OUTPUT);
}

void buzzer::buzzer_freq(uint32_t freq, uint32_t duration)
{//usec
  uint32_t periodo = 1000000/freq;
  uint32_t inter = duration/periodo;
  for( uint16_t i = 1; i <= inter; i++){
  digitalWrite(buzzer_pin, HIGH);
  delayMicroseconds(periodo/2);
  digitalWrite(buzzer_pin, LOW);
  delayMicroseconds(periodo/2);
  }
}
