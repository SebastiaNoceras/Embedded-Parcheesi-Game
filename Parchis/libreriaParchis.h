/**
  @file libreriaParchis
  @author Sebastià A. Noceras Anderson
  @date 09/05/2020
  @brief libreria libreriaParchis.
 */
#ifndef Parchis_H                                       
#define Parchis_H
#include "Arduino.h"

class Parchis
{
public:
 
  Parchis();

  void parchisInit();
  void enviarFicha(byte ficha, byte numero);
  bool haBloqueado(byte ficha);
  bool vaAdesbloquear(byte ficha);
  void bloquear(byte ficha);
  void desbloquear(byte ficha);
  bool hayBloqueo(byte ficha, byte numero);
  bool puedoMover(byte ficha, byte numero);
  bool haMatado(byte ficha);
  bool haMetido(byte ficha);
  
private:
  byte fichas[8];
  byte bloqueo[46];

};

class buzzer
{
public:

  buzzer(uint8_t buzzer_pin);

  void buzzer_freq(uint32_t frequency, uint32_t duration);

private:
  uint8_t buzzer_pin;
};

#endif // Parchis_H
