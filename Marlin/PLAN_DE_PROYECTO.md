# Plan de Acción Definitivo: Firmware Clasificadora de Huevos

## 1. Resumen del Proyecto
El objetivo es desarrollar un firmware a medida, basado en Marlin 2.1.x, para una máquina clasificadora de huevos. El firmware se ejecutará en una placa ESP32 y controlará toda la cinemática, la lógica de pesaje, la liberación de huevos y la interfaz de usuario.

## 2. Lista de Materiales (BOM)
*   **Control Principal e Interfaz:**
    *   1x Placa de Control Principal basada en ESP32.
    *   1x Pantalla LCD MKS MINI 12864 V3.
    *   1x Módulo Expansor de I/O MCP23017.
    *   2x Botones Pulsadores (para Start/Stop).
*   **Sistema de Rotación:**
    *   1x Motor Paso a Paso NEMA 17 (prototipo) o NEMA 23 (producción).
    *   1x Driver de Motor (TMC2208 o similar para prototipo, TB6600 para producción).
    *   1x Sensor Óptico tipo Endstop (para Homing).
*   **Sistema de Pesaje (x7 estaciones):**
    *   1x Celda de Carga (capacidad 100g).
    *   1x Módulo Amplificador HX711.
*   **Sistema de Liberación (x7 estaciones):**
    *   1x Electroimán (12V).
    *   1x Módulo de control con MOSFET.
*   **Alimentación:**
    *   1x Fuente de Alimentación 12V.
    *   1x Fuente de Alimentación para Lógica (5V) y Motor (ej. 24V).

## 3. Arquitectura de Hardware y Lógica Física
*   **Placa Principal:** ESP32, identificada como `BOARD_CLASIFICADORA_ESP32`.
*   **Archivo de Pines:** `Marlin/src/pins/esp32/pins_CLASIFICADORA_ESP32.h`.
*   **Eje Rotacional:** Eje `X` de Marlin. `1 grado = 1 mm` en G-code. Homing con sensor óptico en `X_MIN_PIN`.
*   **Sistema de Pesaje:** Leído directamente por el ESP32.
*   **Sistema de Liberación:** 7 electroimanes en 7 pines GPIO nativos del ESP32.
*   **Lógica de Liberación (Invertida):** Electroimanes **normalmente energizados**. Para liberar, el pin de control se pone en `LOW` para cortar la corriente.
*   **Expansor de E/S:** MCP23017 (I2C) para gestionar señales de entrada.
*   **Interfaz de Usuario:** Pantalla LCD `MKS MINI 12864 V3` y botones físicos.

## 4. Arquitectura e Implementación de Software
*   **Doble Núcleo (ESP32):**
    *   **Núcleo 1 (Tiempo Real):** Reservado para control de motores de Marlin.
    *   **Núcleo 0 (Aplicación):** Tarea C++ personalizada para leer los 7 sensores HX711 de forma no bloqueante.
*   **G-code Personalizado (`M700`):**
    *   Comando para ejecutar el ciclo completo de 4 fases de movimiento.
    *   Soportará la liberación múltiple y simultánea de huevos.
*   **Interfaz de Usuario Personalizada:**
    *   Menús en la LCD para control ("Iniciar Ciclo") y ajuste de parámetros.

## 5. Próximos Pasos
1.  ~~Comenzar la modificación de `Configuration.h` y `Configuration_adv.h` para habilitar el hardware base.~~ **(COMPLETADO)**
2.  ~~Proceder con el desarrollo del código C++ para la tarea de pesaje (Núcleo 0).~~ **(COMPLETADO)**
3.  Continuar con el desarrollo del G-code `M700`.
4.  Integrar los nuevos comandos y ajustes en la interfaz de usuario de la LCD.

---

## Historial de Progreso

**23/09/2025 - Fase 1: Configuración Inicial del Firmware (COMPLETADA)**
*   **Pantalla LCD:** Activada la opción `MKS_MINI_12864` en `Configuration.h`.
*   **Expansor I2C:** Habilitado el soporte mediante la activación de `GCODE_MACROS_M42_P_EXPANDER` en `Configuration_adv.h`.
*   **Menús Personalizados:** Habilitada la opción `CUSTOM_MENU_MAIN` en `Configuration_adv.h` para futura personalización de la interfaz.

**23/09/2025 - Fase 2.1: Tarea de Pesaje en Segundo Plano (COMPLETADA)**
*   **Dependencia:** Añadida la librería `bogde/HX711` al archivo `platformio.ini`.
*   **Módulo de Pesaje:** Creados los archivos `Marlin/src/feature/egg_weigher.h` y `egg_weigher.cpp` con la lógica para la tarea de lectura de sensores en el Núcleo 0.
*   **Integración:** Modificado `MarlinCore.cpp` para incluir y inicializar el módulo de pesaje al arrancar el sistema.