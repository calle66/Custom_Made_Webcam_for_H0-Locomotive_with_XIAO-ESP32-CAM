/************************************************************************************************************************************************************************************/
/*                                                                                                                                                                                  */
/*  Das Programm verwendet einen XIAO ESP32s3 Chip, um eine Kamera in einer Modellbahnlokomotive anzusteuern und die "Lokführerperspektive" mit einfachen Mitteln                   */
/*  via WLAN-Übertragung auf einem Browser live wiederzugeben, hierzu wurde das ArduinoIDE-Beispielprogramm "CameraWebserver" als Grundlage verwendet und auf das                   */
/*  CAMERA_MODEL_XIAO_ESP32S3 mit der Hardware OV2640 reduziert.                                                                                                                    */
/*  Das Modul wird über ein SMD-Relais über einen ESU Lokpilot5 eingeschaltet und läuft dauerhaft ohne automatische Abschaltung.                                                    */
/*  Nach der Aktivierung ist die Adresse (z.B. http://192.168.1.90) über einen seriellen Monitor darstellbar (Verbindung über USB-C-Schnittstelle ist erforderlich)                 */
/*  oder man bezieht die Adresse über den verwendeten Router.                                                                                                                       */
/*  Der Betriebszustand wird über eine 30s rotleuchtende Innenraum-LED angezeigt und und gibt Info über die Verbindung des Schleiferkontakts,                                       */
/*  LED-Blinken informiert über den Ladevorgang eines optional an die Chipplatine angeschlossenen LiPO-Akkus.                                                                       */
/*  Die Ausleuchtung bei dunklen Lichtverhältnissen erfolgt über eine weiße LED an GPIO4 und ist über den Webserver regulierbar.                                                    */
/*  Zur korrekten Programmierung sind folgende Parameter in der Arduino IDE erfolgreich getestet:                                                                                   */
/*                                                                                                                                                                                  */
/*  Board:      XIAO ESP32s                                                                                                                                                         */
/*  Port:       gemäß Gerätemanager                                                                                                                                                 */
/*  PSRAM:      OPI PSRAM                                                                                                                                                           */
/*  Erease:     aktiviert                                                                                                                                                           */
/*                                                                                                                                                                                  */
/*  Zum Programmieren ist das Modul aus dem Lokinnenraum auszubauen und über die integrierte USB-C-Schnittstelle über Arduino-IDE zu programmieren.                                 */
/*                                                                                                                                                                                  */
/************************************************************************************************************************************************************************************/




/************************************************************************************************************************************************************************************/
/*                                Biblothekendeklaration                                                                                                                            */  
/************************************************************************************************************************************************************************************/
#include "esp_camera.h"
#include <WiFi.h>

#define CAMERA_MODEL_XIAO_ESP32S3                                        // mit PSRAM
#include "camera_pins.h"



/************************************************************************************************************************************************************************************/
/*                                Kostantendeklaration                                                                                                                        */  
/************************************************************************************************************************************************************************************/



/************************************************************************************************************************************************************************************/
/*                                Variablendeklaration                                                                                                                            */  
/************************************************************************************************************************************************************************************/
const char *ssid = "HausWLAN";                                           // WiFi Einstellungen
const char *password = "5hD99uJFPXyXHqMKWnnB";



/************************************************************************************************************************************************************************************/
/*                                Unterroutinendeklarationen                                                                                                                        */  
/************************************************************************************************************************************************************************************/
void startCameraServer();
void setupLedFlash(int pin);



/************************************************************************************************************************************************************************************/
/*                                Setuproutine                                                                                                                                      */  
/************************************************************************************************************************************************************************************/
void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();

  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.frame_size = FRAMESIZE_UXGA;
  config.pixel_format = PIXFORMAT_JPEG;                                         // zum Streamen
                                                                                //config.pixel_format = PIXFORMAT_RGB565; // zur Gesichtserkennung
  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
  config.fb_location = CAMERA_FB_IN_PSRAM;
  config.jpeg_quality = 12;
  config.fb_count = 1;

                                                                                // wenn PSRAM vorhanden, initialisiere UXGA-Auflösung und bessere JPEG-Qualitaet
                                                                                //                      fuer größeren vorreservierten Framebuffer
  if (config.pixel_format == PIXFORMAT_JPEG) {
    if (psramFound()) {
      config.jpeg_quality = 10;
      config.fb_count = 2;
      config.grab_mode = CAMERA_GRAB_LATEST;
    } else {
                                                                                // Limitiere Framegröße, wenn kein PSRAM vorhanden
      config.frame_size = FRAMESIZE_SVGA;
      config.fb_location = CAMERA_FB_IN_DRAM;
    }
  } else {
                                                                                // Beste Option für Gesichtserkennung
    config.frame_size = FRAMESIZE_240X240;
#if CONFIG_IDF_TARGET_ESP32S3
    config.fb_count = 2;
#endif
  }
  
  esp_err_t err = esp_camera_init(&config);                                     // Kamerainitialisierung
  if (err != ESP_OK) {
    Serial.printf("Kamera nicht gefunden");
    return;
  }

  sensor_t *s = esp_camera_sensor_get();
  s->set_vflip(s, 0);                                                           // 1: Horizontal spiegeln


#if defined(LED_GPIO_NUM)                                                       // Setup Blitz-LED, wenn LED pin is defined in camera_pins.h
  setupLedFlash(LED_GPIO_NUM);
#endif

  WiFi.begin(ssid, password);                                                   // WLAN-Verbindung herstellen
  WiFi.setSleep(false);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  startCameraServer();                                                          // Kameraserver starten

  Serial.print("Camera bereit! Verwende 'http://");
  Serial.print(WiFi.localIP());
  Serial.println("' zum verbinden");
}



/************************************************************************************************************************************************************************************/
/*                                Programmroutine                                                                                                                                   */  
/************************************************************************************************************************************************************************************/
void loop() {
  delay(10000);
}
