/*
 * water_can.hpp
 *
 *  Created on: 30.10.2025
 *      Author: christophoberle
 *
 * this work is licenced under the Apache 2.0 licence
 */

#ifndef WATER_CAN_HPP_
#define WATER_CAN_HPP_

#include <stdio.h>

#include <cstdint>
#include <string>

#include "shelly_plug.hpp"
#include "hcsr04_sensor.hpp"

class WaterCan {
public:
	WaterCan(std::string tag,
	         float distanceFull,
	         float distanceEmpty,
	         std::string ipAddrPumpSwitch,
	         gpio_num_t triggerPinHcsr04,
	         gpio_num_t echoPinHcsr04);
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

#endif /* WATER_CAN_HPP_ */
