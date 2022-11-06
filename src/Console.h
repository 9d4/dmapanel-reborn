#include <Blynk/BlynkConsole.h>
#include <ArduinoJson.h>

BlynkConsole    edgentConsole;

void console_init()
{
  edgentConsole.init(BLYNK_PRINT);

  edgentConsole.print("\n>");

  edgentConsole.addCommand("reboot", []() {
    String data = (R"json({"status":"OK","msg":"resetting device"})json" "\n");
    edgentConsole.print(data);
    Blynk.virtualWrite(VIRTPIN_TERMINAL, data);
    delay(100);
    restartMCU();
  });

  edgentConsole.addCommand("config", []() {
    String data = (R"json({"status":"OK","msg":"entering configuration mode"})json" "\n");
    edgentConsole.print(data);
    Blynk.virtualWrite(VIRTPIN_TERMINAL, data);
    BlynkState::set(MODE_WAIT_CONFIG);
  });

  edgentConsole.addCommand("devinfo", []() {
    DynamicJsonDocument dat(256);

    dat["board"] = BLYNK_DEVICE_NAME;
    dat["tmpl_id"] = BLYNK_TEMPLATE_ID;
    dat["fw_type"] = BLYNK_FIRMWARE_TYPE;
    dat["fw_ver"] = BLYNK_FIRMWARE_VERSION;

    String data;
    serializeJson(dat, data);    

    edgentConsole.print(data);
    Blynk.virtualWrite(VIRTPIN_TERMINAL, data);
  });

  edgentConsole.addCommand("netinfo", []() {
    DynamicJsonDocument dat(1152);
    String data;
    
    dat["ssid"] = WiFi.SSID();
    dat["bssid"] = WiFi.BSSIDstr();
    dat["ip"] = WiFi.localIP().toString();
    dat["gateway"] = WiFi.gatewayIP().toString();
    dat["mac"] = WiFi.macAddress();
    dat["rssi"] = WiFi.RSSI();

    serializeJson(dat, data);

    edgentConsole.print(data);
    Blynk.virtualWrite(VIRTPIN_TERMINAL, data);
  });

}

BLYNK_WRITE(InternalPinDBG) {
  String cmd = String(param.asStr()) + "\n";
  edgentConsole.runCommand((char*)cmd.c_str());
}

BLYNK_WRITE(VIRTPIN_TERMINAL) {
  String cmd = String(param.asStr()) + "\n";
  edgentConsole.runCommand((char*)cmd.c_str());
}
