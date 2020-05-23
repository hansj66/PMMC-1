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

#include <ESPmDNS.h>
#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoNvs.h>

extern String config_page;

AsyncWebServer server(80);

void notFound(AsyncWebServerRequest *request) 
{
    request->send(404, "text/plain", "Not found");
}

void setup() 
{
    Serial.begin(115200);

    // Initialize non volatile storage for servo parameters
    NVS.begin();

    // Make discovery on the network somewhat easier than having to rely on the IP address.
    // Just visit http://PMMC-1 after boot in order to configure the servos or create poses
    if (!MDNS.begin("PMMC-1"))
    {
        Serial.printf("Error starting mDNS!\n");
    }

    // Enable WiFi station mode. 
    WiFi.mode(WIFI_STA);

    // Connect to your access point
    // Note:    In order to connect to a network, "WIFI_SSID" and "WIFI_PASS" has to be defined as environment 
    //          variables (including quotes) before building the project. Alternatively SSID and password 
    //          can be passed as hard coded  string arguments to WiFi.begin().
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    if (WiFi.waitForConnectResult() != WL_CONNECTED) 
    {
        Serial.printf("WiFi Failed!\n");
        return;
    }

    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    // http://pmmc-1 will return the configuration page. This allows the user to interactively
    // manipulate servo positions, create poses and also store this
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        request->send(200, "text/html", config_page);
    });

    // Let's define some service endpoints.

    // 1) "trim" - Adjusts servo position according to corresponding slider in configuration page
    server.on("/trim", HTTP_GET, [] (AsyncWebServerRequest *request) 
    {
        // TODO:    1) Update PWM settings

        String id;
        String pos;
        if (request->hasParam("id")) 
        {
            id = request->getParam("id")->value();
            Serial.printf("Servo ID: %s\n", id.c_str());
        } 
        if (request->hasParam("pos")) 
        {
            pos = request->getParam("pos")->value();
            Serial.printf("Servo position: %s\n", pos.c_str());
        } 
        request->send(200, "text/plain", "OK");
    });

    // 2) "save" - Stores all servo parameters in non volatile flash memory
    server.on("/save", HTTP_GET, [] (AsyncWebServerRequest *request) 
    {
        // TODO:    1) Loop through servos and store in key value store (under name)
        String servoParam;
        char servo[5];
        String poseName = "default";
        uint8_t parameterBlob[24];

        if (request->hasParam("pose")) 
        {
            poseName = request->getParam("pose")->value();
        }

        for (int i=0; i<24; i++)
        {
            sprintf(servo, "s%d", i);
            if (request->hasParam(servo)) 
            {
                servoParam = request->getParam(servo)->value();
                parameterBlob[i] = atoi(servoParam.c_str());
            } 
        }

        // Write parameters to non volatile key value store 
        NVS.setBlob(poseName, parameterBlob, sizeof(parameterBlob));

        request->send(200, "text/plain", "OK");
    });

    // "load" - Reads all servo parameres associated with a given pose and updates
    //          the sliders in the configuration page accordingly
    server.on("/load", HTTP_GET, [] (AsyncWebServerRequest *request) 
    {
        // TODO:    1) Send parameters back to web page
        //          2) Error if not found by name
        //          3) Disable all servos 

        String poseName;
        if (request->hasParam("pose")) 
        {
            poseName = request->getParam("pose")->value();
            Serial.printf("Pose: %s\n", poseName.c_str());
        }

        // read from flash
        size_t blobLength = NVS.getBlobSize(poseName);   
        Serial.printf("Size of %s pose: %d\n", poseName.c_str(), blobLength);

        uint8_t parameterBlob[24];
        NVS.getBlob(poseName, parameterBlob, sizeof(parameterBlob));
        for (int i=0; i<24; i++)
        {
            Serial.printf("Loaded: s%d : %d\n", i, parameterBlob[i]);
        }

        request->send(200, "text/html", "OK");
    });

    server.onNotFound(notFound);
    server.begin();
}

void loop() 
{
}