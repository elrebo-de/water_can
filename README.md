# Water Can component

This repository contains an ESP-IDF component for Refilling a water can with a pump. It runs on
any ESP32 processor and is built using the ESP-IDF build system in version 6.0.

The component is implemented as C++ class `WaterCan`.

It depends on ESP Components elrebo-de/hcsr04_server and elrebo-de/shelly_plug.
The example depends on ESP component 78/esp-wifi-connect for the Wifi connection.

Because there are incompatibilities of httpclient (used in elrebo-de/shelly_plug) with ESP-IDF version 6.0 it is
added as a component to the example.

Because there are incompatibilities of esp-wifi-connect with ESP-IDF version 6.0 it is
added as a component to the example.

## Connecting the component

The constructor of class `WaterCan` has six parameters:

| Parameter                  | Type of Parameter | Usage                                                         |
|:---------------------------|:------------------|:--------------------------------------------------------------|
| tag                        | std::string       | the tag to be used in the ESP log                             |
| distanceFull               | float             | the the ultrasonic distance of the full can                   |
| distanceEmpty              | float             | the the ultrasonic distance of the empty can                  |
| ipAddrPumpSwitch           | std::string       | the IP Address of the Shelly Plug                             |
| triggerPinHcsr04Sensor     | gpio_num_t        | the GPIO pin used to trigger the HCSR04 sensor                |
| echoPinHcsr04Sensor | gpio_num_t        | the GPIO pin used to read the distance from the HCSR04 sensor |

# Usage

The waterLevel can be measured with method `Measure`.

The PumpSwitch can be controlled with method `ControlPumpSwitch`.

```
      WaterCan myCan(std::string("myCan"),
                     10.0, // ultrasonic distance when water can is full
                     40.0, // ultrasonic distance when water can is empty
                     std::string("192.168.178.102"), // IP address of pump switch (shelly plug)
                     (gpio_num_t) 20, // triggerPin for ultrasonic sensor
                     (gpio_num_t) 21); // echoPin for ultrasonic sensor

      while(1) {
          float waterLevel = myCan.Measure();
          bool pumpIsOn = myCan.ControlPumpSwitch(3U);

          ESP_LOGI(tag, "waterLevel: %f    pump is %s", waterLevel, pumpIsOn ? "ON" : "OFF");

          vTaskDelay(1000 / portTICK_PERIOD_MS);
      }
```
## API
The API of the component is located in the include directory ```include/water_can.hpp``` and defines the
C++ class ```WaterCan```

```C
class WaterCan {
public:
    WaterCan(std::string tag, float distanceFull, float distanceEmpty, std::string ipAddrPumpSwitch, gpio_num_t triggerPinHcsr04, gpio_num_t echoPinHcsr04);
    virtual ~WaterCan();
    float Measure();
    bool ControlPumpSwitch(uint16_t toggleAfter);

private:	
    std::string tag = "WaterCan";
    std::string ipAddrPumpSwitch;
    gpio_num_t triggerPinHcsr04;
    gpio_num_t echoPinHcsr04;

    ShellyPlug *pumpSwitch;
    Hcsr04Sensor *canSensor;

    float distance;
    float distanceFull;
    float distanceEmpty;

    bool pumpIsOn;
    float waterLevel;
    float maxWaterLevel;
};
```

# License

This component is provided under the Apache 2.0 license.
