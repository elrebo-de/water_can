/*
 * Example program to control the waterLevel in a WaterCan with elrebo-de/water_can
 */

#include <string>
#include "water_can.hpp"
#include "esp_log.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "soc/clk_tree_defs.h"

#include "ssid_manager.h"
#include "wifi_station.h"
#include "wifi_configuration_ap.h"

#include "onboard_led.hpp"
#include "sdkconfig.h"


static const char *tag = "WaterCan";

extern "C" void app_main(void)
{
      // Initialize the default event loop
      ESP_ERROR_CHECK(esp_event_loop_create_default());

      // Get the Wi-Fi configuration
      auto& ssid_list = SsidManager::GetInstance().GetSsidList();
      if (ssid_list.empty()) {
          // Start the Wi-Fi configuration AP
          auto& ap = WifiConfigurationAp::GetInstance();
          ap.SetSsidPrefix("ESP32");
          ap.SetLanguage("de-DE");
          ap.Start();
          return;
      }

      // Otherwise, connect to the Wi-Fi network
      WifiStation::GetInstance().Start();

      // wait for max. 20 seconds until WLAN is connected
      WifiStation::GetInstance().WaitForConnected(20000);

      WaterCan myCan(std::string("myCan"),
                     10.0, // ultrasonic distance when water can is full
                     40.0, // ultrasonic distance when water can is empty
                     std::string("192.168.178.102"), // IP address of pump switch (shelly plug)
    	             #if defined(CONFIG_IDF_TARGET_ESP32C6)
                         // ESP32C6
                         (gpio_num_t) 20, // triggerPin for ultrasonic sensor
                         (gpio_num_t) 21); // echoPin for ultrasonic sensor
  	                 #elif defined(CONFIG_IDF_TARGET_ESP32C3)
                     // ES32C3
                         (gpio_num_t) 20, // triggerPin for ultrasonic sensor
                         (gpio_num_t) 21); // echoPin for ultrasonic sensor
	                 #elif defined(CONFIG_IDF_TARGET_ESP32)
                         // M5 Atom Lite
                         (gpio_num_t) 26, // triggerPin for ultrasonic sensor
                         (gpio_num_t) 32); // echoPin for ultrasonic sensor
                     #endif

      float maxWaterLevel = myCan.GetMaxWaterLevel();

      OnBoardLed onBoardLed(
		  std::string("onBoardLed"),
	      #if defined(CONFIG_IDF_TARGET_ESP32C6)
 	          /* ESP32-C6-DevKitM-1 V1.0 */
		      (gpio_num_t)8,
		      std::string("GRB"),
		      std::string("RMT"),
		      LED_MODEL_WS2812,
	      #elif defined(CONFIG_IDF_TARGET_ESP32C3)
	        /* Waveshare ESP32-C3-Zero
		    (gpio_num_t) 10,
		    std::string("RGB"),
		    std::string("RMT"),
		    LED_MODEL_WS2812, */

	        /* ESP32-C3 w/ LCD display */
		    (gpio_num_t) 8,
            (uint8_t) 0, // activeLevel
	      #elif defined(CONFIG_IDF_TARGET_ESP32)
	          /* M5 Atom Lite */
		      (gpio_num_t)27,
		      std::string("GRB"),
		      std::string("RMT"),
		      LED_MODEL_WS2812,
	      #endif
		  500);

      onBoardLed.setLedState(true);
      
      while(1) {
          float waterLevel = myCan.Measure(); // measure waterLevel in cm height
          bool pumpIsOn = myCan.ControlPumpSwitch(3U); // control pump switch.
                                                       // If switched on, toggle to off after 3 seconds

          ESP_LOGI(tag, "waterLevel: %f    pump is %s", waterLevel, pumpIsOn ? "ON" : "OFF");

          // show waterLevel with OnBoardLed color
          if (waterLevel <= 0.) {
              onBoardLed.setLedPixelColor(0, 16, 0, 0); // empty -> red
          }
          else if (waterLevel <= maxWaterLevel) {
              onBoardLed.setLedPixelColor(0,
                                           (uint16_t) (16 * (maxWaterLevel - waterLevel) / maxWaterLevel),
                                           (uint16_t) (16 * waterLevel / maxWaterLevel),
                                           0); // between empty -> red and full -> green
          }
          else {
              onBoardLed.setLedPixelColor(0, 0, 16, 0); // full -> green
          }

          // if pumpIsOn then blink
          if (pumpIsOn == true) {
              onBoardLed.blink();
              onBoardLed.blink();
          }
          // else show
          else {
              onBoardLed.show();
              vTaskDelay(1000 / portTICK_PERIOD_MS); // wait for 1 second
          }
      }
}
