#pragma once
/**
 * Pinout para Prototipo "Clasificadora de Huevos ESP32"
 * Arquitectura: ESP32 nativo + Expansor MCP23017
 * Fecha: 23/09/2025
 */

//================================================================================
//== Pines Nativos del ESP32 (Tareas de alta velocidad)
//================================================================================

// -- Motor de la Ruleta (Eje X)
#define X_STEP_PIN          13
#define X_DIR_PIN           12
#define X_ENABLE_PIN        14



// -- Pantalla LCD (MKS MINI 12864 V3)
#define LCD_PINS_CLK        18  // SPI CLK
#define LCD_PINS_MOSI       23  // SPI MOSI
#define LCD_PINS_CS         5   // SPI CS
#define DOGLCD_CS           LCD_PINS_CS
#define LCD_PINS_D4         19  // D/C (A0)
#define LCD_PINS_EN         4   // RST (Reset) pin - CORREGIDO: era 33
#define LCD_PINS_RS         2   // RS pin - CORREGIDO: era 25
#define DOGLCD_A0           LCD_PINS_D4
#define LCD_RESET_PIN       LCD_PINS_EN



// -- Sensores de Peso (7x HX711)
#define HX711_1_DATA_PIN    34  // Balanza 1 - DATA (GPIO de solo entrada)
#define HX711_1_CLOCK_PIN   33  // Balanza 1 - CLOCK

#define HX711_2_DATA_PIN    35  // Balanza 2 - DATA (GPIO de solo entrada)
#define HX711_2_CLOCK_PIN   32  // Balanza 2 - CLOCK

#define HX711_3_DATA_PIN    36  // Balanza 3 - DATA (GPIO de solo entrada)
#define HX711_3_CLOCK_PIN   25  // Balanza 3 - CLOCK

#define HX711_4_DATA_PIN    39  // Balanza 4 - DATA (GPIO de solo entrada)
#define HX711_4_CLOCK_PIN   26  // Balanza 4 - CLOCK

#define HX711_5_DATA_PIN    27  // Balanza 5 - DATA
#define HX711_5_CLOCK_PIN   17  // Balanza 5 - CLOCK

#define HX711_6_DATA_PIN    16  // Balanza 6 - DATA
#define HX711_6_CLOCK_PIN   15  // Balanza 6 - CLOCK

#define HX711_7_DATA_PIN    2   // Balanza 7 - DATA
#define HX711_7_CLOCK_PIN   0   // Balanza 7 - CLOCK


// -- Bus I2C para comunicación con el expansor
#define EXPANDER_I2C_SDA    21
#define EXPANDER_I2C_SCL    22

// Define standard Marlin I2C pin aliases so PINS_EXIST(I2C_SCL, I2C_SDA) works
#define I2C_SDA_PIN         EXPANDER_I2C_SDA
#define I2C_SCL_PIN         EXPANDER_I2C_SCL


//================================================================================
//== Pines del Expansor MCP23017 (Tareas de ON/OFF)
//== La numeración (p.ej., 0, 1, 2...) corresponde a los pines del chip MCP23017
//================================================================================

// -- Sensor de Home (Conectado al MCP23017)
#define X_MIN_PIN           0   // Pin 0 del expansor (GPB0)
// No se usan Y/Z en esta máquina
#define Y_MIN_PIN           -1
#define Z_MIN_PIN           -1

// -- Botones de Control (Conectados al MCP23017)
#define START_BUTTON_PIN    1   // Pin 1 del expansor (GPB1)
#define STOP_BUTTON_PIN     2   // Pin 2 del expansor (GPB2)

// -- Electroimanes (Conectados al MCP23017)
#define SOLENOID_1_PIN      8   // Pin 8 del expansor (GPA0)
#define SOLENOID_2_PIN      9   // Pin 9 del expansor (GPA1)
#define SOLENOID_3_PIN      10  // Pin 10 del expansor (GPA2)
#define SOLENOID_4_PIN      11  // Pin 11 del expansor (GPA3)
#define SOLENOID_5_PIN      12  // Pin 12 del expansor (GPA4)
#define SOLENOID_6_PIN      13  // Pin 13 del expansor (GPA5)
#define SOLENOID_7_PIN      14  // Pin 14 del expansor (GPA6)


//================================================================================
//== Mapeo estándar para que Marlin compile
//================================================================================
#define ST7920_CLK_PIN      LCD_PINS_CLK
#define ST7920_DAT_PIN      LCD_PINS_MOSI
#define ST7920_CS_PIN       LCD_PINS_CS
#define ST7920_A0_PIN       LCD_PINS_D4
// Map reset pin correctly for ST7920 helper aliases
#define ST7920_RST_PIN      LCD_RESET_PIN


//Otros para compliar

#define TEMP_0_PIN -1
#define E0_STEP_PIN -1

#define Y_STEP_PIN          -1
#define Y_DIR_PIN           -1
#define Y_ENABLE_PIN        -1

#define Z_STEP_PIN          -1
#define Z_DIR_PIN           -1
#define Z_ENABLE_PIN        -1


