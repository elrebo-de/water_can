## Water Can Example

This example demonstrates how to utilize the `water_can` component to control the refill of a water can.

The following loop is repeated every second:
* The water level is measured.
* If the water level of the water falls below 0 it is set in state `pumpIsOn == true`.
* If the water level rises above the maximum water level of the water can the state is switched to state `pumpIsOn == false`
* If the water can is in state `pumpIsOn == true` the pump is switched on for 3 seconds.

## Hardware

* A shelly plug with an IP address in the connected WLAN  and
* an ultrasonic sensor HCSR04 is needed in this example.

## Build and Flash

Build the project and flash it to the board, then run the monitor tool to view the serial output:

* Run `. <directory with ESP-IDF version>/export.sh` to set IDF environment
* Run `idf.py set-target esp32xx` to set target chip
* Run `idf.py -p PORT flash monitor` to build, flash and monitor the project

(To exit the serial monitor, type `Ctrl-]` (or on a Mac `Ctrl-Option-6`).

See the Getting Started Guide for all the steps to configure and use the ESP-IDF to build projects.

## Example Output

```
I (11943) canSensor: GetDistance called
I (11943) canSensor: time_us = 1117
I (11943) canSensor: distance =  19.2
I (11943) WaterCan: waterLevel: 20.840481    pump is OFF
I (12943) canSensor: GetDistance called
I (12943) canSensor: time_us = 2260
I (12943) canSensor: distance =  38.8
I (12943) WaterCan: waterLevel: 1.234989    pump is OFF
I (13943) canSensor: GetDistance called
I (13943) canSensor: time_us = 2885
I (13943) canSensor: distance =  49.5
I (13943) pumpSwitch: ShellyPlug Command: http://192.168.178.102/rpc/Switch.Set?id=0&on=true&toggle_after=3
I (13973) pumpSwitch: HTTP GET code: 200
I (13973) pumpSwitch: HTTP GET successful. Response:
I (14973) pumpSwitch: {"was_on":false}
I (14973) WaterCan: waterLevel: -9.485420    pump is ON
I (15973) canSensor: GetDistance called
I (15973) canSensor: time_us = 2273
I (15973) canSensor: distance =  39.0
I (15973) pumpSwitch: ShellyPlug Command: http://192.168.178.102/rpc/Switch.Set?id=0&on=true&toggle_after=3
I (16003) pumpSwitch: HTTP GET code: 200
I (16003) pumpSwitch: HTTP GET successful. Response:
I (17003) pumpSwitch: {"was_on":true}
I (17003) WaterCan: waterLevel: 1.012005    pump is ON
I (18003) canSensor: GetDistance called
I (18003) canSensor: time_us = 1247
I (18003) canSensor: distance =  21.4
I (18003) pumpSwitch: ShellyPlug Command: http://192.168.178.102/rpc/Switch.Set?id=0&on=true&toggle_after=3
I (18033) pumpSwitch: HTTP GET code: 200
I (18033) pumpSwitch: HTTP GET successful. Response:
I (19033) pumpSwitch: {"was_on":true}
I (19033) WaterCan: waterLevel: 18.610634    pump is ON
I (20033) canSensor: GetDistance called
I (20033) canSensor: time_us = 574
I (20033) canSensor: distance =   9.8
I (20033) WaterCan: waterLevel: 30.154373    pump is OFF
I (21033) canSensor: GetDistance called
I (21033) canSensor: time_us = 247
I (21033) canSensor: distance =   4.2
I (21033) WaterCan: waterLevel: 35.763294    pump is OFF
I```