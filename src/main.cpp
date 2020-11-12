#include <Esp.h>
#include <NeoPixelBrightnessBus.h>
#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>
#include <PubSubClient.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <Ticker.h>

#include <ArtnetnodeWifi.h>

#include <esp_log.h>
static const char module_tag[] = "main";

#define LED_COUNT 80
NeoPixelBrightnessBus<NeoRgbFeature, NeoEsp8266Dma800KbpsMethod> strip(LED_COUNT);

Ticker blink_timer;

void board_led_blink(float period) {
    blink_timer.attach(period, []() {
            digitalWrite(14, !digitalRead(14));
            });
}

static ArtnetnodeWifi artnetnode;

void artnet_init() {
    artnetnode.setShortName("ESP-Art-Net Debug");
    artnetnode.setLongName("ESP8266 - Art-Net (debug)");
    artnetnode.setNumPorts(4);
    artnetnode.enableDMXOutput(0);
    artnetnode.enableDMXOutput(1);
    artnetnode.enableDMXOutput(2);
    artnetnode.enableDMXOutput(3);
    artnetnode.begin();

    artnetnode.setArtDmxCallback(
            [] (uint16_t universe, uint16_t length, uint8_t sequence, uint8_t *data) {
//                ESP_ILOG(module_tag, "DMX frame: %d universe, %d len, %d seq", universe, length, sequence);
                digitalWrite(16, !digitalRead(16));
                for (int i = 0; i < length / 3; i++)
                {
                    int led = i;
                    if (led < LED_COUNT) {
                        strip.SetPixelColor(led, RgbColor(data[i * 3], data[i * 3 + 1], data[i * 3 + 2]));
                    } else {
                        break;
                    }
                }
                strip.Show();
            });
}

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

    pinMode(14, OUTPUT);
    pinMode(16, OUTPUT);
}

namespace MQTT {
    extern PubSubClient mqtt;

    void setup();
    void connect();
    void doThing();
}

void setup() {
    setup_base();
    setup_ota();
    MQTT::setup();
    MQTT::connect();
    MQTT::doThing();
    artnet_init();
    strip.Begin();
    strip.SetBrightness(50);
    board_led_blink(0.5);
    ESP_ILOG(module_tag,"Startup is done");
}

void loop() {
    ArduinoOTA.handle();
    MQTT::mqtt.loop();

    if (WiFi.status() == WL_CONNECTED)
        artnetnode.read();
}
