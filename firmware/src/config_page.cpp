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

String config_page = R"TEMPLATE(
<!DOCTYPE html>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="icon" href="data:,">
  <style>
    body {
      text-align: center;
      font-family: "Trebuchet MS", Arial;
      margin-left:auto;
      margin-right:auto;
    }
    .slider {
      width: 300px;
    }
  </style>
  <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js"></script>
</head>
<body>
    <center>
        <h1>Poor Man's Machine Crawler mk I</h1>
        <h2>Pose Editor</h2>
        <a href="https://www.TimeExpander.com">Time Expander</a>
    </center>

    <hr>
    <p align="left">Servo parameters can be stored in and later be read back from non volatile flash partition or exported as an array for later use in the kinematics controller.</p>
    <label for="poseName">Pose name:</label>
    <input type="text" id="poseName" name="poseName" value="neutral"></input>
    <button onclick="FlashSave()">Save</button>
    <button onclick="FlashLoad()">Load</button>
    <button onclick="ExportPose()">Export</button>
    <hr>

    <table id="servotable" style="width:100%">
      <tr>
        <th>Id</th>
        <th>Adjust</th>
        <th>Position</th>
        <th>Enable</th>
      </tr>
    </table>  
    <hr>
    
  

  <script>
    function addServoControl(servoId) 
    {
      var table = document.getElementById("servotable");
      var rowCount = table.rows.length;
      
      var row = table.insertRow(rowCount);

      if (servoId < 8)
        row.insertCell(0).innerHTML= 'Hip (#' + servoId + ')';
      else if (servoId < 16)
        row.insertCell(0).innerHTML= 'Knee (#' + servoId + ')';
      else if (servoId < 24)
      row.insertCell(0).innerHTML= 'Ankle (#' + servoId + ')';


      row.insertCell(1).innerHTML= '<input type="range" min="0" max="180" class="slider" id="servoSlider' + servoId + '" onchange="servoAdjust(' + servoId + ',' + this.value + ')"/>';
      row.insertCell(2).innerHTML= '<p><span id="servoPos' + servoId + '"></span></p>';
      row.insertCell(3).innerHTML= '<input type="checkbox" id="servo' + servoId + '" name="servo' + servoId + '" value="servo' + servoId + '" onclick="enable_servo(' + servoId + ',this)">';
    }

    function displayServoPosition(id)
    {
      var slider = document.getElementById("servoSlider" + id);
      var servoP = document.getElementById("servoPos" + id);
      servoP.innerHTML = slider.value;
      slider.oninput = function() 
      {
        slider.value = this.value;
        servoP.innerHTML = this.value;
      }
    }

    for (i = 0; i < 24; i++) 
    {
      addServoControl(i);
      displayServoPosition(i);
    }

    var table = document.getElementById("servotable");
    console.log(table)

    $.ajaxSetup({timeout:1000});
    function servoAdjust(id, pos) 
    {
      var slider = document.getElementById("servoSlider" + id);
      var servoP = document.getElementById("servoPos" + id);
      servoP.innerHTML = slider.value;
      slider.oninput = function() 
      {
        slider.value = this.value;
        servoP.innerHTML = this.value;
      }

      $.get("/trim?id=" + id + "&pos=" + pos); 
      {Connection: close};
    }
  
    function enable_servo(id, cb) 
    {
      $.get("/enable?id=1&enable=" + cb.checked + "&");
      {Connection: close};
    }

    function FlashSave()
    {
      console.log("Flash Save");
      var params
      for (i=0; i<24; i++)
      {
        var slider = document.getElementById("servoSlider" + i);
        prefix = "s" + i + "="
        if (i==0)
          params = prefix + slider.value + '&'
        else if (i<23)
          params += prefix + slider.value + '&'
        else
          params += prefix + slider.value
      }
      console.log(params)

      var poseName = document.getElementById("poseName");
      $.get("/save?pose=" + poseName.value + "&" + params);
      {Connection: close};
    }

    function FlashLoad()
    {
      console.log("Flash Load");

      var poseName = document.getElementById("poseName");
      $.get("/load?pose=" + poseName.value, function(data, status) 
      {
        console.log("Data:" + data);
        console.log("Status:" + status);
      });

      var slider = document.getElementById("servoSlider0");
      var servoP = document.getElementById("servoPos0");
      slider.value = 180;
      servoP.innerHTML = 180;
    }

    function ExportPose() 
    {
      // TODO: Popup with servo parameters

      console.log("Export Pose");
    }



    </script>
</body>
</html>
)TEMPLATE";

