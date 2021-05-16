#include "mqtt.h"
AsyncMqttClient mqttClient;

const char *ssid = "505";
const char *password = "Nopassword~";

int state = 0;

int speedx = 0, speedy = 0;

void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {
    String commandTopic = "home/speed";
    char payloadbuf[20];//new some memory?
    memset(payloadbuf,0,sizeof(payloadbuf));
    strncpy(payloadbuf,payload,(len>19)?19:len);
    // Serial.println(payloadbuf);
    if (commandTopic == topic)
    {
        if (payloadbuf[0] == 'S' && payloadbuf[10] == 'E'){
            sscanf (payloadbuf,"Sx%03d:y%03dE",&speedx,&speedy);
            speedx += 50;
            speedy += 50;
            speedx = speedx > 100?100:speedx;speedx = speedx < -100?-100:speedx; 
            speedy = speedy > 100?100:speedy;speedy = speedy < -100?-100:speedy; 
        }
    }
    else{
        Serial.print("topic error: ");
        Serial.println(topic);
    }

}

void onMqttConnect(bool sessionPresent)
{
  Serial.println("Connected to MQTT.");
  Serial.print("Session present: ");
  Serial.println(sessionPresent);
  mqttClient.subscribe("home/speed", 2);
}

// void onMqttPublish(uint16_t packetId)
// {
//   Serial.println("Publish acknowledged.");
//   Serial.print("  packetId: ");
//   Serial.println(packetId);
// }


void mqtt_init(){
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.println(".");
    }
    IPAddress mqttServerIP;
    WiFi.hostByName("h.kisonhe.xyz", mqttServerIP);

    mqttClient.onConnect(onMqttConnect);
    mqttClient.onMessage(onMqttMessage);
    mqttClient.setCredentials("esp32General","azxcvbnm");
    mqttClient.setServer(mqttServerIP, 51883);
    mqttClient.connect();
}
// void loop(){
//     if (!mqttClient.connected()){
//         Serial.print("Not connected!");
//         delay(100);
//         return;
//     }
        
//     char charBuf[50];
//     // static int data = 0;
//     // data = (data>100)?-100:data++;
//     String payload;
//     if (state){
//         payload = "ON";
//         digitalWrite(4,1);
//     }
//     else{
//         payload = "OFF";
//         digitalWrite(4,0);
//     }
//     payload.toCharArray(charBuf,49);
//     mqttClient.publish("ha/StateSwitch",2,true,(const char*)charBuf);
//     delay(1000);
// }