# HC165

Libreria Arduino/ESP32 para leer uno o varios 74HC165 (shift registers de
entrada paralelo / salida serial) conectados en cascada.

## Caracteristicas

- Configuracion de N chips en cascada.
- Lectura completa de todos los bits en una sola operacion.
- Acceso por chip y por bit individual.
- Deteccion de flancos (boton recien pulsado o soltado).
- No usa shiftIn() bloqueante; control manual de CLK/LOAD.

## Uso basico

#include <HC165.h>

HC165 buttons(2, 15, 4, 2);

void setup() {
  Serial.begin(115200);
  buttons.begin();
}

void loop() {
  uint32_t state = buttons.readAll();
  if (buttons.changed()) {
    Serial.printf("Estado: 0x%08X\n", state);
  }
}

## API

- HC165(load, clk, miso, numChips) : Constructor.
- begin()                         : Configura pines.
- readAll()                       : Lee todos los chips, devuelve uint32_t.
- readChip(n)                     : Byte del chip n de la ultima lectura.
- readBit(n)                      : Bit n de la ultima lectura.
- pressed(n)                      : true si el bit n paso de 0 a 1.
- released(n)                     : true si el bit n paso de 1 a 0.
- changed()                       : true si algun bit cambio.
- getNumChips()                   : Numero de chips configurados.
- getNumBits()                    : Numero total de bits (numChips * 8).
- getLastState()                  : Ultima lectura completa.
- getPrevState()                  : Lectura anterior.

## Licencia

MIT