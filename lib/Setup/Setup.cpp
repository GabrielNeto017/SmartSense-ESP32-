
#include <Setup.h>


WiFiMQTTManager wmm(SW2, AP_PASSWORD);  // AP_PASSWORD is defined in the secrets.h file

//MCP3909 - GAIN
float G1, G0 = 1;

void Setup_Init()
{

    //Declaração de variáveis
    pinMode (DI0, INPUT);
    pinMode (DI1, INPUT);
    pinMode (DI2, INPUT);
    pinMode (DI3, INPUT);
    pinMode (SW1, INPUT);
    
    pinMode (DO0, OUTPUT);
    pinMode (DO1, OUTPUT);
    pinMode (DO2, OUTPUT);

    //ANALISADOR - MCP 3909
    pinMode (MCP_MISO, INPUT);
    pinMode (MCP_MOSI, OUTPUT);
    pinMode (MCP_SCK, OUTPUT);
    pinMode (MCP_CS, OUTPUT);
    pinMode (MCLR, OUTPUT);
    pinMode (G1_gain, OUTPUT);
    pinMode (G0_gain, OUTPUT);

    //ENTRADAS ANALÓGICAS
    pinMode (AI0, INPUT_PULLUP);
    pinMode (AI1, INPUT_PULLUP);
    analogReadResolution(12);
    //analogSetWidth(12);

    //LEDS
    pinMode (LED1, OUTPUT);
    pinMode (LED2, OUTPUT);
}

void Wifi_init()
{
     //---------------------------------------------
    //STATIC IP
    /*IPAddress local_IP(192, 168, 150, 19);   /////////////////////////////// mudar o ip da planta
    IPAddress gateway(192, 168, 150, 1);

    IPAddress subnet(255, 255, 0, 0);
    IPAddress primaryDNS(8, 8, 8, 8);   //optional
    IPAddress secondaryDNS(0, 0, 0, 0); //optional

    // Configures static IP address
    if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("STA Failed to configure");
    }*/
    //-----------------------------------------------------------



    //WIFI-MQTT config
    Serial.println(F("WiFiMQTTManager Basic Example"));
    // set debug to true to get verbose logging
    // wm.wm.setDebugOutput(true);
    // most likely need to format FS but only on first use
    // wmm.formatFS = true;
    // optional - define the function that will subscribe to topics if needed
    wmm.subscribeTo = subscribeTo;
    // required - allow WiFiMQTTManager to do it's setup
    wmm.setup(__SKETCH_NAME__);
    // optional - define a callback to handle incoming messages from MQTT
    wmm.client->setCallback(subscriptionCallback);
}

void Analisador_Init()
{
    digitalWrite (MCLR, HIGH);
    digitalWrite (MCP_CS, HIGH);
    digitalWrite (G0, HIGH);
    digitalWrite (G1, HIGH);

    digitalWrite (MCLR, 1);
    digitalWrite (MCP_CS, 1);

    //ENTRY CODE
    digitalWrite (MCLR, LOW);
    digitalWrite (MCP_CS, HIGH);
    digitalWrite (MCP_SCK, LOW);

    digitalWrite (MCP_SCK, LOW);
    digitalWrite (MCLR, HIGH);
    digitalWrite (MCP_CS, LOW);

    //POST HP1 DUAL CHANNEL OUTPUT (0xAC) - check manual MCP3909

    //1
    digitalWrite (MCP_SCK, 0);
    digitalWrite (MCP_MOSI, 1);
    digitalWrite (MCP_SCK, 1);

    //2
    digitalWrite (MCP_SCK, 0);
    digitalWrite (MCP_MOSI, 0);
    digitalWrite (MCP_SCK, 1);

    //3
    digitalWrite (MCP_SCK, 0);
    digitalWrite (MCP_MOSI, 1);
    digitalWrite (MCP_SCK, 1);

    //4
    digitalWrite (MCP_SCK, 0);
    digitalWrite (MCP_MOSI, 0);
    digitalWrite (MCP_SCK, 1);

    //5
    digitalWrite (MCP_SCK, 0);
    digitalWrite (MCP_MOSI, 1);
    digitalWrite (MCP_SCK, 1);

    //6
    digitalWrite (MCP_SCK, 0);
    digitalWrite (MCP_MOSI, 1);
    digitalWrite (MCP_SCK, 1);

    //7
    digitalWrite (MCP_SCK, 0);
    digitalWrite (MCP_MOSI, 0);
    digitalWrite (MCP_SCK, 1);

    //8
    digitalWrite (MCP_SCK, 0);
    digitalWrite (MCP_MOSI, 0);
    digitalWrite (MCP_SCK, 1);

    digitalWrite (MCP_CS, 1);

    digitalWrite (MCP_SCK, 0);

}


// optional function to subscribe to MQTT topics
void subscribeTo() {
  Serial.println("subscribing to some topics...");  
  // subscribe to some topic(s)
  char topic[100];
  snprintf(topic, sizeof(topic), "%s%s%s", "switch/", wmm.deviceId, "/TESTE");
  wmm.client->subscribe(topic);
}

// optional function to process MQTT subscribed to topics coming in
void subscriptionCallback(char* topic, byte* message, unsigned int length) 
{
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  //if (String(topic) == "switch/esp1234/led1/output") {
  //  Serial.print("Changing led1 output to ");
  //}
}