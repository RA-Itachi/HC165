/*
 * HC165.h
 * ------------------------------------------------------------
 * Libreria para leer uno o varios 74HC165 en cascada.
 * ------------------------------------------------------------
 * Autor: Rafa
 * Licencia: MIT
 * ------------------------------------------------------------
 */

#ifndef HC165_H
#define HC165_H

#include <Arduino.h>

class HC165 {
public:
  // Constructor
  //  loadPin  : pin conectado a PL (Load) de todos los HC165
  //  clkPin   : pin conectado a CP (Clock) de todos los HC165
  //  misoPin  : pin conectado a QH del ultimo HC165 de la cadena
  //  numChips : cantidad de HC165 en cascada (1..4 recomendado)
  HC165(uint8_t loadPin, uint8_t clkPin, uint8_t misoPin, uint8_t numChips = 1);

  // Inicializa los pines. Llamar en setup().
  void begin();

  // Lee todos los chips y devuelve el estado completo como uint32_t.
  // Soporta hasta 32 bits (4 chips). Para mas, usar readChip().
  uint32_t readAll();

  // Devuelve el byte del chip n de la ultima lectura.
  uint8_t readChip(uint8_t n) const;

  // Devuelve el bit n (0..numBits-1) de la ultima lectura.
  bool readBit(uint16_t n) const;

  // Deteccion de flancos sobre la ultima lectura.
  bool pressed(uint16_t n) const;   // 0 -> 1
  bool released(uint16_t n) const;  // 1 -> 0
  bool changed() const;             // algun bit cambio

  // Info
  uint8_t  getNumChips()  const { return _numChips; }
  uint16_t getNumBits()   const { return (uint16_t)_numChips * 8; }
  uint32_t getLastState() const { return _lastState; }
  uint32_t getPrevState() const { return _prevState; }

private:
  uint8_t  _loadPin;
  uint8_t  _clkPin;
  uint8_t  _misoPin;
  uint8_t  _numChips;

  uint32_t _lastState;
  uint32_t _prevState;
  bool     _firstRead;

  // Lee un byte crudo de la cadena. Uso interno.
  uint8_t _readRawByte();
};

#endif // HC165_H