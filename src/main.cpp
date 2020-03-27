#include <Esp.h>
#include <NeoPixelBus.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#include <esp_log.h>

static const char module_tag[] = "main";

static void setup_ota() {
    ESP_ILOG(module_tag,"Starting OTA");
//    ArduinoOTA.setPort(8266);
//    ArduinoOTA.setHostname(config.hostname);
//    ArduinoOTA.setPassword(config.password);

    WiFi.mode(WIFI_STA);
    WiFi.begin("RT-WiFi147", "9037399429");
    while (WiFi.waitForConnectResult() != WL_CONNECTED) {
        ESP_CLOG(module_tag, "Connection failed! Rebooting...");
        delay(5000);
        ESP.restart();
    }

    ArduinoOTA.onStart([]() {
        ESP_ILOG(module_tag,"Arduino OTA: Start updating");
    });

    ArduinoOTA.onEnd([]() {
        ESP_ILOG(module_tag,"Arduino OTA: End");
    });

    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        ESP_ILOG(module_tag,"Arduino OTA Progress: %u%%\r", (progress / (total / 100)));
    });

    ArduinoOTA.onError([](ota_error_t error) {
        const char *msg;
        switch (error) {
            case OTA_AUTH_ERROR:
                msg = "Auth Failed";
                break;
            case OTA_BEGIN_ERROR:
                msg = "Begin Failed";
                break;
            case OTA_CONNECT_ERROR:
                msg = "Connect Failed";
                break;
            case OTA_RECEIVE_ERROR:
                msg = "Receive Failed";
                break;
            case OTA_END_ERROR:
                msg = "End Failed";
                break;
            default:
                msg = "unknown";
                break;
        }
        ESP_CLOG(module_tag,"Arduino OTA Error[%u]: %s", error, msg);
    });

    ArduinoOTA.begin();

    ESP_ILOG(module_tag, "Ready");
    ESP_ILOG(module_tag, "IP address: %s", WiFi.localIP().toString().c_str());
}

void setup_base() {
    Serial.begin(115200);
    Serial.println();
}

void setup() {
    setup_base();
    setup_ota();

    ESP_ILOG(module_tag,"Startup is done");
}

void loop() {
    ArduinoOTA.handle();
}
