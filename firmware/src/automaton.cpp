/*
*  Copyright [2020] [Hans Jørgen Grimstad]
*  
*  Licensed under the Apache License, Version 2.0 (the "License");
*  you may not use this file except in compliance with the License.
*  You may obtain a copy of the License at
*  
*      http://www.apache.org/licenses/LICENSE-2.0
*  
*  Unless required by applicable law or agreed to in writing, software
*  distributed under the License is distributed on an "AS IS" BASIS,
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*  See the License for the specific language governing permissions and
*  limitations under the License.
*/

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include "automaton.h"

Adafruit_PWMServoDriver driver_A(0x41);
Adafruit_PWMServoDriver driver_B(0x42);

Automaton::Automaton() 
{
    // Initialize pwm parameter array to a default mid level 
    // This will have to be tuned in the configuration settings later.
    for (int i : _pwm)
    {
        _pwm[i] = SERVOMID;
        _enable[i] = false;
    }
}

void Automaton::begin()
{
    // Initialize the wire library (necessary for I2C)
    Wire.begin(GPIO_NUM_21, GPIO_NUM_22);
    // Initialize the Adafruit driver for each of the onboard PCA9685 PWM controllers
    driver_A.begin();
    driver_B.begin();
    driver_A.setOscillatorFrequency(25000000);
    driver_B.setOscillatorFrequency(25000000);
    driver_A.setPWMFreq(50);
    driver_B.setPWMFreq(50);

    // Initialize the GPIO pins that control the PCA9685 enable signals.
    pinMode(GPIO_NUM_23, OUTPUT);
    pinMode(GPIO_NUM_25, OUTPUT);
    digitalWrite(GPIO_NUM_23, LOW);
    digitalWrite(GPIO_NUM_25, LOW);
}


void Automaton::disableAll()
{
    // This is not a "true" enable, since the servos will still have power.
    for (int i=0; i<MAX_SERVO_NUM; i++)
    {
        driver_A.setPWM(i, 0, 0);
    }
}

void Automaton::enable(int servoId, bool enable)
{
    if ((servoId < 0) || (servoId > 31))
        return;

    _enable[servoId] = enable;

    Adafruit_PWMServoDriver * driver;
    int internalId = servoId;

    if (servoId < 16)
        driver = &driver_A;
    else
    {
        internalId = servoId-16;
        driver = &driver_B;
    }
    
    if (enable)
        driver->setPWM(internalId, 0, _pwm[servoId]);
    else
        driver->setPWM(internalId, 0, 4096);
}

void Automaton::trim(int servoId, int value)
{
    _pwm[servoId] = value;
    enable(servoId, _enable[servoId]);
}


