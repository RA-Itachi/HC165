/*
 * ReadButtons.ino
 * ------------------------------------------------------------
 * Ejemplo de uso de la libreria HC165.
 * Lee 2 chips HC165 en cascada (16 botones) y muestra
 * por Serial que boton se ha pulsado o soltado.
 * ------------------------------------------------------------
 */

#include <HC165.h>

// Pines
#define PIN_165_LOAD   2
#define PIN_165_CLK    15
#define PIN_165_MISO   4

// Cantidad de chips en cascada
#define NUM_CHIPS      2

HC165 buttons(PIN_165_LOAD, PIN_165_CLK, PIN_165_MISO, NUM_CHIPS);

void setup() {
  Serial.begin(115200);
  delay(200);
  Serial.println(F("HC165 - Ejemplo ReadButtons"));
  buttons.begin();
}

void loop() {
  uint32_t state = buttons.readAll();

  if (buttons.changed()) {
    Serial.printf("Estado: 0x%08X\n", state);

    for (uint16_t i = 0; i < buttons.getNumBits(); i++) {
      if (buttons.pressed(i)) {
        Serial.printf("  Boton %u PULSADO\n", i);
      }
      if (buttons.released(i)) {
        Serial.printf("  Boton %u SOLTADO\n", i);
      }
    }
  }

  delay(10);
}