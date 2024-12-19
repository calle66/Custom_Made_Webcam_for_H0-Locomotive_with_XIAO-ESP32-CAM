The program uses a XIAO ESP32S3 chip to control a camera inside a model train locomotive and provides a "driver's perspective" using simple means via WLAN streaming to a browser.
The Arduino IDE example program "CameraWebserver" was used as the basis and adapted for the CAMERA_MODEL_XIAO_ESP32S3 with the OV2640 hardware.

The module is powered on via an SMD relay controlled by an ESU Lokpilot 5 and runs continuously without automatic shutdown.
Once activated, the address (e.g. http://192.168.1.90) can be displayed using a serial monitor (requires a connection via the USB-C interface) or obtained through the router used.

The operational status is indicated by a red interior LED that lights up for 30 seconds, providing information about the connection of the slider contact.
The LED blinking indicates the charging status of an optional LiPo battery connected to the chip board.

For illumination under low light conditions, a white LED connected to GPIO4 is used and can be controlled via the web server.

The following parameters have been successfully tested for correct programming in the Arduino IDE:
    
    Board: XIAO ESP32S
    Port: As per Device Manager
    PSRAM: OPI PSRAM
    Erase Flash: Enabled

To program the module, it must be removed from the locomotive interior and programmed via the integrated USB-C interface using the Arduino IDE.

Install the Arduino IDE as described in https://wiki.seeedstudio.com/xiao_esp32s3_getting_started/ and copy the following files:
    
    ESP32s_XIAO_LokCamBR152.ino
    app_httpd.cpp
    camera_index.h
    camera_pins.h
    ci.json
into any directory. Then, open the file ESP32s_XIAO_LokCamBR152.ino with the Arduino IDE and set the following parameters:

    Select the Board:
    Go to Tools > Board > ESP32 Arduino > Seeed Studio XIAO ESP32S3.
    (Ensure the correct board is selected, as the PSRAM option is specific to ESP32 models.)

    Enable PSRAM: Go to Tools > PSRAM > Enabled.
    Erase Flash: Go to Tools > Erase Flash > Enabled.

To flash the ESP32: Compile and upload the program.

ESP32Cam-Flashen: https://wiki.seeedstudio.com/xiao_esp32s3_camera_usage/

Pins: https://wiki.seeedstudio.com/xiao_esp32s3_pin_multiplexing/
