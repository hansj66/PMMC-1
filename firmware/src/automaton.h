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

#pragma once

const int MAX_SERVO_NUM = 32;

const int SERVO_MIN = 200;
const int SERVO_MAX = 400;
const int SERVO_MID = (SERVO_MAX + SERVO_MIN) / 2;

class Automaton
{
    public:
        Automaton();
        void begin();
        void enable(int servoId, bool enable);
        void trim(int servoId, int degrees);

        void selectPose(String name);

    private:
        int _pwm[MAX_SERVO_NUM];
        bool _enable[MAX_SERVO_NUM];

        void disableAll();
};

