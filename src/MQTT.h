#ifndef MQTT_H
#define MQTT_H

#include <PubSubClient.h>

void mqttCallback(char *topic, byte *payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

BearSSL::WiFiClientSecure cliSSL;
PubSubClient pubSubCli(cliSSL);

void mqttBegin() {
  cliSSL.setTrustAnchors(&MQTTCert);
  cliSSL.setBufferSizes(512, 512);
  pubSubCli.setServer(MQTT_SERVER_HOST, MQTT_SERVER_PORT);
  pubSubCli.setCallback(mqttCallback);
}

boolean mqttReconnect() {
  Serial.println("MQTT: reconnecting...");
  char err_buf[256];
  if (!pubSubCli.connect(BLYNK_TEMPLATE_NAME, MQTT_SERVER_USER, MQTT_SERVER_PASS, "home/dmapanel/available", 0, true, "offline")) {
    Serial.print("MQTT: failed, rc=");
    Serial.println(pubSubCli.state());
    cliSSL.getLastSSLError(err_buf, sizeof(err_buf));
    Serial.print("SSL error: ");
    Serial.println(err_buf);
    return false;
  }
  pubSubCli.publish("home/dmapanel/available", "online");
  return pubSubCli.connected();

}

void mqttLoop() {
  if (!WiFi.isConnected()) {
    Serial.println("MQTT: wifi not connected");
    return;
  }

  static long lastReconnectAttempt = 0;

  if (!pubSubCli.connected()) {
    long now = millis();
    if (now - lastReconnectAttempt > 5000) {
      lastReconnectAttempt = now;
      // Attempt to reconnect
      if (mqttReconnect()) {
        lastReconnectAttempt = 0;
        Serial.println("MQTT: connected");
      }
    }
  }
  pubSubCli.loop();
}

#endif