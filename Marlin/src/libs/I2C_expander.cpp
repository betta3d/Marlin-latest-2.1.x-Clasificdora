#include "I2C_expander.h"

#if MOTHERBOARD == BOARD_CLASIFICADORA_ESP32

// Instancia global del expansor
MCP23017_Expander mcp23017;

void MCP23017_Expander::begin() {
  // Inicializar I2C con los pines definidos en pins_CLASIFICADORA_ESP32.h
  Wire.begin(EXPANDER_I2C_SDA, EXPANDER_I2C_SCL);
  
  SERIAL_ECHOLNPGM("Inicializando MCP23017...");
  
  // Configurar direcciones de pines según el mapeo
  // Puerto A (pines 8-15): Solenoides (salidas)
  writeRegister(MCP23017_IODIRA, 0x00);  // Todo como salidas
  
  // Puerto B (pines 0-7): Sensores y botones (entradas)
  writeRegister(MCP23017_IODIRB, 0xFF);  // Todo como entradas
  
  // Habilitar pull-ups en puerto B para sensores y botones
  writeRegister(MCP23017_GPPUB, 0xFF);
  
  // Inicializar estados
  portA_state = 0x7F;  // Solenoides en HIGH (reteniendo huevos)
  portB_state = 0x00;
  
  // Escribir estado inicial de solenoides
  writeRegister(MCP23017_GPIOA, portA_state);
  writeRegister(MCP23017_GPIOB, portB_state);
  
  SERIAL_ECHOLNPGM("MCP23017 inicializado correctamente.");
}

void MCP23017_Expander::writeRegister(uint8_t reg, uint8_t value) {
  Wire.beginTransmission(MCP23017_ADDRESS);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission();
}

uint8_t MCP23017_Expander::readRegister(uint8_t reg) {
  Wire.beginTransmission(MCP23017_ADDRESS);
  Wire.write(reg);
  Wire.endTransmission();
  
  Wire.requestFrom((uint8_t)MCP23017_ADDRESS, (uint8_t)1);
  if (Wire.available()) {
    return Wire.read();
  }
  return 0;
}

void MCP23017_Expander::digitalWrite(uint8_t pin, uint8_t state) {
  if (pin > 15) return;
  
  uint8_t reg;
  uint8_t* port_state;
  
  if (pin >= 8) {
    // Puerto A (pines 8-15)
    pin -= 8;
    reg = MCP23017_GPIOA;
    port_state = &portA_state;
  } else {
    // Puerto B (pines 0-7)
    reg = MCP23017_GPIOB;
    port_state = &portB_state;
  }
  
  if (state == HIGH) {
    *port_state |= (1 << pin);
  } else {
    *port_state &= ~(1 << pin);
  }
  
  writeRegister(reg, *port_state);
}

uint8_t MCP23017_Expander::digitalRead(uint8_t pin) {
  if (pin > 15) return LOW;
  
  uint8_t reg;
  
  if (pin >= 8) {
    // Puerto A (pines 8-15)
    pin -= 8;
    reg = MCP23017_GPIOA;
  } else {
    // Puerto B (pines 0-7)
    reg = MCP23017_GPIOB;
  }
  
  uint8_t gpio_state = readRegister(reg);
  return (gpio_state & (1 << pin)) ? HIGH : LOW;
}

void MCP23017_Expander::pinMode(uint8_t pin, uint8_t mode) {
  if (pin > 15) return;
  
  uint8_t reg;
  uint8_t current_dir;
  
  if (pin >= 8) {
    // Puerto A (pines 8-15)
    pin -= 8;
    reg = MCP23017_IODIRA;
  } else {
    // Puerto B (pines 0-7)
    reg = MCP23017_IODIRB;
  }
  
  current_dir = readRegister(reg);
  
  if (mode == INPUT) {
    current_dir |= (1 << pin);
  } else {
    current_dir &= ~(1 << pin);
  }
  
  writeRegister(reg, current_dir);
}

void MCP23017_Expander::setPullup(uint8_t pin, bool pullup) {
  if (pin > 15) return;
  
  uint8_t reg;
  uint8_t current_pullup;
  
  if (pin >= 8) {
    // Puerto A (pines 8-15)
    pin -= 8;
    reg = MCP23017_GPPUA;
  } else {
    // Puerto B (pines 0-7)
    reg = MCP23017_GPPUB;
  }
  
  current_pullup = readRegister(reg);
  
  if (pullup) {
    current_pullup |= (1 << pin);
  } else {
    current_pullup &= ~(1 << pin);
  }
  
  writeRegister(reg, current_pullup);
}

#endif // MOTHERBOARD == BOARD_CLASIFICADORA_ESP32