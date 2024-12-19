The program uses a XIAO ESP32S3 chip to control a camera inside a model train locomotive and provides a "driver's perspective" using simple means via WLAN streaming to a browser.
The Arduino IDE example program "CameraWebserver" was used as the basis and adapted for the CAMERA_MODEL_XIAO_ESP32S3 with the OV2640 hardware.

![grafik](https://github.com/user-attachments/assets/ea44c011-0c8e-4dc5-b395-ff33295a599d)

The module is powered on via an SMD relay controlled by an ESU Lokpilot 5 and runs continuously without automatic shutdown.
Once activated, the address (e.g. http://192.169.10.20) can be displayed using a serial monitor (requires a connection via the USB-C interface) or obtained through the router used.

![grafik](https://github.com/user-attachments/assets/0086866f-3aae-4d6c-b335-5c60a77eda08)

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

Don't forget to adjust lines 46 and 47 for your WiFi settings in the file 'ESP32s_XIAO_LokCamBR152.ino:
    
    const char *ssid = "WLAN";                                           
    const char *password = "XXXXXXXX";

To flash the ESP32: Compile and upload the program.

ESP32Cam-Flashen: https://wiki.seeedstudio.com/xiao_esp32s3_camera_usage/

Pins: https://wiki.seeedstudio.com/xiao_esp32s3_pin_multiplexing/

Here’s how it works: You can watch it here: https://www.youtube.com/watch?v=oGMQHV2rfZA
