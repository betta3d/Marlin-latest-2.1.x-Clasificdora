#include "egg_weigher.h"
#include <Arduino.h>
#include "HX711.h"

// Array global para almacenar los pesos
float egg_weights[7] = {0.0f};

// Pines para los 7 sensores HX711 (deben coincidir con pins_CLASIFICADORA_ESP32.h)
const byte Hx711DataPin[7] = {
  HX711_1_DATA_PIN, HX711_2_DATA_PIN, HX711_3_DATA_PIN, HX711_4_DATA_PIN,
  HX711_5_DATA_PIN, HX711_6_DATA_PIN, HX711_7_DATA_PIN
};

const byte Hx711ClockPin[7] = {
  HX711_1_CLOCK_PIN, HX711_2_CLOCK_PIN, HX711_3_CLOCK_PIN, HX711_4_CLOCK_PIN,
  HX711_5_CLOCK_PIN, HX711_6_CLOCK_PIN, HX711_7_CLOCK_PIN
};

// Array de objetos para cada sensor
HX711 scales[7];

// Tarea de FreeRTOS para leer los sensores en el Núcleo 0
void weighing_task(void *pvParameters) {
  SERIAL_ECHOLN("Weighing task started on Core 0.");

  for (;;) { // Bucle infinito de la tarea
    for (int i = 0; i < 7; i++) {
      if (scales[i].is_ready()) {
        egg_weights[i] = scales[i].get_units(1); // Leer 1 vez
        // SERIAL_printf("Balanza %i: %.2fg\n", i + 1, egg_weights[i]); // Descomentar para depuración
      }
    }
    vTaskDelay(pdMS_TO_TICKS(20)); // Pausa de 20ms para no saturar el núcleo
  }
}

// Función de inicialización
void initialize_egg_weigher() {
  SERIAL_ECHOLN("Initializing Egg Weigher...");

  for (int i = 0; i < 7; i++) {
    scales[i].begin(Hx711DataPin[i], Hx711ClockPin[i]);
    if (scales[i].is_ready()) {
      scales[i].set_scale(1.0f); // Placeholder - ¡NECESITA CALIBRACIÓN!
      scales[i].tare();          // Poner a cero al inicio
      SERIAL_printf("Scale %i initialized.\n", i + 1);
    } else {
      SERIAL_printf("ERROR: Scale %i not found.\n", i + 1);
    }
  }

  // Crear la tarea en el Núcleo 0
  xTaskCreatePinnedToCore(
    weighing_task,      // Función de la tarea
    "WeighingTask",     // Nombre de la tarea
    4096,               // Tamaño de la pila (stack size)
    NULL,               // Parámetros de la tarea
    1,                  // Prioridad de la tarea
    NULL,               // Handle de la tarea
    0                   // Pin al Núcleo 0
  );
}
