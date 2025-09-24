#pragma once

#include "../../inc/MarlinConfig.h"

/**
 * egg_sorter_settings.h
 *
 * Estructura para almacenar las configuraciones de la clasificadora de huevos.
 * Estos valores se guardarán y leerán de la EEPROM.
 */

struct EggSorterSettings {
  // Ángulos de las fases (en grados)
  float phase1_start_angle; // Inicio de la carga
  float phase1_end_angle;   // Fin de la carga / Inicio de transporte
  float phase2_end_angle;   // Fin de transporte / Inicio de descarga
  float phase3_end_angle;   // Fin de descarga / Inicio de retorno

  // Velocidades de las fases (en mm/min, que equivalen a grados/min)
  float phase1_feedrate;    // Velocidad Lenta (Carga)
  float phase2_feedrate;    // Velocidad Media (Transporte)
  float phase3_feedrate;    // Velocidad Lenta (Descarga)
  float phase4_feedrate;    // Velocidad Rápida (Retorno)

  // Pesos de calibración para cada una de las 7 balanzas (en gramos)
  float weight_thresholds[7];

  // Función para establecer los valores por defecto
  void reset() {
    phase1_start_angle = 350.0f;
    phase1_end_angle   = 10.0f;
    phase2_end_angle   = 170.0f;
    phase3_end_angle   = 190.0f;

    phase1_feedrate    = 500.0f;   // Ejemplo: 500 grados/min
    phase2_feedrate    = 2000.0f;  // Ejemplo: 2000 grados/min
    phase3_feedrate    = 500.0f;
    phase4_feedrate    = 6000.0f;  // Ejemplo: 6000 grados/min

    // Pesos de ejemplo (de mayor a menor)
    weight_thresholds[0] = 68.0f;
    weight_thresholds[1] = 61.0f;
    weight_thresholds[2] = 54.0f;
    weight_thresholds[3] = 47.0f;
    weight_thresholds[4] = 40.0f;
    weight_thresholds[5] = 0.0f;
    weight_thresholds[6] = 0.0f;
  }
};

extern EggSorterSettings sorter_settings;
