#include "../../inc/MarlinConfig.h"
#include "../gcode.h"
#include "../../module/planner.h"
#include "../../module/motion.h"
#include "../../feature/egg_weigher.h"
#include "../../feature/settings/egg_sorter_settings.h"
#include "../../libs/I2C_expander.h"

// Función auxiliar para mover el eje X a una posición con una velocidad específica
static void move_to(const float position, const float feedrate) {
  current_position.x = position;
  planner.buffer_line(current_position, feedrate / 60.0f, active_extruder);
}

// Función auxiliar para controlar un solenoide a través del MCP23017
static void set_solenoid(const uint8_t solenoid_index, const bool release) {
  if (solenoid_index >= 7) return;
  const uint8_t pin = SOLENOID_1_PIN + solenoid_index;
  // Lógica invertida: LOW para liberar (desactivar MOSFET)
  mcp23017.digitalWrite(pin, release ? LOW : HIGH);
}

/**
 * M700: Iniciar ciclo de la clasificadora de huevos
 */
void GcodeSuite::M700() {
  SERIAL_ECHOLN("Iniciando ciclo de la clasificadora de huevos...");

  // Fase 1: Carga (Lento)
  SERIAL_ECHOLNPGM("Fase 1: Carga");
  move_to(sorter_settings.phase1_end_angle, sorter_settings.phase1_feedrate);
  planner.synchronize();

  // Fase 2: Transporte (Medio)
  SERIAL_ECHOLNPGM("Fase 2: Transporte");
  move_to(sorter_settings.phase2_end_angle, sorter_settings.phase2_feedrate);
  planner.synchronize();

  // Fase 3: Descarga (Lento)
  SERIAL_ECHOLNPGM("Fase 3: Descarga y Liberación");
  move_to(sorter_settings.phase3_end_angle, sorter_settings.phase3_feedrate);
  planner.synchronize();

  // Lógica de liberación
  for (int i = 0; i < 7; i++) {
    if (egg_weights[i] >= sorter_settings.weight_thresholds[i]) {
      SERIAL_ECHO_START();
      SERIAL_ECHO("Liberando huevo en estación "); SERIAL_ECHO(i + 1);
      SERIAL_ECHO(" (Peso: "); SERIAL_ECHO_F(egg_weights[i], 2); SERIAL_ECHOLNPGM("g)");
      set_solenoid(i, true); // Liberar
    }
  }
  // Pequeña pausa para asegurar que los solenoides actúen
  hal.delay_ms(200);
  // Resetear solenoides (retener de nuevo)
  for (int i = 0; i < 7; i++) {
    set_solenoid(i, false);
  }

  // Fase 4: Retorno (Rápido)
  SERIAL_ECHOLNPGM("Fase 4: Retorno");
  move_to(sorter_settings.phase1_start_angle, sorter_settings.phase4_feedrate);
  planner.synchronize();

  SERIAL_ECHOLNPGM("Ciclo completado.");
}