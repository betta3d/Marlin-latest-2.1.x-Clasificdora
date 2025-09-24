/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

//
// Main Menu
//

#include "../../inc/MarlinConfigPre.h"

#if HAS_MARLINUI_MENU

#include "menu_item.h"
#include "../../module/temperature.h"
#include "../../gcode/queue.h"
#include "../../module/printcounter.h"
#include "../../module/stepper.h"
#include "../../sd/cardreader.h"

#if ENABLED(PSU_CONTROL)
  #include "../../feature/power.h"
#endif

#if HAS_GAMES && DISABLED(LCD_INFO_MENU)
  #include "game/game.h"
#endif

#if ANY(HAS_MEDIA, HOST_PROMPT_SUPPORT) || defined(ACTION_ON_CANCEL)
  #define MACHINE_CAN_STOP 1
#endif
#if ANY(HAS_MEDIA, HOST_PROMPT_SUPPORT, PARK_HEAD_ON_PAUSE) || defined(ACTION_ON_PAUSE)
  #define MACHINE_CAN_PAUSE 1
#endif

#if ENABLED(MMU2_MENUS)
  #include "menu_mmu2.h"
#endif

#if ENABLED(PASSWORD_FEATURE)
  #include "../../feature/password/password.h"
#endif

#if (ENABLED(HOST_START_MENU_ITEM) && defined(ACTION_ON_START)) || (ENABLED(HOST_SHUTDOWN_MENU_ITEM) && defined(SHUTDOWN_ACTION))
  #include "../../feature/host_actions.h"
#endif

#if ENABLED(GCODE_REPEAT_MARKERS)
  #include "../../feature/repeat.h"
#endif

void menu_tune();
void menu_cancelobject();
void menu_motion();
void menu_temperature();
void menu_configuration();

#if HAS_POWER_MONITOR
  void menu_power_monitor();
#endif

#if ENABLED(MIXING_EXTRUDER)
  void menu_mixer();
#endif

#if ENABLED(ADVANCED_PAUSE_FEATURE)
  void menu_change_filament();
#endif

#if ENABLED(LCD_INFO_MENU)
  void menu_info();
#endif

#if ANY(LED_CONTROL_MENU, CASE_LIGHT_MENU)
  void menu_led();
#endif

#if HAS_CUTTER
  void menu_spindle_laser();
#endif

#if ENABLED(PREHEAT_SHORTCUT_MENU_ITEM)
  void menu_preheat_only();
#endif

#if HAS_MULTI_LANGUAGE
  void menu_language();
#endif

#if ENABLED(CUSTOM_MENU_MAIN)

  #include "../../feature/settings/egg_sorter_settings.h"

  // Declaraciones de los submenús
  void menu_sorter_settings();
  void menu_sorter_settings_angles();
  void menu_sorter_settings_speeds();
  void menu_sorter_settings_weights();

  void _lcd_custom_menu_main_gcode(FSTR_P const fstr) {
    queue.inject(fstr);
    TERN_(CUSTOM_MENU_MAIN_SCRIPT_AUDIBLE_FEEDBACK, ui.completion_feedback());
    TERN_(CUSTOM_MENU_MAIN_SCRIPT_RETURN, ui.return_to_status());
  }

  void custom_menus_main() {
    START_MENU();
    BACK_ITEM(MSG_MAIN_MENU);

  MENU_ITEM_F(gcode, FPSTR(PSTR("Iniciar Ciclo")), FPSTR(PSTR("M700")));
  SUBMENU_F(FPSTR(PSTR("Ajustes")), menu_sorter_settings);

    END_MENU();
  }

  // Submenú principal de Ajustes
  void menu_sorter_settings() {
    START_MENU();
  BACK_ITEM_F(FPSTR(PSTR("Clasificadora")));
  SUBMENU_F(FPSTR(PSTR("Ángulos de Fase")), menu_sorter_settings_angles);
  SUBMENU_F(FPSTR(PSTR("Velocidades")), menu_sorter_settings_speeds);
  SUBMENU_F(FPSTR(PSTR("Pesos (gramos)")), menu_sorter_settings_weights);
  MENU_ITEM_F(gcode, FPSTR(PSTR("Guardar Ajustes")), FPSTR(PSTR("M500")));
  MENU_ITEM_F(gcode, FPSTR(PSTR("Cargar Ajustes")), FPSTR(PSTR("M501")));
  MENU_ITEM_F(gcode, FPSTR(PSTR("Restaurar Defectos")), FPSTR(PSTR("M502")));
    END_MENU();
  }

  // Submenú para Ángulos
  void menu_sorter_settings_angles() {
    START_MENU();
  BACK_ITEM_F(FPSTR(PSTR("Ajustes")));
    EDIT_ITEM_F(float3, FPSTR(PSTR("Inicio Carga")), &sorter_settings.phase1_start_angle, 0, 360);
    EDIT_ITEM_F(float3, FPSTR(PSTR("Fin Carga")), &sorter_settings.phase1_end_angle, 0, 360);
    EDIT_ITEM_F(float3, FPSTR(PSTR("Fin Transporte")), &sorter_settings.phase2_end_angle, 0, 360);
    EDIT_ITEM_F(float3, FPSTR(PSTR("Fin Descarga")), &sorter_settings.phase3_end_angle, 0, 360);
    END_MENU();
  }

  // Submenú para Velocidades
  void menu_sorter_settings_speeds() {
    START_MENU();
  BACK_ITEM_F(FPSTR(PSTR("Ajustes")));
    EDIT_ITEM_F(float5, FPSTR(PSTR("Vel. Carga")), &sorter_settings.phase1_feedrate, 10, 10000);
    EDIT_ITEM_F(float5, FPSTR(PSTR("Vel. Transporte")), &sorter_settings.phase2_feedrate, 10, 10000);
    EDIT_ITEM_F(float5, FPSTR(PSTR("Vel. Descarga")), &sorter_settings.phase3_feedrate, 10, 10000);
    EDIT_ITEM_F(float5, FPSTR(PSTR("Vel. Retorno")), &sorter_settings.phase4_feedrate, 10, 10000);
    END_MENU();
  }

  // Submenú para Pesos
  void menu_sorter_settings_weights() {
    START_MENU();
  BACK_ITEM_F(FPSTR(PSTR("Ajustes")));
    EDIT_ITEM_F(float3, FPSTR(PSTR("Peso Est. 1")), &sorter_settings.weight_thresholds[0], 1, 100);
    EDIT_ITEM_F(float3, FPSTR(PSTR("Peso Est. 2")), &sorter_settings.weight_thresholds[1], 1, 100);
    EDIT_ITEM_F(float3, FPSTR(PSTR("Peso Est. 3")), &sorter_settings.weight_thresholds[2], 1, 100);
    EDIT_ITEM_F(float3, FPSTR(PSTR("Peso Est. 4")), &sorter_settings.weight_thresholds[3], 1, 100);
    EDIT_ITEM_F(float3, FPSTR(PSTR("Peso Est. 5")), &sorter_settings.weight_thresholds[4], 1, 100);
    EDIT_ITEM_F(float3, FPSTR(PSTR("Peso Est. 6")), &sorter_settings.weight_thresholds[5], 1, 100);
    EDIT_ITEM_F(float3, FPSTR(PSTR("Peso Est. 7")), &sorter_settings.weight_thresholds[6], 1, 100);
    END_MENU();
  }

#endif // CUSTOM_MENU_MAIN

#if ENABLED(ADVANCED_PAUSE_FEATURE)
  // This menu item is last with an encoder. Otherwise, somewhere in the middle.
  #if E_STEPPERS == 1 && DISABLED(FILAMENT_LOAD_UNLOAD_GCODES)
    #define FILAMENT_CHANGE_ITEM() YESNO_ITEM(MSG_FILAMENTCHANGE, menu_change_filament, nullptr, \
                                    GET_TEXT_F(MSG_FILAMENTCHANGE), (const char *)nullptr, F("?"))
  #else
    #define FILAMENT_CHANGE_ITEM() SUBMENU(MSG_FILAMENTCHANGE, menu_change_filament)
  #endif
#endif

void menu_main() {
  const bool busy = printingIsActive();
  #if HAS_MEDIA
    const bool card_detected = card.isMounted(),
               card_open = card_detected && card.isFileOpen();
  #endif

  START_MENU();
  BACK_ITEM(MSG_INFO_SCREEN);

  #if HAS_MEDIA && !defined(MEDIA_MENU_AT_TOP) && !HAS_ENCODER_WHEEL
    #define MEDIA_MENU_AT_TOP
  #endif

  if (busy) {
    #if MACHINE_CAN_PAUSE
      ACTION_ITEM(MSG_PAUSE_PRINT, ui.pause_print);
    #endif
    #if MACHINE_CAN_STOP
      SUBMENU(MSG_STOP_PRINT, []{
        MenuItem_confirm::select_screen(
          GET_TEXT_F(MSG_BUTTON_STOP), GET_TEXT_F(MSG_BACK),
          ui.abort_print, nullptr,
          GET_TEXT_F(MSG_STOP_PRINT), (const char *)nullptr, F("?")
        );
      });
    #endif

    #if ENABLED(GCODE_REPEAT_MARKERS)
      if (repeat.is_active())
        ACTION_ITEM(MSG_END_LOOPS, repeat.cancel);
    #endif

    SUBMENU(MSG_TUNE, menu_tune);

    #if ENABLED(CANCEL_OBJECTS) && DISABLED(SLIM_LCD_MENUS)
      SUBMENU(MSG_CANCEL_OBJECT, []{ editable.int8 = -1; ui.goto_screen(menu_cancelobject); });
    #endif
  }
  else {
    #if ALL(HAS_MEDIA, MEDIA_MENU_AT_TOP)
      // BEGIN MEDIA MENU
      if (card_detected) {
        if (!card_open) {
          #if ENABLED(MENU_ADDAUTOSTART)
            ACTION_ITEM(MSG_RUN_AUTO_FILES, card.autofile_begin); // Run Auto Files
          #endif

          #if HAS_SD_DETECT
            GCODES_ITEM(MSG_CHANGE_MEDIA, F("M21" TERN_(MULTI_VOLUME, "S"))); // M21 Change Media
            #if ENABLED(MULTI_VOLUME)
              GCODES_ITEM(MSG_ATTACH_USB_MEDIA, F("M21U")); // M21 Attach USB Media
            #endif
          #else                                             // - or -
            ACTION_ITEM(MSG_RELEASE_MEDIA, []{              // M22 Release Media
              queue.inject(F("M22"));
              #if ENABLED(TFT_COLOR_UI)
                // Menu display issue on item removal with multi language selection menu
                if (encoderTopLine > 0) encoderTopLine--;
                ui.refresh(LCDVIEW_CLEAR_CALL_REDRAW);
              #endif
            });
          #endif
          SUBMENU(MSG_MEDIA_MENU, MEDIA_MENU_GATEWAY);      // Media Menu (or Password First)
        }
      }
      else {
        #if HAS_SD_DETECT
          ACTION_ITEM(MSG_NO_MEDIA, nullptr);               // "No Media"
        #else
          #if ENABLED(MULTI_VOLUME)
            GCODES_ITEM(MSG_ATTACH_SD_MEDIA, F("M21S"));    // M21S Attach SD Card
            GCODES_ITEM(MSG_ATTACH_USB_MEDIA, F("M21U"));   // M21U Attach USB Media
          #else
            GCODES_ITEM(MSG_ATTACH_MEDIA, F("M21"));        // M21 Attach Media
          #endif
        #endif
      }
      // END MEDIA MENU
    #endif

    if (TERN0(MACHINE_CAN_PAUSE, printingIsPaused()))
      ACTION_ITEM(MSG_RESUME_PRINT, ui.resume_print);

    #if ENABLED(HOST_START_MENU_ITEM) && defined(ACTION_ON_START)
      ACTION_ITEM(MSG_HOST_START_PRINT, hostui.start);
    #endif

    #if ENABLED(PREHEAT_SHORTCUT_MENU_ITEM)
      SUBMENU(MSG_PREHEAT_CUSTOM, menu_preheat_only);
    #endif

    SUBMENU(MSG_MOTION, menu_motion);
  }

  #if ENABLED(ADVANCED_PAUSE_FEATURE) && (!HAS_ENCODER_WHEEL || ENABLED(DISABLE_ENCODER))
    FILAMENT_CHANGE_ITEM();
  #endif

  #if HAS_CUTTER
    SUBMENU(MSG_CUTTER(MENU), STICKY_SCREEN(menu_spindle_laser));
  #endif

  #if HAS_TEMPERATURE
    SUBMENU(MSG_TEMPERATURE, menu_temperature);
  #endif

  #if HAS_POWER_MONITOR
    SUBMENU(MSG_POWER_MONITOR, menu_power_monitor);
  #endif

  #if ENABLED(MIXING_EXTRUDER)
    SUBMENU(MSG_MIXER, menu_mixer);
  #endif

  #if ENABLED(MMU2_MENUS)
    if (!busy) SUBMENU(MSG_MMU2_MENU, menu_mmu2);
  #endif

  SUBMENU(MSG_CONFIGURATION, menu_configuration);

  #if ENABLED(CUSTOM_MENU_MAIN)
    if (TERN1(CUSTOM_MENU_MAIN_ONLY_IDLE, !busy)) {
      #ifdef CUSTOM_MENU_MAIN_TITLE
        SUBMENU_F(F(CUSTOM_MENU_MAIN_TITLE), custom_menus_main);
      #else
        SUBMENU(MSG_CUSTOM_COMMANDS, custom_menus_main);
      #endif
    }
  #endif

  #if ENABLED(LCD_INFO_MENU)
    SUBMENU(MSG_INFO_MENU, menu_info);
  #endif

  #if ANY(LED_CONTROL_MENU, CASE_LIGHT_MENU)
    SUBMENU(MSG_LEDS, menu_led);
  #endif

  //
  // Switch power on/off
  //
  #if ENABLED(PSU_CONTROL)
    if (powerManager.psu_on)
      #if ENABLED(PS_OFF_CONFIRM)
        CONFIRM_ITEM(MSG_SWITCH_PS_OFF,
          MSG_YES, MSG_NO,
          ui.poweroff, nullptr,
          GET_TEXT_F(MSG_SWITCH_PS_OFF), (const char *)nullptr, F("?")
        );
      #else
        ACTION_ITEM(MSG_SWITCH_PS_OFF, ui.poweroff);
      #endif
    else
      GCODES_ITEM(MSG_SWITCH_PS_ON, F("M80"));
  #endif

  #if HAS_MEDIA && DISABLED(MEDIA_MENU_AT_TOP)
    // BEGIN MEDIA MENU
    if (card_detected) {
      if (!card_open) {
        #if ENABLED(MENU_ADDAUTOSTART)
          ACTION_ITEM(MSG_RUN_AUTO_FILES, card.autofile_begin); // Run Auto Files
        #endif

        #if HAS_SD_DETECT
          GCODES_ITEM(MSG_CHANGE_MEDIA, F("M21" TERN_(MULTI_VOLUME, "S"))); // M21 Change Media
          #if ENABLED(MULTI_VOLUME)
            GCODES_ITEM(MSG_ATTACH_USB_MEDIA, F("M21U")); // M21 Attach USB Media
          #endif
        #else                                             // - or -
          ACTION_ITEM(MSG_RELEASE_MEDIA, []{              // M22 Release Media
            queue.inject(F("M22"));
            #if ENABLED(TFT_COLOR_UI)
              // Menu display issue on item removal with multi language selection menu
              if (encoderTopLine > 0) encoderTopLine--;
              ui.refresh(LCDVIEW_CLEAR_CALL_REDRAW);
            #endif
          });
        #endif
        SUBMENU(MSG_MEDIA_MENU, MEDIA_MENU_GATEWAY);      // Media Menu (or Password First)
      }
    }
    else {
      #if HAS_SD_DETECT
        ACTION_ITEM(MSG_NO_MEDIA, nullptr);               // "No Media"
      #else
          #if ENABLED(MULTI_VOLUME)
            GCODES_ITEM(MSG_ATTACH_SD_MEDIA, F("M21S"));    // M21S Attach SD Card
            GCODES_ITEM(MSG_ATTACH_USB_MEDIA, F("M21U"));   // M21U Attach USB Media
          #else
            GCODES_ITEM(MSG_ATTACH_MEDIA, F("M21"));        // M21 Attach Media
          #endif
      #endif
    }
    // END MEDIA MENU
  #endif

  #if HAS_SERVICE_INTERVALS
    static auto _service_reset = [](const int index) {
      print_job_timer.resetServiceInterval(index);
      ui.completion_feedback();
      ui.reset_status();
      ui.return_to_status();
    };
    #if SERVICE_INTERVAL_1 > 0
      CONFIRM_ITEM_F(F(SERVICE_NAME_1),
        MSG_BUTTON_RESET, MSG_BUTTON_CANCEL,
        []{ _service_reset(1); }, nullptr,
        GET_TEXT_F(MSG_SERVICE_RESET), F(SERVICE_NAME_1), F("?")
      );
    #endif
    #if SERVICE_INTERVAL_2 > 0
      CONFIRM_ITEM_F(F(SERVICE_NAME_2),
        MSG_BUTTON_RESET, MSG_BUTTON_CANCEL,
        []{ _service_reset(2); }, nullptr,
        GET_TEXT_F(MSG_SERVICE_RESET), F(SERVICE_NAME_2), F("?")
      );
    #endif
    #if SERVICE_INTERVAL_3 > 0
      CONFIRM_ITEM_F(F(SERVICE_NAME_3),
        MSG_BUTTON_RESET, MSG_BUTTON_CANCEL,
        []{ _service_reset(3); }, nullptr,
        GET_TEXT_F(MSG_SERVICE_RESET), F(SERVICE_NAME_3), F("?")
      );
    #endif
  #endif

  #if HAS_GAMES && DISABLED(LCD_INFO_MENU)
    #if ENABLED(GAMES_EASTER_EGG)
      SKIP_ITEM();
      SKIP_ITEM();
      SKIP_ITEM();
    #endif
    // Game sub-menu or the individual game
    {
      SUBMENU(
        #if HAS_GAME_MENU
          MSG_GAMES, menu_game
        #elif ENABLED(MARLIN_BRICKOUT)
          MSG_BRICKOUT, brickout.enter_game
        #elif ENABLED(MARLIN_INVADERS)
          MSG_INVADERS, invaders.enter_game
        #elif ENABLED(MARLIN_SNAKE)
          MSG_SNAKE, snake.enter_game
        #elif ENABLED(MARLIN_MAZE)
          MSG_MAZE, maze.enter_game
        #endif
      );
    }
  #endif

  #if HAS_MULTI_LANGUAGE
    SUBMENU(LANGUAGE, menu_language);
  #endif

  #if ENABLED(HOST_SHUTDOWN_MENU_ITEM) && defined(SHUTDOWN_ACTION)
    SUBMENU(MSG_HOST_SHUTDOWN, []{
      MenuItem_confirm::select_screen(
        GET_TEXT_F(MSG_BUTTON_PROCEED), GET_TEXT_F(MSG_BUTTON_CANCEL),
        []{ ui.return_to_status(); hostui.shutdown(); }, nullptr,
        GET_TEXT_F(MSG_HOST_SHUTDOWN), (const char *)nullptr, F("?")
      );
    });
  #endif

  #if ENABLED(ADVANCED_PAUSE_FEATURE) && HAS_ENCODER_WHEEL && DISABLED(DISABLE_ENCODER)
    FILAMENT_CHANGE_ITEM();
  #endif

  END_MENU();
}

#endif // HAS_MARLINUI_MENU
