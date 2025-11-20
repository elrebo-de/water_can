/*
 * WaterCan.cpp
 *
 *  Created on 01.11.2025
 *      Author: christophoberle
 *
 * this work is licenced under the Apache 2.0 licence
 */

#include "water_can.hpp"
#include "esp_log.h"

WaterCan::WaterCan(std::string tag,
                   float distanceFull,
                   float distanceEmpty,
                   std::string ipAddrPumpSwitch,
                   gpio_num_t triggerPinHcsr04,
                   gpio_num_t echoPinHcsr04) {
	this->tag = tag;
    this->ipAddrPumpSwitch = ipAddrPumpSwitch;
    this->distanceFull = distanceFull;
    this->distanceEmpty= distanceEmpty;
    this->triggerPinHcsr04 = triggerPinHcsr04;
    this->echoPinHcsr04 = echoPinHcsr04;

    this->pumpIsOn = false;
    this->maxWaterLevel = this->distanceEmpty - this->distanceFull;

    this->pumpSwitch = new ShellyPlug(std::string("pumpSwitch"), this->ipAddrPumpSwitch);
    this->canSensor = new Hcsr04Sensor(std::string("canSensor"),
                                       this->triggerPinHcsr04,
                                       this->echoPinHcsr04,
                                       100.);
}

WaterCan::~WaterCan() {
}

float WaterCan::Measure() {
	this->distance = this->canSensor->GetDistance();
	this->waterLevel = this->distanceEmpty - this->distance;
	return this->waterLevel;
}

bool WaterCan::ControlPumpSwitch(uint16_t toggleAfter) {
          // Pumpenstatus ermitteln
          if (this->pumpIsOn == false) {
              if (this->waterLevel <= 0.) {
                  this->pumpIsOn = true;
              }
          }
          if (this->pumpIsOn == true) {
              if (this->waterLevel >= this->maxWaterLevel) {
                  this->pumpIsOn = false;
              }
          }

          if (this->pumpIsOn == true) {
              this->pumpSwitch->Switch(true, (uint16_t) 3);
          }
          return this->pumpIsOn;
}

float WaterCan::GetMaxWaterLevel() {
	return this->maxWaterLevel;
}

