#pragma once

/**
 * I2C_expander.h
 * 
 * Librería para controlar el expansor MCP23017 vía I2C
 * Utiliza los pines I2C definidos en pins_CLASIFICADORA_ESP32.h
 * 
 * Funcionalidades:
 * - Control de 7 solenoides/electroimanes
 * - Lectura de sensor de home
 * - Lectura de botones de control
 */

#include "../inc/MarlinConfig.h"

#if MOTHERBOARD == BOARD_CLASIFICADORA_ESP32

#include <Wire.h>

// Dirección I2C del MCP23017 (configuración por defecto: A0=A1=A2=0)
#define MCP23017_ADDRESS 0x20

// Registros del MCP23017
#define MCP23017_IODIRA   0x00  // Dirección puerto A (1=entrada, 0=salida)
#define MCP23017_IODIRB   0x01  // Dirección puerto B
#define MCP23017_GPIOA    0x12  // Registro GPIO puerto A
#define MCP23017_GPIOB    0x13  // Registro GPIO puerto B
#define MCP23017_GPPUA    0x0C  // Pull-up puerto A
#define MCP23017_GPPUB    0x0D  // Pull-up puerto B

class MCP23017_Expander {
private:
  uint8_t portA_state = 0;  // Estado actual del puerto A
  uint8_t portB_state = 0;  // Estado actual del puerto B
  
  // Funciones auxiliares para comunicación I2C
  void writeRegister(uint8_t reg, uint8_t value);
  uint8_t readRegister(uint8_t reg);

public:
  /**
   * @brief Inicializa el expansor MCP23017
   * Configura los pines según el mapeo definido en pins_CLASIFICADORA_ESP32.h
   */
  void begin();
  
  /**
   * @brief Escribe un valor digital a un pin del expansor
   * @param pin Pin del expansor (0-15)
   * @param state Estado a escribir (HIGH/LOW)
   */
  void digitalWrite(uint8_t pin, uint8_t state);
  
  /**
   * @brief Lee el valor digital de un pin del expansor
   * @param pin Pin del expansor (0-15)
   * @return Estado del pin (HIGH/LOW)
   */
  uint8_t digitalRead(uint8_t pin);
  
  /**
   * @brief Configura un pin como entrada o salida
   * @param pin Pin del expansor (0-15)
   * @param mode Modo (INPUT/OUTPUT)
   */
  void pinMode(uint8_t pin, uint8_t mode);
  
  /**
   * @brief Habilita pull-up interno en un pin de entrada
   * @param pin Pin del expansor (0-15)
   * @param pullup true para habilitar pull-up
   */
  void setPullup(uint8_t pin, bool pullup);
};

// Instancia global del expansor
extern MCP23017_Expander mcp23017;

#endif // MOTHERBOARD == BOARD_CLASIFICADORA_ESP32