#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include <memory>
#include <cstring>
typedef std::unique_ptr<char, void(*)(void*)> _auto_char_ptr;
static inline _auto_char_ptr auto_char_ptr(char *ptr) {
	return _auto_char_ptr(ptr, std::free);
}

static inline _auto_char_ptr auto_strdup(const char *ptr) {
	return auto_char_ptr(strdup(ptr));
}

static inline _auto_char_ptr auto_strndup(const char *ptr, size_t len) {
	return auto_char_ptr(strndup(ptr, len));
}

#include <esp_log.h>
static const char module_tag[] = "mqtt";

namespace MQTT {
static WiFiClient client;
PubSubClient mqtt(client);

void doThing() {
    mqtt.subscribe("/control", 0);

//    mqttMeta();
//    mqttPublish();
}

void connect() {
    ESP_ILOG(module_tag, "MQTT connect");
    if (!mqtt.connect("192.168.0.11", "", "")) {
        ESP_WLOG(module_tag, "MQTT connection failed");
        return;
    }
}

static void mqtt_callback(char *topic, uint8_t *payload, unsigned int len) {
    auto msg = auto_strndup((char *) payload, len);
    ESP_ILOG(module_tag, "Received %s: %s", topic, msg.get());
}

void setup() {
    ESP_ILOG(module_tag, "MQTT Setup");
    mqtt.setServer("192.168.0.11", atoi("1883"));
    mqtt.setCallback(mqtt_callback);
}

}
