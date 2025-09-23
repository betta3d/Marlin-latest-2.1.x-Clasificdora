#pragma once

/**
 * egg_weigher.h
 *
 * Interfaz para la tarea de pesaje de huevos en segundo plano.
 * Se ejecuta en el Núcleo 0 del ESP32 para no interferir con Marlin.
 */

#include "../inc/MarlinConfig.h"

// El array que contendrá los pesos leídos de las 7 balanzas.
// 'extern' significa que está definido en otro lugar (en egg_weigher.cpp)
// y es accesible desde otros archivos que incluyan este header.
extern float egg_weights[7];

/**
 * @brief Inicializa los sensores HX711 y lanza la tarea de pesaje en el Núcleo 0.
 *
 * Esta función debe ser llamada una sola vez desde el setup() de Marlin.
 */
void initialize_egg_weigher();

/**
 * @brief La función de la tarea que se ejecuta en bucle en el Núcleo 0.
 *
 * No llamar directamente. Es lanzada por initialize_egg_weigher.
 *
 * @param pvParameters Parámetros para la tarea de FreeRTOS (no se usa).
 */
void weighing_task(void *pvParameters);
