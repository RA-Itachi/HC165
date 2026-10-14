/*
 * HC165.cpp
 * ------------------------------------------------------------
 * Implementacion de la libreria HC165.
 * ------------------------------------------------------------
 * Autor: Rafa
 * Licencia: MIT
 * ------------------------------------------------------------
 */

#include "HC165.h"

HC165::HC165(uint8_t loadPin, uint8_t clkPin, uint8_t misoPin, uint8_t numChips)
  : _loadPin(loadPin),
    _clkPin(clkPin),
    _misoPin(misoPin),
    _numChips(numChips == 0 ? 1 : numChips),
    _lastState(0),
    _prevState(0),
    _firstRead(true)
{
}

void HC165::begin() {
  pinMode(_loadPin, OUTPUT);
  pinMode(_clkPin,  OUTPUT);
  pinMode(_misoPin, INPUT);

  digitalWrite(_loadPin, HIGH);
  digitalWrite(_clkPin,  HIGH);

  _lastState = 0;
  _prevState = 0;
  _firstRead = true;
}

// Lee un byte crudo de la cadena.
// El primer bit que sale es el bit 7 del chip mas lejano.
uint8_t HC165::_readRawByte() {
  uint8_t value = 0;

  for (int i = 7; i >= 0; i--) {
    digitalWrite(_clkPin, LOW);
    int bitVal = digitalRead(_misoPin);
    if (bitVal) value |= (1 << i);
    digitalWrite(_clkPin, HIGH);
  }

  return value;
}

uint32_t HC165::readAll() {
  _prevState = _lastState;

  // Pulso de LOAD: captura el estado paralelo en todos los chips.
  digitalWrite(_loadPin, LOW);
  digitalWrite(_loadPin, HIGH);

  uint32_t value = 0;

  // Leemos los chips en orden inverso: el ultimo chip de la cascada
  // es el primero en salir por QH.
  for (int chip = _numChips - 1; chip >= 0; chip--) {
    uint8_t b = _readRawByte();
    if (chip < 4) {
      value |= ((uint32_t)b) << (chip * 8);
    }
  }

  _lastState = value;

  if (_firstRead) {
    _prevState = _lastState;
    _firstRead = false;
  }

  return _lastState;
}

uint8_t HC165::readChip(uint8_t n) const {
  if (n >= _numChips) return 0;
  return (uint8_t)((_lastState >> (n * 8)) & 0xFF);
}

bool HC165::readBit(uint16_t n) const {
  if (n >= getNumBits()) return false;
  return (_lastState >> n) & 0x01;
}

bool HC165::pressed(uint16_t n) const {
  if (n >= getNumBits()) return false;
  uint32_t mask = (1UL << n);
  return ((_lastState & mask) != 0) && ((_prevState & mask) == 0);
}

bool HC165::released(uint16_t n) const {
  if (n >= getNumBits()) return false;
  uint32_t mask = (1UL << n);
  return ((_lastState & mask) == 0) && ((_prevState & mask) != 0);
}

bool HC165::changed() const {
  return _lastState != _prevState;
}