# Programming and debugging

In order to connect to a network, "WIFI_SSID" and "WIFI_PASS" has to be defined as environment 
variables (including quotes) before building the project

Connect a FTDI cable to the 6 pin programming header. Make sure to get the orientation right or you will risk frying the board (Black wire is Gnd).

The board can be programmed using [PlatformIO](https://platformio.org/). Just open the project the firmware folder in PiO and build.

1. Press and hold down the button while switching on power to enter programming mode. 
1. Then press the "upload" button on the PiO toolbar to program the board

# Calibration

The robot is running a webserver where you can do zero calibration of servos and also create poses for saving in the robot's non volatile memory (4k available).

After power on, the robot will connect you your SSID using the credentials provided in the environment variables. It then announces the web server name on the network using mDNS.

Open a web browser on a device connected to the same network and open the configuration page:

Your servos will probably not be in the neutral position out of the box. Attach one servo at a time. Enable the servo by ticking the corresponding "enable" box. Then use the slider to adjust the servo to the desired neutral position.

After all the servos has been calibrated, press the save button. This will save the parameters in non volatile memory and be used as default startup parameters on the next boot.

http://pmmc-1/

![Configuration page](https://github.com/hansj66/PMMC-1/raw/master/images/webserver.png)

The same web server exposes a simple API that allows you to manipulate the servos in order to create poses. These poses can then be associated with a key and stored in NV memory.

# Servo wiring

PORT	PIN	SERVO

TBD
